#include <vector>
#include "simpleleds.h"


uint32_t color(uint8_t r, uint8_t g, uint8_t b)
{
  return r << 16 | g << 8 | b;
}

int main(int argc, char** argv)
{
  init_leds(400);
  std::vector<uint32_t> ledy;
  ledy.resize(400);
  for (int i = 0; i < 400; i++) {
    ledy[i] = color(i * 0.5, i, i * 0.2);
  }
  draw_leds(ledy.data());
  fini_leds();
}
