#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"
#include "AircraftFactory.hpp"
#include <cassert>
#include <cstdlib>
#include <ctime>
 #include <algorithm>

using namespace std::string_literals;

const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();
    GL::move_queue.emplace(&manager);

}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

/*void TowerSimulation::create_aircraft(const AircraftType& type)
{
    assert(airport); // make sure the airport is initialized before creating aircraft

    const std::string flight_number = factory.get_airlines()[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    manager.add(std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower() ) );
}*/
/*
void TowerSimulation::create_random_aircraft() 
{
    create_aircraft(*(aircraft_types[rand() % 3]));
}
*/
void TowerSimulation::create_keystrokes() 
{  
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { manager.add(factory.create_random_aircraft(airport->get_tower())); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('p', []() { GL::is_paused = !GL::is_paused; });
    GL::keystrokes.emplace('z', []() { GL::ticks_per_sec = std::max(GL::ticks_per_sec - 1u, 1u);});
    GL::keystrokes.emplace('a', []() { GL::ticks_per_sec = std::min(GL::ticks_per_sec + 1u, 180u);});
    GL::keystrokes.emplace('m', [this]() { manager.get_crashed_aircrafts();});
    GL::keystrokes.emplace('0', [this]() { manager.aircraft_from_airline(airlines[0]); });
    GL::keystrokes.emplace('1', [this]() { manager.aircraft_from_airline(airlines[1]); });
    GL::keystrokes.emplace('2', [this]() { manager.aircraft_from_airline(airlines[2]); });
    GL::keystrokes.emplace('3', [this]() { manager.aircraft_from_airline(airlines[3]); });
    GL::keystrokes.emplace('4', [this]() { manager.aircraft_from_airline(airlines[4]); });
    GL::keystrokes.emplace('5', [this]() { manager.aircraft_from_airline(airlines[5]); });
    GL::keystrokes.emplace('6', [this]() { manager.aircraft_from_airline(airlines[6]); });
    GL::keystrokes.emplace('7', [this]() { manager.aircraft_from_airline(airlines[7]); });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [k ,f] : GL::keystrokes)
    {
        std::cout << k << " \n";
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport{ one_lane_airport, manager, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    GL::Displayable::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    //init_aircraft_types();
    factory.init_aircraft_types();
    GL::loop();
}
