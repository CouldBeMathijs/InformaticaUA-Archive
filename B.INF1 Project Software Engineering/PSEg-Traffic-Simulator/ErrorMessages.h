//============================================================================
// Name        : ErrorMessages.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Centralized Error Messages to make editing them easier
//============================================================================

#ifndef ERRORMESSAGES_H
#define ERRORMESSAGES_H

// Put ✅ for when a test is written for that error
// Put ✔️ for when a test is impossible for that error
// Put ❌ for when a no test is written for that error

namespace ErrorMessagesInstructions {

    /**
     * @brief This function is called as an error when the constructor of the class has not successfully been initialized.
     * @note ENSURE
     *
     * @usecases Simulation
     * \n ✔️properInitializationEnsure("Simulation") 1x
     *
     * @usecases Input
     * \n ✔️properInitializationEnsure("Input") 1x
     *
     * @usecases Road
     * \n ✔️properInitializationEnsure("Road") 1x
     *
     * @usecases TrafficLight
     * \n ✔️properInitializationEnsure("TrafficLight") 1x
     *
     * @usecases Vehicle
     * \n ✔️properInitializationEnsure("Vehicle") 6x
     *
     * @usecases VehicleGenerator
     * \n ✔️properInitializationEnsure("VehicleGenerator") 1x
     *
     * @usecases BusStop
     * \n ✔properInitializationEnsure("BusStop") 1x
     *
     * @usecases Intersection
     * \n ✔properInitializationEnsure("Intersection") 1x
     *
     * @usecases Button
     * \n ✔properInitializationEnsure("Button") 1x
     *
     * @usecases Camera
     * \n ✔️properInitializationEnsure("Camera") 1x
     *
     * @usecases ResourceManager
     * \n ✔️properInitializationEnsure("ResourceManager") 1x
     *
     * @usecases RoadInfoPanel
     * \n ✔️properInitializationEnsure("RoadInfoPanel") 1x
     */
    inline std::string properInitializationEnsure(const std::string& className) {
        return "Error: Constructor of '" + className + "' must end in a correct initialized state.";
    }

    /**
     * @brief This function is called as an error when the constructor of the class has not successfully been initialized.
     * @note REQUIRE
     *
     * @usecases Simulation
     * \n ✔️properInitializationRequire("Simulation") 6x
     *
     * @usecases Road
     * \n ✔️properInitializationRequire("Road") 12x
     *
     * @usecases TrafficLight
     * \n ✔️properInitializationRequire("TrafficLight") 5x
     *
     * @usecases Vehicle
     * \n ✔️properInitializationRequire("Vehicle") 13x
     *
     * @usecases VehicleGenerator
     * \n ✔️properInitializationRequire("VehicleGenerator") 4x
     *
     * @usecases Smart Traffic Light
     * \️n ✔️properInitializationRequire("SmartTrafficLight") 2x
     *
     * @usecases Button
     * \n ✔️properInitializationRequire("Button")
     *
     * @usecases ButtonManager
     * \n ✔️properInitializationRequire("Button")
     *
     * @usecases Camera
     * \n ✔️properInitializationRequire("Camera")
     *
     * @usecases Gui
     * \n ✔️properInitializationRequire("Simulation"); 2x
     * \n ✔️properInitializationRequire("Road"); 4x
     *
     * @usecases ResourceManager
     * \n ✔️properInitializationRequire("ResourceManager") 3x
     *
     * @usecases RoadInfoPanel
     * \n ✔️properInitializationRequire("RoadInfoPanel") 3x
     * \n ✔️properInitializationRequire("Road") 3x
     *
     * @usecases SimulationRenderer
     * \n ✔️properInitializationRequire("Road") 1x
     * \n ✔️properInitializationRequire("Vehicle") 1x
     * \n ✔️properInitializationRequire("Simulation") 1x
     *
     * @usecases LayoutUtils
     * \n ✔️properInitializationRequire("Road") 4x
     *
     * @usecases SimulationRenderer
     * \n ✔️properInitializationRequire("Simulation") 3x
     * \n ✔️properInitializationRequire("Road") 3x
     * \n ✔️properInitializationRequire("Vehicle") 1x
     *
     * @usecases SimulationExporter
     * All functions
     */
    inline std::string properInitializationRequire(const std::string& className) {
        return "Error: " + className + "'s constructor did not initialize properly";
    }

    /**
     * @brief This function is called as an error when a nullptr is given as input for a function.
     * @note REQUIRE
     *
     * @usecases Simulation
     * \n ✅missingParameter("filename"); 1x
     * \n ✅missingParameter("road"); 1x
     *
     * @usecases Input
     * \n ✅missingParameter("filename"); 1x
     * \n ✅missingParameter("simulation"); 1x
     *
     * @usecases Road
     * \n ✅missingParameter("name"); 1x
     * \n ✅missingParameter("trafficLight"); 1x
     * \n ❌missingParameter("vehicle"); 3x
     * \n ❌missingParameter("road"); 6x
     * \n ❌missingParameter("intersection"); 1x
     *
     * @usecases TrafficLight
     * \n ✅missingParameter("road"); 1x
     *
     * @usecases VehicleGenerator
     * \n ✅missingParameter("road"); 1x
     *
    *  @usecases Intersection
     * \n ❌missingParameter("road"); 1x
     *
     * @usecases Button
     * \n ❌missingParameter("label"); 1x
     * \n ❌missingParameter("font"); 1x
     *
    *  @usecases ButtonManager
     * \n ❌missingParameter("label"); 1x
     * \n ❌missingParameter("font"); 1x
     * \n ❌missingParameter("callback") 1x
     *
     * @usecases Gui
     * \n ❌ missingParameter("simulation"); 1x
     * \n ❌ missingParameter("font"); 4x
     * \n ❌ missingParameter("road"); 3x
     * \n ❌ missingParameter("vehicleGenerator")
     *
     * @usecases ResourceManager
     * \n ❌ missingParameter("id") 4x
     * \n ❌ missingParameter("path") 4x
     *
     * @usecases RoadInfoPanel
     * \n ❌ missingParameter("font") 2x
     * \n ❌ missingParameter("road") 1x
     * \n ❌ missingParameter("vehicleGenerator") 1x
     *
     * @usecases SimulationRenderer
     * \n ❌ missingParameter("road") 1x
     * \n ❌ missingParameter("vehicle") 1x
     * \n ❌ missingParameter("simulation") 1x
     *
     * @usecases LayoutUtils
     * \n ❌ missingParameter("road") 4x
     *
     * @usecases SimulatieExporter
     * \n ❌ missingParameter("road") 2x
     * \n ❌ missingParameter("vehicle") 1x
     */
    inline std::string missingParameter(const std::string& param) {
        return "Error: '" + param + "' cannot be NULL or empty.";
    }

    /**
     * @brief This function is called as an error when something is negative but has to be positive.
     * @note REQUIRE
     *
     * @usecases Simulation
     * \n ✅mustBePositive("seconds"); 1x
     *
     * @usecases Road
     * \n ✅mustBePositive("time"); 1x
     * \n ✅mustBePositive("timeEachTick"); 1x
     * \n ✅mustBePositive("length"); 1x
     *
     * @usecases TrafficLight
     * \n ✅mustBePositive("position"); 1x
     * \n ✅mustBePositive("cycle"); 2x
     * \n ✅mustBePositive("time"); 1x
     *
     * @usecases Vehicle
     * \n ✅mustBePositive("dt"); 1x
     *
     * @usecases VehicleGenerator
     * \n ✅mustBePositive("frequency"); 1x
     * \n ✅mustBePositive("time"); 3x
     *
     * @usecases ButtonManager
     * \n ✅mustBePositive("Button size")
     */
    inline std::string mustBePositive(const std::string& param) {
        return "Error: " + param + " cannot be negative.";
    }

    /**
     * @brief This function is called as an error when getting a empty value from a variable while it should not.
     * @note ENSURE
     *
     * @usecases Simulation
     * \n ✔️notUpdatedSuccessfully("road") 1x
     *
     * @usecases Road
     * \n ✔️notUpdatedSuccessfully("name"); 1x
     * \n ✔️notUpdatedSuccessfully("trafficLight"); 1x
     * \n ✔️notUpdatedSuccessfully("vehicle"); 2x
     * \n ✔️notUpdatedSuccessfully("vehicleGenerator"); 1x
     * \n ✔️notUpdatedSuccessfully("intersectionBindings"); 1x
     *
     * @usecases Vehicle
     * \n ✔️notUpdatedSuccessfully("road") 1x
     * \n ✔️notUpdatedSuccessfully("position"); 2x
     * \n ✔️notUpdatedSuccessfully("texture"); 6x
     *
     * @usecases TrafficLight
     * \n ✔️notUpdatedSuccessfully("road"); 1x
     * \n ✔️notUpdatedSuccessfully("lastChange"); 1x
     *
     * @usecases Intersection
     * \n ✔️notUpdatedSuccessfully("incomingDetailed"); 1x
     * \n ✔️notUpdatedSuccessfully("outgoingDetailed"); 1x
     *
     * @usecases ResourceManager
     * \n ✔️notUpdatedSuccessfully("texture"); 1x
     * \n ✔️notUpdatedSuccessfully("texture"); 1x
     *
     * @usecases RoadInfoPanel
     * \n ✔️notUpdatedSuccessfully("font") 1x
     * \n ✔️notUpdatedSuccessfully("selectedRoad") 1x
     *
     * @usecases LayoutUtils
     * \n ✔️notUpdatedSuccessfully("components") 1x
     * \n ✔️notUpdatedSuccessfully("layout") 1x
     */
    inline std::string notUpdatedSuccessfully(const std::string& item) {
        return "Error: " + item + " was not updated correctly";
    }

    /**
     * @brief This function is called as an error when getting a not empty value from a variable while it should.
     * @note ENSURE
     *
     * @usecases Road
     * \n ✔️notDeletedSuccessfully("vehicle") 1x
     *
     * @usecases ResourceManager
     * \n ✔️notDeletedSuccessfully("resources") 1x
     */
    inline std::string notDeletedSuccessfully(const std::string& item) {
        return "Error: " + item + " was not deleted successfully";
    }

    /**
     * @brief This function is called as an error when a value does is not the same as the getter.
     * @note ENSURE
     *
     * @usecases Road
     * \n ✔️mismatchedComputedValue("length", "getLength"); 1x
     * \n ✔️mismatchedComputedValue("name", "getName"); 1x
     * \n ✔️mismatchedComputedValue("vehicleGenerator", "getVehicleGenerator"); 1x
     *
     * @usecases Intersection
     * \n ✔️mismatchedComputedValue("returnvalue", "outgoingroads"); 1x
     * \n ✔️mismatchedComputedValue("incoming roads", "incomingDetailed"); 1x
     * \n ✔️mismatchedComputedValue("outgoing roads", "outgoingDetailed"); 1x
     *
     * @usecases Button
     * \n ✔️mismatchedComputedValue("label", "text.getString"); 1x
     * \n ✔️mismatchedComputedValue("font", "text.getFont"); 1x
     *
     */
    inline std::string mismatchedComputedValue(const std::string& variableName, const std::string& functionName) {
        return "Error: Calculated value of '" + variableName + "' does not match said value in " + functionName + "()";
    }

    /**
     * @brief This function is called as an error when a value doesn't increase while it should.
     * @note ENSURE
     *
     * @usecases Simulation
     * \n ✔️valueNotIncreased("time"); 1x
     *
     * @usecases Vehicle
     * \n ✔️valueNotIncreased("position"); 2x
     */
    inline std::string valueNotIncreased(const std::string& variableName) {
        return "Error: Calculated value '" + variableName + "' did not increase";
    }

    /**
     * @brief This function is called as an error when a value is out of bounds.
     * @note REQUIRE
     *
     * @usecases TrafficLight
     * \n ✅outOfBounds("position"); 1x
     *
     * @usecases Road
     * \n ✅outOfBounds("position"); 1x
     */
    inline std::string outOfBoundsRequire(const std::string& variableName) {
        return "Error: '" + variableName + "' cannot be out of bounds";
    }

    /**
     * @brief This function is called as an error when a value is out of bounds.
     * @note ENSURE
     *
     * @usecases TrafficLight
     * \n ✔outOfBoundsEnsure("position"); 1x
     * \n outOfBoundsEnsure("timeLastGenerated"); 1x
     */
    inline std::string outOfBoundsEnsure(const std::string& variableName) {
        return "Error: '" + variableName + "' is out of bounds";
    }

    /**
     * @brief This function is called as an error when this code should never be reached
     * @note ENSURE
     *
     * @usecases SimulatieExporter
     * \n 1x
     *
     * @usecases DesignByContract
     * \n NeverReached 4x
     */
    constexpr auto NeverReached = "Error: a switch case or if statement was called when no case for the input was available";

    /**
     * @brief used for invalid vehicle types
     * @note XML, REQUIRE
     *
     * @usecases XMLInput
     *
     */
    inline std::string invalidVehicleType(const std::string& vehicleType) {
        return "Error: '" + vehicleType + "' is not a supported vehicle type";
    }

    /**
     * \REQUIRE && ENSURE
     * @usecases SmartTrafficLight 1x
     * @brief used when A should be smaller than B but isn't
     * @param A the one which should be greater
     * @param B the one which should be smaller
     *
     * @usecases Road
     * \n ❌outOfBoundsEnsure("position");
     */
    inline std::string invalidComparisonAsmallerthanB(const std::string &A, const std::string &B) {
        return "Error: '" + A + "' cannot be smaller than '"  + B + "'";
    }

    /**
     * \REQUIRE
     * @usecases Road 1x
     * @brief used when you try to delete a vehicle that is not on the road
     * @param A the road name
     *
     * @usecases Road
     */
    inline std::string faultyDelete(const std::string &A) {
        return "Tried to delete a vehicle that is not in " + A + ", was this intended?";
    }





}

namespace ErrorMessagesReadXml {

    /**
     * @brief This function is called as an error when a file is not found.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✅invalidFile(filename); 1x
     */
    inline std::string invalidFile(const std::string& filename) {
        return "Error: file '" + filename + "' does not exist or has no valid contents";
    }

    /**
     * @brief This function is called as an error when a required parameter is not found in the file.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ❌noValidParameter("root"); 1x
     */
    inline std::string noValidParameter(const std::string& param) {
        return "Error: No valid " + param + " element found";
    }

    /**
     * @brief This function is called as an error when a parameter is missing for an object in the file.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✅missingParameter("name", "Road"); 1x
     * \n ✅missingParameter("length", "Road"); 1x
     *
     * \n ✅missingParameter("name", "TrafficLight"); 1x
     * \n ✅missingParameter("position", "TrafficLight"); 1x
     * \n ✅missingParameter("cycle", "TrafficLight"); 1x
     *
     * \n ✅missingParameter("name", "Vehicle"); 1x
     * \n ✅missingParameter("position", "Vehicle"); 1x
     *
     * \n ✅missingParameter("name", "VehicleGenerator"); 1x
     * \n ✅missingParameter("frequency", "VehicleGenerator"); 1x
     */
    inline std::string missingParameter(const std::string& param, const std::string& object) {
        return "Error: No valid " + param + " found for " + object;
    }

    /**
     * @brief This function is called as an error when a parameter’s value in the file is empty or NULL.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✅missingValue("road"); 3x
     * \n ❌missingValue("name"); 4x
     */
    inline std::string missingValue(const std::string& param) {
        return "Error: Value of " + param + " cannot be empty or NULL";
    }

    /**
     * @brief This function is called as an error when a numerical value must be positive but is not.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✔ mustBePositive("length"); 1x
     * \n ✅ mustBePositive("position"); 2x
     * \n ✅mustBePositive("cycle"); 1x
     * \n ✔ mustBePositive("frequency"); 1x
     *
     * @usecases Button
     * \n ❌mustBePositive("Button size")
     */
    inline std::string mustBePositive(const std::string& param) {
        return "Error: " + param + " must be positive";
    }

    /**
     * @brief This function is called as an error when only one instance is allowed, but multiple instances are found.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✅notTheSame("road", "Simulation"); 1x
     * \n ✅onlyOneAllowed("vehicleGenerator", "Road"); 1x
     */
    inline std::string onlyOneAllowed(const std::string& objectType, const std::string& context) {
        return "Error: Every " + objectType + " must be unique within each " + context;
    }

    /**
     * @brief This function is called as an error when an argument is found that should not exist in the file context.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✅invalidArgumentForContext(elem->Value()); 1x
     */
    inline std::string invalidArgumentForContext(const std::string& argument) {
        return "Error: " + argument + " is not valid for this simulation";
    }

    /**
     * @brief This function is called as an error when a type conversion fails or is invalid.
     * @note REQUIRE
     *
     * @usecases Input
     * \n ✅invalidType("length", "integer"); 1x
     * \n ✅invalidType("position", "integer"); 2x
     * \n ✅invalidType("cycle", "integer"); 1x
     * \n ✅nvalidType("frequency", "integer"); 1x
     */
    inline std::string invalidType(const std::string& variable, const std::string& type) {
        return "Error: " + variable + " must be a " + type;
    }
}
#endif //ERRORMESSAGES_H
