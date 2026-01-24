//============================================================================
// Name        : Vehicle.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : This file represents different vehicles, with position,
//               speed, and acceleration.
//============================================================================

#ifndef VEHICLE_H
#define VEHICLE_H

#include "Stop.h"
#include <string>

class Intersection;
class Road;
class TrafficLight;

struct RoadIntersectionBinding;

/**
 * @brief The base class for all vehicles
 */
class Vehicle {
protected:
    Vehicle* _initCheck;

    Road* road;
    double l;
    double x;
    double v;
    double vmax;
    double Vmax;
    double a;
    double amax;
    double bmax;
    double minFollowingDistance;
    double decelerationDistance;
    double stoppingDistance;
    double decelerationFactor;

    bool decidedToStopForLight = false;

    Road* plannedNextRoad = nullptr;
    const RoadIntersectionBinding* closestIntersectionBinding = nullptr;

protected:
    std::string texture;

public:

    /**
     * @brief Creates a new vehicle with all parameters.
     * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
     * \n REQUIRE(position <= road->getLength(), "Error: 'position' cannot be out of bounds");
     * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Vehicle' must end in a correct initialized state.");
     * @param road        Pointer to the road this vehicle is on (not null).
     * @param position    Starting position of the vehicle on the road (≤ road->getLength()).
     * @param speed       Current speed of the vehicle.
     * @param length      The length of the vehicle.
     * @param maxSpeed    (Temporary) maximum speed of the vehicle.
     * @param maxAcceleration Max acceleration.
     * @param maxBreakFactor Max break factor.
     * @param minFollowDistance The minimum follow distance of the vehicle.
     */
    Vehicle(Road* road, unsigned int position, double speed, unsigned int length, double maxSpeed, double maxAcceleration, double maxBreakFactor, double minFollowDistance);

    /**
     * @brief Checks whether this Vehicle object is properly initialized (Design by Contract).
     * @return true if properly initialized, false otherwise.
     */
    [[nodiscard]] bool properlyInitialized() const;

    virtual ~Vehicle() = default;

    /**
     * @brief Updates the vehicle's speed and position based on acceleration and elapsed time.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n REQUIRE(dt >= 0, "Error: dt cannot be negative.");
     * \n ENSURE(x_old <= getPosition(), "Error: Calculated value 'position' did not increase");
     * @param dt Time elapsed in this simulation step (in seconds or unit).
     */
    void updateSpeedPosition(double dt);

    /**
     *
     * @return the front vehicles
     */
    [[nodiscard]] std::pair<const Vehicle *, double> findFrontVehicle() const;

    /**
     * @brief Calculates acceleration, considering the vehicle in front (if any).
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     */
    void calculateAcceleration();

    std::pair<const TrafficLight *, double> findRelevantTrafficLight() const;

    void resetVmax();

    /**
     * @brief Brakes the vehicle if it's approaching a traffic light too closely, based on stopping and deceleration distance.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(x_old <= getPosition(), "Error: Calculated value 'position' did not increase");
     * @param
     */
    void brakeBeforeLight();

    [[nodiscard]] std::pair<BusStop *, double> findRelevantBusStop() const;

    /**
     * @brief Stops the vehicle if needed (e.g. if vmax == 0).
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     */
    void stopIfNeeded();

    /**
     * @brief Returns the road this vehicle is on.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(result != nullptr, "Error: road was not updated correctly");
     * @return Pointer to the Road object.
     */
    [[nodiscard]] Road * getRoad() const;

    /**
     * @brief Returns the current position of the vehicle.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(result >= 0, "Error: position was not updated correctly");
     * @return The current position on the road.
     */
    [[nodiscard]] double getPosition() const;

    /**
     * @brief Returns the current speed of the vehicle.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * @return The current speed.
     */
    [[nodiscard]] double getSpeed() const;

    /**
     * @brief Returns the length of this vehicle.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(result > 0, "Error: position was not updated correctly");
     * @return The vehicle length (e.g. 4 meters).
     */
    [[nodiscard]] double getLength() const;

    /**
     * @brief Returns the road this vehicle will take at the next intersection
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
     * not initialize properly");
     * @return The next planned road
     */
    [[nodiscard]] Road* getPlannedNextRoad() const;

    /**
     * @brief Returns the next intersection binding on the road this vehicle is
     * on
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
     * not initialize properly");
     * @return The next intersection binding on this road
     */
    [[nodiscard]] const RoadIntersectionBinding *
    getClosestIntersectionBinding() const;

    /**
     * @brief Way to set the closest intersection binding
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
     * not initialize properly");
     * \n ENSURE(this->getClosestIntersectionBinding() == intersectionBinding,
     * "Error: intersectionBinding was not updated successfully")
     * @param intersectionBinding
     */
    void setClosestIntersectionBinding(const RoadIntersectionBinding* intersectionBinding);

    /**
     * @brief Sets the position of this vehicle.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(this->getPosition() == position, "Error: position was not updated successfully")
     * @param position The new position of the vehicle.
     */
    void setPosition(double position);

    /**
     * @brief Sets the speed of this vehicle.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(this->getSpeed() == speed, "Error: speed was not updated successfully")
     * @param speed The new speed.
     */
    void setSpeed(double speed);

    /**
     * @brief Sets the road of this vehicle.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
     * not initialize properly");
     * @param road The road this car is on
     */
    void setRoad(Road* road);

    /**
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
     * not initialize properly");
     * \n ENSURE(this->getPlannedNextRoad() == plannedRoad, "Error: plannedRoad
     * was not updated successfully")
     * @param plannedRoad
     */
    void setPlannedNextRoad(Road* plannedRoad);

    /**
     * @brief Returns the texture path of the vehicle's sprite.
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did not initialize properly");
     * \n ENSURE(!result.empty(), "Error: texture was not updated correctly");
     * @return A string path to the texture file used for rendering the vehicle.
     */
    std::string getTexture();

    /**
     * Getter for the deceleration distance
     * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
     * not initialize properly");
     * @return the deceleration distance
     */
    [[nodiscard]] double getDecelerationDistance() const;
};

/**
 * @brief Represents a car on the road
 */
class Car : public Vehicle {
public:
    /**
    * @brief Constructs a car on a road
    * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
    * \n REQUIRE(this->properlyInitialized(), "Error: Vehicle's constructor did
    * not initialize properly"); * \n ENSURE(!texture.empty(), "Error: texture was not updated correctly");
    * @param road Pointer to the road the car is on.
    * @param position Starting position on the road.
    */
    Car(Road* road, unsigned int position);
};

/**
 * @brief Represents a bus on the road
 */
class Bus : public Vehicle {
private:
    BusStop* lastStop;
public:
    /**
    * @brief Represents a bus on the road.
    * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
    * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Bus' must end in a correct initialized state.");
    * \n ENSURE(!texture.empty(), "Error: texture was not updated correctly");
    * @param road Pointer to the road the bus is on.
    * @param position Starting position on the road.
    */
    Bus(Road* road, unsigned int position);

  /**
   * @brief brakes before the relevant bus stops
   * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of 'Bus' was
   * not properly initialized.");
   * \n REQUIRE(currentTime > 0, "Error: currentTime must be positive")
   * \n ENSURE(previousPosition <= getPosition(), "Error: the position was not
   * updated correctly");
   * @param currentTime The current time of the simulation
   */
    void brakeBeforeBusStop(double currentTime);

    /**
     * @brief sets the last busstop this bus stopped at
     * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of 'Bus' was
     * not properly initialized.");
     * \n REQUIRE(busstop == nullptr || busstop->properlyInitialized(), Error:
     * Constructor of 'BusStop' was not properly initialized.");
     * \n ENSURE(getLastStop() == busstop, "Error: busstop was not updated
     * correctly")
     * @param busstop the last busstop this bus stopped at
     */
    void setLastStop(BusStop *busstop);

    /**
     * @brief getter for the last busstop
     * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of 'Bus' was
     * not properly initialized.");
     * @return the last busstop
     */
    [[nodiscard]] BusStop *getLastStop() const;
};

/**
 * @brief Represents a fire truck on a road
 */
class FireTruck : public Vehicle {
public:
    /**
    * @brief Represents a fire truck on the road.
    * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
    * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'FireTruck' must end in a correct initialized state.");
    * \n ENSURE(!texture.empty(), "Error: texture was not updated correctly");
    * @param road Pointer to the road the fire truck is on.
    * @param position Starting position on the road.
    */
    FireTruck(Road* road, unsigned int position);
};

/**
 * @brief Represents an ambulance on the road
 */
class Ambulance : public Vehicle {
public:
    /**
    * @brief Represents an ambulance on the road.
    * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
    * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Ambulance' must end in a correct initialized state.");
    * \n ENSURE(!texture.empty(), "Error: texture was not updated correctly");
    * @param road Pointer to the road the ambulance is on.
    * @param position Starting position on the road.
    */
    Ambulance(Road* road, unsigned int position);
};

/**
 * @brief Represents a police car on the road
 */
class Police : public Vehicle {
public:
    /**
    * @brief Represents a police car on the road.
    * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
    * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Police' must end in a correct initialized state.");
    * \n ENSURE(!texture.empty(), "Error: texture was not updated correctly");
    * @param road Pointer to the road the police car is on.
    * @param position Starting position on the road.
    */
    Police(Road* road, unsigned int position);
};
#endif //VEHICLE_H
