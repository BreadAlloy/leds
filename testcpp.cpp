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
float colorphase = 0;

float mod = 0.288;
bool refreshstatics = true;

std::vector<uint8_t> ledsetting;
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

int spectrum(float point, power = 2){
  int col[3];
  for(int i = 0; i <= 2; i++, point = point + 0.333){
    if (point > 1){
      point--;
    }
    if (point < 2 * specmod){
      col[i] = round(-abs(pow((point/specmod - 1), power) + 1)*1000);
      }else{
      col[i] = 0;
  }}
  return color(col[0], col[1], col[2], true);
  }

void rainbow(float i){
  rainbowpoint = rainbowpoint + (i - previ)/lenght;
  previ = i;
  while (rainbowpoint > 1)
    {
    rainbowpoint--;
    }
  ledcolor[i] = spectrum(rainbowpoint);
}

void colorcycle(int i){
  ledcolor[i] = spectrum(colorphase);
  colorphase = colorphase + speed/100.;
  if (colorphase > lenght) {
    colorphase = colorphase - lenght;
  }
}

void repeatingfuncs (){
for(int i = 0; i < led_count; i++){
  if (ledsetting[i] == 21){
    rainbow(i+rainbowphase);
  }else{
    if (ledsetting[i] == 31){
      colorcycle(i);
    }else{
}}
rainbowphase = rainbowphase + speed/(100.;
}

void ledsetall()
    {
    for (int i = 0; i < led_count; i++)
    {
        ledsetting[i] = 0;
    }
    for (int i = desk[0]; i <= desk[1]; i++)
    {
        ledsetting[i] = 21;
    }
    for (int i = monitor[0]; i <= monitor[1]; i++)
    {
        ledsetting[i] = 21;
    }
    for (int i = abovedesk[0]; i <= abovedesk[1]; i++)
    {
        ledsetting[i] = 21;
    }
    for (int i = flowers[0]; i <= flowers[1]; i++)
    {
        ledsetting[i] = 21;
    }
    for (int i = bed[0]; i <= bed[1]; i++)
    {
        ledsetting[i] = 21;
    }}

int constant(int);

void onetimefuncs(){
for(int i = 0; i < led_count; i++){

    if(ledsetting[i] == 0)
    {
    ledcolor[i] = 0;
    } else {

        if(ledsetting[i] == 11)
        {
        ledcolor[i] = constant(0);
        } else {

    if(ledsetting[i] == 12)
    {
    ledcolor[i] = constant(1);
    } else {

        if(ledsetting[i] == 13)
        {
        ledcolor[i] = constant(2);
        } else {

    if(ledsetting[i] == 14 
    {
    ledcolor[i] = constant(3);
    } else {

        if(ledsetting[i] == 15)
        {
        ledcolor[i] = constant(4);
        } else {

}}}}}}}}


void ledcolorset()
  {
  previ = -1;
  rainbowpoint = rainbowphase/lenght;
    if(refreshstatics){
      onetimefuncs();
      refreshstatics = false;
    }
    repeatingfuncs();
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

void ledresize(bool all = false, bool everyn = false)
    {
    int from;
    int to;
    int setting;
    int n = 1;
    if(all){
      from = 0;
      to = led_count;
    } else {
      from = std::stoi(input("set form pixel"));
      to = std::stoi(input("set to pixel"));
    }
    if (everyn){
      n = std::stoi(input("one per));
    }
//////////////////////SETTINGS TABLE////////////////////
    cout << "xi-index, 0-empty, 1-static, 2-rainbow, 3-colorcycle" << endl;
//////////////////////SETTINGS TABLE////////////////////

    setting = std::stoi(input("chose setting"));
        for(int i = from; i < to; i++)
        {
            if (i % n == 0) {
                ledsetting[i] = setting;
            }
        }
    }

void off(){
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
    ledresize();
    refreshstatics = true;
    } else {

        if (op == "b") {
        brightness = std::stoi(input("Brightness"));
        cout << brightness << endl;
        refreshstatics = true;
        } else {

    if (op == "rainbow") {
    lenght = std::stoi(input("lenght"));
    speed = std::stoi(input("speed"));
    } else {

        if (op == "off") {
        sleep(std::stoi(input("delay(min)")) * 60);
        running = false;
        off();
        } else {

    if (op == "resizeall") {
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "static") {
        int n = std::stoi(input("index"));
        r[n] = std::stoi(input("red"));
        g[n] = std::stoi(input("green"));
        b[n] = std::stoi(input("blue"));
        refreshstatics = true;
        } else {

    if (op == "resizeeveryn") {
    ledresize(true, true);
    refreshstatics = true;
    } else {

        if (op == "mod") {
        mod = std::stoi(input("modify mod/1000"))/1000.;
        cout << mod << endl;
        } else {

}}}}}}}}}}

int main(int argc, char** argv)
{
  init_leds(led_count);
  ledsetting.resize(led_count);
  ledcolor.resize(led_count);

  std::thread thread1(operating);
  ledsetall();

while (running)
  {
  sleep(1);
  ledcolorset();
  draw_leds(ledcolor.data());
  }

  fini_leds();
}
