#include "Gui.hpp"

Gui::Gui()
{
    window = new sf::RenderWindow(sf::VideoMode(1280, 720), "NES Emulator");
}

Gui::~Gui()
{

}

void Gui::show() 
{

  sf::Font font;
  font.loadFromFile("fonts/BigBlueTerm437NerdFontMono-Regular.ttf");

  sf::Text text;
  text.setFont(font);
  text.setFillColor(sf::Color::White);
  text.setString("teste de texto\n1001 0C56");
  text.setCharacterSize(15);

  sf::Font font2;
  font2.loadFromFile("fonts/ProFontWindowsNerdFontMono-Regular.ttf");

  sf::Text text2;
  text2.setFont(font2);
  text2.setFillColor(sf::Color::White);
  text2.setString("teste de texto\n1001 0C56");
  text2.setCharacterSize(20);
  text2.setPosition(0, 50);

  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window->close();
    }

    window->clear();
    window->draw(text);
    window->draw(text2);
    window->display();
  }

}