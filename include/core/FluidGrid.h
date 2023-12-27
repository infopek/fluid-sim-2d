#pragma once

#include <vector>

/**
 * Represents an Eulerian grid for fluid simulation.
 * This implementation uses a staggering grid.
*/
class FluidGrid
{
public:
    FluidGrid(size_t width, size_t height);

    size_t getWidth() const { return m_width; }
    size_t getHeight() const { return m_height; }

    /// @return The horizontal component of the velocity vector at `[row, col]`
    /// if `row` and `col` are within bounds, `0` otherwise (wall).
    float getU(size_t row, size_t col) const;

    /// Sets the horizontal component of the velocity vector at `[row, col]`
    /// if `row` and `col` are within bounds.
    void setU(size_t row, size_t col, float newU);

    /// @return The vertical component of the velocity vector at `[row, col]`
    /// if `row` and `col` are within bounds, `0` otherwise (wall).
    float getV(size_t row, size_t col) const;

    /// Sets the vertical component of the velocity vector at `[row, col]`
    /// if `row` and `col` are within bounds.
    void setV(size_t row, size_t col, float newV);

    /// @return `1` if the cell at `[row, col]` is a fluid cell, `0` otherwise (wall).
    /// Out of bounds access is treated as wall.
    uint8_t getS(size_t row, size_t col) const;

    /// Set the cell state at `[row, col]` to fluid cell (`1`) or wall (`0`).
    void setS(size_t row, size_t col, uint8_t newS);

    float getP(size_t row, size_t col) const { return m_p[row * m_width + col]; }

    void setP(size_t row, size_t col, float newP) { m_p[row * m_width + col] = newP; }

private:
    std::vector<float> m_u{};   // horizontal components of velocity vectors
    std::vector<float> m_v{};   // vertical components of velocity vectors

    std::vector<uint8_t> m_s{};   // 0 if wall, 1 if fluid cell

    std::vector<float> m_p{};   // pressure field

    size_t m_width{};
    size_t m_height{};
};