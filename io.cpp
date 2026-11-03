#include "io.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>

//fce na vypocet poskozeni castice (rozdil poctu aktiv a neaktiv vazeb)
double compute_particle_damage(size_t i, const Particles& p) {
    size_t total_bonds = p.active_bonds[i].size();
    //if kontrola ze pocet vazeb neni nula - vyhnuti se deleni nulou ve phi
    if (total_bonds == 0) return 0.0;
    //vypocet poctu aktivnich vazeb - projdu vsechny s for cyklem
    size_t active_count = 0;
    for (int is_active : p.active_bonds[i]) {
        active_count += is_active;
    }
    //vypocet poskozeni phi 1 - aktiv/total
    return 1.0 - (static_cast<double>(active_count) / total_bonds);
}

//fce na vypis vysledku - vtk soubory
void write_vtk_file(const std::string& filename, const Particles& p) {

    //tvorba slozky pro ulozeni vtk
    std::filesystem::create_directories("paraview");

    //tvorba adresy + nazvu pro ukladani vtk
    std::string filepath = "paraview/" + filename;

    //kontrola ze existuje vtk
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << " for writing.\n";
        return;
    }

    size_t num_particles = p.positions.size();

    //Header
    file << "# vtk DataFile Version 3.0\n";
    file << "Peridynamics Simulation Snapshot\n";
    file << "ASCII\n";
    file << "DATASET UNSTRUCTURED_GRID\n";

    //export poloh jednotlivych bodu
    file << "POINTS " << num_particles << " float\n";
    for (size_t i = 0; i < num_particles; ++i) {
        file << p.positions[i][0] << " " << p.positions[i][1] << " 0.0\n";
    }
    //tvorba bunek pro paraview
        //ulozeni indexu kazde bunky
        file << "CELLS " << num_particles << " " << (2 * num_particles) << "\n";
        for (size_t i = 0; i < num_particles; ++i) {
            file << "1 " << i << "\n";
        }
        // prirazeni cell type vertex kazde bunce
        file << "CELL_TYPES " << num_particles << "\n";
        for (size_t i = 0; i < num_particles; ++i) {
            file << "1\n";
        }
    //ulozeni floatu rychlosti
    file << "POINT_DATA " << num_particles << "\n";
    file << "VECTORS velocity float\n";
    for (size_t i = 0; i < num_particles; ++i) {
        file << p.velocities[i][0] << " " << p.velocities[i][1] << " 0.0\n";
    }

    //ulozeni skalarni veliciny poskozeni castice phi
    file << "SCALARS damage float 1\n";
    file << "LOOKUP_TABLE default\n";
    for (size_t i = 0; i < num_particles; ++i) {
        double phi = compute_particle_damage(i, p);
        file << phi << "\n";
    }

    //file.close()
    file.close();
}