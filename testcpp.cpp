#include <vector>
#include "simpleleds.h"
#include <string>
#include <cmath>
using namespace std;

int led_count = 621;
int brightness = 100;

int desk[2] = {0, 187};
int monitor[2] = {188, 220};
int abovedesk[2] = {219, 312};
int transmiter = 313;
int flowers[2] = {314, 530};
int bed[2] = {531, 620};

uint32_t color(uint8_t r, uint8_t g, uint8_t b, brightcorrect = false)
{
  if (brightcorrect){
    r = round(r/1000 * brightness);
    g = round(g/1000 * brightness);
    b = round(b/1000 * brightness);
    }
  return r << 16 | g << 8 | b;
}
//c1 = constant, e = empty

void ledsetall(std::vector<string>& ledsetting)
    {
    for (int i = 0; i < led_count; i++)
    {
        ledsetting[i] = "e";
    }
    for (int i = desk[0]; i <= desk[1]; i++)
    {
        ledsetting[i] = "c1";
    }
    for (int i = monitor[0]; i <= monitor[1]; i++)
    {
        ledsetting[i] = "c1";
    }
    for (int i = abovedesk[0]; i <= abovedesk[1]; i++)
    {
        ledsetting[i] = "c1";
    }
    for (int i = flowers[0]; i <= flowers[1]; i++)
    {
        ledsetting[i] = "c1";
    }
    for (int i = bed[0]; i <= bed[1]; i++)
    {
        ledsetting[i] = "c1";
    }}

int constant1();
int constant2();
int constant3();
int constant4();
int constant5();

void ledcolorset(const std::vector<string>& ledsetting, std::vector<uint32_t>& ledcolor)
    {
    for(int i = 0; i < led_count; i++)
        {
        if(ledsetting[i] == "e")
            {
            
            } else {
            if(ledsetting[i] == "c1")
                {
                ledcolor[i] = constant1();
                } else {
                    if(ledsetting[i] == "c2")
                    {
                    ledcolor[i] = constant2();
                    } else {
                        if(ledsetting[i] == "c3")
                        {
                        ledcolor[i] = constant3();
                        } else {
                            if(ledsetting[i] == "c4")
                            {
                            ledcolor[i] = constant4();
                            } else {
                                if(ledsetting[i] == "c5")
                                {
                                ledcolor[i] = constant5();
                                } else {

        }}}}}}}
    }

int r = 0;
int g = 100;
int b = 0;

int constant1()
    {
    return color(b ,g ,r);
    }

int constant2()
    {
    return color(0 ,100 ,0);
    }

int constant3()
    {
    return color(0, 0, 100);
    }

int constant4()
    {
    return color(50, 50, 50);
    }

int constant5()
    {
    return color(0, 0, 0);
    }

int linearspectrum(float point){
  float mod = 0.333;
  int col[3];
  for(int i = 0; i <= 3; i++, point = point + 0.333){
    if (point > 1){
      point--;
    }
    if (point < 2 * mod){
      col[i] = (-abs((point - 1) * mod) + 1)*1000;
      }else{
      col[i] = 0;
  }}
  return color(col[1] ,col[2] ,col[3], true);
  }

int parabolicspectrum(float point){
  float mod = 0.333;
  int col[3];
  for(int i = 0; i <= 3; i++, point = point + 0.333){
    if (point > 1){
      point--;
    }
    if (point < 2 * mod){
      col[i] = (pow((point - 1) * mod, 2) + 1)*1000;
      }else{
      col[i] = 0;
  }}
  return color(col[1] ,col[2] ,col[3], true);
  }

void everynled(int i, int n, string function, std::vector<string>& ledsetting, int phase = 0){
  if(i %% n == phase){
    ledsetting[i] = function;
  }else{
    ledsetting[i] = "e";
}}

previ = 0;
float rainbowpoint = 0;
int lenght = 100;
int speed = 2;

void rainbow(int i, float& rainbowpoint, std::vector<uint32_t>& ledcolor){
  rainbowpoint = rainbowpoint + (i - previ)/lenght;
  previ = i;
  while (rainbowpoint > 1)
    {
    rainbowpoint--;
    }
  ledcolor[i] = parabolicspectrum(rainbowpoint);
}

void repeatingfuncs(int i; const std::vector<string>& ledsetting, std::vector<uint32_t>& ledcolor){
  if (ledsetting[i] == "r1"){
    rainbow(i, rainbowpoint, ledcolor);
  }else{
  
}}

int main(int argc, char** argv)
{
  init_leds(led_count);

  std::vector<string> ledsetting;
  ledsetting.resize(led_count);

  std::vector<uint32_t> ledcolor;
  ledcolor.resize(led_count);

  ledsetall(ledsetting);
  ledcolorset(ledsetting, ledcolor);

  draw_leds(ledcolor.data());
  sleep(1);
  fini_leds();
}
