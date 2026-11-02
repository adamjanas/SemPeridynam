#include <iostream>
#include "particles.hpp"
#include "grid.hpp"
#include "solver.hpp"
#include "solver.cpp"

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

// fce pro vysíťování desky s dírou, pozor - přidává body k již existující Particles desce, nevytvoří novou desku
void boardMeshing(Particles& deska, double tloustka, double Sx, double Sy, double R, double dl, int Nx, int Ny) {    
    for (int i = 0; i <= Ny; i++) {
        for (int j = 0; j <= Nx; j++) {
            if ((dl * j - Sx)*(dl * j - Sx) + (dl * i - Sy) * (dl * i - Sy) < R * R) { // x^2+y^2<R^2 - podmína pro díru
                continue; // přeskočí krok - add_particle() se nespustí - přeskočí body se souřadnicemi v díře
            }
            if ((i == 0 || i == Ny) && (j == 0 || j == Nx)) {
                deska.add_particle({j * dl, i * dl}, 0.25 * dl * dl * tloustka);
            }
            else if (i == 0 || i == Ny || j == 0 || j == Nx) {
                deska.add_particle({j * dl, i * dl}, 0.5 * dl * dl * tloustka);
            }
            else {
                deska.add_particle({j * dl, i * dl}, dl * dl * tloustka);
            }
        }
    }
}

// fce pro vizualizaci zadaného objektu - využívá mapovací funkce, složitost O(n)
void visualization(const Grid& g, int Nx, int Ny) {
    for (int i = 0; i <= Ny; i++) {
        for (int j = 0; j <= Nx; j++) {
            if (g.particle_indices[i * (Nx + 1) + j] == -1) {
                std::cout << ' ';
            } else {
                std::cout << "O";
            }
        }
        std::cout << std::endl;
    }
}

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

    double dt = 0.1;

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
    boardMeshing(board, tloustka, Sx, Sy, R, dl, Nx, Ny);

    // sestavení mapy pro hledání sousedů
    boardMap.build_map(board, dl, Nx, Ny);
  
    // hledání a sestavení sousedů
    boardMap.build_neighbors(board, parameters, dl, Nx, Ny);
    

    //kontrola Nx, Ny a dalších šikovných veličin

    // std::cout << boardMap.particle_indices[0 * (Nx + 1) + 0] <<std::endl;
    // std::cout << board.neighbors[boardMap.particle_indices[0 * (Nx + 1) + 0]][0] <<std::endl;

    int a = 0;
    for (size_t k = 0; k < board.initial_bond_lengths[boardMap.particle_indices[a]].size(); k++) {
        std::cout << board.initial_bond_lengths[boardMap.particle_indices[a]][k] << std::endl;
    };

    std::cout << Nx << ' ' << Ny << std::endl;

    // std::cout << deska.positions.size() << std::endl;
    // std::cout << particles_volume_sum << std::endl;
    // std::cout << deska.initial_positions[100][0] << std::endl;

    // využití mapovací funkce - složitost O(n)
    // visualization(boardMap, Nx, Ny);

    return 0;
}
