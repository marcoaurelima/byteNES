#include "Gui.hpp"

Gui::Gui()
{
  // Program screen
  window = new sf::RenderWindow(sf::VideoMode(1280, 720), "NES Emulator");
  window->setFramerateLimit(2);

  gameScreen = new sf::RectangleShape(sf::Vector2f(256 * 2, 240 * 2));
  gameScreen->setPosition(50, 50);
  gameScreen->setFillColor(sf::Color::Black);
  gameScreen->setOutlineColor(sf::Color::White);
  gameScreen->setOutlineThickness(1);

  font = new sf::Font();
  font->loadFromFile("fonts/ProFontWindowsNerdFontMono-Regular.ttf");

  // Game screen
  gameScreenTitle = new sf::Text();
  gameScreenTitle->setFont(*font);
  gameScreenTitle->setFillColor(sf::Color::White);
  gameScreenTitle->setString("NES Game Screen - 256x240px");
  gameScreenTitle->setCharacterSize(20);
  gameScreenTitle->setPosition(50, 15);

  // Flags monitor
  for (int i = 0; i < flagsTiles.size(); i++)
  {
    flagsTiles[i] = new sf::RectangleShape(sf::Vector2f(19.8, 19));
    flagsTiles[i]->setFillColor(sf::Color(60, 60, 60));
    flagsTiles[i]->setOutlineColor(sf::Color::Black);
    flagsTiles[i]->setOutlineThickness(2);
    flagsTiles[i]->setPosition(130 + ((flagsTiles.size() - i - 1) * 19.8), 543);
  }

  flagsBar = new sf::RectangleShape(sf::Vector2f(242, 25));
  flagsBar->setPosition(50, 540);
  flagsBar->setFillColor(sf::Color(20, 20, 20));
  flagsBar->setOutlineColor(sf::Color::White);
  flagsBar->setOutlineThickness(1);

  flagsText = new sf::Text();
  flagsText->setFont(*font);
  flagsText->setFillColor(sf::Color::White);
  flagsText->setString("FLAGS   N V   B D I Z C");
  flagsText->setCharacterSize(20);
  flagsText->setPosition(55, 538);

  // Registers monitor


}

Gui::~Gui()
{
}

void Gui::updateFlag()
{

  for (int i = 0; i < flagsTiles.size(); i++)
  {
    if (flags & (0x01 << i))
    {
      flagsTiles[i]->setFillColor(sf::Color::Red);
    }
    else
    {
      flagsTiles[i]->setFillColor(sf::Color(40, 40, 40));
    }
  }
}

void Gui::show()
{

  while (window->isOpen())
  {
    sf::Event event;
    while (window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window->close();
    }

    window->clear();
    window->draw(*gameScreenTitle);
    window->draw(*gameScreen);

    updateFlag();

    window->draw(*flagsBar);
    for (auto &flag : flagsTiles)
    {
      window->draw(*flag);
    }
    window->draw(*flagsText);
    window->display();
    flags++;
  }
}