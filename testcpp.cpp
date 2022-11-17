#include <vector>
#include "simpleleds.h"
#include <string>
#include <cmath>
#include <thread>
#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h> //
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <functional>

#include "overinternet.h"
#include "framerender.h"
#include "initsetup.h"
#include "inputbyterminal.h"
#include "directinput.h"
#include "settingsfuncs.h"
#include "remoteinput.h"
#include "colorfuncs.h"
#include "timefuncs.h"

using namespace std;

int led_count = 621;
int brightness = 100;

int desk[2] = {0, 187};
int monitor[2] = {188, 219};
int abovedesk[2] = {220, 313};
int transmitter = 313;
int flowers[2] = {314, 531};
int bed[2] = {531, 621};

int rainbowpoint = 0;
int lenght = 50;
int speed = 2;
int rainbowphase = 0;
int colorphase = 0;

//struct thread thread3;
//struct thread thread4;

int slowness = 10;
int defaultsetting = 21;
int onbrightness = 0;
int startbrightness = 3;
int colorcyclecolor = 0;

int offdelay = 0;

float specmod = 0.288;
bool refreshstatics = true;
bool sleeping = false;
//bool turningon = false;
bool running = true;

int bfps = 1;
int perbfps = 1000;


bool remotecontrol = false;
const int motionblur = 4;

//int new_socket; //

std::vector<uint8_t> ledsetting;
std::vector<uint32_t> ledcolor;
std::vector<std::vector<uint8_t>> ledrgb;
std::vector<std::vector<uint16_t>> ledrainbow;
std::vector<std::vector<uint8_t>> remoteled;


int main(int argc, char** argv)
{
  setup();

  std::thread thread1(reciever);

  string sterchoice;
  sterchoice = input("Choose method for direct input");
  if(sterchoice == "m") {
    std::thread thread2(inputbyterminal);
    thread2.detach();
    } else {
    std::thread thread2(inputbykeyboard);
    std::thread thread5(inputbymouse);
    thread2.detach();
    thread5.detach();
  }

//while(true) {
//  sleep(1);
//}

while (running)
  {
  ledcolorset();
  draw_leds(ledcolor.data());
  }

  sleep(0.5);
  fini_leds();
  return 0;
}
