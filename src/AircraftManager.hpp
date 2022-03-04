#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"
#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    int crashed_aircrafts = 0;    

public:
    void add(std::unique_ptr<Aircraft> aircraft);
    void aircraft_from_airline(const std::string_view airlines);
    float get_required_fuel() const;
    unsigned int  get_crashed_aircrafts() const { return crashed_aircrafts;};
    bool update() override;
};