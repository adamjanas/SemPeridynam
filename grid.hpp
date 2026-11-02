// definice gridu
// hledání sousedů pomocí vnořených for cyklů, jeden x jeden y
    // definice okolí pro hledání
    // hledání částic v okolí
        // přeskočit sama sebe
    // výpočet vzdálenosti jednotlivých částic od částice pro niž běží for cykly
    // kontrola zda částice jsou v okolí delta částice for cyklu

#pragma once
#include <vector>
#include <algorithm>
#include "particles.hpp"
#include "solver.hpp"
#include <cmath>

struct Grid {
    std::vector<signed int> particle_indices;

    // vytvoření mapy pro možnost lokality bodu vůči jiným podle indexu 
    void build_map(const Particles& p, double dl, int Nx, int Ny) { 
        
        // změní velikost inicializovaného particle_indi. na (Nx + 1) * (Ny + 1)
        particle_indices.resize((Nx + 1) * (Ny + 1));

        // naplní vektor -1
        for (int i = 0; i <= Ny; i++) {
            for (int j = 0; j <= Nx; j++) {
                int i_map = i * (Nx + 1) + j;
                particle_indices[i_map] = -1;
            }
        }

        // z polohy částic poskládá mapu, taky aby v oblasti díry ležěly pouze indexy -1
        for (std::size_t k = 0; k < p.initial_positions.size(); k++) {
            int j = std::round(p.initial_positions[k][0] / dl);
            int i = std::round(p.initial_positions[k][1] / dl);
            int i_map = i * (Nx + 1) + j;
            particle_indices[i_map] = k;
        }
    }

    // hledání a postavení sousedů
    void build_neighbors(Particles& p, const MatParams& mat, double dl, int Nx, int Ny) {
        
        // ulozeni do int pro porovnani s int dále + jediné převedení std::round - delta se zadava do MatParams v jednotkách, tady je bezrozmerne
        int build_delta = std::round(mat.delta / dl); 
        
        // double pro absolutní měření
        double measure_delta = mat.delta / dl;
        for (int i = 0; i <= Ny; i++) {
            for (int j = 0; j <= Nx; j++) {

                // budeme postupně sestavovat vektory jednotlivých veličin, nakonec se v celku vloží k jednotlivým částicím
                std::vector<int> built_neighbors;
                std::vector<bool> built_active_bonds;
                std::vector<double> built_bond_lengths;
                int i_map = i * (Nx + 1) + j;
                if (particle_indices[i_map] == -1) {
                    continue;
                } 
                
                // hledáme v 2*delta * 2*delta čtverci kolem částice, nesmíme hledat mimo particle_indices mapu
                int i_low_bound = std::max(i - build_delta, 0);
                int j_low_bound = std::max(j - build_delta, 0);
                int i_high_bound = std::min(i + build_delta, Ny);
                int j_high_bound = std::min(j + build_delta, Nx);
                for (int i_neigh = i_low_bound; i_neigh <= i_high_bound; i_neigh++) {
                    for (int j_neigh = j_low_bound; j_neigh <= j_high_bound; j_neigh++) {

                        // bezrozměrná vzdálenost mezi bodem a sousedem
                        double distance = std::sqrt((i_neigh - i) * (i_neigh - i) + (j_neigh - j) * (j_neigh - j)); 
                        
                        // index souseda na particle_indices mapě
                        int i_map_neigh = i_neigh * (Nx + 1) + j_neigh;

                        // přeskakujeme svojí částici a neexistující částice, kontrolujeme podmínku vzdálenosti < delta
                        if (particle_indices[i_map_neigh] == -1) {
                            continue;
                        } 
                        else if (i_map_neigh == i_map) {
                            continue;
                        } 
                        else if (distance > measure_delta) {
                            continue;
                        }
                        else {
                            built_neighbors.push_back(particle_indices[i_map_neigh]);
                            built_active_bonds.push_back(true);
                            built_bond_lengths.push_back(distance * dl);
                        } 
                    }
                }
                p.neighbors[particle_indices[i_map]] = built_neighbors;
                p.active_bonds[particle_indices[i_map]] = built_active_bonds;
                p.initial_bond_lengths[particle_indices[i_map]] = built_bond_lengths;
            }
        }
    }

    // vizualizace zadání - využívá mapovací funkce, složitost O(n)
    void visualization(int Nx, int Ny) {
        for (int i = 0; i <= Ny; i++) {
            for (int j = 0; j <= Nx; j++) {
                if (particle_indices[i * (Nx + 1) + j] == -1) {
                    std::cout << ' ';
                } else {
                    std::cout << "O";
                }
            }
            std::cout << std::endl;
        }
    }
};