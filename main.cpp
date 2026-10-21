#include <iostream>
#include "particles.hpp"

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

    double dx = 1; // vzdálenost mezi body sítě
    double dy = 1;
    double t = 1;  // tloušťka

// vysíťování obdelníku - síťuji i hraniční body - mají menší charakteristický objem
    for (int i = 0; i <= 100; i++) {
        for (int j = 0; j <= 50; j++) {
            if ((i == 0 || i == 100) && (j == 0 || j == 50)) {
            particles.add_particle({i * dx, j * dy}, 0.25*dx*dy*t);
            }
            else if (i == 0 || i == 100 || j == 0 || j == 50) {
                    particles.add_particle({i * dx, j * dy}, 0.5*dx*dy*t);
            }
            else {
                particles.add_particle({i * dx, j * dy}, dx*dy*t);
            }
        }
    }
    
    //kontrola objemu - sedí
    double particles_volume_sum = 0;

    for (std::size_t i = 0; i < particles.volumes.size(); i++) { // std::size_t je stejný typ jako .size()
        particles_volume_sum += particles.volumes[i];
    }
  
    std::cout << particles.positions[1][1] << std::endl;
    std::cout << particles_volume_sum << std::endl;

    return 0;
}
