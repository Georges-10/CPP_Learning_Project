#pragma once

#include "GL/displayable.hpp"
#include "GL/dynamic_object.hpp"
#include "GL/texture.hpp"
#include "airport_type.hpp"
#include "geometry.hpp"
#include "img/image.hpp"
#include "runway.hpp"
#include "terminal.hpp"
#include "tower.hpp"
#include "AircraftManager.hpp"
#include <vector>

#include <algorithm>
class Airport : public GL::Displayable, public GL::DynamicObject
{
private:
    const AirportType& type;
    const Point3D pos;
    const GL::Texture2D texture;
    std::vector<Terminal> terminals;
    Tower tower;
    const AircraftManager& manager;
    float fuel_stock = 0.0;
    float ordered_fuel = 0.0;
    int next_refill_time = 0;

    // reserve a terminal
    // if a terminal is free, return
    // 1. a sequence of waypoints reaching the terminal from the runway-end and
    // 2. the number of the terminal (used for liberating the terminal later)
    // otherwise, return an empty waypoint-vector and any number
    std::pair<WaypointQueue, size_t> reserve_terminal(Aircraft& aircraft)
    {
        const auto it =
            std::find_if(terminals.begin(), terminals.end(), [](const Terminal& t) {
                 return !t.in_use(); 
                 });

        if (it != terminals.end())
        {
            it->assign_craft(aircraft);
            const auto term_idx = std::distance(terminals.begin(), it);
            return { type.air_to_terminal(pos, 0, term_idx), term_idx };
        }
        else
        {
            return { {}, 0u };
        }
    }

    WaypointQueue start_path(const size_t terminal_number)
    {
        return type.terminal_to_air(pos, 0, terminal_number);
    }

    Terminal& get_terminal(const size_t terminal_num) { return terminals.at(terminal_num); }

public:
    Airport(const AirportType& type_, const AircraftManager& manager_,const Point3D& pos_, const img::Image* image, const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals() },
        tower { *this },
        manager { manager_ }
    {}

    Tower& get_tower() { return tower; }

    void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }

    void move() override
    {
        if (next_refill_time == 0)/*si c'est le moment de la prochaine recharge*/
        {
            fuel_stock += ordered_fuel; /*on  recharge le stock de kerozen*/
            auto receive_fuel = ordered_fuel;
            ordered_fuel = std::min((float)5000.0, manager.get_required_fuel());/*on replis la citerne soit au max soit à la quantité nécessaire pour les avions <= 5000*/
            next_refill_time = 100;/*on remet la prochaine livraison à 100 mouvement*/

            std::cout << "Fuel stock: " << fuel_stock << "\nFuel received: " << receive_fuel << "\nFuel ordered: " << ordered_fuel << std::endl;
        }
        else
        {
            next_refill_time--;/*sinon on se rapproche de la prochaine livraison*/
        }
        for (auto& t : terminals)
        {
            t.refill_aircraft_if_needed(fuel_stock);
            t.move();

        }

    }

    friend class Tower;
};
