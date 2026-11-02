// Definice jednotlivých částic a jejich okolí peridynamického modelu
    // Matice počáteční polohy bodů, matice aktuální polohy bodů, matice rychlostí
    // Matice silové hustoty f
    // vektor elementárních objemů V
    // Matice sousedů ovlivňujích daný bod, matice aktivních vazeb, matice délek vazeb
    // vektory potenciální a disipované energie
    // příprava přidávání bodů pomocí void add_particle

#pragma once
#include <array>
#include <vector>

struct Particles {
    std::vector<std::array<double, 2>> initial_positions;
    std::vector<std::array<double, 2>> positions;
    
    std::vector<std::array<double, 2>> velocities;

    std::vector<std::array<double, 2>> force_densities;

    std::vector<double> volumes;
    
    // 0 - klasická, 1 - fixní, 2 - předepsaný posun
    std::vector<int> boundary_type;

    // sousedé a vazby
    std::vector<std::vector<int>> neighbors;
    std::vector<std::vector<bool>> active_bonds;
    std::vector<std::vector<double>> initial_bond_lengths;

    void add_particle(std::array<double, 2> position, double volume) {
        initial_positions.push_back(position);
        positions.push_back(position);
        volumes.push_back(volume);
        velocities.push_back({0, 0}); 
        force_densities.push_back({0, 0});
        neighbors.push_back({});
        active_bonds.push_back({});
        initial_bond_lengths.push_back({});
        boundary_type.push_back(0);
    }

    // fce pro vysíťování desky s dírou, pozor - přidává body k již existující Particles desce, nevytvoří novou desku
    void boardMeshing(double tloustka, double Sx, double Sy, double R, double dl, int Nx, int Ny) {    
        for (int i = 0; i <= Ny; i++) {
            for (int j = 0; j <= Nx; j++) {
                if ((dl * j - Sx)*(dl * j - Sx) + (dl * i - Sy) * (dl * i - Sy) < R * R) { // x^2+y^2<R^2 - podmína pro díru
                    continue; // přeskočí krok - add_particle() se nespustí - přeskočí body se souřadnicemi v díře
                }
                if ((i == 0 || i == Ny) && (j == 0 || j == Nx)) {
                    add_particle({j * dl, i * dl}, 0.25 * dl * dl * tloustka);
                }
                else if (i == 0 || i == Ny || j == 0 || j == Nx) {
                    add_particle({j * dl, i * dl}, 0.5 * dl * dl * tloustka);
                }
                else {
                    add_particle({j * dl, i * dl}, dl * dl * tloustka);
                }
            }
        }
    }
};
