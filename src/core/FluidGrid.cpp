#include "core/FluidGrid.h"

FluidGrid::FluidGrid(size_t width, size_t height)
    : m_width{ width }, m_height{ height },
    m_u(width* height, 0.0f),
    m_v(width* height, 0.0f),
    m_s(width* height, 1),
    m_p(width* height, 0.0f)
{
}

float FluidGrid::getU(size_t row, size_t col) const
{
    return row < m_height && col < m_width
        ? m_u[row * m_width + col]
        : 0.0f;
}

void FluidGrid::setU(size_t row, size_t col, float newU)
{
    if (row >= m_height || col >= m_width)
        return; // wall doesn't change

    m_u[row * m_width + col] = newU;
}

float FluidGrid::getV(size_t row, size_t col) const
{
    return row < m_height && col < m_width
        ? m_v[row * m_width + col]
        : 0.0f;
}

void FluidGrid::setV(size_t row, size_t col, float newV)
{
}

uint8_t FluidGrid::getS(size_t row, size_t col) const
{
    return row < m_height && col < m_width
        ? m_s[row * m_width + col]
        : 0;
}

void FluidGrid::setS(size_t row, size_t col, uint8_t newS)
{
    if (row >= m_height || col >= m_width)
        return;

    m_s[row * m_width + col] = newS;
}
