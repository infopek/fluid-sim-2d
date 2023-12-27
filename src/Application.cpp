#include <SFML/Graphics.hpp>
#include "core/FluidGrid.h"

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

    FluidGrid fluidGrid(width, height);

    while (true)
    {
        // Modify velocity values (e.g. add gravity)
            // Make the fluid incompressible (projection)
            // Move the velocity filed (advection)
    }

    return 0;
}