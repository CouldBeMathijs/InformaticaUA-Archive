// Functions made for the intro session on 14/02/2024
#include "Intro.h"
#include <cmath>

bool IntroColorRectangle(const ini::Configuration &configuration, img::EasyImage &out) {
    // Colors the image with repeating color patterns
    if (configuration["General"]["type"].as_string_or_die() != "IntroColorRectangle") {
        return false;
    }
    int width = configuration["ImageProperties"]["width"].as_int_or_die();
    int height = configuration["ImageProperties"]["height"].as_int_or_die();
    for (unsigned int i = 0; i < width; i++) {
        for (unsigned int j = 0; j < height; j++) {
            out(i, j).red = i;
            out(i, j).green = j;
            out(i, j).blue = (i + j) % 256;
        }
    }
    return true;
}

bool IntroBlocks(const ini::Configuration &configuration, img::EasyImage &out) {
    // Creates a checkerboard pattern
    if (configuration["General"]["type"].as_string_or_die() != "IntroBlocks") {
        return false;
    }
    int width = configuration["ImageProperties"]["width"].as_int_or_die();
    int height = configuration["ImageProperties"]["height"].as_int_or_die();
    int nrXBlocks = configuration["BlockProperties"]["nrXBlocks"].as_int_or_die();
    int nrYBlocks = configuration["BlockProperties"]["nrYBlocks"].as_int_or_die();
    bool invertColors = configuration["BlockProperties"]["invertColors"].as_bool_or_default(false);
    img::Color colorBlack, colorWhite;
    if (invertColors) {
        colorBlack = vecToColor(configuration["BlockProperties"]["colorWhite"].as_double_tuple_or_die());
        colorWhite = vecToColor(configuration["BlockProperties"]["colorBlack"].as_double_tuple_or_die());
    } else {
        colorWhite = vecToColor(configuration["BlockProperties"]["colorWhite"].as_double_tuple_or_die());
        colorBlack = vecToColor(configuration["BlockProperties"]["colorBlack"].as_double_tuple_or_die());
    }
    // For every pixel in the width
    for (unsigned int i = 0; i < width; i++) {
        // For every pixel in the height
        for (unsigned int j = 0; j < height; j++) {
            // If the pixel is placed on an spot were the number of blocks is even, color it in color 1
            if ((((i / (width / nrXBlocks)) + (j / (height / nrYBlocks))) % 2) == 0) {
                out(i, j) = colorWhite;
            } else {
                // Else color it in color2
                out(i, j) = colorBlack;
            }
        }
    }
    return true;
}

bool IntroLines(const ini::Configuration &conf, img::EasyImage &out) {
    // Creates a line drawing of type Quatercircle, Eye or Diamond
    if (conf["General"]["type"].as_string_or_die() != "IntroLines") {
        return false;
    }
    // Initialisations
    int width = conf["ImageProperties"]["width"].as_int_or_die();
    int height = conf["ImageProperties"]["height"].as_int_or_die();
    std::string figure = conf["LineProperties"]["figure"].as_string_or_die();
    int nrLines = conf["LineProperties"]["nrLines"].as_int_or_die();
    std::vector<double> lineColorVector = conf["LineProperties"]["lineColor"].as_double_tuple_or_die();

    img::Color backgroundcolor = vecToColor(conf["LineProperties"]["backgroundcolor"].as_double_tuple_or_die());
    img::Color lineColor = vecToColor(conf["LineProperties"]["lineColor"].as_double_tuple_or_die());

    double hs = static_cast<double>(width - 1) / (nrLines - 1);
    double ws = static_cast<double>(height - 1) / (nrLines - 1);
    // Color in the background
    out.clear(backgroundcolor);
    if (figure == "QuarterCircle" || figure == "Eye") {
        for (int i = 0; i < nrLines; ++i) {
            int y_start = std::lround(i * ws);
            int x_end = std::lround(width - 1 - i * hs);

            out.draw_line(0, height - 1 - y_start, x_end, height - 1, lineColor);
        }
        if (figure == "Eye") {
            for (int i = 0; i < nrLines; ++i) {
                int y_start = std::lround(height - 1 - i * ws);
                int x_end = std::lround(width - 1 - i * hs);

                out.draw_line(width - 1, y_start, x_end, 0, lineColor);
            }
        }
    } else if (figure == "Diamond") {
        double quarterwidth = width / 2;
        double quarterheight = height / 2;
        for (int i = 0; i < nrLines; ++i) {
            int y_start = std::lround(quarterheight - i * ws);
            int x_end = std::lround(quarterwidth - i * hs);
            if (y_start < 0) {
                y_start = 0;
            }
            if (x_end < 0) {
                x_end = 0;
            }
            // Top Left
            out.draw_line(std::lround(quarterwidth), std::lround(y_start + quarterheight - 1), x_end,
                          std::lround(quarterheight),
                          lineColor);
            out.draw_line(std::lround(quarterwidth - x_end), std::lround(quarterheight), std::lround(quarterwidth),
                          height - y_start - 1, lineColor);
            // Bottom Right
            out.draw_line(std::lround(x_end + quarterwidth - 1), std::lround(quarterheight), std::lround(quarterwidth),
                          y_start,
                          lineColor);
            out.draw_line(std::lround(quarterwidth), std::lround(quarterheight - y_start), width - 1 - x_end,
                          std::lround(quarterheight), lineColor);
            // Bottom Left
            out.draw_line(std::lround(quarterwidth - x_end), std::lround(quarterheight), std::lround(quarterwidth),
                          y_start,
                          lineColor);
            out.draw_line(std::lround(quarterwidth), std::lround(quarterheight - y_start), x_end,
                          std::lround(quarterheight),
                          lineColor);
            // Top Left
            out.draw_line(std::lround(quarterwidth + x_end - 1), std::lround(quarterheight), std::lround(quarterwidth),
                          height - 1 - y_start, lineColor);
            out.draw_line(std::lround(quarterwidth), std::lround(quarterheight + y_start - 1), width - 1 - x_end,
                          std::lround(quarterheight), lineColor);
        }
    } else {
        std::cerr << figure << " is not a supported figure of type IntroLines" << std::endl;
        return false;
    }
    return true;
}
