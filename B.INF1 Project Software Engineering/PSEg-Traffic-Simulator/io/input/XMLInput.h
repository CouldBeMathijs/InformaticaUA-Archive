//============================================================================
// Name        : XMLInput.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Class that processes the input file
//============================================================================
#ifndef INPUT_H
#define INPUT_H
#include "../../classes/Simulation.h"

#include <iostream>

namespace XMLInput {

    /**
     * @brief Creates a new Input class based on an external file.
     * \n REQUIRE(filename != nullptr, "Error: 'filename' cannot be NULL or empty.");
     * \n REQUIRE(simulation != nullptr, "Error: 'simulation' cannot be NULL or empty.");
     * @param input The filename or path to the input file (XML).
     * @param simulation Pointer to the Simulation object to which roads, vehicles, etc. will be added.
     * @param errout The error stream used, defaults to std::cerr
     */
    void XMLInput(const char * input, Simulation* simulation, std::ostream& errout = std::cerr);

    /**
    * @brief Creates a new Input class based on an external file.
    * \n REQUIRE(filename != nullptr, "Error: 'filename' cannot be NULL or empty.");
    * \n REQUIRE(simulation != nullptr, "Error: 'simulation' cannot be NULL or empty.");
    * \n ENSURE(out->properlyInitialized(), "Error: Constructor of 'Input' must end in a correct initialized state.");
    * @param input The filename or path to the input file (XML).
    * @param errout The error stream used, defaults to std::cerr
    */
    Simulation XMLInput(const char *input, std::ostream &errout = std::cerr);
    };

#endif //INPUT_H
