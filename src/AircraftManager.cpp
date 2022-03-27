#include "AircraftManager.hpp"
#include <utility>
#include <algorithm>


void AircraftManager::move(){

    for (auto& aircraft : aircrafts){
            aircraft->move();
           
    }

    auto end = std::remove_if(aircrafts.begin(),
                            aircrafts.end(),
                            [](std::unique_ptr<Aircraft> const &air) {
                                return air->is_lift();    // remove odd numbers
                            });
 
   aircrafts.erase(end, aircrafts.end());

    
}


void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
   
    aircrafts.emplace_back(std::move(aircraft));
}
   
