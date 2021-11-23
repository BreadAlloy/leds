#include <vector>
#include "simpleleds.h"
#include <string>
#include <cmath>
#include <thread>
#include <iostream>
#include <chrono>

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
int speed = 20;
int rainbowphase = 0;
int colorphase = 0;

int slowness = 10;
int defaultsetting = 21;
int onbrightness = 0;
int startbrightness = 3;
int colorcyclecolor = 0;

int offdelay = 0;

float specmod = 0.288;
bool refreshstatics = true;
bool sleeping = false;
bool turningon = false;
bool running = true;

int bfps = 1;
int perbfps = 1000;

std::vector<uint8_t> ledsetting;
std::vector<uint32_t> ledcolor;
std::vector<std::vector<uint8_t>> ledrgb;
std::vector<std::vector<uint8_t>> ledrainbow;

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

uint32_t color(uint16_t r, uint16_t g, uint16_t b, bool brightcorrect = false)
{
  if (brightcorrect){
    r = round(r/255. * brightness);
    g = round(g/255. * brightness);
    b = round(b/255. * brightness);
    }
  return r << 16 | g << 8 | b;
}

////////////////////////
int r[5] = {0,0,40,1,0};
int g[5] = {100,50,40,1,1};
int b[5] = {0,0,40,1,0};

int constant(int i)
    {
    return color(b[i] ,g[i] ,r[i] ,true);
    }

void getspectrum() {
  float point = 0;
  for(int i = 0; i < led_count * 20; i++) {
    point = point + i/(20*lenght);
    if(point > 1) point--;
    ledrainbow[i][0] = spectrum(point);
    ledrainbow[i][1] = spectrum(point+0.333);
    ledrainbow[i][2] = spectrum(point+0.666);
}}


void getrgb(){
  for (int i = 0; i < led_count; i++) {
     for (int j = 0; j < 3; j++) {
  ledrgb[i][j] = ledcolor[i] >> (j*8) & 0xff;
  }}
}

uint32_t secondsto(int sec = 0, int min = 0, int hour = 0, int days = 0){
  time_t now = time(0);
  tm* ltm = localtime(&now);
  hour --;
  min = min - offdelay;
  hour = hour - ltm->tm_hour;
  min = min - ltm->tm_min;
  sec = sec - ltm->tm_sec;
  if (hour < 0) {
    hour = hour + 24; }
  if (min < 0) {
    min = min + 60; }
  if (sec < 0) {
    sec = sec + 60; }
  return (hour+days*24)*3600+min*60+sec;
}

string input(const string& com)
    {
    string in;
    cout << com << endl;
    cin >> in;
    return in;
    }

void ledresize(bool all = false, bool everyn = false, int setting = 0)
    {
    int from;
    int to;
    int n = 1;
    if(all){
      from = 0;
      to = led_count;
    } else {
      from = std::stoi(input("set form pixel"));
      to = std::stoi(input("set to pixel"));
    }
    if (everyn){
      n = std::stoi(input("one per"));
    }
    if(setting == 0){
//////////////////////SETTINGS TABLE////////////////////
       cout << "xi-index, 0-empty, 1-static, 2-rainbow, 3-colorcycle, 4-off" << endl;
//////////////////////SETTINGS TABLE////////////////////
       setting = std::stoi(input("chose setting"));
    }
        for(int i = from; i < to; i++)
        {
            if (i % n == 0) {
                ledsetting[i] = setting;
            }
        }
    }

void off(int i){
  for (int j = 0; j < 3; j++){
     if (ledrgb[i][j] - 1 >= 0){
        ledrgb[i][j]--;
     }
  }
  ledcolor[i] = color(ledrgb[i][2], ledrgb[i][1], ledrgb[i][0]);
}

void on() {
    ledresize(true, false, defaultsetting);
    sleeping = false;
    if (perbfps - bfps == 0) {
        if (onbrightness - brightness > 0) {
            bfps = 1;
            brightness++;
            refreshstatics = true;
        } else {
            bfps = 1;
            turningon = false;
            cout << "Is on" << endl;
        }
    } else {
        bfps++;
    }
}

int spectrum(float point,float power = 2){
    if (point > 1){
      point--;
    if (point < 2 * specmod){
      return round((-1 * ((point/specmod - 1) * (point/specmod - 1)) + 1)*255);
      }else{
      return = 0;
  }}

void rainbow(int i){
  rainbowpoint = rainbowpoint + (i - previ)/lenght;
  previ = i;
  while (rainbowpoint > 20 * lenght){
    rainbowpoint = rainbowpoint - lenght * 20;
    }
  ledcolor[i] = spectrum(rainbowpoint);
}

void colorcycle(int i){
  ledcolor[i] = color(ledrainbow[i*20+rainbowphase][0], ledrainbow[i*20+rainbowphase][1], ledrainbow[i*20+rainbowphase][2], true);
  if (colorphase > lenght * 20) {
    colorphase = colorphase - lenght * 20;
  }
}

void timer(){
  int wait = secondsto(0, 50, 4);
  cout << wait << endl;
  sleep(wait);
  onbrightness = brightness;
  brightness = startbrightness;
  turningon = true;
}

void repeatingfuncs() {
if (turningon) {
  sleeping = false;
  on();
}

for(int i = 0; i < led_count; i++){

    if (ledsetting[i] == 21){
    rainbow(i);
    }else{

        if (ledsetting[i] == 31){
        colorcycle(i);
        }else{

    if (ledsetting[i] == 41){
    off(i);
    }else{


}}}}
colorphase = colorphase + speed;
rainbowphase = rainbowphase + speed;
colorcyclecolor = color(ledrainbow[colorphase][0], ledrainbow[colorphase][1], ledrainbow[colorphase][2]);
}

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

    if(ledsetting[i] == 14)
    {
    ledcolor[i] = constant(3);
    } else {

        if(ledsetting[i] == 15)
        {
        ledcolor[i] = constant(4);
        } else {

}}}}}}}}

void ledcolorset() {
  if (sleeping) {
  sleep(10);
    } else {
    if (refreshstatics) {
      onetimefuncs();
      refreshstatics = false;
    }
  previ = -1;
  rainbowpoint = rainbowphase/lenght;
  repeatingfuncs();
}}

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
    } else {

        if (op == "off") {
        sleep(std::stoi(input("delay(min)"))*60);
        getrgb();
        ledresize(true, false, 41);
        } else {

    if (op == "resizeall") {
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "static") {
        int n = std::stoi(input("index")) - 1;
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
        specmod = std::stoi(input("modify mod/1000"))/1000.;
        cout << specmod << endl;
        } else {

    if (op == "slowness") {
    slowness = std::stoi(input("modify slowness/1000"));
    cout << slowness << endl;
    } else {

        if (op == "help") {
        cout << "slowness, mod, resizeeveryn, static, resizeall, off, rainbow, resize, b, speed" << endl;
        } else {

    if (op == "speed") {
    speed = std::stoi(input("speed"));
    } else {

        if (op == "on") {
        sleep(std::stoi(input("delay(min)"))*60);
        sleeping = false;
        ledresize(true, false, defaultsetting);
        } else {

    if (op == "test") {
    int test = secondsto(50,34,16);
    sleep(test);
    turningon = true;
    } else {

        if (op == "timeron") {
        timer();
        } else {

}}} }}} }}} }}} }}
    ledsetting[transmiter] = 0;
}}

int main(int argc, char** argv)
{
  init_leds(led_count);
  ledsetting.resize(led_count);
  ledcolor.resize(led_count);

  ledrainbow.resize(led_count * 20);
  for (int i = 0; i < led_count * 20; i++){
  ledrgb[i].resize(3); }


  ledrgb.resize(led_count);
  for (int i = 0; i < led_count; i++){
  ledrgb[i].resize(3); }

  std::thread thread1(operating);
  ledsetall();

while (running)
  {
  sleep(slowness/1000.);
  ledcolorset();
  draw_leds(ledcolor.data());
  }

  fini_leds();
}
