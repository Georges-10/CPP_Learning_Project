#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"

#include <cmath>
#include <cassert>

void Aircraft::turn_to_waypoint()
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
   /* assert(direction!=nullptr && "haaaa direction is null");*/

    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal= true;

}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}


void Aircraft::move()
{   
    if (is_lift()){
        return ;
    }

      if (waypoints.empty())
    {
        const bool front = false;
        for(const auto& wp: control.get_instructions(*this)){
         add_waypoint<front>(wp);
        }
        
    }

    if (!is_at_terminal)
    {
        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed;

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
            }
            else
            {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }
       
       
        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {                
                crashed=true;
                using namespace std::string_literals;
                throw AircraftCrash { flight_number + " crashed into the ground"s };

            }
        }
        else
        {
           if(is_circling()){

                WaypointQueue wp = control.reserve_terminal(*this);
                if(!wp.empty()){
                     waypoints.clear();
                    std::copy(wp.begin(), wp.end(),std::back_inserter(waypoints));
                
                    std::cout << flight_number << " j'ai réservé!!! haha : "<< waypoints.back().type <<std::endl;
                    cirl = false;
                }
            }
            const float speed_len = speed.length();
            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
            if (fuel <= 0.0){
                crashed=true;
                throw AircraftCrash { flight_number + " crashed into the ground because he has used up all his fuel."};
             }
            fuel -= 1.0;
        
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
        

        
    }
}





void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}



bool Aircraft::has_terminal() const
{
    if (waypoints.empty())
    {
        return false;
    }
    

    return waypoints.back().type == WaypointType::wp_terminal;
}


bool Aircraft::is_circling() const
{
    if (!has_terminal() && !is_at_terminal && !is_on_ground() && !is_service_done)
    {
        return true;
    }
    return false;
}


void Aircraft::refill(float& fuel_stock)
{ 
    assert(fuel_stock>=0 && "haaaaa fuel_stock is negative");
    
    float needed_fuel = 3000.0 - fuel;
    float refill_fuel = needed_fuel;
    if(fuel_stock >= needed_fuel)/*si l'odffre de fuel est suffisante*/
    {
        fuel = 3000; /*on met le plein*/
        fuel_stock -= needed_fuel; /*on retir du stock le fuel donné*/
    }
    else
    {
        refill_fuel = fuel_stock;
        fuel += fuel_stock;/*sinon on donne le fuel restant à l'avion */
        fuel_stock = 0;/*du coup le stock est vide*/
    }
    if(refill_fuel > 0){
        std::cout << flight_number << " receive " << refill_fuel << " fuel." << std::endl;
    }
}