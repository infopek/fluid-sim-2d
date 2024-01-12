#include <SFML/Graphics.hpp>
#include "core/FluidGrid.h"
#include "core/Solver.h"
#include "core/Simulator.h"

int main()
{
	// sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	// sf::CircleShape shape(100.f);
	// shape.setFillColor(sf::Color::Green);

	// while (window.isOpen())
	// {
	//     sf::Event event;
	//     while (window.pollEvent(event))
	//     {
	//         if (event.type == sf::Event::Closed)
	//             window.close();
	//     }

	//     window.clear();
	//     window.draw(shape);
	//     window.display();
	// }

	size_t width = 450;
	size_t height = 300;

	float gridSpacing = 1.0f / 100;
	float density = 1000.0f;
	float gravity = -9.81f;
	float dt = 1.0f / 60.0f;
	int iters = 40;

	FluidGrid fluidGrid(width, height);
	Solver solver(fluidGrid, gridSpacing, density);
	Simulator simulator(solver);

	simulator.simulate(dt, gravity, iters);

	return 0;
}