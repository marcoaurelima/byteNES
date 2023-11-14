#include "Gui.hpp"

Gui::Gui()
{
  // Program screen
  window = new sf::RenderWindow(sf::VideoMode(880, 640), "NES Emulator");
  window->setFramerateLimit(2);

  gameScreen = new sf::RectangleShape(sf::Vector2f(256 * 2, 240 * 2));
  gameScreen->setPosition(50, 50);
  gameScreen->setFillColor(sf::Color::Black);
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

  // Disassembler monitor
  disassemblerScreen = new sf::RectangleShape(sf::Vector2f(226, 540));
  disassemblerScreen->setPosition(600, 50);
  disassemblerScreen->setFillColor(sf::Color(20, 20, 20));
  disassemblerScreen->setOutlineColor(sf::Color(80, 80, 80));
  disassemblerScreen->setOutlineThickness(1);

  disassemblerScreenMasks[0] = new sf::RectangleShape(sf::Vector2f(226, 253));
  disassemblerScreenMasks[0]->setPosition(600, 50);
  disassemblerScreenMasks[0]->setFillColor(sf::Color(0, 0, 0, 180));

  disassemblerScreenMasks[1] = new sf::RectangleShape(sf::Vector2f(4, 19));
  disassemblerScreenMasks[1]->setPosition(600, 304);
  disassemblerScreenMasks[1]->setFillColor(sf::Color::Blue);

  disassemblerScreenMasks[2] = new sf::RectangleShape(sf::Vector2f(4, 19));
  disassemblerScreenMasks[2]->setPosition(822, 304);
  disassemblerScreenMasks[2]->setFillColor(sf::Color::Blue);

  disassemblerScreenMasks[3] = new sf::RectangleShape(sf::Vector2f(226, 262));
  disassemblerScreenMasks[3]->setPosition(600, 325);
  disassemblerScreenMasks[3]->setFillColor(sf::Color(0, 0, 0, 180));

  for (int i = 0; i < 25; i++)
  {
    disassemblerSequenceStr.push_back("2F3B  ORG  $80AF");
  }

  std::string temp;
  for (int i = 0; i < 25; i++)
  {
    temp += disassemblerSequenceStr[i] + '\n';
  }

  disassemblerSequenceText = new sf::Text();
  disassemblerSequenceText->setFont(*font);
  disassemblerSequenceText->setFillColor(sf::Color::White);
  disassemblerSequenceText->setString(temp);
  disassemblerSequenceText->setCharacterSize(20);
  disassemblerSequenceText->setPosition(620, 60);
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

    window->draw(*RegistersBar);
    for (auto &reg : registersTiles)
    {
      window->draw(*reg);
    }
    window->draw(*registersText);
    window->draw(*registersLabelText);

    window->draw(*disassemblerScreen);
    window->draw(*disassemblerSequenceText);
    window->draw(*disassemblerScreenMasks[0]);
    window->draw(*disassemblerScreenMasks[1]);
    window->draw(*disassemblerScreenMasks[2]);
    window->draw(*disassemblerScreenMasks[3]);

    window->display();
    flags++;
  }
}