#include <iostream>
#include "particles.hpp"
#include <algorithm>

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
    Particles particles;
    // particles.add_particle({0.01, 0.02}, 1e-6);   //test - funguje
    // particles.add_particle({0.03, 0.04}, 1e-6);

    // velikost objektu - rozměry desky
    double tloustka = 1;  // tloušťka
    double Lx = 100; 
    double Ly = 50;
    double Sx = Lx/2; //střed díry
    double Sy = Ly/2; 
    double R = 10; //poloměr díry

    // parametry sítě
    double dl = 1; // vzdálenost mezi body sítě
    int Nx = Lx / dl; // interval bodů - máme x = 0 až x = 100 pro jeden řádek (počet sloupců bodů je Nx + 1) - nutné převést na int
    int Ny = Ly / dl; // interval bodů

// vysíťování obdelníku - síťuji po řádcích - síťuji i hraniční body - mají menší objem přiřazený k materiálovému bodu - proto obsahují if funkce - pokud jsou přesně na kraji, tak mají poloviční objem, v rohu mají čtvrtinový objem
    for (int i = 0; i <= Ny; i++) {
        for (int j = 0; j <= Nx; j++) {
            if ((dl * j - Sx)*(dl * j - Sx) + (dl * i - Sy) * (dl * i - Sy) < R * R) { // x^2+y^2<R^2 - podmína pro díru
                continue; // přeskočí krok - add_particle() se nespustí - přeskočí body se souřadnicemi v díře
            }
            if ((i == 0 || i == Ny) && (j == 0 || j == Nx)) {
                particles.add_particle({j * dl, i * dl}, 0.25 * dl * dl * tloustka);
            }
            else if (i == 0 || i == Ny || j == 0 || j == Nx) {
                particles.add_particle({j * dl, i * dl}, 0.5 * dl * dl * tloustka);
            }
            else {
                particles.add_particle({j * dl, i * dl}, dl * dl * tloustka);
            }
        }
    }
    
    //kontrola objemu - sedí
    double particles_volume_sum = 0; //počáteční objem

    for (std::size_t i = 0; i < particles.volumes.size(); i++) { // std::size_t je stejný typ jako .size(), vhodné pro porovnání i a ...size()
        particles_volume_sum += particles.volumes[i];
    }
  
    //kontrola Nx, Ny a dalších šikovných veličin
    std::cout << Nx << ' ' << Ny << std::endl;

    std::cout << particles.positions.size() << std::endl;
    std::cout << particles_volume_sum << std::endl;
    std::cout << particles.initial_positions[100][1] << std::endl;

    //vizualizace - OMFG TOTO DĚLÁM POPRVÉ A JE TO HUSTÝ

    for (int i = Ny; i >= 0; i--) {
        for (int j = 0; j <= Nx; j++) {
            bool found = false;
            double x = j * dl;
            double y = i * dl;
            for (std::size_t k = 0; k < particles.initial_positions.size(); k++) {
                if (x == particles.initial_positions[k][0] && y == particles.initial_positions[k][1]) {
                    found = true;
                    break;
                }
            }
            if (found == true) {
                std::cout << "O";
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
