#pragma once

#include "FluidGrid.h"

enum class FieldType
{
    u_field,
    v_field,
    s_field,
};

/**
 * A class for managing a fluid grid.
*/
class Solver
{
public:
    Solver(const FluidGrid& fluidGrid, float gridSpacing, float fluidDensity);

    /// Applies gravity to all cells in the grid by updating their vertical components.
    /// @param dt Delta time.
    void updateVelocities(float dt, float gravity);

    void extrapolate();

    /// Updates the whole grid so that it remains incompressible using the Gauss-Seidel method.
    /// @param dt Delta time.
    /// @param density Fluid density.
    /// @param gridSpacing Size of a cell in the grid.
    /// @param numIters Number of iterations of projection.
    void project(float dt, size_t numIters);

    float sampleField(float& x, float& y, FieldType fieldType) const;

    void advectVel(float dt);

    void advectSmoke(float dt);

private:
    /// Calculates the divergence at a given cell.
        /// The divergence at cell [i, j] is div[i, j] = u[i + 1, j] - u[i, j] + v[i, j + 1] - v[i,j]
        ///
        /// There are three possible outcomes:
        /// - Positive -> too much outflow
        /// - Negative -> too much inflow
        /// - Zero -> incompressible
    float calcDivergence(size_t row, size_t col) const;

    /// @return The average of the horizontal components at cell `[row, col]`.
    float getAverageU(size_t row, size_t col) const;

    /// @return The average of the vertical components at cell `[row, col]`.
    float getAverageV(size_t row, size_t col) const;

private:
    FluidGrid m_fluidGrid;

    float m_h{};

    float m_density{};
};
