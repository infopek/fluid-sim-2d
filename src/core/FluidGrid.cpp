#include "core/FluidGrid.h"

#include <cstdint>

FluidGrid::FluidGrid(size_t width, size_t height)
	: m_width{ width }, m_height{ height },
	m_u((width + 2)* (height + 2), 0.0f),
	m_v((width + 2)* (height + 2), 0.0f),
	m_m((width + 2)* (height + 2), 1.0f),
	m_nextU((width + 2)* (height + 2), 0.0f),
	m_nextV((width + 2)* (height + 2), 0.0f),
	m_nextM((width + 2)* (height + 2), 0.0f),
	m_s((width + 2)* (height + 2), 1),
	m_p((width + 2)* (height + 2), 0.0f)
{
	m_width += 2;
	m_height += 2;
}

void FluidGrid::updateU()
{
	m_u = m_nextU;
}

void FluidGrid::updateV()
{
	m_v = m_nextV;
}

void FluidGrid::updateNextU()
{
	m_nextU = m_u;
}

void FluidGrid::updateNextV()
{
	m_nextV = m_v;
}

void FluidGrid::updateM()
{
	m_m = m_nextM;
}

void FluidGrid::updateNextM()
{
	m_nextM = m_m;
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
	if (row >= m_height || col >= m_width)
		return; // wall doesn't change

	m_v[row * m_width + col] = newV;
}

float FluidGrid::getNextU(size_t row, size_t col) const
{
	return row < m_height && col < m_width
		? m_nextU[row * m_width + col]
		: 0.0f;
}

void FluidGrid::setNextU(size_t row, size_t col, float newU)
{
	if (row >= m_height || col >= m_width)
		return; // wall doesn't change

	m_nextU[row * m_width + col] = newU;
}

float FluidGrid::getNextV(size_t row, size_t col) const
{
	return row < m_height && col < m_width
		? m_nextV[row * m_width + col]
		: 0.0f;
}

void FluidGrid::setNextV(size_t row, size_t col, float newV)
{
	if (row >= m_height || col >= m_width)
		return; // wall doesn't change

	m_nextV[row * m_width + col] = newV;
}

float FluidGrid::getNextM(size_t row, size_t col) const
{
	return row < m_height && col < m_width
		? m_nextM[row * m_width + col]
		: 0.0f;
}

void FluidGrid::setNextM(size_t row, size_t col, float newM)
{
	if (row >= m_height || col >= m_width)
		return; // wall doesn't change

	m_nextM[row * m_width + col] = newM;
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

float FluidGrid::getM(size_t row, size_t col) const
{
	return row < m_height && col < m_width
		? m_m[row * m_width + col]
		: 0.0f;
}

void FluidGrid::setM(size_t row, size_t col, float newM)
{
	if (row >= m_height || col >= m_width)
		return;

	m_m[row * m_width + col] = newM;
}
