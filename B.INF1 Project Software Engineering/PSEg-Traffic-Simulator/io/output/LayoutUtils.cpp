#include "LayoutUtils.h"
#include "../../DesignByContract.h"
#include "../../ErrorMessages.h"
#include "../../classes/Road.h"
#include "../../classes/Intersection.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <stdexcept>

namespace Utils {

// ──────────────────────────────────────────────────────────────────────────
static void validateRoadVector(const std::vector<Road*>& roads) {
    for (Road* r : roads) {
        REQUIRE(r != nullptr, ErrorMessagesInstructions::missingParameter("road"));
        REQUIRE(r->properlyInitialized(),
                ErrorMessagesInstructions::properInitializationRequire("Road"));
    }
}
// ──────────────────────────────────────────────────────────────────────────

std::vector<std::vector<Road*>> groupConnectedRoads(const std::vector<Road*>& allRoads) {
    validateRoadVector(allRoads);

    std::unordered_map<Road*, std::unordered_set<Road*>> adjacency;
    adjacency.reserve(allRoads.size());

    for (Road* r : allRoads) {
        adjacency[r];
        for (const auto& bind : r->getIntersectionBindings()) {
            Intersection* inter = bind.intersection;
            if (!inter) continue;

            // Outgoing roads
            for (Road* outR : inter->getOutgoingRoads()) {
                if (outR && outR != r) {
                    adjacency[r].insert(outR);
                    adjacency[outR].insert(r);
                }
            }
            // Incoming roads
            for (Road* inR : inter->getIncomingRoads()) {
                if (inR && inR != r) {
                    adjacency[r].insert(inR);
                    adjacency[inR].insert(r);
                }
            }
        }
    }

    std::unordered_set<Road*> visited;
    visited.reserve(allRoads.size());

    std::vector<std::vector<Road*>> components;
    components.reserve(allRoads.size() / 2 + 1);

    for (Road* road : allRoads) {
        if (!visited.count(road)) {
            std::vector<Road*> comp;
            std::queue<Road*>  q;

            visited.insert(road);
            q.push(road);

            while (!q.empty()) {
                Road* cur = q.front(); q.pop();
                comp.push_back(cur);

                for (Road* nb : adjacency[cur]) {
                    if (!visited.count(nb)) {
                        visited.insert(nb);
                        q.push(nb);
                    }
                }
            }
            components.push_back(std::move(comp));
        }
    }

    ENSURE(components.size() > 0,
           ErrorMessagesInstructions::notUpdatedSuccessfully("components"));
    return components;
}

// ──────────────────────────────────────────────────────────────────────────
static void processRoad(Road* currentRoad,
                        std::unordered_set<Road*>& placed,
                        std::unordered_map<Road*, sf::Vector2f>& roadStartPositions,
                        std::unordered_map<Road*, float>&        roadAngles)
{
    REQUIRE(currentRoad != nullptr,
            ErrorMessagesInstructions::missingParameter("road"));
    REQUIRE(currentRoad->properlyInitialized(),
            ErrorMessagesInstructions::properInitializationRequire("Road"));

    constexpr float PI = 3.14159f;

    float currAngleRad = roadAngles[currentRoad] * PI / 180.f;
    sf::Vector2f currDir(std::cos(currAngleRad), std::sin(currAngleRad));
    sf::Vector2f currOrigin = roadStartPositions[currentRoad];

    for (const auto& bind : currentRoad->getIntersectionBindings()) {
        Intersection* inter = bind.intersection;
        if (!inter) continue;

        sf::Vector2f interPt = currOrigin + currDir * static_cast<float>(bind.position);

        Road* otherRoad = nullptr;
        for (Road* r : inter->getOutgoingRoads()) if (r != currentRoad) { otherRoad = r; break; }
        if (!otherRoad) {
            for (Road* r : inter->getIncomingRoads()) if (r != currentRoad) { otherRoad = r; break; }
        }
        if (!otherRoad || placed.count(otherRoad)) continue;

        float otherPos = -1.f;
        for (const auto& b : otherRoad->getIntersectionBindings()) {
            if (b.intersection == inter) { otherPos = b.position; break; }
        }
        if (otherPos < 0.f) continue;

        sf::Vector2f newDir(-currDir.y, currDir.x);           // simpele 90°
        float newAngleDeg = std::atan2(newDir.y, newDir.x) * 180.f / PI;

        roadAngles[otherRoad]       = newAngleDeg;
        roadStartPositions[otherRoad] = interPt - newDir * otherPos;

        placed.insert(otherRoad);
        processRoad(otherRoad, placed, roadStartPositions, roadAngles);
    }
}
// ──────────────────────────────────────────────────────────────────────────

void computeLayoutForChain(const std::vector<Road*>& chain,
                           std::unordered_map<Road*, sf::Vector2f>& roadStartPositions,
                           std::unordered_map<Road*, float>&        roadAngles)
{
    validateRoadVector(chain);
    if (chain.empty()) return;

    for (Road* r : chain) {
        roadStartPositions[r] = {0.f, 0.f};
        roadAngles[r]         = -9999.f;
    }

    Road* first = chain.front();
    roadStartPositions[first] = {0.f, 0.f};
    roadAngles[first]         = 0.f;

    std::unordered_set<Road*> placed;
    placed.reserve(chain.size());
    placed.insert(first);

    processRoad(first, placed, roadStartPositions, roadAngles);

    ENSURE(placed.size() == chain.size(),
           ErrorMessagesInstructions::notUpdatedSuccessfully("layout"));
}

}