//============================================================================
// Name        : Stop.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Every stop and subclass thereof, this file holds
// traffic lights and bus stops
//============================================================================

#ifndef STOP_H
#define STOP_H
#include <limits>


class Bus;
class Road;
/**
 * @brief The base class for all objects on roads at which vehicles should stop
 */
class Stop {
protected:
  Stop* _initCheck;
  double lastChange;
  unsigned int position;
  Road *road;
  double currentTime = 0;

  /**
 * @brief Constructor for the stop class.
 * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
 * \n REQUIRE(position < road->getLength(), "Error: 'position' cannot be out of bounds");
 * \n REQUIRE(position >= 0, "Error: position cannot be negative.");
 * \n REQUIRE(cycle > 0, "Error: cycle cannot be negative.");
 * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'Stop' must end in a correct initialized state.");
 * @param road     A valid Road* (must not be null)
 * @param position The position of the Stop on the road (must be < road->getLength() and positive)
 */
  Stop(Road *road, unsigned int position);
public:

  virtual ~Stop() = default;
  /**
   * @brief Checks whether this Stop object is properly initialized.
   * @return true if the Stop is properly initialized, false otherwise.
   */
  [[nodiscard]] virtual bool properlyInitialized() const;

  /**
   * @brief getter for the current time
   * \n REQUIRE(this->properlyInitialized(), "Error: Stop's constructor did not initialize properly");
   * @return the current time stored in the stop
   */
  [[nodiscard]] double getCurrentTime() const;

  /**
  * @brief Returns the road to which this Stop is attached.
  * \n REQUIRE(this->properlyInitialized(), "Error: Stop's constructor did not initialize properly");
  * \n ENSURE(result != nullptr, "Error: road was not updated correctly");
  * @return A pointer to the Road object.
  */
  [[nodiscard]] Road *getRoad() const;

  /**
  * @brief Returns the position of this Stop on the road.
  * \n REQUIRE(this->properlyInitialized(), "Error: Stop's constructor did not initialize properly");
  * \n ENSURE(result <= getRoad()->getLength(), "Error: 'position' is out of bounds");
  * @return The position of the Stop (in meters or chosen unit).
  */
  [[nodiscard]] double getPosition() const;

  /** @brief Returns the time of the last change in state for this stop
  * \n REQUIRE(this->properlyInitialized(), "Error: Stop's constructor did not initialize properly");
   * @return
   */
  [[nodiscard]] double getLastChange() const;

  /**
  * @brief Virtual class, does nothing.
  * \n ENSURE(false, "Error: this code should not be reached")
  */
  virtual void update(double);
  ;
};

/**
 * @brief the color of traffic lights
 */
enum class LightState { GREEN, ORANGE, RED };

/**
 * @brief traffic light, a toggleable stop
 */
class TrafficLight: public Stop{
protected:

  unsigned int cycle;
  LightState currentState;
public:

  /**
   * @brief Constructor for the traffic light.
   * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
   * \n REQUIRE(position < road->getLength(), "Error: 'position' cannot be out of bounds");
   * \n REQUIRE(position >= 0, "Error: position cannot be negative.");
   * \n REQUIRE(cycle > 0, "Error: cycle cannot be negative.");
   * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'TrafficLight' must end in a correct initialized state.");
   * @param road     A valid Road* (must not be null)
   * @param position The position of the traffic light on the road (must be < road->getLength())
   * @param cycle    The duration of the green/red cycle in seconds (must be > 0)
   */
  TrafficLight(Road *road, unsigned int position, unsigned int cycle);

  /**
   * @brief Updates the traffic light based on elapsed simulation time.
   * \n REQUIRE(this->properlyInitialized(), "Error: TrafficLight's constructor did not initialize properly");
   * \n REQUIRE(currentTime >= 0, "Error: time cannot be negative.");
   * @param currentTime The current global simulation time in seconds.
   */
  void update(double currentTime) override;

  /**
   * @brief Indicates whether the light is green (true) or red (false).
   * \n REQUIRE(this->properlyInitialized(), "Error: TrafficLight's constructor did not initialize properly");
   * @return true if the light is green, false otherwise.
   */
  [[nodiscard]] bool allowedToDrive() const;

  void toggle();

  /**
   * @brief Returns the duration of this traffic light's cycle.
   * \n REQUIRE(this->properlyInitialized(), "Error: TrafficLight's constructor did not initialize properly");
   * \n ENSURE(result >= 0, "Error: cycle cannot be negative.");
   * @return The duration of the cycle (in seconds).
   */
  [[nodiscard]] double getCycle() const;

  /**
   * @brief Returns the current color state as an Enum value also defined within this file
   * \n REQUIRE(this->properlyInitialized(), "Error: TrafficLight's constructor did not initialize properly");
   * @return The current color state
   */
  [[nodiscard]] LightState getCurrentState() const;

  /**
   * @brief Sets the current LightState for this traffic light
   * \n REQUIRE(this->properlyInitialized(), "Error: TrafficLight's constructor
   * did not initialize properly");
   * \n ENSURE(currenttime == getLastChange()), "
   * @param lightstate
   */
  void setCurrentState(LightState lightstate);
};

/**
 * @brief traffic light which tries to maximise the amount of passing traffic
 * always needs to be linked
 */
class SmartTrafficLight: public TrafficLight {
private:
  SmartTrafficLight* linkedLight = nullptr;

  double minStaticTime;
  double maxStaticTime;

  bool visible;
  double orangeDuration;

public:
  /**
   * @brief Constructor for smart traffic lights
   * \n ENSURE(this->properlyInitialized(), "Error: Smart Traffic Light's
   * constructor did not initialize properly");
   * \n REQUIRE(road->properlyInitialized(), "Error: the road was not properly
   * initialized")
   * \n REQUIRE(position >= 0 && position <= road->getLength(), "Error: the
   * position is out of bounds")
   * \n REQUIRE(minStaticTime <= maxStaticTime, "Error: minStaticTime should be
   * smaller than maxStaticTime")
   * @param road The road the SmartTrafficLight is on
   * @param position The position on said road
   * @param isVisible Whether the trafficLight should be visible in graphical modes
   * @param minStaticTime The least amount of time a traffic light should remain
   * a certain colour (defaults to 0)
   * @param maxStaticTime The most amount of time a traffic light is allowed to
   * remain a certain colour (defaults to + infinity)
   */
  SmartTrafficLight(Road *road, unsigned int position, bool isVisible, double minStaticTime=0, double maxStaticTime=std::numeric_limits<double>::infinity());

  /**
   * @brief Sets the linked light of an intersection trafficLight pair
   * @param linkedLight The light with which this light should be linked
   * \n REQUIRE(this->properlyInitialized(), "Error: Smart Traffic Light's
   * constructor did not initialize properly");
   * \n REQUIRE(linkedLight->properlyInitialized(), "Error: Smart Traffic Light's
   * constructor did not initialize properly");
   * \n ENSURE(this->linkedLight == linkedLight, "Error: linkedLight was not updated correctly")
   * ENSURE(this->getCurrentState() == LightState::GREEN,  "Error: this light state was not updated correctly")
   * ENSURE(light->getCurrentState() == LightState::GREEN, "Error: linkedLight's lightstate was not updated correctly")
   */
  void setLinkedLight(SmartTrafficLight* linkedLight);

  /**
   * @brief Starts setting this light to green and sets the linked light to
   * become red
   * \n REQUIRE(this->properlyInitialized(), "Error: Smart Traffic Light's
   * constructor did not initialize properly");
   */
  void intersectionLightsSetStateToGreen();

  /**
   * @brief Updates the state of the traffic light
   * \n REQUIRE(this->properlyInitialized(),
   * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
   * @param currentTime The current time of the simulation
   */
  void update(double currentTime) override;

  /**
   * @brief Checks if this smart traffic light wants to be green
   * \n REQUIRE(this->properlyInitialized(),
   * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
   */
  bool wantsToBeGreen() const;

  /**
   * @brief Switch het huidige smartlight enkel als het gelinkte licht kan switchen
   * \n REQUIRE(this->properlyInitialized(),
   * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
   * @return
   */
  bool checkAndSwitch();

  /**
   * @brief Returns the visibility status
   * \n REQUIRE(this->properlyInitialized(),
   * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
   */
  bool isVisible() const;

  /**
   * @brief sets the visibility of this traffic light on visualizations
   * \n REQUIRE(this->properlyInitialized(),
   * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
   * \n ENSURE(this->isVisible == v, "Error: visibility not updated correctly)
   */
  void setVisibility(bool v);

 /**
  * @brief sets the minimum static time for this traffic light
  * \n REQUIRE(this->properlyInitialized(),
  * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
  */
  void setMinStaticTime(double m);

  /**
   * @brief sets the maximum static time for this traffic light
   * \n REQUIRE(this->properlyInitialized(),
   * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
   */
  void setMaxStaticTime(double m);

  /**
 * @brief returns the pointer for the linked traffic light
 * \n REQUIRE(this->properlyInitialized(),
 * ErrorMessagesInstructions::properInitializationRequire("SmartTrafficLight"));
 */
  [[nodiscard]] SmartTrafficLight* getLinkedLight() const;
};

/**
 * @brief stops the bus for a set amount of time, then lets it pass
 */
class BusStop : public Stop {
private:
  Bus* occupied;
  double lastOccupiedTime = -1;
  double cooldownPeriod = 10;
  double waitTime;
public:

   /**
   * @brief Constructor for the bus stop.
   * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
   * \n REQUIRE(position < road->getLength(), "Error: 'position' cannot be out
   * of bounds");
   * \n REQUIRE(position >= 0, "Error: position cannot be negative.");
   * \n REQUIRE(cycle > 0, "Error: cycle cannot be negative.");
   * \n ENSURE(this->properlyInitialized(), "Error: Constructor of
   * 'TrafficLight' must end in a correct initialized state.");
   * @param road     A valid Road* (must not be null)
   * @param position The position of the traffic light on the road (must be <
   * road->getLength())
   */
    BusStop(Road *road, unsigned int position);

    /**
    * @brief Constructor for the bus stop.
    * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
    * \n REQUIRE(position < road->getLength(), "Error: 'position' cannot be out
    * of bounds");
    * \n REQUIRE(position >= 0, "Error: position cannot be negative.");
    * \n REQUIRE(cycle > 0, "Error: cycle cannot be negative.");
    * \n ENSURE(this->properlyInitialized(), "Error: Constructor of
    * 'TrafficLight' must end in a correct initialized state.");
    * @param road     A valid Road* (must not be null)
    * @param position The position of the traffic light on the road (must be <
    * road->getLength())
    * @param waitTime A positive time, at which the bus will look to check how long it must wait
    */
    BusStop(Road *road, unsigned int position, double waitTime);

    /**
     * @brief Returns a pointer to the bus currently at the stop or the nullptr
     * \n ENSURE(returnval == nullptr || returnval->properlyInitialized(), "Error: the return value is not valid")
     * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of
     * 'Bus' must end in a correct initialized state.");
    */
    [[nodiscard]] Bus* getOccupied() const;

    /**
     * @brief updates this busstop to agree with the current time
     * @param currentTime the current time
     * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of
     * 'BusStop' must end in a correct initialized state.");
     */
    void update(double currentTime) override;

    /**
     * @brief Checks whether the busstop is allowed to have a new bus stop there
     * @param currentTime
     * @return availability
     */
    bool isAvailable(double currentTime) const;

    /**
     * @brief sets the bus which should occupy the busstop
     * @param bus the bus which should occupy the busstop
     * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of
     * 'BusStop' must end in a correct initialized state.");
     * \n REQUIRE(bus == nullptr || bus->properlyInitialized(), "Error: Constructor of
     * 'Bus' must end in a correct initialized state.");
     */
    void setOccupied(Bus* bus);

   /**
    * @brief Gets the last change time if the bus is at the stop or it's unoccupied.
    * @param busToCheck Bus to verify against the current occupant.
    * @return Last change time or -inf if the bus doesn't match the occupant.
    * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of
    * 'BusStop' must end in a correct initialized state.");
    * \n REQUIRE(busToCheck->properlyInitialized(), "Error: Constructor of
    * 'Bus' must end in a correct initialized state.");
    */
    double getLastChange(const Bus * busToCheck) const;


    /**
     * @brief gets the current remaining wait time for this busstop
     * \n REQUIRE(this->properlyInitialized(), "Error: Constructor of
     * 'BusStop' must end in a correct initialized state.");
     * @return the current remaining wait time for this busstop
     * */
    [[nodiscard]] double getWaitTime() const;
};
#endif // STOP_H
