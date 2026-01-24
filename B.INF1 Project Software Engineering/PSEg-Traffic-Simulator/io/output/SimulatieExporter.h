//============================================================================
// Name        : SimulatieExporter.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Simple terminal outputs
//============================================================================
#ifndef SIMULATIEEXPORTER_H
#define SIMULATIEEXPORTER_H

#include "../../classes/Simulation.h"
#include "../../classes/Stop.h"
#include <ostream>

namespace SimulatieExporter {
/**
 * @brief Prints vehicle stats as layed out in Specification 1.0
 * \n REQUIRE(simulation->properlyInitialized(),
 * ErrorMessagesInstructions::properInitializationEnsure("Simulation");
 * @param onStream The ostream to write the output to
 * @param simulation The simulation to print
 */
void exportOn(std::ostream &onStream, const Simulation &simulation);

/**
 * @brief Prints a road as required by Specification 2.0
 * \n REQUIRE(road->properlyInitialized(),
 * ErrorMessagesInstructions::properInitializationEnsure("Road");
 * @param out The ostream to write the output to
 * @param road The road to read out
 * @param indenting The minimal amount of space to keep between The name and
 * the impression (default is 20)
 */
void doGraphicsImpression(std::ostream &out, const Road *road,
                          unsigned int indenting);

/**
 * @brief Prints a road as required by Specification 2.0
 * \n REQUIRE(simulation->properlyInitialized(),
 * ErrorMessagesInstructions::properInitializationEnsure("Simulation");
 * @param out The ostream to write the output to
 * @param sim The simulation to print
 */
void doGraphicsImpression(std::ostream &out, const Simulation &sim);

/**
 * @brief Prints the simulation using Emoji
 * @deprecated
 * \n REQUIRE(simulation->properlyInitialized(),
 * ErrorMessagesInstructions::properInitializationEnsure("Simulation");
 * @param onStream The ostream to write the output to
 * @param sim The simulation to print
 */
void visualizeSimulationState(std::ostream &onStream, const Simulation &sim);
}; // namespace SimulatieExporter

#endif // SIMULATIEEXPORTER_H