#include "tower_sim.hpp"
#include "geometry.hpp"

int main(int argc, char** argv)
{    
    
    test_generic_points();
    TowerSimulation simulation { argc, argv };
    simulation.launch();
    return 0;
}