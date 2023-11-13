#ifndef GUI_H
#define GUI_H

#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <random>
class Gui
{
public:
    Gui();
    ~Gui();

    void show();

private:
    sf::RenderWindow *window;
    sf::Font *font;

    sf::RectangleShape *gameScreen;
    sf::Text *gameScreenTitle;

    uint8_t flags{0};
    sf::RectangleShape *flagsBar;
    std::array<sf::RectangleShape *, 8> flagsTiles;
    sf::Text *flagsText;
    void updateFlag();

    std::string registerStr{};
    sf::RectangleShape *RegistersBar;
    sf::Text *registersLabelText;
    sf::Text *registersText;
    std::array<sf::RectangleShape *, 6> registersTiles;
    void updateRegRadom();
};

#endif