#include <cmath>
#include "particles.hpp"
#include "solver.hpp"
//vnitrni fce pro vypocet sil pro verlet
void Solver::compute_forces(Particles& p, const MatParams& mat) {
    size_t num_particles = p.initial_positions.size();
    //for loop pres vsechny castice
    for (size_t i=0; i < num_particles; ++i) {
        //inic akumulace sil pro vypocet
        double f_accum_x = 0.0;
        double f_accum_y = 0.0;

        //nacteni pozic i-te castice
        double p0i_x = p.initial_positions[i][0];
        double p0i_y = p.initial_positions[i][1];
        double pi_x = p.positions[i][0];
        double pi_y = p.positions[i][1];

        //for loop pres vsechny sousedy
        for (size_t k = 0; k < p.neighbors[i].size(); ++k) {

            //preskoc znicene vazby
            if (p.active_bonds[i][k] == 0){
                continue;
            }

            int j = p.neighbors[i][k];

            //délka vazby
            double init_length = p.initial_bond_lengths[i][k];

            //preskoceni pro cleny bez deformace
            if (init_length == 0) {
                continue;
            }

            //vypocet aktualni deformovane delky
            double r_x = p.positions[j][0] - pi_x;
            double r_y = p.positions[j][1] - pi_y;
            double current_length = std::hypot(r_x, r_y);

            //vypocet jednotkoveho prodlouzeni a kontrola krit prodlouzeni
            double s = (current_length - init_length) / init_length;

            if (s > mat.s0){
                p.active_bonds[i][k] = 0;
                continue;
            }
            //vypocet sily mezi casticemi pro ktou castici na ite castici
            double force_mag = mat.c0 * s;

            //akumulace sily na i-te castici od k-sousedu - integral pres vsechny sousedy
            f_accum_x += force_mag * (r_x / current_length) * p.volumes[j];
            f_accum_y += force_mag * (r_y / current_length) * p.volumes[j];
        }
        
        //ulozeni aktualniho siloveho pusobeni
        p.force_densities[i][0] = f_accum_x;
        p.force_densities[i][1] = f_accum_y;
    }
}
//time step verlet
void Solver::time_step_verlet(Particles& p, const MatParams& mat, double dt) {
    size_t num_particles = p.initial_positions.size();

    //priprava halfstep
    double half_dt = 0.5*dt;
    //optimalizace - nasobeni misto deleni v for cyklu
    double inv_rho = 1.0 / mat.rho;
    //for cyklus pres vsechny castice
    for (size_t i = 0; i < num_particles; ++i){
    //výpočet počátečních sil a zrychlení
    double acc_x = p.force_densities[i][0] * inv_rho;
    double acc_y = p.force_densities[i][1] * inv_rho;
    //vypocet noveho pulkroku rychlosti a celeho posuvu
    p.velocities[i][0] += half_dt * acc_x;
    p.velocities[i][1] += half_dt * acc_y;

    p.positions[i][0] += dt * p.velocities[i][0];
    p.positions[i][1] += dt * p.velocities[i][1];
    }
    //vypocet sil, vazeb, energie
        //Výpočet sil působících na každou částici v aktuálním časovém kroku
            // kontrola zda vazby jsou aktivní
            // vypocet vektoru polohy a posuvu => vypocet delky vazby
            // vypocet relativniho prodlouzeni a kontrola zda nebylo prekroceno max
            // vypocet hustoty siloveho pole a akumulace elasticke potenc. energie
    compute_forces(p, mat);
    // dokonceni casoveho kroku
    for (size_t i = 0; i < num_particles; ++i){
    double acc_x = p.force_densities[i][0] * inv_rho;
    double acc_y = p.force_densities[i][1] * inv_rho;

    p.velocities[i][0] +=half_dt * acc_x;
    p.velocities[i][1] +=half_dt * acc_y;
    }
}