#pragma once

#include "FluidGrid.h"

/**
 * A class for managing a fluid grid.
*/
class Solver
{
public:
    Solver(const FluidGrid& fluidGrid);

    /// Applies gravity to all cells in the grid by updating their vertical components.
    /// @param dt Delta time.
    void updateVelocities(float dt, float gravity);

    /// Calculates the divergence at a given cell.
    /// The divergence at cell [i, j] is div[i, j] = u[i + 1, j] - u[i, j] + v[i, j + 1] - v[i,j]
    ///
    /// There are three possible outcomes:
    /// - Positive -> too much outflow
    /// - Negative -> too much inflow
    /// - Zero -> incompressible
    float calcDivergence(size_t row, size_t col) const;

    /// Updates the whole grid so that it remains incompressible using the Gauss-Seidel method.
    void project(float dt, float density, float gridSpacing);

private:
    FluidGrid m_fluidGrid;
};
