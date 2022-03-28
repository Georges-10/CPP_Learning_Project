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
   

void AircraftManager::aircraft_from_airline(std::string_view airlines){
    const auto& nb_aircrafts = std::count_if(aircrafts.begin(), aircrafts.end(),
                                                [airlines](std::unique_ptr<Aircraft>& aircraft) {
                                                     return aircraft->get_flight_num().compare(0, 2, airlines, 0, 2) == 0;
                                                }
                                            );
    std::cout << "Il y a " << nb_aircrafts << " avions pour le terminal " << airlines << std::endl;
}

