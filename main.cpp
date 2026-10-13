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