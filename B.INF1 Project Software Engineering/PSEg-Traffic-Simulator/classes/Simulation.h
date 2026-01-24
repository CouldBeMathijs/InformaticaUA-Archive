//============================================================================
// Name        : Simulation.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class that represents the simulation, including roads and time progression.
//============================================================================

#ifndef SIMULATION_H
#define SIMULATION_H

#include "Road.h"
#include <string>
#include <vector>

/**
 * @brief The top level class, manages roads and time progression
 */
class Simulation {
private:
    Simulation* _initCheck;

    double timeEachTick;
    std::vector<Road*> roads;
    std::vector<Intersection*> intersections;
    double time;

public:

    /**
     * @brief Creates an empty simulation with no roads and a default tick interval.
     * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Simulation' must end in a correct initialized state.");
     */
    Simulation();

    /**
     * @brief Checks whether this Simulation object is properly initialized.
     * @return true if the simulation is properly initialized, false otherwise.
     */
    [[nodiscard]] bool properlyInitialized() const; // Should not be marked no-discard else design-by-contract can't be used

    /**
     * @brief Destructor: cleans up dynamically allocated roads.
     */
    ~Simulation();

    /**
     * @brief Runs the simulation for a number of seconds.
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * \n REQUIRE(seconds >= 0, "Error: seconds cannot be negative.");
     * \n ENSURE(time >= \old(time), "Error: Calculated value 'time' did not increase");
     * @param seconds The number of seconds to run the simulation.
     */
    void runSec(double seconds);

    /**
     * @brief Runs the simulation for a number of ticks.
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * \n ENSURE(time >= \old(time), "Error: Calculated value 'time' did not increase");
     * @param steps The number of ticks to simulate.
     */
    void runTicks(unsigned int steps);

    /**
     * @brief Returns all roads in this simulation.
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * @return A const reference to a vector of Road* objects.
     */
    [[nodiscard]] const std::vector<Road*> &getRoads() const;

    /**
     * @brief Returns the time duration of one tick (in seconds).
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * @return The duration of a tick.
     */
    [[nodiscard]] double getTimePerTick() const;

    /**
     * @brief Returns the current simulation time.
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * @return The current time in the simulation.
     */
    [[nodiscard]] double getTime() const;

    std::vector<Intersection*> &getIntersections();

    /**
     * @brief Finds a road by its name.
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * @param name The name of the road to search for.
     * @return A pointer to the requested Road, or nullptr if not found.
     */
    [[nodiscard]] Road *findRoadWithName(const std::string &name) const;

    /**
     * @brief Adds a road to the simulation.
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * \n REQUIRE(road->properlyInitialized(), "Error: Road's constructor did not initialize properly");
     * \n ENSURE(std::find(getRoads().begin(), getRoads().end(), road) != getRoads().end(), "Error: road was not updated correctly");
     * @param road A valid pointer to a Road.
     */
    void addRoad(Road* road);

    /**
     * /brief Adds an intersection to the simulation
     * \n REQUIRE(this->properlyInitialized(), "Error: Simulation's constructor did not initialize properly");
     * \n REQUIRE(road->properlyInitialized(), "Error: Road's constructor did not initialize properly");
     * @param intersection The intersection to add to the simulation
     */
    void addIntersection(Intersection* intersection);
};

#endif //SIMULATION_H
