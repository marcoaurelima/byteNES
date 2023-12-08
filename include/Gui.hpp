#ifndef GUI_H
#define GUI_H

#include "Cpu.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstdint>
#include <sstream>

class Gui {
public:
  Gui(Cpu &cpu);
  ~Gui();

  void show();

private:
  sf::RenderWindow *window;
  sf::Font *font;

  sf::RectangleShape *gameScreen;
  sf::Text *gameScreenTitle;
  sf::Text *gameScreenInfo;
  uint64_t clock {1};

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

  std::array<bool, 3> buttonsLock{};
  std::array<sf::RectangleShape *, 3> buttonsPress;

  Cpu &cpu;
};

#endif
