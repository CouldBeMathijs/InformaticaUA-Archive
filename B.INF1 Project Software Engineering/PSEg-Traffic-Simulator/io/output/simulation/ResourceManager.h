//============================================================================
// Name        : ResourceManager.h
// Authors     : DeKaeyman, Luca Letroye & CouldBeMathijs
// Description : General management for all SFML related resources
//============================================================================
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>

/**
 * @brief General management for all SFML related resources
 */
class ResourceManager {
public:
  /**
   * @brief Constructor
   * \n ENSURE(this->properlyInitialized(), "Error: Constructor of
   * 'ResourceManager' must end in a correct initialized state.");
   */
    ResourceManager();

  /**
   * @brief Checks whether this function is properly initialized
   * @return Whether this function is properly initialized
   */
    bool properlyInitialized() const;

    /**
     * @brief Loads all textures
     * REQUIRE(properlyInitialized(), "Error: ResourceManager was not properly
     * initialized");
     */
    void loadAllTextures();

    /**
     * @brief Loads a texture
     * REQUIRE(properlyInitialized(), "Error: ResourceManager was not properly
     * initialized");
     * REQUIRE(!id.empty(), "Error: id cannot be empty or NULL");
     * REQUIRE(!path.empty(), "Error: path cannot be empty or NULL);
     * @param id the id for where to put the texture on
     * @param path the path to the texture file
     * @return success
     */
    bool loadTexture(const std::string& id, const std::string& path);

    /**
     * @brief Loads a font
     * REQUIRE(properlyInitialized(), "Error: ResourceManager was not properly
     * initialized");
     * REQUIRE(!id.empty(), "Error: id cannot be empty or NULL");
     * REQUIRE(!path.empty(), "Error: path cannot be empty or NULL);
     * @param id the id for where to put the font in
     * @param path the path to the font file
     * @return success
     */
    bool loadFont(const std::string& id, const std::string& path);

    /**
     * @brief Getter for a texture
     * REQUIRE(properlyInitialized(), "Error: ResourceManager was not properly
     * initialized");
     * @param id the id for the texture to get
     * @return the SFML texture or nullptr
     */
    sf::Texture* getTexture(const std::string& id);

    /**
     * @brief Getter for a font
     * REQUIRE(properlyInitialized(), "Error: ResourceManager was not properly
     * initialized");
     * @param id the id fot the font to get
     * @return the SFML font or nullptr
     */
    sf::Font* getFont(const std::string& id);

    /**
     * @brief Clear all fonts and textures
     * REQUIRE(properlyInitialized(), "Error: ResourceManager was not properly
     * initialized");
     */
    void clear();

private:
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    ResourceManager* _initCheck = nullptr;
};
#endif