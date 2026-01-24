//============================================================================
// Name        : LayoutUtils.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : Namespace which helps GUI's by calculating the coordinates of things
//============================================================================

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>

class Road;
class Intersection;

namespace Utils {
/**
 * @brief Groups all roads into connected components via shared intersections.
 *
 * @param allRoads List of all roads in the simulation.
 * @return Vector of subsets (chains) of connected roads.
 *
 * \n REQUIRE(for each road in allRoads: road != nullptr, "Error: 'road' cannot be NULL or empty.")
 * \n REQUIRE(for each road in allRoads: road->properlyInitialized(), "Error: Road's constructor did not initialize properly")
 *
 * @usage auto components = Utils::groupConnectedRoads(simulation->getRoads());
 */
std::vector<std::vector<Road*>> groupConnectedRoads(const std::vector<Road*>& allRoads);

/**
 * @brief Computes a simple 2D layout for a single connected road chain.
 *
 * Each subsequent road segment is given a start position and an angle, where
 * at each intersection a +90° turn is applied.
 *
 * @param chain A connected chain of roads (e.g., result of `groupConnectedRoads`).
 * @param roadStartPositions [OUT] start positions (pixels) per road.
 * @param roadAngles [OUT] angles (degrees) per road.
 *
 * \n REQUIRE(!chain.empty(), "Error: 'chain' cannot be NULL or empty.")
 * \n REQUIRE(for each road in chain: road != nullptr,  "Error: 'road' cannot be NULL or empty.")
 * \n REQUIRE(for each road in chain: road->properlyInitialized(), "Error: Road's constructor did not initialize properly")
 *
 * @usage Utils::computeLayoutForChain(chain, roadStartPositions, roadAngles);
 */
void computeLayoutForChain(
    const std::vector<Road*>&                            chain,
    std::unordered_map<Road*, sf::Vector2f>&            roadStartPositions,
    std::unordered_map<Road*, float>&                   roadAngles);

} // namespace Utils


#endif // UTILS_H