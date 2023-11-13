#ifndef GUI_H
#define GUI_H

#pragma once

#include <SFML/Graphics.hpp>
#include <array>
class Gui
{
public:
    Gui();
    ~Gui();

    void show();

private:
    sf::RenderWindow* window;
    sf::Font* font;

    sf::RectangleShape* gameScreen;
    sf::Text* gameScreenTitle;

    sf::RectangleShape* flagsTittleBar;
    std::array<sf::RectangleShape*, 8> flags;
    sf::Text* flagsText;
};

#endif