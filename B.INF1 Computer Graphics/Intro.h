// Functions made for the intro session on 14/02/2024
#ifndef INTRO_H
#define INTRO_H
#include "General.h"
#include "external/ini_configuration.h"

bool IntroColorRectangle(const ini::Configuration &configuration, img::EasyImage &out);

bool IntroBlocks(const ini::Configuration &configuration, img::EasyImage &out);

bool IntroLines(const ini::Configuration &conf, img::EasyImage &out);
#endif //INTRO_H
