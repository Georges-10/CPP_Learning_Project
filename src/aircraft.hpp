#pragma once

#include "GL/displayable.hpp"
#include "aircraft_types.hpp"
#include "config.hpp"
#include "geometry.hpp"
#include "tower.hpp"
#include "waypoint.hpp"
#include <string>
#include <string_view>

class Aircraft : public GL::Displayable, public GL::DynamicObject
{
private:
    const AircraftType& type;
    const std::string flight_number;
    Point3D pos, speed; // note: the speed should always be normalized to length 'speed'
    WaypointQueue waypoints = {};
    Tower& control;
    bool landing_gear_deployed = false; // is the landing gear deployed?
    bool is_at_terminal = false;
    bool is_lift_off = false;
    bool is_service_done  = false;
    bool cirl = false;
   float fuel  = 0.0;
   bool crashed = false;
    // turn the aircraft to arrive at the next waypoint
    // try to facilitate reaching the waypoint after the next by facing the
    // right way to this end, we try to face the point Z on the line spanned by
    // the next two waypoints such that Z's distance to the next waypoint is
    // half our distance so: |w1 - pos| = d and [w1 - w2].normalize() = W and Z
    // = w1 + W*d/2
    void turn_to_waypoint();
    void turn(Point3D direction);

    // select the correct tile in the plane texture (series of 8 sprites facing
    // [North, NW, W, SW, S, SE, E, NE]) 
    unsigned int get_speed_octant() const;
    // when we arrive at a terminal, signal the tower
    void arrive_at_terminal();
    // deploy and retract landing gear depending on next waypoints
    void operate_landing_gear();
    //void add_waypoint(const Waypoint& wp, const bool front);
    float max_speed() const { return is_on_ground() ? type.max_ground_speed : type.max_air_speed; }

    Aircraft(const Aircraft&) = delete;
    Aircraft& operator=(const Aircraft&) = delete;


    template <bool front>
    void add_waypoint(const Waypoint& wp)
    {
        if constexpr (front)
        {
            waypoints.push_front(wp);
        }
        else
        {
            waypoints.push_back(wp);
        }
    }

public:
    Aircraft(const AircraftType& type_, const std::string_view& flight_number_, const Point3D& pos_,
             const Point3D& speed_, Tower& control_, float fuel_) :
        GL::Displayable { pos_.x() + pos_.y() },
        type { type_ },
        flight_number { flight_number_ },
        pos { pos_ },
        speed { speed_ },
        control { control_ },
        fuel { fuel_}
    {
        speed.cap_length(max_speed());
    }

    const std::string& get_flight_num() const { return flight_number; }
    float distance_to(const Point3D& p) const { return pos.distance_to(p); }
    bool is_on_ground() const { return pos.z() < DISTANCE_THRESHOLD; }
    
    void display() const override;
    void move() override;
    bool is_lift() const{return is_service_done && waypoints.empty();}
    void set_lift_off();
    void consume_fuel(){
        fuel-=1.0;
    }
    bool is_low_on_fuel()const { return fuel < 200; }
    bool is_crashed()const{ return crashed;}
    float get_fuel()const{return fuel;}
    bool at_terminal(){return is_at_terminal;}

    void refill(float& fuel_stock);
    bool has_terminal() const;
      
    bool is_circling() const;
    friend class Tower;
};
