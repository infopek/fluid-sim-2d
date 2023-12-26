#pragma once

#include "Vec2.h"

#include <vector>

/*
* Represents an Eulerian grid for fluid simulation.
*/
class FluidGrid
{
public:
    FluidGrid();

private:
    std::vector<Vec2[4]> m_grid;    // staggered grid
};