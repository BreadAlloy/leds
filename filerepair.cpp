#include <vector>
#include <iostream>
#include <string>

#include "savingstatus.h"

using namespace std;

std::vector<unsigned char> settings;
int led_count = 406;
int brightness = 100;
std::vector<uint32_t> ledcolor;
std::vector<uint8_t> ledsetting;
//std::vector<std::vector<uint16_t>> ledrainbow;
bool stats = false;
bool sleeping = false;
int lenght = 50;
int speed = 5;
int rainbowphase = 0;
std::vector<uint8_t> r;
std::vector<uint8_t> g;
std::vector<uint8_t> b;
int file;
int onanim = 0;
int offanim = 0;
std::vector<std::vector<uint8_t>> remoteled;

void getspectrum(){};

int main(int argc, char** argv){

   ledsetting.resize(led_count);
   for(int i = 0; i < led_count; i++) ledsetting[i] = 21;

   ledcolor.resize(led_count);
   for(int i = 0; i < led_count; i++) ledcolor[i] = 0x00FF00;

   r.resize(10);
   g.resize(10);
   b.resize(10);
   for(int i = 0; i < 10; i++){
      r[i] = 0;
      g[i] = 100;
      b[i] = 0;
   }

#if 0
   ledrainbow.resize(led_count * 20);
   for (int i = 0; i < led_count * 20; i++){
      ledrainbow[i].resize(3);
      ledrainbow[i][0] = 0;
      ledrainbow[i][1] = 50;
      ledrainbow[i][2] = 0;
   }
#endif

   settings.resize(ledsetting.size()+3*ledcolor.size()+12+r.size()+g.size()+b.size());



   file = open("save.txt", O_RDWR);

   cout << "Enter amount of pixels:";
   cin >> led_count;
   led_count = 406;

   Save(file);
   cout << "\n" << "Repair should be done";
   close(file);
}

