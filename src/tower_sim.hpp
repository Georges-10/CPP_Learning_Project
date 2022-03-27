#pragma once
#include "AircraftManager.hpp"
#include "AircraftFactory.hpp"
class Airport;
struct AircraftType;

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    AircraftManager manager;
    AircraftFactory factory;
    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

   // void create_aircraft(const AircraftType& type);
    
  //  void create_random_aircraft() ;

    void create_keystrokes() ;
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
