#include "Gui.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Gui::Gui(Cpu &cpu) : cpu(cpu) {
  // Program screen
  window = new sf::RenderWindow(sf::VideoMode(1170, 660), "byteNES");
  // window->setVerticalSyncEnabled(true);

  gameScreen = new sf::RectangleShape(sf::Vector2f(256, 240));
  gameScreen->setPosition(50, 50);
  gameScreen->setFillColor(sf::Color(30, 30, 30));
  gameScreen->setOutlineColor(sf::Color(80, 80, 80));
  gameScreen->setOutlineThickness(1);
  gameScreen->setScale(sf::Vector2f(2, 2));

  gameImage = new sf::Image();
  gameImage->create(32, 30, sf::Color(15, 15, 15));

  gameTexture = new sf::Texture();
  gameTexture->loadFromImage(*gameImage);

  gameSprite = new sf::Sprite();
  gameSprite->setTexture(*gameTexture);
  gameSprite->setPosition(50, 50);
  gameSprite->setScale(16, 16);

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
  gameScreenInfo->setPosition(230, 15);

  // Game screen
  gameScreenCount = new sf::Text();
  gameScreenCount->setFont(*font);
  gameScreenCount->setFillColor(sf::Color::Magenta);
  gameScreenCount->setString("CONT: 1");
  gameScreenCount->setCharacterSize(20);
  gameScreenCount->setPosition(370, 15);

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

  colors[0x00] = sf::Color(0, 0, 0);
  colors[0x01] = sf::Color(255, 255, 255);
  colors[0x02] = sf::Color(0, 255, 0);
  colors[0x03] = sf::Color(0, 0, 255);
  colors[0x04] = sf::Color(255, 255, 0);
  colors[0x05] = sf::Color(255, 0, 255);
  colors[0x06] = sf::Color(0, 255, 255);
  colors[0x07] = sf::Color(255, 165, 0);
  colors[0x08] = sf::Color(255, 182, 193);
  colors[0x09] = sf::Color(128, 0, 128);
  colors[0x0A] = sf::Color(50, 205, 50);
  colors[0x0B] = sf::Color(64, 224, 208);
  colors[0x0C] = sf::Color(165, 42, 42);
  colors[0x0D] = sf::Color(255, 215, 0);
  colors[0x0E] = sf::Color(65, 105, 225);
  colors[0x0F] = sf::Color(128, 128, 0);
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

void Gui::updateCpuCount() {
  std::stringstream ss;
  ss << "COUNT: " << cpu.getCount();

  gameScreenCount->setString(ss.str());
}

void Gui::show() {

  sf::Clock timer;
  timer.restart();

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
        isDebugMode = false;
      } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E) &&
                 buttonsLock[2]) {
        buttonsLock[2] = false;
        buttonsPress[2]->setFillColor(sf::Color(0, 0, 120));
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        buttonsLock[3] = true;
        clock += 10;
        std::stringstream ss;
        ss << "CLOCK " << clock << " Hz";
        gameScreenInfo->setString(ss.str());
      } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
                 buttonsLock[3]) {
        buttonsLock[3] = false;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        buttonsLock[4] = true;
        clock -= 10;
        std::stringstream ss;
        ss << "CLOCK " << clock << " Hz";
        gameScreenInfo->setString(ss.str());
      } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
                 buttonsLock[4]) {
        buttonsLock[4] = false;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
       cpu.getMemory().write(0xFF, 0x77);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
       cpu.getMemory().write(0xFF, 0x64);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
       cpu.getMemory().write(0xFF, 0x73);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
       cpu.getMemory().write(0xFF, 0x61);
      }

    }

    window->clear();
    window->draw(*gameScreenTitle);
    window->draw(*gameScreenInfo);
    window->draw(*gameScreenCount);
    window->draw(*gameScreen);

    updateFlag();
    updateRegisters();
    updateZeroPageMemory();
    updateCpuCount();

    loadFrameInMemory(0x0200);

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

    window->draw(*gameSprite);

    window->display();
    flags++;

    if (timer.getElapsedTime().asMilliseconds() > (1000 / clock)) {
      if (!isDebugMode) {
        timer.restart();
        cpu.next();
      }
    }
  }
}

// 256 x 240
void Gui::loadFrameInMemory(uint16_t begin) {
  uint16_t i = 0;
  for (size_t y = 0; y < 30; y++) {
    for (size_t x = 0; x < 32; x++) {
      uint8_t value = cpu.getMemory().read(begin + i++);

      uint8_t index = value & 0x0F;
      sf::Color color = colors[index];
      gameImage->setPixel(x, y, color);
      gameTexture->loadFromImage(*gameImage);
      gameSprite->setTexture(*gameTexture);
    }
  }
}
