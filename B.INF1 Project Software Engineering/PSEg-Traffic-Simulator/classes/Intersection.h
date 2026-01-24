//============================================================================
// Name        : Intersection.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class that represents an intersection on a road
//============================================================================

#ifndef PSEG_TRAFFIC_ENGINE_INTERSECTION_H
#define PSEG_TRAFFIC_ENGINE_INTERSECTION_H

#include "Road.h"
#include "Stop.h"
#include <vector>

/**
 * @brief A struct to keep roads and the position of the intersection together
 */
struct ConnectedRoad {
  /**
   * The road on which the intersection is situated
   */
    Road* road;
  /**
   * The position on the road onto what the position is situated
   */
    double position;

};

/**
 * @brief Represents intersections between roads
 */
class Intersection {
private:
  /**
   * All ConnectedRoad's from where vehicles can enter the intersection
   */
  std::vector<ConnectedRoad> incomingDetailed;
  /**
   * All ConnectedRoad's from where vehicles can leave the intersection
   */
  std::vector<ConnectedRoad> outgoingDetailed;

  /**
   * The controlling light on this intersection
   */
    SmartTrafficLight* firstLight = nullptr;
    Intersection* _initCheck;
public:
    /**
     * Creates an empty intersection object
     * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Intersection' must end in a correct initialized state.");
     */
    Intersection();

    /**
    * @brief Checks if this Road object is properly initialized.
    * @return true if the object is properly initialized, false otherwise.
    */
    [[nodiscard]] bool properlyInitialized() const;

  /**
   * @brief Adds a road to this intersection where vehicles can choose to get off
   * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's constructor did not initialize properly");
   * \n REQUIRE(outRoad != nullptr, "Error: 'road' cannot be NULL or empty.");
   * \n REQUIRE(outRoad->properlyInitialized(), "Error: Road's constructor did not initialize properly");
   * \n REQUIRE(position == -1 || (position > 0 && position <= outRoad->getLength()), "Error: 'position' cannot be out of bounds");
   * \n ENSURE(outgoingDetailed.back().road == outRoad, "Error: outgoingDetailed was not updated correctly");
   * @param outRoad the road to leave
   * @param position the position of the intersection on the road (-1 puts it at the end of the road)
   */
    void addOutgoingRoad(Road *outRoad, double position);
    std::array<SmartTrafficLight *, 2> getTrafficLights();

    /**
     * @brief Adds a road to this intersection where vehicles can choose to get
     * onto the intersection
     * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's
     * constructor did not initialize properly");
     * \n REQUIRE(inRoad != nullptr, "Error: 'road' cannot be NULL or empty.");
     * \n REQUIRE(inRoad->properlyInitialized(), "Error: Road's constructor did
     * not initialize properly");
     * \n REQUIRE(position == -1 || (position > 0 && position <=
     * inRoad->getLength()), "Error: 'position' cannot be out of bounds");
     * \n ENSURE(incomingDetailed.back().road == inRoad, "Error:
     * incomingDetailed was not updated correctly");
     * @param inRoad the road to leave
     * @param position the position of the intersection on the road (-1 puts it
     * at the end of the road)
     */
    void addIncomingRoad(Road *inRoad, double position);

    /**
     * @brief Gets all incoming roads and their respective positions
     * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's constructor did not initialize properly");
     */
    [[nodiscard]] std::vector<ConnectedRoad> getIncomingRoadsDetailed() const;

    /**
    * @brief Gets all outgoing roads and their respective positions
    * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's constructor did not initialize properly");
    */
    [[nodiscard]] std::vector<ConnectedRoad> getOutgoingRoadsDetailed() const;

    /**
    * @brief Gets all incoming roads
    * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's constructor did not initialize properly");
    * \n ENSURE(returned.size() == incomingDetailed.size(), "Error: Calculated value of 'incoming roads' does not match said value in incomingDetailed()");
    */
    [[nodiscard]] const std::vector<Road*>& getIncomingRoads() const;

    /**
    * @brief Gets all outgoing roads
    * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's constructor did not initialize properly");
    * \n ENSURE(returned.size() == outgoingDetailed.size(), "Error: Calculated value of 'outgoing roads' does not match said value in outgoingDetailed()");
    */
    [[nodiscard]] const std::vector<Road*>& getOutgoingRoads() const;

    /**
     * @brief Picks a random road the car will choose to take once it reaches the intersection
     * \n REQUIRE(this->properlyInitialized(), "Error: Intersection's constructor did not initialize properly");
     * \n ENSURE(std::find(outgoingRoads.begin(), outgoingRoads.end(), out) != outgoingRoads.end(), "Error: Road not found in outgoing roads")
     * @return A pointer to the road it will take
     */
    [[nodiscard]] Road* getRandomNextRoad() const;
};



#endif //PSEG_TRAFFIC_ENGINE_INTERSECTION_H