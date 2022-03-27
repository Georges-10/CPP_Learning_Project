#include "AircraftFactory.hpp"


std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(const AircraftType& type, Tower& tower)const
{   

    std::string tst ="";
    do{
      tst  = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));

    }while( !created_aircraft.empty()   && *(std::find(created_aircraft.begin(), created_aircraft.end(), tst)) == tst);
    
    const std::string flight_number = tst;

    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    return std::make_unique<Aircraft>(type, flight_number, start, direction, tower);
}

std::unique_ptr<Aircraft> AircraftFactory::create_random_aircraft(Tower& tower) 
{

    std::unique_ptr<Aircraft> air= create_aircraft(*(aircraft_types[rand() % 3]), tower);
    created_aircraft.emplace_back(air->get_flight_num());

    return air;
}

const std::string AircraftFactory::get_airlines(int index)
{
    return airlines[index];
} 