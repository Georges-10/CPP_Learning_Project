#pragma once

#include <string>
#include "aircraft_types.hpp"
#include "aircraft.hpp"
class AircraftFactory {
    private:
       const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
       std::vector<AircraftType*> aircraft_types ;   
       std::vector<std::string> created_aircraft;

    public:

    void init_aircraft_types(){
      aircraft_types.emplace_back(new AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" }});
      aircraft_types.emplace_back(new AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" }});
      aircraft_types.emplace_back(new AircraftType { .02f, .08f, .03f, MediaPath { "concorde_af.png" }});
    }
    std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type, Tower& tower)const;
    std::unique_ptr<Aircraft> create_random_aircraft(Tower& tower) ;
    const std::string   get_airlines(int index);
};