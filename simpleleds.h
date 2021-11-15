#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


extern "C"
{
  int init_leds(size_t led_count);
  int draw_leds(uint32_t* leds);
  void fini_leds();
}
