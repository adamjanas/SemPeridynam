#pragma once

#include <array>
#include <vector>

// Definice jednotlivých částic a jejich okolí peridynamického modelu
    // Matice počáteční polohy bodů, matice aktuální polohy bodů, matice rychlostí
    // Matice silové hustoty f
    // vektor elementárních objemů V
    // Matice sousedů ovlivňujích daný bod, matice aktivních vazeb, matice délek vazeb
    // vektory potenciální a disipované energie
    // příprava přidávání bodů pomocí void add_particle

struct Particles {
    std::vector<std::array<double, 2>> initial_positions;
    std::vector<std::array<double, 2>> positions;
    
    std::vector<std::array<double, 2>> velocities;

    std::vector<std::array<double, 2>> force_densities;

    std::vector<double> volumes;

    void add_particle(std::array<double, 2> position, double volume) {
        initial_positions.push_back(position);
        positions.push_back(position);
        volumes.push_back(volume);
        velocities.push_back({0.0, 0}); //0 nebo 0.0 je jedno, 0.0 je jasné, že dat. typ je double
        force_densities.push_back({0, 0});
    }
};