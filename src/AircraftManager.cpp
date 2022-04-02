#include "AircraftManager.hpp"
#include <utility>
#include <algorithm>


void AircraftManager::move(){
    
    
    std::sort(aircrafts.begin(), aircrafts.end(), [](std::unique_ptr<Aircraft>& aircraft1, std::unique_ptr<Aircraft>& aircraft2) 
    { 
        if(aircraft1->has_terminal() == aircraft2->has_terminal())
        {
            return aircraft1->get_fuel() <= aircraft2->get_fuel(); 
        }
        return aircraft1->has_terminal();
    });


    for (auto& aircraft : aircrafts){
          std::cout << "Aircraft " << aircraft->get_flight_num()<<"  " << (aircraft->has_terminal()?"reserved":"Notreserved") <<"fuel: " << aircraft->get_fuel()  <<std::endl;
        try{
           aircraft->move();
        }catch(const AircraftCrash& err){
             std::cout << err.what() << std::endl;
                crashed_aircrafts ++;
        }
    }
   std::cout << "\n------------------------------------------------------------------------\n" <<std::endl;

    auto end = std::remove_if(aircrafts.begin(),
                            aircrafts.end(),
                            [](std::unique_ptr<Aircraft> const &air) {
                                if(air->is_crashed()){
                                    return true;
                                }
                                return air->is_lift() ;
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

float AircraftManager::get_required_fuel() const
{

    return accumulate(aircrafts.begin(), aircrafts.end(), 0.0, [] (float sum, const std::unique_ptr<Aircraft>& a1) {
        if(a1->is_low_on_fuel() && a1->at_terminal())
        {
            std::cout << "Sum : " << sum << " Fuel " << a1->get_fuel() << std::endl; 
            return sum + (3000 - a1->get_fuel());
        }
        return sum;
    });
}
