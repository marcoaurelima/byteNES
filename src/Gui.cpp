#include "Gui.hpp"

Gui::Gui(Cpu &cpu) : cpu(cpu)
{
  // Program screen
  window = new sf::RenderWindow(sf::VideoMode(1170, 640), "NES Emulator");
  window->setVerticalSyncEnabled(true);

  gameScreen = new sf::RectangleShape(sf::Vector2f(256 * 2, 240 * 2));
  gameScreen->setPosition(50, 50);
  gameScreen->setFillColor(sf::Color(30, 30, 30));
  gameScreen->setOutlineColor(sf::Color(80, 80, 80));
  gameScreen->setOutlineThickness(1);

  font = new sf::Font();
  font->loadFromFile("fonts/ProFontWindowsNerdFontMono-Regular.ttf");

  // Game screen
  gameScreenTitle = new sf::Text();
  gameScreenTitle->setFont(*font);
  gameScreenTitle->setFillColor(sf::Color::White);
  gameScreenTitle->setString("NES Emulator");
  gameScreenTitle->setCharacterSize(20);
  gameScreenTitle->setPosition(50, 15);

  // Flags monitor
  for (int i = 0; i < flagsTiles.size(); i++)
  {
    flagsTiles[i] = new sf::RectangleShape(sf::Vector2f(19.8, 19));
    flagsTiles[i]->setFillColor(sf::Color(40, 40, 40));
    flagsTiles[i]->setOutlineColor(sf::Color::Black);
    flagsTiles[i]->setOutlineThickness(2);
    flagsTiles[i]->setPosition(130 + ((flagsTiles.size() - i - 1) * 19.8), 543);
  }

  flagsBar = new sf::RectangleShape(sf::Vector2f(242, 25));
  flagsBar->setPosition(50, 540);
  flagsBar->setFillColor(sf::Color(20, 20, 20));
  flagsBar->setOutlineColor(sf::Color(80, 80, 80));
  flagsBar->setOutlineThickness(1);

  flagsText = new sf::Text();
  flagsText->setFont(*font);
  flagsText->setFillColor(sf::Color::White);
  flagsText->setString("FLAGS   N V   B D I Z C");
  flagsText->setCharacterSize(20);
  flagsText->setPosition(55, 538);

  // Registers monitor
  for (int i = 0; i < registersTiles.size(); i++)
  {
    registersTiles[i] = new sf::RectangleShape(sf::Vector2f(29.7, 41));
    registersTiles[i]->setFillColor(sf::Color(40, 40, 40));
    registersTiles[i]->setOutlineColor(sf::Color(20, 20, 20));
    registersTiles[i]->setOutlineThickness(2);
    registersTiles[i]->setPosition(374 + ((registersTiles.size() - i - 1) * 29.7), 544);
  }

  RegistersBar = new sf::RectangleShape(sf::Vector2f(262, 50));
  RegistersBar->setPosition(300, 540);
  RegistersBar->setFillColor(sf::Color(20, 20, 20));
  RegistersBar->setOutlineColor(sf::Color(80, 80, 80));
  RegistersBar->setOutlineThickness(1);

  registersLabelText = new sf::Text();
  registersLabelText->setFont(*font);
  registersLabelText->setFillColor(sf::Color::White);
  registersLabelText->setString("REGS   PC SP A  X  Y  P");
  registersLabelText->setCharacterSize(20);
  registersLabelText->setPosition(310, 538);

  registerStr = "12 34 56 78 9A BC";
  registersText = new sf::Text();
  registersText->setFont(*font);
  registersText->setFillColor(sf::Color::Green);
  registersText->setString(registerStr);
  registersText->setCharacterSize(20);
  registersText->setPosition(379, 558);

  // Zero Page monitor
  zeroPageScreenTitle = new sf::Text();
  zeroPageScreenTitle->setFont(*font);
  zeroPageScreenTitle->setFillColor(sf::Color::White);
  zeroPageScreenTitle->setString("ZERO PAGE");
  zeroPageScreenTitle->setCharacterSize(20);
  zeroPageScreenTitle->setPosition(600, 15);

  zeropageScreen = new sf::RectangleShape(sf::Vector2f(526, 540));
  zeropageScreen->setPosition(600, 50);
  zeropageScreen->setFillColor(sf::Color(20, 20, 20));
  zeropageScreen->setOutlineColor(sf::Color(80, 80, 80));
  zeropageScreen->setOutlineThickness(1);

  zeroPageColumnsLabel = "0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F";
  zeroPageColumnsLabelText = new sf::Text();
  zeroPageColumnsLabelText->setFont(*font);
  zeroPageColumnsLabelText->setFillColor(sf::Color::White);
  zeroPageColumnsLabelText->setString(zeroPageColumnsLabel);
  zeroPageColumnsLabelText->setCharacterSize(20);
  zeroPageColumnsLabelText->setPosition(635, 65);

  zeroPageLinesLabel = "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\nA\nB\nC\nD\nE\nF\n";
  zeroPageLinesLabelText = new sf::Text();
  zeroPageLinesLabelText->setFont(*font);
  zeroPageLinesLabelText->setFillColor(sf::Color::White);
  zeroPageLinesLabelText->setString(zeroPageLinesLabel);
  zeroPageLinesLabelText->setCharacterSize(20);
  zeroPageLinesLabelText->setPosition(615, 90);
  zeroPageLinesLabelText->setLineSpacing(1.5);

  for (int i = 1; i <= 256; i++)
  {
    zeroPageDataStr += "00 ";
    if (i % 16 == 0)
    {
      zeroPageDataStr += "\n";
    }
  }

  zeroPageDataText = new sf::Text();
  zeroPageDataText->setFont(*font);
  zeroPageDataText->setFillColor(sf::Color::Green);
  zeroPageDataText->setString(zeroPageDataStr);
  zeroPageDataText->setCharacterSize(20);
  zeroPageDataText->setPosition(635, 90);
  zeroPageDataText->setLineSpacing(1.5);
}

Gui::~Gui()
{
}

void Gui::updateFlag()
{
  for (int i = 0; i < flagsTiles.size(); i++)
  {
    if (cpu.getSR() & (0x01 << i))
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

    window->draw(*RegistersBar);
    for (auto &reg : registersTiles)
    {
      window->draw(*reg);
    }
    window->draw(*registersText);
    window->draw(*registersLabelText);

    window->draw(*zeroPageScreenTitle);
    window->draw(*zeropageScreen);

    window->draw(*zeroPageColumnsLabelText);
    window->draw(*zeroPageLinesLabelText);

    window->draw(*zeroPageDataText);

    window->display();
    flags++;
  }
}