//
// Created by adria on 12-3-2025.
//

#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H
#include <memory>

#include "../external/easy_image.h"
#include "../external/ini_configuration.h"

class ImageGenerator {
public:
    virtual img::EasyImage generate(const ini::Configuration& config) = 0;
    virtual ~ImageGenerator() = default;
};

inline std::unique_ptr<ImageGenerator> createGenerator(const ini::Configuration &config);

#endif //IMAGEGENERATOR_H
