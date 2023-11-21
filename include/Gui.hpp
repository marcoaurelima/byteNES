#ifndef GUI_H
#define GUI_H
#pragma once

#include "Bus.hpp"

#include <SFML/Graphics.hpp>
#include <array>
#include <deque>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <random>
class Gui
{
public:
    Gui(Bus& bus);
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

    std::stringstream registerSStr{};
    sf::RectangleShape *RegistersBar;
    sf::Text *registersLabelText;
    sf::Text *registersText;
    std::array<sf::RectangleShape *, 6> registersTiles;
    void updateRegisters();

    sf::Text *zeroPageScreenTitle;
    sf::RectangleShape *zeropageScreen;
    std::string zeroPageColumnsLabel{};
    std::string zeroPageLinesLabel{};
    sf::Text *zeroPageColumnsLabelText;
    sf::Text *zeroPageLinesLabelText;

    std::string zeroPageDataStr{};
    sf::Text *zeroPageDataText;
    void updateZeroPageMemory();

    sf::Text *filePathText;
    sf::Text *keyMappingText;

    Bus& bus;
};

#endif