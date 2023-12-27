#include "core/Constants.h"
#include "core/Solver.h"

Solver::Solver(const FluidGrid& fluidGrid)
    : m_fluidGrid{ fluidGrid }
{
}

void Solver::updateVelocities(float dt, float gravity)
{
    for (size_t i = 0; i < m_fluidGrid.getHeight(); i++)
    {
        for (size_t j = 0; j < m_fluidGrid.getWidth(); j++)
        {
            // vel_v[i, j] <- vel_v[i, j] + dt * g
            float newVel = m_fluidGrid.getV(i, j) + dt * gravity;
            m_fluidGrid.setV(i, j, newVel);
        }
    }
}

float Solver::calcDivergence(size_t row, size_t col) const
{
    return m_fluidGrid.getU(row + 1, col)
        - m_fluidGrid.getU(row, col)
        + m_fluidGrid.getV(row, col + 1)
        - m_fluidGrid.getV(row, col);
}

void Solver::project(float dt, float density, float gridSpacing)
{
    // Reset pressure values
    for (size_t i = 0; i < m_fluidGrid.getHeight(); i++)
    {
        for (size_t j = 0; j < m_fluidGrid.getWidth(); j++)
        {
            m_fluidGrid.setP(i, j, 0.0f);
        }
    }

    // Solve grid
    for (size_t i = 0; i < m_fluidGrid.getHeight(); i++)
    {
        for (size_t j = 0; j < m_fluidGrid.getWidth(); j++)
        {
            float div = k_overrelaxation * calcDivergence(i, j);    // overrelaxation makes it converge quicker

            // # of neighboring fluid cells
            float s = m_fluidGrid.getS(i + 1, j)
                + m_fluidGrid.getS(i - 1, j)
                + m_fluidGrid.getS(i, j + 1)
                + m_fluidGrid.getS(i, j - 1);

            // u[i, j] <- u[i, j] + d * s[i - 1, j] / s
            float newU1 = m_fluidGrid.getU(i, j) + div * m_fluidGrid.getS(i - 1, j) / s;

            // u[i + 1, j] <- u[i + 1, j] - d * s[i + 1, j] / s
            float newU2 = m_fluidGrid.getU(i + 1, j) - div * m_fluidGrid.getS(i + 1, j) / s;

            // v[i, j] <- v[i, j] + d * s[i, j - 1] / s
            float newV1 = m_fluidGrid.getV(i, j) + div * m_fluidGrid.getS(i, j - 1) / s;

            // v[i, j + 1] <- v[i, j + 1] - d * s[i, j + 1] / s
            float newV2 = m_fluidGrid.getV(i, j + 1) - div * m_fluidGrid.getS(i, j + 1) / s;

            m_fluidGrid.setU(i, j, newU1);
            m_fluidGrid.setU(i + 1, j, newU2);

            m_fluidGrid.setV(i, j, newV1);
            m_fluidGrid.setV(i, j + 1, newV2);

            // p[i, j] <- p[i, j] + (d / s) * (rho * h) / dt
            float newP = m_fluidGrid.getP(i, j) + (div / s) * (density * gridSpacing / dt);
        }
    }

}