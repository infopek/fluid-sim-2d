#pragma once

#include "core/Solver.h"

class Simulator
{
public:
    Simulator(const Solver& solver);

    void simulate(float dt, float gravity, int iters);

private:
    Solver m_solver;
};