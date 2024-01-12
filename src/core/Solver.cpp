#include "core/Constants.h"
#include "core/Solver.h"
#include <core/FluidGrid.h>
#include <math.h>
#include <utility>

Solver::Solver(const FluidGrid& fluidGrid, float gridSpacing, float fluidDensity)
    : m_fluidGrid{ fluidGrid },
    m_h{ gridSpacing },
    m_density{ fluidDensity }
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

void Solver::extrapolate()
{
    const size_t width = m_fluidGrid.getWidth();
    const size_t height = m_fluidGrid.getHeight();

    for (size_t i = 0; i < height; i++)
    {
        float u1 = m_fluidGrid.getU(i, 1);
        m_fluidGrid.setU(i, 0, u1);

        float u2 = m_fluidGrid.getU(i, width - 1);
        m_fluidGrid.setU(i, width - 2, u2);
    }

    for (size_t j = 0; j < width; j++)
    {
        float v1 = m_fluidGrid.getV(1, j);
        m_fluidGrid.setV(0, j, v1);

        float v2 = m_fluidGrid.getV(height - 2, j);
        m_fluidGrid.setV(height - 1, j, v2);
    }
}

void Solver::project(float dt, size_t numIters)
{
    float pressure = m_density * m_h / dt;

    // Solve grid
    for (size_t iter = 0; iter < numIters; iter++)
    {
        for (size_t i = 0; i < m_fluidGrid.getHeight(); i++)
        {
            for (size_t j = 0; j < m_fluidGrid.getWidth(); j++)
            {
                if (m_fluidGrid.getS(i, j) == 0)
                    continue;   // wall

                float sx0 = static_cast<float>(m_fluidGrid.getS(i - 1, j));
                float sx1 = static_cast<float>(m_fluidGrid.getS(i + 1, j));

                float sy0 = static_cast<float>(m_fluidGrid.getS(i, j - 1));
                float sy1 = static_cast<float>(m_fluidGrid.getS(i, j + 1));

                // Number of neighboring fluid cells
                float s = sx0 + sx1 + sy0 + sy1;
                if (s == 0.0f)
                    continue;   // surrounded by walls

                float div = calcDivergence(i, j);

                // p[i, j] <- p[i, j] + (d / s) * (rho * h) / dt
                float p = -div / s;
                p *= k_overrelaxation;
                m_fluidGrid.setP(i, j, m_fluidGrid.getP(i, j) + p * pressure);

                // u[i, j] <- u[i, j] + d * s[i - 1, j] / s
                float newU1 = m_fluidGrid.getU(i, j) - sx0 * p;

                // u[i + 1, j] <- u[i + 1, j] - d * s[i + 1, j] / s
                float newU2 = m_fluidGrid.getU(i + 1, j) + sx1 * p;

                // v[i, j] <- v[i, j] + d * s[i, j - 1] / s
                float newV1 = m_fluidGrid.getV(i, j) - sy0 * p;

                // v[i, j + 1] <- v[i, j + 1] - d * s[i, j + 1] / s
                float newV2 = m_fluidGrid.getV(i, j + 1) + sy1 * p;

                m_fluidGrid.setU(i, j, newU1);
                m_fluidGrid.setU(i + 1, j, newU2);

                m_fluidGrid.setV(i, j, newV1);
                m_fluidGrid.setV(i, j + 1, newV2);
            }
        }
    }
}

float Solver::getAverageU(size_t row, size_t col) const
{
    float sum = m_fluidGrid.getU(row, col - 1)
        + m_fluidGrid.getU(row, col)
        + m_fluidGrid.getU(row + 1, col - 1)
        + m_fluidGrid.getU(row + 1, col);

    return 0.25f * sum;
}

float Solver::getAverageV(size_t row, size_t col) const
{
    float sum = m_fluidGrid.getV(row - 1, col)
        + m_fluidGrid.getV(row, col)
        + m_fluidGrid.getV(row - 1, col + 1)
        + m_fluidGrid.getV(row, col + 1);

    return 0.25f * sum;
}

float Solver::sampleField(float& x, float& y, FieldType fieldType) const
{
    const size_t width = m_fluidGrid.getWidth();
    const size_t height = m_fluidGrid.getHeight();

    float h1 = 1.0f / m_h;
    float h2 = 0.5f * m_h;

    x = std::max(std::min(x, width * m_h), m_h);
    y = std::max(std::min(y, height * m_h), m_h);

    float dx = 0.0f;
    float dy = 0.0f;

    std::vector<float> grid{};
    switch (fieldType)
    {
    case FieldType::u_field:
        grid = m_fluidGrid.getUGrid();
        dy = h2;
        break;

    case FieldType::v_field:
        grid = m_fluidGrid.getVGrid();
        dx = h2;
        break;

    case FieldType::s_field:
        grid = m_fluidGrid.getMGrid();
        dx = h2;
        dy = h2;
        break;

    default:
        break;
    }

    float x0 = std::min(floorf((x - dx) * h1), static_cast<float>(height - 1));
    float tx = ((x - dx) - x0 * m_h) * h1;
    float x1 = std::min(x0 + 1, static_cast<float>(height - 1));

    float y0 = std::min(floorf((y - dy) * h1), static_cast<float>(width - 1));
    float ty = ((y - dy) - y0 * m_h) * h1;
    float y1 = std::min(y0 + 1, static_cast<float>(width - 1));

    float sx = 1.0f - tx;
    float sy = 1.0f - ty;

    float val = sx * sy * grid[x0 * width + y0]
        + tx * sy * grid[x1 * width + y0]
        + tx * ty * grid[x1 * width + y1]
        + sx * ty * grid[x0 * width + y1];

    return val;
}

void Solver::advectVel(float dt)
{
    m_fluidGrid.updateNextU();
    m_fluidGrid.updateNextV();

    float h = m_h;
    float h2 = 0.5f * m_h;

    for (size_t i = 0; i < m_fluidGrid.getHeight(); i++)
    {
        for (size_t j = 0; j < m_fluidGrid.getWidth(); j++)
        {
            if (m_fluidGrid.getS(i, j) != 0 && m_fluidGrid.getS(i - 1, j) != 0 && j < m_fluidGrid.getWidth() - 1)
            {
                float x = static_cast<float>(i) * h;
                float y = static_cast<float>(j) * h2;

                float u = m_fluidGrid.getU(i, j);
                float v = getAverageV(i, j);

                x -= dt * u;
                y -= dt * v;
                u = sampleField(x, y, FieldType::u_field);
                m_fluidGrid.setNextU(i, j, u);
            }
            if (m_fluidGrid.getS(i, j) != 0 && m_fluidGrid.getS(i, j - 1) != 0 && i < m_fluidGrid.getWidth() - 1)
            {
                float x = static_cast<float>(i) * h + h2;
                float y = static_cast<float>(j) * h;

                float u = getAverageU(i, j);
                float v = m_fluidGrid.getV(i, j);

                x -= dt * u;
                y -= dt * v;
                v = sampleField(x, y, FieldType::v_field);
                m_fluidGrid.setNextV(i, j, v);
            }
        }
    }

    m_fluidGrid.updateU();
    m_fluidGrid.updateV();
}

void Solver::advectSmoke(float dt)
{
    m_fluidGrid.updateNextM();

    float h = m_h;
    float h2 = 0.5f * h;

    for (size_t i = 1; i < m_fluidGrid.getHeight() - 1; i++)
    {
        for (size_t j = 1; j < m_fluidGrid.getWidth() - 1; j++)
        {
            if (m_fluidGrid.getS(i, j) != 0)
            {
                float u = 0.5f * (m_fluidGrid.getU(i, j) + m_fluidGrid.getU(i + 1, j));
                float v = 0.5f * (m_fluidGrid.getU(i, j) + m_fluidGrid.getU(i, j + 1));
                float x = i * h + h2 - dt * u;
                float y = j * h + h2 - dt * v;

                float m = sampleField(x, y, FieldType::s_field);
                m_fluidGrid.setNextM(i, j, m);
            }
        }
    }

    m_fluidGrid.updateM();
}