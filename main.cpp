#include <iostream>
#include <SFML/Window.hpp>

#include "renderer.h"
#include "closedworld.h"
#include "weather.h"
#include "gpuweather.h"
#include "comprgassim.h"
#include "shallowwater.h"

//#define INCOMP_FLUID
#define SHALLOW_WATER

using namespace std;

sf::Window window;
Renderer renderer;

int main()
{
    int res_x = 1024;
    int res_y = res_x;

    unsigned int fluid_sim_size = 512;

    ClosedWorld world((int)(log2(fluid_sim_size)));
    Weather weather(world.m_height_map, 128);

    for (int i = 0; i<10; i++)
    {
        weather.step();
        std::cout << "time-step number... " << i << "\n";
    }

    // init
    window.create(sf::VideoMode(res_x, res_y), "my test window");
    window.setVerticalSyncEnabled(true);

    renderer.init(res_x, res_y);

    // load the layers
    //renderer.loadLayer(0, res_x, world.getHeightMap()->getDataPtr());

    float my_max = -9999;
    float my_min = 9999;
    for (int i = 0; i<fluid_sim_size; i++)
    {
        for (int j = 0; j<fluid_sim_size; j++)
        {
            float height = world.m_height_map(i,j);
            if (height > my_max) my_max = height;
            if (height < my_min) my_min = height;
        }
    }
    std::cout << "max height " << my_max << "\n";
    std::cout << "min height " << my_min << "\n";


//    renderer.getLayer(0)->setMax(5000.f);
//    renderer.getLayer(0)->setMin(-5000.f);
//    renderer.getLayer(0)->setCutAbove(10000.0);
//    renderer.getLayer(0)->setCutBelow(0.0);
////
////    float color0[3] = {0.2, 0.7, 0.3};
//    float color0[3] = {0.7, 0.7, 0.4};
//    renderer.getLayer(0)->setColorTransf(color0);
//
//    renderer.loadLayer(1, 128, weather.getConc()->getDataPtr());
//
//    renderer.getLayer(1)->setMax(2.0);
//    renderer.getLayer(1)->setMin(0.0);
//    renderer.getLayer(1)->setCutAbove(10.0);
//    renderer.getLayer(1)->setCutBelow(0.1);
//
//    float color1[3] = {0.7, 0.0, 0.0};
////    float color1[3] = {1, 0, 1};
//    renderer.getLayer(1)->setColorTransf(color1);
//    renderer.getLayer(1)->setAlpha(0.6);
//
//    float* radiationData = world.getRadiationData(0.7);
//    renderer.loadLayer(2, 1024, radiationData);
//    delete [] radiationData;
//
//    renderer.getLayer(2)->setMax(1.0);
//    renderer.getLayer(2)->setMin(0.0);
//    renderer.getLayer(2)->setCutAbove(10.0);
//    renderer.getLayer(2)->setCutBelow(0.1);
//
////    float color1[3] = {0.7, 0.0, 0.0};
//    float color2[3] = {1, 1, 1};
//    renderer.getLayer(2)->setColorTransf(color2);
//    renderer.getLayer(2)->setAlpha(0.6);

//    renderer.loadLayer(1, weather.m_params.S_vx.getDataPtr());

#ifdef INCOMP_FLUID
    // GPU weather
    gpuWeather gpu_weather(fluid_sim_size, world.getHeightData(), res_x, res_y);
#endif // INCOMP_FLUID

    // GPU compressible gas
    //ComprGasSim compr_gas_sim(fluid_sim_size, world.getHeightData(), res_x, res_y);
#ifdef SHALLOW_WATER
    // shallow water
    ShallowWater shallow_water(fluid_sim_size, world.getHeightData(), res_x, res_y);
#endif

// Slow event based input
    bool running = true;

    sf::Clock clock;
    float dt = 0.0;

    while (running==true)
    {
        clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            case sf::Event::Resized:
                // adjust the viewport when the window is resized
    //            renderer.resizeWindow(event.size.width, event.size.height);
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                    running = false;
                    break;
                case sf::Keyboard::R:
                    // do something
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        } // finished processing events

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // left click...
            // get global mouse position
            sf::Vector2i position = sf::Mouse::getPosition(window);

            float rel_pos_x = (float)(position.x)/(float)(res_x);
            float rel_pos_y = (float)(position.y)/(float)(res_y);

#ifdef SHALLOW_WATER
            shallow_water.stirWater(rel_pos_x, rel_pos_y);
#endif
        }

//        for (int i = 0; i<1; i++)
//        {
//            weather.step();
//            std::cout << "time-step number... " << i << "\n";
//        }
//
//        renderer.updateLayer(1, weather.getConc()->getDataPtr());



        // draw
        //renderer.draw();
#ifdef INCOMP_FLUID
        for (int i = 0; i<10; i++)
            gpu_weather.step(0.01666);
#endif
        //compr_gas_sim.step(0.01666);
#ifdef SHALLOW_WATER
        for (int i = 0; i<10; i++)
            shallow_water.step(0.01666);
#endif
        window.display();

        dt = clock.getElapsedTime().asSeconds();
        //std::cout << "dt = " << dt << "\n";

        //std::cin.get();
    }

    return 0;
}
