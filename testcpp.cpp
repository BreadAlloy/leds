#include <vector>
#include "simpleleds.h"
#include <string>
#include <cmath>
#include <thread>
#include <iostream>

using namespace std;

int led_count = 621;
int brightness = 100;

int desk[2] = {0, 187};
int monitor[2] = {188, 220};
int abovedesk[2] = {219, 312};
int transmiter = 313;
int flowers[2] = {314, 530};
int bed[2] = {531, 620};

float previ = 0;
float rainbowpoint = 0;
int lenght = 100;
int speed = 100;
float rainbowphase = 0;

bool refreshstatics = true;

std::vector<string> ledsetting;

std::vector<uint32_t> ledcolor;

bool running = true;

uint32_t color(uint16_t r, uint16_t g, uint16_t b, bool brightcorrect = false)
{
  if (brightcorrect){
    r = round(r/1000. * brightness);
    g = round(g/1000. * brightness);
    b = round(b/1000. * brightness);
    }
  return r << 16 | g << 8 | b;
}

int linearspectrum(float point){
  float mod = 0.333;
  int col[3];
  for(int i = 0; i <= 2; i++, point = point + 0.333){
    if (point > 1){
      point--;
    }
    if (point < 2 * mod){
      col[i] = round((-abs((point - 1) * mod) + 1)*1000);
      }else{
      col[i] = 0;
  }}
  return color(255 ,192 ,192 , false);
  }

int parabolicspectrum(float point){
  float mod = 0.288;
  int col[3];
  for(int i = 0; i <= 2; i++, point = point + 0.333){
    if (point > 1){
      point--;
    }
    if (point < 2 * mod){
      col[i] = round((-1 * (point/mod - 1) * (point/mod - 1) + 1)*1000);
      }else{
      col[i] = 0;
  }}
  return color(col[0], col[1], col[2], true);
  }

void rainbow(float i, float& previ, int lenght, float& rainbowpoint, std::vector<uint32_t>& ledcolor){
  rainbowpoint = rainbowpoint + (i - previ)/lenght;
  previ = i;
  while (rainbowpoint > 1)
    {
    rainbowpoint--;
    }
  ledcolor[i] = parabolicspectrum(rainbowpoint);
}

void repeatingfuncs(const std::vector<string>& ledsetting, std::vector<uint32_t>& ledcolor, float& rainbowphase){
for(int i = 0; i < led_count; i++){
  if (ledsetting[i] == "r1"){
    rainbow(i+rainbowphase, previ, lenght, rainbowpoint, ledcolor);
  }else{
}}
cout << rainbowphase << endl;
rainbowphase = rainbowphase + speed/100.;
if (rainbowphase > lenght) {
  rainbowphase = rainbowphase - lenght;
cout << rainbowphase << endl;
}}
//c1 = constant, e = empty

void ledsetall(std::vector<string>& ledsetting)
    {
    for (int i = 0; i < led_count; i++)
    {
        ledsetting[i] = "e";
    }
    for (int i = desk[0]; i <= desk[1]; i++)
    {
        ledsetting[i] = "r1";
    }
    for (int i = monitor[0]; i <= monitor[1]; i++)
    {
        ledsetting[i] = "r1";
    }
    for (int i = abovedesk[0]; i <= abovedesk[1]; i++)
    {
        ledsetting[i] = "r1";
    }
    for (int i = flowers[0]; i <= flowers[1]; i++)
    {
        ledsetting[i] = "r1";
    }
    for (int i = bed[0]; i <= bed[1]; i++)
    {
        ledsetting[i] = "r1";
    }}

int constant(int);

void onetimefuncs(const std::vector<string>& ledsetting, std::vector<uint32_t>& ledcolor){
for(int i = 0; i < led_count; i++){
  if(ledsetting[i] == "e")
    {
      ledcolor[i] = 0;
    } else {
    if(ledsetting[i] == "c1")
      {
        ledcolor[i] = constant(0);
      } else {
      if(ledsetting[i] == "c2")
        {
          ledcolor[i] = constant(1);
        } else {
        if(ledsetting[i] == "c3")
          {
            ledcolor[i] = constant(2);
          } else {
          if(ledsetting[i] == "c4")
            {
              ledcolor[i] = constant(3);
            } else {
            if(ledsetting[i] == "c5")
              {
                ledcolor[i] = constant(4);
              } else {
        }}}}}}}}


void ledcolorset(const std::vector<string>& ledsetting, std::vector<uint32_t>& ledcolor)
  {
  previ = -1;
  rainbowpoint = 0;
    if(refreshstatics){
      onetimefuncs(ledsetting ,ledcolor);
      refreshstatics = false;
    }
    repeatingfuncs(ledsetting ,ledcolor, rainbowphase);
}

////////////////////////
int r[5] = {0,0,40,1,0};
int g[5] = {100,50,40,1,1};
int b[5] = {0,0,40,1,0};

int constant(int i)
    {
    return color(b[i] ,g[i] ,r[i]);
    }

string input(const string& com)
    {
    string in;
    cout << com << endl;
    cin >> in;
    return in;
    }

void ledresize(std::vector<string>& ledsetting)
    {
    int from;
    int to;
    string setting;
    from = std::stoi(input("set form pixel"));
    to = std::stoi(input("set to pixel"));
    cout << "settings" << endl;
    setting = input("chose setting");
        for(int i = from; i <= to; i++)
        {
        ledsetting[i] = setting;
        }
    }

void off(std::vector<uint32_t>& ledcolor){
//for(int j = 0; j < 255; j++){
   for(int i = 0; i < led_count; i++){
      ledcolor[i] = 0;
      }
   draw_leds(ledcolor.data());
   }
//}

void operating(){
    string op;
    while (running) {
    op = input("Operation");
    if (op == "resize"){
    ledresize(ledsetting);
    refreshstatics = true;
    } else {
    if (op == "b") {
    brightness = std::stoi(input("Brightness"));
    } else {
    if (op == "rainbow") {
    lenght = std::stoi(input("lenght"));
    speed = std::stoi(input("speed"));
    } else {
    if (op == "off") {
    sleep(std::stoi(input("delay(min)")) * 60);
    running = false;
    off(ledcolor);
    } else {
    }}}}}}

void everynled(int i, int n, string function, std::vector<string>& ledsetting, int phase = 0){
  if(i % n == phase){
    ledsetting[i] = function;
  }else{
    ledsetting[i] = "e";
}}

int main(int argc, char** argv)
{
  init_leds(led_count);
  ledsetting.resize(led_count);
  ledcolor.resize(led_count);

  std::thread thread1(operating);
  ledsetall(ledsetting);

while (running)
  {
  sleep(1);
  ledcolorset(ledsetting, ledcolor);
  draw_leds(ledcolor.data());
  }

  fini_leds();
}
