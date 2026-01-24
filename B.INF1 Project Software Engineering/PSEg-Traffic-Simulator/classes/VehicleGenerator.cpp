#include "VehicleGenerator.h"

#include <iostream>

#include "../DesignByContract.h"
#include "../ErrorMessages.h"
#include "Road.h"
#include <random>
#include <stdexcept>

VehicleGenerator::VehicleGenerator(unsigned int frequency, Road* road, const std::string& vehicleType)
    : frequency(frequency), timeLastGenerated(0), currentRoad(road), generationType(stringToVehicleType(vehicleType)) {

    REQUIRE(frequency >= 0, ErrorMessagesInstructions::mustBePositive("frequency"));
    REQUIRE(road != nullptr, ErrorMessagesInstructions::missingParameter("road"));

    _initCheck = this;

    ENSURE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationEnsure("VehicleGenerator"));
}

VehicleType VehicleGenerator::stringToVehicleType(const std::string& vehicleType) {
    if (vehicleType == "auto") return VehicleType::CAR;
    if (vehicleType == "ziekenwagen") return VehicleType::AMBULANCE;
    if (vehicleType == "politiewagen") return VehicleType::POLICE;
    if (vehicleType == "brandweerwagen") return VehicleType::FIRETRUCK;
    if (vehicleType == "bus") return VehicleType::BUS;
    if (vehicleType == "random") return VehicleType::RANDOM;

    REQUIRE(false, ErrorMessagesInstructions::invalidVehicleType(vehicleType));
}


bool VehicleGenerator::properlyInitialized() const {
    return _initCheck == this;
}

unsigned int VehicleGenerator::getFrequency() const {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire(
              "VehicleGenerator"))
  return frequency;
}

VehicleType VehicleGenerator::getVehicleType() const {
    REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
    return generationType;
}

void VehicleGenerator::setVehicleType(VehicleType generationtype) {
  REQUIRE(this->properlyInitialized(),
          "Error: VehicleGenerator's constructor did not initialize properly");
  generationType = generationtype;
    ENSURE(this->getVehicleType() == generationtype, ErrorMessagesInstructions::notUpdatedSuccessfully("generationtype"));
}
double VehicleGenerator::getTimeLastGenerated() const {
  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire(
              "VehicleGenerator"));
  ENSURE(timeLastGenerated >= 0, "Error: 'timeLastGenerated' is out of bounds");
  return timeLastGenerated;
}

bool VehicleGenerator::canGenerateVehicle(double time) const {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("VehicleGenerator"))
    REQUIRE(time >= 0, ErrorMessagesInstructions::mustBePositive("time"));
    return (time - timeLastGenerated) >= frequency;
}

bool VehicleGenerator::tryGenerate() const {

  REQUIRE(this->properlyInitialized(),
          ErrorMessagesInstructions::properInitializationRequire(
              "VehicleGenerator"));
    bool startIsClear = true;
    for (Vehicle* vehicle : currentRoad->getVehicles()) {
        if(vehicle->getPosition() < vehicle->getLength()*2) {
            startIsClear = false;
        }
    }
    if(!startIsClear){
        return false;
    }

  VehicleType typeToGenerate = generationType;

  if (generationType == VehicleType::RANDOM) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<VehicleType> vehiclePool = {
        VehicleType::CAR,       VehicleType::CAR,    VehicleType::CAR,
        VehicleType::CAR,       VehicleType::CAR,    VehicleType::CAR,
        VehicleType::AMBULANCE, VehicleType::POLICE, VehicleType::BUS, VehicleType::CAR, VehicleType::CAR, VehicleType::CAR, VehicleType::FIRETRUCK};

    std::uniform_int_distribution<int> dist(0, (int)vehiclePool.size() - 1);
    typeToGenerate = vehiclePool[dist(gen)];
  }
  Vehicle *newVehicle;
  switch (typeToGenerate) {
  case VehicleType::CAR:
    newVehicle = new Car(currentRoad, 0);
    break;
  case VehicleType::AMBULANCE:
    newVehicle = new Ambulance(currentRoad, 0);
    break;
  case VehicleType::POLICE:
    newVehicle = new Police(currentRoad, 0);
    break;
  case VehicleType::BUS:
    newVehicle = new Bus(currentRoad, 0);
    break;
  case VehicleType::FIRETRUCK:
    newVehicle = new FireTruck(currentRoad, 0);
    break;
  default:
    ENSURE(false, ErrorMessagesInstructions::NeverReached);
  }
  currentRoad->addVehicle(newVehicle);
    return true;
}

void VehicleGenerator::tick(double time) {
    REQUIRE(this->properlyInitialized(), ErrorMessagesInstructions::properInitializationRequire("VehicleGenerator"))
    REQUIRE(time >= 0, ErrorMessagesInstructions::mustBePositive("time"));

    if (canGenerateVehicle(time)) { // Check based on time
      if (tryGenerate()) {
            timeLastGenerated = time;
        }
    }
}

std::string VehicleGenerator::to_string(const VehicleType vh) {
    switch (vh){
        case VehicleType::CAR:
            return "Car";
        case VehicleType::FIRETRUCK:
            return "FireTruck";
        case VehicleType::BUS:
            return "Bus";
        case VehicleType::AMBULANCE:
            return "Ambulance";
        case VehicleType::POLICE:
            return "Police";
        case VehicleType::RANDOM:
            return "Random";
        default:
            throw std::invalid_argument(ErrorMessagesInstructions::NeverReached);
    }
}

void VehicleGenerator::toggleVehicleType() {
    REQUIRE(this->properlyInitialized(), "Error: VehicleGenerator's constructor did not initialize properly");
    switch (generationType) {
        case VehicleType::CAR:
            generationType = VehicleType::AMBULANCE;
        break;
        case VehicleType::AMBULANCE:
            generationType = VehicleType::POLICE;
        break;
        case VehicleType::POLICE:
            generationType = VehicleType::FIRETRUCK;
        break;
        case VehicleType::FIRETRUCK:
            generationType = VehicleType::BUS;
        break;
        case VehicleType::BUS:
            generationType = VehicleType::RANDOM;
        break;
        case VehicleType::RANDOM:
            generationType = VehicleType::CAR;
        break;
    }
}