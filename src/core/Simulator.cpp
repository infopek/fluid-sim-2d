#include "core/Simulator.h"

Simulator::Simulator(const Solver& solver)
    : m_solver{ solver }
{
}

void Simulator::simulate(float dt, float gravity, int iters)
{
    m_solver.updateVelocities(dt, gravity);

    m_solver.project(dt, iters);

    m_solver.extrapolate();
    m_solver.advectVel(dt);
    m_solver.advectSmoke(dt);
}