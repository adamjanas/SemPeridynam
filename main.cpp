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

    //====================NASTAVUJEME===============================
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

    //==============================================================
    // není potřeba nastavit
    double delta = 3 * dl;

    int Nx = Lx / dl; // interval bodů - máme x = 0 až x = 100 pro jeden řádek (počet sloupců bodů je Nx + 1) - nutné převést na int
    int Ny = Ly / dl; // interval bodů

    double Sx = Lx/2; //střed díry
    double Sy = Ly/2; 

    MatParams parameters;

    parameters.E = E;
    parameters.rho = rho;
    parameters.Gc = Gc;
    parameters.delta = delta;
    parameters.tloustka = tloustka;

    parameters.vypocet_zavislych_param();

    // vysíťování obdelníku - síťuji po řádcích 
    // síťuji i hraniční body - mají menší objem přiřazený k materiálovému bodu - proto obsahují if - pokud jsou přesně na kraji, tak mají poloviční objem, v rohu mají čtvrtinový objem
    board.boardMeshing(tloustka, Sx, Sy, R, dl, Nx, Ny);

    // sestavení mapy pro hledání sousedů
    boardMap.build_map(board, dl, Nx, Ny);
  
    // hledání a sestavení sousedů
    boardMap.build_neighbors(board, parameters, dl, Nx, Ny);
 
    //Hlavni loop simulace - volani solveru
        //for cyklus pres vsechny steps
            //volame solver
            // if step % output_frequency == 0
                //generovani nazvu frame_xxxxxx.vtk
                //volani io - write_vtk
                //oznameni do konzole o exportu
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
    boardMap.visualization(Nx, Ny);

    return 0;
}
