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
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>

#define PORT 8080

using namespace std;

int led_count = 621;
int brightness = 100;

int desk[2] = {0, 187};
int monitor[2] = {188, 219};
int abovedesk[2] = {220, 313};
int transmiter = 313;
int flowers[2] = {314, 531};
int bed[2] = {531, 621};

int rainbowpoint = 0;
int lenght = 50;
int speed = 2;
int rainbowphase = 0;
int colorphase = 0;

struct thread thread3;
struct thread thread4;

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

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
//char *hello = "Hello from server";

bool remotecontrol = false;

std::vector<uint8_t> ledsetting;
std::vector<uint32_t> ledcolor;
std::vector<std::vector<uint8_t>> ledrgb;
std::vector<std::vector<uint16_t>> ledrainbow;
std::vector<uint32_t> remoteled;

double now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec/1000000.;
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

uint32_t color(uint16_t r, uint16_t g, uint16_t b, bool brightcorrect = false)
{
  if (brightcorrect){
    r = round(r/255. * brightness);
    b = round(b/255. * brightness);
    g = round(g/255. * brightness);
    }
  return r << 16 | g << 8 | b;
}

////////////////////////
int r[5] = {0,0,255,1,0};
int g[5] = {255,255,255,1,1};
int b[5] = {0,0,255,1,0};

int constant(int i)
    {
    return color(b[i] ,g[i] ,r[i] ,true);
    }

int spectrum(float point,float power = 2){
    if (point > 1) {
      point--; }
    if (point < 2 * specmod){
      return round((-1 * ((point/specmod - 1) * (point/specmod - 1)) + 1)*255);
    }else{
      return 0;
  }}

void getspectrum() {
  double point = 0;
  for(int i = 0; i < led_count * 20; i++) {
    point = point + 1./(20*lenght);
     if(point > 1) {point--;}
    ledrainbow[i][0] = spectrum(point);
    ledrainbow[i][1] = spectrum(point+0.3333);
    ledrainbow[i][2] = spectrum(point+0.6666);
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

int chartoint(char ch){
  return (unsigned char)ch;
}

string input(const string& com)
    {
    string in;
    cout << com << endl;
    cin >> in;
    return in;
    }

void ConnStart() {
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                  sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Connection() {
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
}
void ledresize(bool manual, int setting = defaultsetting, int from = 0, int to = led_count, int n = 1)
    {
    if(manual){
      from = std::stoi(input("set form pixel"));
      to = std::stoi(input("set to pixel"));
      n = std::stoi(input("one per"));
//////////////////////SETTINGS TABLE////////////////////
      cout << "xi-index, 0-empty, 1-static, 2-rainbow, 3-colorcycle, 4-off" << endl;
//////////////////////SETTINGS TABLE////////////////////
      setting = std::stoi(input("chose setting"));
    }
        for(int i = from; i < to; i++)
        {
            if (i % n == 0) {
                ledsetting[i] = setting;
            } else {
                ledsetting[i] = 0;
        }}
    refreshstatics = true;
    }

void indivde() {
  getrgb();
  for (int r = 0; r < brightness + 1; r++) {
  for (int i = 0; i < led_count; i++) {
  for (int j = 0; j < 3; j++){
     if (ledrgb[i][j] - 1 >= 0){
        ledrgb[i][j]--;
     }
     ledcolor[i] = color(ledrgb[i][2], ledrgb[i][1], ledrgb[i][0]);
  }}
  sleep(0.5);
  draw_leds(ledcolor.data());
}}

void allempty() {
  for(int i = 0; i < led_count; i++) {
    ledcolor[i] = color(0, 0, 0);
}}

void onebyone() {
  for(int i = 0; i < led_count; i++) {
    ledcolor[i] = color(0, 0, 0);
    draw_leds(ledcolor.data());
    sleep(0.5);
}}

void off(int delay = 0, bool night = false, int offanim = 0){
  sleep(60*delay);
  if(!sleeping) {
  sleeping = true;
  usleep(500);
  if(offanim == 0) {
    allempty();
  } else {

  if(offanim == 1) {
    indivde();
  } else {

  if(offanim == 2) {
    onebyone();
  } else {

  allempty();
}}}
  cout << "Is asleep" << endl;
}

if(night) {
  for(int i = 0; i < led_count; i = i + 2) {
    ledcolor[i] = color(0, 1, 0);
}}
draw_leds(ledcolor.data());
}

void on1() {
  std::vector<uint8_t> temp = ledsetting;
  ledresize(false, 0, 0, led_count);
  sleeping = false;
  for(int i = 0; i < led_count; i++) {
    ledsetting[i] = temp[i];
    sleep(0.24);
  }
}

void on(bool now = true, int hour = 0, int minute = 0, int onanim = 0, int b = brightness) {
  cout << "Timer on" << endl;
  if(!now) {
    sleep(secondsto(0, minute, hour));
  }

  if(sleeping){
  brightness = b;

  if(onanim == 1) {
    on1();
  } else {

  sleeping = false;
}}
  cout << "Is awake" << endl;
}

void rainbow(int i){
  rainbowpoint = i*20+rainbowphase;
  if (rainbowpoint > 20 * lenght){
    rainbowpoint = rainbowpoint - lenght * 20;
    }
  ledcolor[i] = color(ledrainbow[rainbowpoint][0], ledrainbow[rainbowpoint][1], ledrainbow[rainbowpoint][2], true);
}

void colorcycle(int i){
  ledcolor[i] = colorcyclecolor;
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
  sleeping = false;
  turningon = true;
}

void repeatingfuncs() {
if (turningon) {
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
    //off();
    }else{

        if (ledsetting[i] == 51){
        ledcolor[i] = remoteled[i];
        }else{

}}}}}
colorphase = colorphase + speed;
if (colorphase > 20 * lenght) {colorphase = colorphase - 20 * lenght;}
rainbowphase = rainbowphase + speed;
if (rainbowphase > 20 * lenght) {rainbowphase = rainbowphase - 20 * lenght;}
colorcyclecolor = color(ledrainbow[colorphase][0], ledrainbow[colorphase][1], ledrainbow[colorphase][2], true);
rainbowpoint = 0;
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
    sleep(1);
  } else {
    if (refreshstatics) {
      onetimefuncs();
      refreshstatics = false;
    }
  repeatingfuncs();
}}

void operatingwords(){
    string op;
    while (running) {
    op = input("Operation");

    if (op == "resize"){
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "b") {
        brightness = std::stoi(input("Brightness"));
        cout << brightness << endl;
        refreshstatics = true;
        } else {

    if (op == "rainbow") {
    lenght = std::stoi(input("lenght"));
    cout << lenght << endl;
    getspectrum();
    } else {

        if (op == "off") {
        sleep(std::stoi(input("delay(min)"))*60);
        getrgb();
        ledresize(false, 41);
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
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "mod") {
        specmod = std::stoi(input("modify mod/1000"))/1000.;
        cout << specmod << endl;
        getspectrum();
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
    getspectrum();
    } else {

        if (op == "on") {
        sleep(std::stoi(input("delay(min)"))*60);
        sleeping = false;
        ledresize(false);
        } else {

    if (op == "sleep") {
    sleeping = true;
    } else {

        if (op == "timeron") {
        timer();
        } else {

    if (op == "remoteoff") {
    remotecontrol = false;
    close(new_socket);
    } else {

        if (op == "settings") {
        for(int i = 0; i < led_count; i++){
        cout << ledsetting[i] << endl;
        }} else {

}}} }}} }}} }}} }}} }
    ledsetting[transmiter] = 0;
}}

char prevpress ='N';
int presses = 0;
int avsets[] = {0,11,12,13,14,15,21,31,51};
int xpresses = 0;
const char *dev = "/dev/input/by-path/platform-3f980000.usb-usb-0\:1.4\:1.0-event-kbd";
struct input_event ev;
ssize_t n;
int fd;


int chset(int prevset, bool norp) {
  int i = 0;
  int arrs = sizeof(avsets);
  for(i; (avsets[i] != prevset) && (i <= arrs); i++){}
  //cout << arrs << endl;          //arrs is wrong
  if (norp) {
    //[
    if (i-1 < 0) {
    i = 9; }
    //cout << avsets[i-1] << "," << i-1 << endl;
    return avsets[i-1];
  }
    //]
  if (i+2 > arrs) {
  i = 0; }
  //cout << avsets[i+1] << "," << i+1 <<endl;
  return avsets[i+1];
  //return prevset;
}

bool multipress(char op, int max) {
  if(prevpress == op) {
    presses ++;
    } else {
      prevpress = op;
      presses = 1;
    }

  if(presses >= max) {
    return true; }
  return false;
}

//static const char *const evval[3] = {
//    "RELEASED",
//    "PRESSED ",
//    "REPEATED"
//};

bool readkey(int &op) {
    while(true){
    n = read(fd, &ev, sizeof ev);
      if (n == (ssize_t)-1) {
        if (errno == EINTR){
          //continue;
        } else {
          cout << "Nope" << endl;
        }
      } else
      if (n != sizeof ev) {
        errno = EIO;
        cout << "Nope" << endl;
      }

      if (ev.type == EV_KEY && ev.value >= 1 && ev.value <= 2){
        //printf("%s 0x%04x (%d)\n", evval[ev.value], (int)ev.code, (int)ev.code);
        op = (int)ev.code;
        cout << (int)op << endl;

        if (op == 1) {
          xpresses ++;
        if(xpresses > 6) {
          off(0, false, 0);
          running = false;
          fflush(stdout);
        }
        } else { xpresses = 0; }
      if(op == 14) {
        return false;
        cout << "Back" << endl; }
      return true;
      break;
      }
}}

void operatingpress() {

  fd = open(dev, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
    cout << "Nope" << endl;
  }

  int op = 0;
  while(running) {
    readkey(op);

    if (op == 0) {///1
      cout << "emptey" << endl;
    } else {

    if (op == 41) { //`/2
      cout << "on" << endl;
      std::thread thread4(on, true, 0, 0, 1, brightness);
      thread4.detach();
    } else {

    if (op <= 11 && op >= 2) { //1234567890/3
      cout << "off" << endl;
      if(op == 11) {
        op = 0;
          } else {
        op = op - 1; }
      std::thread thread3(off, op, false, 1);
      thread3.detach();
    } else {

    if (op == 12) { //-/4
      cout << "b-" << endl;
      brightness = brightness - 5;
      if(brightness < 0) {
        brightness = 0; }
      refreshstatics = true;
    } else {

    if (op == 13) { //=/5
      cout << "b+" << endl;
      brightness = brightness + 5;
      if(brightness > 255) {
        brightness = 255; }
      refreshstatics = true;
    } else {

    if (op == 43) { //\/6
      cout << "setting change all" << endl;
      while(readkey(op)){
        if (op == 26) ledresize(false, chset(ledsetting[10], true));
        if (op == 27) ledresize(false, chset(ledsetting[10], false));
      }
    } else {

    if (op == 16) { //q/7
      cout << "setting change 1" << endl;
      while(readkey(op)){
        if (op == 26) ledresize(false, chset(ledsetting[desk[1]-10], true), desk[0], desk[1]);
        if (op == 27) ledresize(false, chset(ledsetting[desk[1]-10], false), desk[0], desk[1]);
      }
    } else {

    if (op == 17) { //w/8
      cout << "setting change 2" << endl;
      while(readkey(op)){
        if (op == 26) ledresize(false, chset(ledsetting[monitor[1]-10], true), monitor[0], monitor[1]);
        if (op == 27) ledresize(false, chset(ledsetting[monitor[1]-10], false), monitor[0], monitor[1]);
      }
    } else {

    if (op == 18) { //e/9
      cout << "setting change 3" << endl;
      while(readkey(op)){
        if (op == 26) ledresize(false, chset(ledsetting[abovedesk[1]-10], true), abovedesk[0], abovedesk[1]);
        if (op == 27) ledresize(false, chset(ledsetting[abovedesk[1]-10], false), abovedesk[0], abovedesk[1]);
      }
    } else {

    if (op == 19) { //r/10
      cout << "setting change 4" << endl;
      while(readkey(op)){
        if (op == 26) ledresize(false, chset(ledsetting[flowers[1]-10], true), flowers[0], flowers[1]);
        if (op == 27) ledresize(false, chset(ledsetting[flowers[1]-10], false), flowers[0], flowers[1]);
      }
    } else {

    if (op == 20) { //t/11
      cout << "setting change 5" << endl;
      while(readkey(op)){
        if (op == 26) ledresize(false, chset(ledsetting[bed[1]-10], true), bed[0], bed[1]);
        if (op == 27) ledresize(false, chset(ledsetting[bed[1]-10], false), bed[0], bed[1]);
      }
    } else {

    if (op == 31) { //s/12
      cout << "Speed" << endl;
      while(readkey(op)){
        if (op == 26) speed --;
        if (op == 27) speed ++;
        if (speed < 0) speed = 0;
      }
    } else {

    if (op == 30) { //a/13
      cout << "Length" << endl;
      while(readkey(op)){
        if (op == 26) lenght = lenght - 5;
        if (op == 27) lenght = lenght + 5;
        if (lenght < 1) lenght = 0;
        getspectrum();
      }
    } else {

    }}}} }}}} }}}} }
    op = 0;
  }
}



void reciever() {
  ConnStart();
  int op[1024] = {0};
  char buffer[1024];
  while(running) {
    if (remotecontrol) {
    valread = recv(new_socket, buffer, 1024, 0);
    for(int i = 0; i < 1024; i ++) {
    op[i] = chartoint(buffer[i]);
    }
    cout << op << endl;

    if (op[0] == 0) {
    remotecontrol = false;
    close(new_socket);
    } else {

    if (op[0] == 1) {
    cout << "frame" << endl;
    cout << op[1] << "," << op[2] << "," << op[3] << endl;
    for(int i = 0; i < led_count; i++ ) {
    remoteled[i] = color(op[1], op[2], op[3], true);
    //ledresize(true, false, defaultsetting);
    }} else {

    if (op[0] == 2) {
    int from = op[1] << 8 | op[2];
    int to = op[3] << 8 | op[4];
    cout << from << endl;
    cout << to << endl;
    cout << op[5] << endl;
    ledresize(false, op[5], from, to, op[6]);
    refreshstatics = true;
    } else {

    if (op[0] == 3) {
    brightness = op[1];
    refreshstatics = true;
    } else {

    if (op[0] == 4) {
    std::thread thread3(off, op[1], op[2], op[3]);
    thread3.detach();
    } else {

    if (op[0] == 5) {
    std::thread thread4(on, op[1], op[2], op[3], op[4], op[5]);
    thread4.detach();
    } else {

    if (op[0] == 6) {
        for(int i = 0; i < 341; i ++) {
            remoteled[i] = color(op[3*i], op[3*i-1], op[3*i-2], true);
        }
    valread = recv(new_socket, buffer, 1024, 0);
    for(int i = 0; i < 1024; i ++) {
    op[i] = chartoint(buffer[i]);
    }
        for(int i = 1; i < led_count-341; i ++) {
            remoteled[i+340] = color(op[3*i], op[3*i-1], op[3*i-2], true);
        }
    } else {

}}}}}}}
  } else {
  Connection();
  remotecontrol = true;
}}}

int main(int argc, char** argv)
{
  init_leds(led_count);
  ledsetting.resize(led_count);
  ledcolor.resize(led_count);

  remoteled.resize(led_count);

  ledrainbow.resize(led_count * 20);
  for (int i = 0; i < led_count * 20; i++){
    ledrainbow[i].resize(3); }

  ledrgb.resize(led_count);
  for (int i = 0; i < led_count; i++){
    ledrgb[i].resize(3); }

  ledsetall();
  getspectrum();
  std::thread thread1(reciever);

  string sterchoice;
  sterchoice = input("Choose method for direct input");
  if(sterchoice == "m") {
    std::thread thread2(operatingwords);
    thread2.detach();
    } else {
    std::thread thread2(operatingpress);
    thread2.detach();
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
