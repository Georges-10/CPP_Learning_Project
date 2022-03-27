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
  
    void move() override;
                 
   void add(std::unique_ptr<Aircraft> aircraft);                          
};