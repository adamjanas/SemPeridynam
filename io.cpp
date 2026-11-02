#include "io.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

//fce na vypocet poskozeni castice (rozdil poctu aktiv a neaktiv vazeb)
    //if kontrola ze pocet vazeb neni nula - vyhnuti se deleni nulou ve phi
    //vypocet poctu aktivnich vazeb - projdu vsechny s for cyklem
    //vypocet poskozeni phi 1 - aktiv/total