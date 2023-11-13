#ifndef GUI_H
#define GUI_H

#pragma once

#include <SFML/Graphics.hpp>

class Gui
{
public:
    Gui();
    ~Gui();

    void show();

private:
    sf::RenderWindow* window;

};

#endif