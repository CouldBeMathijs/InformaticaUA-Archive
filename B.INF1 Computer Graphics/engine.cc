#include "Draw3DLines.h"
#include "Intro.h"
#include "LSystems2D.h"
#include "external/ImageComparator.h"

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>

// Functions used by main
img::EasyImage generate_image(const ini::Configuration &conf) {
    bool recognised = false;
    int width, height;
    if (conf["ImageProperties"]["width"].exists() && conf["ImageProperties"]["height"].exists()) {
        width = conf["ImageProperties"]["width"].as_int_or_die();
        height = conf["ImageProperties"]["height"].as_int_or_die();
    } else if (conf["General"]["size"].exists()) {
        width = conf["General"]["size"].as_int_or_die();
        height = width;
    } else {
        throw std::invalid_argument("Width and height, or size not found");
    }
    img::EasyImage image(width, height);
    // Run every one of these tests
    std::vector<std::function<bool(const ini::Configuration &, img::EasyImage &)> > functions = {
        drawWireframe,
        drawZBufferedWireframe,
        drawZBuffering,
        Lsystem2DLSystem,
        IntroColorRectangle,
        IntroBlocks,
        IntroLines
    };
    // If one of theme is true, we've succeeded
    for (const auto &func: functions) {
        if (func(conf, image)) {
            recognised = true;
            break;
        }
    }
    if (recognised == false) {
        throw std::logic_error(
            "Image of type " + conf["General"]["type"].as_string_or_die() +
            " was not recognised or formatted correctly");
    }
    return image;
}

std::string findValidFile(const std::string &fileName) {
    return fileName;
}

int main(int argc, char const *argv[]) {
    int retVal = 0;
    int amountOfFails = 0;
    auto startall = std::chrono::high_resolution_clock::now();
    try {
        std::vector<std::string> args = std::vector<std::string>(argv + 1, argv + argc);
        if (args.empty()) {
            std::string validFile = findValidFile("filelist");
            std::ifstream fileIn(validFile);
            std::string filelistName;
            while (std::getline(fileIn, filelistName)) {
                args.push_back(filelistName);
            }
        }
        for (std::string fileName: args) {
            ini::Configuration conf;
            try {
                std::string validFile = findValidFile(fileName);
                if (validFile.empty()) {
                    std::cout << "Ini file appears empty or does not exist: '" << fileName << "'" << std::endl;
                    continue;
                }

                std::ifstream fin(validFile);
                fin >> conf;
                fin.close();
            } catch (ini::ParseException &ex) {
                std::cerr << "[ERROR] parsing file: " << fileName << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image;
            try {
                auto start = std::chrono::high_resolution_clock::now();
                image = generate_image(conf);
                auto end = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                std::cout << "Image '" << fileName << "' generation took " << duration << " ms" << std::endl;
            } catch (const std::exception &exception) {
                std::cerr << "[ERROR] whilst generating '" << fileName << "': " << exception.what() << std::endl;
                retVal = 1;
                image = img::EasyImage(100, 100);
            }


            if (image.get_height() > 0 && image.get_width() > 0) {
                std::string::size_type pos = fileName.rfind('.');
                if (pos == std::string::npos) {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                } else {
                    fileName = fileName.substr(0, pos) + ".bmp";
                }
                try {
                    std::ofstream f_out(fileName.c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;
                } catch (std::exception &ex) {
                    std::cerr << "[Error]: Failed to write" << fileName << " image to file: " << ex.what() << std::endl;
                    amountOfFails++;
                }
            } else {
                std::cerr << "[Error]: Could not generate image for '" << fileName <<
                        "' something wrong with the height and width?" <<
                        std::endl;
            }
        }
    } catch (const std::bad_alloc &exception) {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
        //mark the test as failed while in reality it just needed a bit more memory
        std::cerr << "[Error]: insufficient memory" << std::endl;
        retVal = 100;
    }
    auto endall = std::chrono::high_resolution_clock::now();
    auto durationall = std::chrono::duration_cast<std::chrono::milliseconds>(endall - startall).count();
    std::cout << "[Notice]: Total run time: " << durationall << "ms" << std::endl;
    constexpr auto folderToCheck = ".";
    std::cout << "Checking images in " << folderToCheck << std::endl;
    ImageComparator image_comparator(folderToCheck);
    image_comparator.compare();
    return retVal;
}
