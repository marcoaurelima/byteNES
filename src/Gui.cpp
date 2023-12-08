#include "Gui.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>

Gui::Gui(Cpu &cpu) : cpu(cpu) {
  // Program screen
  window = new sf::RenderWindow(sf::VideoMode(1170, 660), "byteNES");
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

  // Game screen
  gameScreenInfo = new sf::Text();
  gameScreenInfo->setFont(*font);
  gameScreenInfo->setFillColor(sf::Color::Yellow);
  std::stringstream ss;
  ss << "CLOCK " << clock << " Hz";
  gameScreenInfo->setString(ss.str());
  gameScreenInfo->setCharacterSize(20);
  gameScreenInfo->setPosition(470, 15);

  // Flags monitor
  for (size_t i = 0; i < flagsTiles.size(); i++) {
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
  flagsText->setString("FLAG    N V   B D I Z C");
  flagsText->setCharacterSize(20);
  flagsText->setPosition(55, 538);

  // Registers monitor
  for (size_t i = 0; i < registersTiles.size(); i++) {
    if (i == 0) {
      registersTiles[i] = new sf::RectangleShape(sf::Vector2f(49.8, 41));
    } else {
      registersTiles[i] = new sf::RectangleShape(sf::Vector2f(1129.8, 41));
    }
    registersTiles[i]->setFillColor(sf::Color(40, 40, 40));
    registersTiles[i]->setOutlineColor(sf::Color(20, 20, 20));
    registersTiles[i]->setOutlineThickness(2);
    if (i == 0) {
      registersTiles[i]->setPosition(
          354 + ((registersTiles.size() - i - 1) * 49.8), 544);
    } else {
      registersTiles[i]->setPosition(
          354 + ((registersTiles.size() - i - 1) * 29.8), 1544);
    }
  }

  RegistersBar = new sf::RectangleShape(sf::Vector2f(262, 50));
  RegistersBar->setPosition(300, 540);
  RegistersBar->setFillColor(sf::Color(20, 20, 20));
  RegistersBar->setOutlineColor(sf::Color(80, 80, 80));
  RegistersBar->setOutlineThickness(1);

  registersLabelText = new sf::Text();
  registersLabelText->setFont(*font);
  registersLabelText->setFillColor(sf::Color::White);
  registersLabelText->setString("REG    PC SP AC X  Y  SR");
  registersLabelText->setCharacterSize(20);
  registersLabelText->setPosition(310, 538);

  registersText = new sf::Text();
  registersText->setFont(*font);
  registersText->setFillColor(sf::Color::Green);
  registersText->setString(registerSStr.str());
  registersText->setCharacterSize(20);
  registersText->setPosition(359, 558);

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

  for (int i = 1; i <= 0xFF; i++) {
    zeroPageDataStr += "00 ";
    if (i % 16 == 0) {
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

  std::string keyMappingStr = "(R)eset   (N)ext instruction   R(E)sume";
  keyMappingText = new sf::Text();
  keyMappingText->setFont(*font);
  keyMappingText->setFillColor(sf::Color::White);
  keyMappingText->setString(keyMappingStr);
  keyMappingText->setCharacterSize(20);
  keyMappingText->setPosition(655, 600);
  keyMappingText->setLineSpacing(1.5);

  std::string filePathstr = cpu.getMemory().getFilePath();
  filePathText = new sf::Text();
  filePathText->setFont(*font);
  filePathText->setFillColor(sf::Color(190, 190, 190));
  filePathText->setString(filePathstr);
  filePathText->setCharacterSize(20);
  filePathText->setPosition(55, 600);
  filePathText->setLineSpacing(1.5);

  buttonsPress[0] = new sf::RectangleShape(sf::Vector2f(80, 22));
  buttonsPress[0]->setFillColor(sf::Color(0, 0, 120));
  buttonsPress[0]->setOutlineColor(sf::Color::Blue);
  buttonsPress[0]->setOutlineThickness(1);
  buttonsPress[0]->setPosition(654, 604);

  buttonsPress[1] = new sf::RectangleShape(sf::Vector2f(190, 22));
  buttonsPress[1]->setFillColor(sf::Color(0, 0, 120));
  buttonsPress[1]->setOutlineColor(sf::Color::Blue);
  buttonsPress[1]->setOutlineThickness(1);
  buttonsPress[1]->setPosition(754, 604);

  buttonsPress[2] = new sf::RectangleShape(sf::Vector2f(95, 22));
  buttonsPress[2]->setFillColor(sf::Color(0, 0, 120));
  buttonsPress[2]->setOutlineColor(sf::Color::Blue);
  buttonsPress[2]->setOutlineThickness(1);
  buttonsPress[2]->setPosition(960, 604);
}

Gui::~Gui() {}

void Gui::updateFlag() {
  for (size_t i = 0; i < flagsTiles.size(); i++) {
    if (cpu.getSR() & (0x01 << i)) {
      flagsTiles[i]->setFillColor(sf::Color::Red);
    } else {
      flagsTiles[i]->setFillColor(sf::Color(40, 40, 40));
    }
  }
}

std::string intTohexU8(int value) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << std::uppercase << std::hex
     << value;
  return ss.str();
}

std::string intTohexU16(int value) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(4) << std::uppercase << std::hex
     << value;
  return ss.str();
}
void Gui::updateRegisters() {
  registerSStr.str("");
  registerSStr << intTohexU16(cpu.getPC()) << " " << intTohexU8(cpu.getSP())
               << " " << intTohexU8(cpu.getAC()) << " "
               << intTohexU8(cpu.getX()) << " " << intTohexU8(cpu.getY()) << " "
               << intTohexU8(cpu.getSR()) << " ";

  registersText->setString(registerSStr.str());
}

void Gui::updateZeroPageMemory() {
  zeroPageDataStr = "";

  for (size_t i = 1; i <= 256; i++) {
    zeroPageDataStr += intTohexU8(cpu.getMemory().read(i - 1)) + " ";
    if (i % 16 == 0) {
      zeroPageDataStr += "\n";
    }
  }
  zeroPageDataText->setString(zeroPageDataStr);
}

void Gui::show() {

  while (window->isOpen()) {
    sf::Event event;
    while (window->pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window->close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && !buttonsLock[0]) {
        buttonsLock[0] = true;
        buttonsPress[0]->setFillColor(sf::Color::Blue);
        cpu.reset();

      } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::R) &&
                 buttonsLock[0]) {
        buttonsLock[0] = false;
        buttonsPress[0]->setFillColor(sf::Color(0, 0, 120));
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && !buttonsLock[1]) {
        buttonsLock[1] = true;
        buttonsPress[1]->setFillColor(sf::Color::Blue);
        cpu.next();
      } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::N) &&
                 buttonsLock[1]) {
        buttonsLock[1] = false;
        buttonsPress[1]->setFillColor(sf::Color(0, 0, 120));
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !buttonsLock[2]) {
        buttonsLock[2] = true;
        buttonsPress[2]->setFillColor(sf::Color::Blue);
      } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E) &&
                 buttonsLock[2]) {
        buttonsLock[2] = false;
        buttonsPress[2]->setFillColor(sf::Color(0, 0, 120));
      }
    }

    window->clear();
    window->draw(*gameScreenTitle);
    window->draw(*gameScreenInfo);
    window->draw(*gameScreen);

    updateFlag();
    updateRegisters();
    updateZeroPageMemory();

    window->draw(*flagsBar);
    for (auto &flag : flagsTiles) {
      window->draw(*flag);
    }
    window->draw(*flagsText);

    window->draw(*RegistersBar);
    for (auto &reg : registersTiles) {
      window->draw(*reg);
    }
    window->draw(*registersText);
    window->draw(*registersLabelText);

    window->draw(*zeroPageScreenTitle);
    window->draw(*zeropageScreen);

    window->draw(*zeroPageColumnsLabelText);
    window->draw(*zeroPageLinesLabelText);

    window->draw(*zeroPageDataText);

    for (auto &button : buttonsPress) {
      window->draw(*button);
    }

    window->draw(*keyMappingText);

    window->draw(*filePathText);

    window->display();
    flags++;
  }
}
