#include <iostream>
#include <string>
#include "particles.hpp"
#include "grid.hpp"
#include "solver.hpp"
#include "solver.cpp"
#include "io.hpp"

//hlavni fce
    //nastaveni hodnot materialovych param - E, rho atd.
    //natavení rozteče bodů, horizontu, tloušťky, objemu částice
    //nastaveni casovych kroku
    //tvorba obdelnikove site s kruhovou dirou --> vytvoření objektu particles
        //definice obdelniku
        //for cyklus, pro vzd od stredu vetsi nez R uloz castici
    //volame Grid, tvorba seznamu sousedu
    //zadani okrajovych podminek
    //vytvoření objektu solver
    //export počátečního stavu
    //casovy cyklus
        //vyvozeni napeti posouvanim x rad okrajovych castic v kazdem casovem kroku
        //volani resice time-step-verlet
        //volani fce na export dat
    // definice fce na export dat
        //export deformovanych souradnic
        //export skalarniho pole disipovane energie

int main()
{
    Particles board;

    Grid boardMap;

    //====================NASTAVUJEME=====================================
    // velikost desky [m]
    double tloustka = 1e-3;  // tloušťka
    double Lx = 100e-3; 
    double Ly = 50e-3;
    double R = 10e-3; //poloměr díry

    // vlastnosti materiálu
    double E = 210e9; // Pa
    double rho = 7850; // kg/m^3
    double Gc = 12500; // N/m

    // parametry sítě
    double dl = 1e-3; // vzdálenost mezi body sítě (m)

    // konfigurace parametru simulace
    int total_steps = 5000;
    //pocet exportovanych kroku
    int num_steps = 100;
    int output_frequency = total_steps / num_steps;
    double dt = 1e-5;

    //=====================================================================
    // není potřeba nastavit
    double delta = 3 * dl;

    int Nx = Lx / dl; // interval bodů - máme x = 0 až x = 100 pro jeden řádek (počet sloupců bodů je Nx + 1) - nutné převést na int
    int Ny = Ly / dl; // interval bodů

    double Sx = Lx/2; //střed díry - prozatím uprostřed
    double Sy = Ly/2; 

    MatParams parameters;

    parameters.E = E;
    parameters.rho = rho;
    parameters.Gc = Gc;
    parameters.delta = delta;
    parameters.tloustka = tloustka;

    boardMap.Nx = Nx;
    boardMap.Ny = Ny;
    boardMap.dl = dl;

    parameters.vypocet_zavislych_param();

    // vysíťování desky - síťuji po řádcích 
    // síťuji i hraniční body - mají menší objem přiřazený k materiálovému bodu - proto obsahují if - pokud jsou přesně na kraji, tak mají poloviční objem, v rohu mají čtvrtinový objem
    board.boardMeshing(tloustka, Sx, Sy, R, dl, Nx, Ny);

    // sestavení mapy pro hledání sousedů
    boardMap.build_map(board);
  
    // hledání a sestavení sousedů
    boardMap.build_neighbors(board, parameters);
 
    // zadání OP - 5 sloupců vlevo se nebude hýbat, 5 sloupců vpravo má předepsané posunutí
    boardMap.set_fixed_boundary_type (board, 0, 4, 0, Ny);
    boardMap.set_fixed_displacement(board, dt, total_steps, 20e-3, 0, Nx - 4, Nx, 0, Ny);

    //Hlavni loop simulace - volani solveru
        //for cyklus pres vsechny steps
            for (int i = 0; i < total_steps; i++) {
            //volame solver
            Solver::time_step_verlet(board, parameters, dt);
            // if step % output_frequency == 0
                if (i % output_frequency == 0) {
                    //generovani nazvu frame_xxxxxx.vtk
                    std::string step_str = std::to_string(i);
                    std::string padded_step = std::string(6 - std::min(size_t(6), step_str.length()), '0') + step_str;
                    std::string filename = "frame_" + padded_step + ".vtk";
                    //volani io - write_vtk
                    write_vtk_file(filename, board);
                    //oznameni do konzole o exportu
                    std::cout << "Exported snapshot at step " << i << " -> paraview/" << filename << "\n";
                }
        //konec cyklu

    //kontrola Nx, Ny a dalších šikovných veličin

    // std::cout << boardMap.particle_indices[0 * (Nx + 1) + 0] <<std::endl;
    // std::cout << board.neighbors[boardMap.particle_indices[0 * (Nx + 1) + 0]][0] <<std::endl;

    // int a = 0;
    // for (size_t k = 0; k < board.initial_bond_lengths[boardMap.particle_indices[a]].size(); k++) {
    //     std::cout << board.initial_bond_lengths[boardMap.particle_indices[a]][k] << std::endl;
    // };

    std::cout << "Nx = " << Nx << ' ' << "Ny = " << Ny << std::endl;

    // využití mapovací funkce - složitost O(n)
    boardMap.visualization();

    std::cout << "Simulation finished successfully!\n";
    return 0;
}
