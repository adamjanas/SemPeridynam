//Definice řešitele peridynamické simulace
//pametovy kontejner - Parametry materiálu
    // E - mikromodul c, rho, delta, kritické prodloužení s_c
    // výpočet bezrozměrných parametrů

#ifndef SOLVER_HPP
#define SOLVER_HPP
#include <cmath>
#include "particles.hpp"


//struct - kontejner obsahujici materialove parametry
struct MatParams {

    double E; //younguv modul
    double rho; //hustota
    double Gc; //kriticka energie pro poruseni
    double delta; //velikost okoli bodu
    double tloustka; //tloustka

    double c0; // mikromodul
    double s0; //kriticke prodlouzeni pro poruseni

    void vypocet_zavislych_param() {

        //vypocet mikromodulu a krit prodlouzeni z param pro ulozeni do struct
        c0 = (9.0 * E) / (M_PI * tloustka * delta * delta * delta);
        s0 = std::sqrt((4.0 * M_PI * Gc)/((9.0 * E * delta)));
    }

};

//class pro uvedeni vnitrnich funkci vypoctu uvnitr solver.cpp pro main.cpp
class Solver {
public:
    // vnitrni fce solver.cpp
    static void compute_internal_forces(Particles& p, const MatParams& mat);
    static void update_damage(Particles& p);
    static void time_step_verlet(Particles& p, const MatParams& mat, double dt);
};


#endif //SOLVER_HPP