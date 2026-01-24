//============================================================================
// Name        : VehicleGenerator.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Generator that spawns vehicles onto a road at a given frequency.
//============================================================================

#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

#include "Vehicle.h"

class Road;
/**
 * @brief All possible types of vehicle the generator can spawn
 */
enum class VehicleType {CAR, AMBULANCE, POLICE, BUS, FIRETRUCK, RANDOM};

/**
 * @brief Generates vehicles on the road every \a frequency seconds
 */
class VehicleGenerator {
private:
  VehicleGenerator* _initCheck;
  unsigned int frequency;
  double timeLastGenerated;
  Road* currentRoad;
  VehicleType generationType;
public:

  /**
   * @brief Creates a vehicle generator that places a new vehicle on the road every \a frequency seconds.
   * \n REQUIRE(frequency >= 0, "Error: frequency cannot be negative.");
   * \n REQUIRE(road != nullptr, "Error: 'road' cannot be NULL or empty.");
   * \n ENSURE(this->properlyInitialized(), "Error: Constructor of 'VehicleGenerator' must end in a correct initialized state.");
   * @param frequency The time interval (in seconds) between generating new vehicles.
   * @param road Pointer to the road where new vehicles will be added (must not be null).
   */
  explicit VehicleGenerator(unsigned int frequency, Road *road,
                            const std::string &VehicleType);
  /**
   * @brief Converts a string to a VehicleType enum state
   * \n REQUIRE(vehicleTypeValid,
   * ErrorMessagesInstructions::invalidVehicleType(vehicleType));
   * @param vehicleType The vehicle type to return as string
   * @return a VehicleType enum state
   */
  static VehicleType stringToVehicleType(const std::string &vehicleType);

  /**
   * @brief Converts a vehicleType enum state to a string
   * @param vh a vehicle type state
   * @return the string name of this enum state
   */
  static std::string to_string(const VehicleType vh);

  /**
   * @brief Changes the generationType to the next vehicle type
   * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's
   * constructor did not initialize properly");
   */
  void toggleVehicleType();;

  /**
   * @brief Checks whether this VehicleGenerator object is properly initialized
   * (Design by Contract).
   * @return true if the generator is properly initialized, false otherwise.
   */
  [[nodiscard]] bool properlyInitialized() const;

  /**
   * @brief Determines if enough time has passed to generate a new vehicle.
   * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
   * \n REQUIRE(time >= 0, "Error: time cannot be negative.");
   * @param time The current simulation time.
   * @return true if enough time has passed since the last generation, false otherwise.
   */
  [[nodiscard]] bool canGenerateVehicle(double time) const;

  /**
   * Force generates a vehicle no matter the time, but doesn't spawn when the vehicle woudn't fit
   * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
   * @return success
   * \n ENSURE(currentRoad->getVehicles.size() >= old(currentRoad->getVehicles.size()), "Error: updated incorrectly")
   */
  bool tryGenerate() const;
  ;

  /**
   * @brief Executes a tick: checks if a vehicle may be generated and adds it to
   * the road if allowed.
   * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's
   * constructor did not initialize properly");
   * \n REQUIRE(time >= 0, "Error: time cannot be negative.");
   * @param time The current simulation time.
   */
  void tick(double time);

  /**
   * @brief Returns the configured frequency.
   * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
   * @return The frequency used to generate new vehicles.
   */
  [[nodiscard]] unsigned int getFrequency() const;

 /**
 * @brief Returns the configured frequency.
 * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
 * @return The frequency used to generate new vehicles.
 */
  [[nodiscard]] VehicleType getVehicleType() const;

  /**
  * @brief Sets the generationtype of this vehicle generator
  * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
  * \n ENSURE(generationtype == getVehicleType(), "Error: generationtype was not updated successfully")
  * @param generationtype
  */
  void setVehicleType(VehicleType generationtype);

  /**
  * @brief Returns the last time the vehicle was generated
  * \n REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
  * \n ENSURE(@return >= 0, "Error: 'timeLastGenerated' is out of bounds");
  */
  [[nodiscard]] double getTimeLastGenerated() const;
};

#endif // VEHICLEGENERATOR_H
