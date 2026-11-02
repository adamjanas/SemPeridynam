#ifndef IO_HPP
#define IO_HPP

#include <string>
#include "particles.hpp"

// Writes simulation state to a VTK file inside the 'paraview' folder
void write_vtk_file(const std::string& filename, const Particles& p);

#endif