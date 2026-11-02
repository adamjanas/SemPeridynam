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
    }
};
