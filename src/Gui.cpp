#include "Gui.hpp"

Gui::Gui()
{
  // Program screen
  window = new sf::RenderWindow(sf::VideoMode(1280, 720), "NES Emulator");
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

  // Flags
  for (int i=0;i<flags.size();i++)
  {
    flags[i] = new sf::RectangleShape(sf::Vector2f(20, 19));
    flags[i]->setFillColor(sf::Color(60,60,60));
    flags[i]->setOutlineColor(sf::Color::Black);
    flags[i]->setOutlineThickness(2);
    flags[i]->setPosition(130+(i*20), 543);
  }

  flagsTittleBar = new sf::RectangleShape(sf::Vector2f(244, 25));
  flagsTittleBar->setPosition(50, 540);
  flagsTittleBar->setFillColor(sf::Color(20,20,20));
  flagsTittleBar->setOutlineColor(sf::Color::White);
  flagsTittleBar->setOutlineThickness(1);

  flagsText = new sf::Text();
  flagsText->setFont(*font);
  flagsText->setFillColor(sf::Color::White);
  flagsText->setString("FLAGS   N V ~ B D I Z C");
  flagsText->setCharacterSize(20);
  flagsText->setPosition(55, 538);


}

Gui::~Gui()
{
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
    
    window->draw(*flagsTittleBar);
    for(auto& flag : flags)
    {
      window->draw(*flag);
    }
    window->draw(*flagsText);
    window->display();
  }
}