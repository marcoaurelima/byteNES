#ifndef GUI_H
#define GUI_H

#include "Cpu.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
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
  sf::Text *gameScreenCount;
  int64_t clock{60};

  sf::Image *gameImage;
  sf::Texture *gameTexture;
  sf::Sprite *gameSprite;

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

  std::array<bool, 5> buttonsLock{};
  std::array<sf::RectangleShape *, 3> buttonsPress;

  Cpu &cpu;

  std::array<sf::Color, 0xFF> colors{};

  void loadFrameInMemory(uint16_t begin);

  void updateCpuCount();

  // Se for false, significa que está em modo 
  // de passo a passo; se não,
  // está em modo de R(E)sume.
  bool isDebugMode {true};
};

#endif
