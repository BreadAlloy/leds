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

#define PORT 8080

using namespace std;

int led_count = 621;
int brightness = 100;

int desk[2] = {0, 187};
int monitor[2] = {188, 220};
int abovedesk[2] = {219, 312};
int transmiter = 313;
int flowers[2] = {314, 530};
int bed[2] = {531, 620};

int rainbowpoint = 0;
int lenght = 100;
int speed = 2;
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

void off(){
  sleeping = true;
  for(int r = 0; r < 255; r++) {
  for(int i = 0; i < led_count; i++) {
  for (int j = 0; j < 3; j++){
     if (ledrgb[i][j] - 1 >= 0){
        ledrgb[i][j]--;
     }
     ledcolor[i] = color(ledrgb[i][2], ledrgb[i][1], ledrgb[i][0]);
  }}
  sleep(0.5);
  draw_leds(ledcolor.data());
}}

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
    off();
    }else{


}}}}
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
    sleep(10);
  } else {
    if (refreshstatics) {
      onetimefuncs();
      refreshstatics = false;
    }
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
    cout << lenght << endl;
    getspectrum();
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
        ledresize(true, false, defaultsetting);
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

}}} }}} }}} }}} }}}
    ledsetting[transmiter] = 0;
}}

void reciever() {
  ConnStart();
  char op[1024] = {0};
  while(running) {
    if (remotecontrol) {
    valread = recv(new_socket, op, 1024, 0);
    cout << op << endl;

    if (op[0] == 's') {
    remotecontrol = false;
    close(new_socket);
    } else {

    if (op[0] == 'd') {
    int coltemp[3];
    coltemp[0] = *(uint32_t*)(&op[1]);
    coltemp[1] = *(uint32_t*)(&op[2]);
    coltemp[2] = *(uint32_t*)(&op[3]);
    coltemp[0] = color(coltemp[0], coltemp[1], coltemp[2]);
    for (int i = 0; i<led_count; i++) {
    ledcolor[i] = coltemp[0]; }
    } else {

    if (op[0] == 'R') {
    ledresize(true, false, -1);
    } else {

    if (op[0] == 'A') {
    ledresize(true, false, defaultsetting);
    } else {

}}}}
  } else {
  Connection();
  remotecontrol = true;
}}}

int main(int argc, char** argv)
{
//  valread = read( new_socket , buffer, 1024);
//  printf("%s\n",buffer );
//  send(new_socket , hello , strlen(hello) , 0 );
//  printf("Hello message sent\n");

  init_leds(led_count);
  ledsetting.resize(led_count);
  ledcolor.resize(led_count);

  ledrainbow.resize(led_count * 20);
  for (int i = 0; i < led_count * 20; i++){
    ledrainbow[i].resize(3); }


  ledrgb.resize(led_count);
  for (int i = 0; i < led_count; i++){
    ledrgb[i].resize(3); }

  //Connection();
  ledsetall();
  getspectrum();
  std::thread thread1(reciever);
  std::thread thread2(operating);


while (running)
  {
  ledcolorset();
  draw_leds(ledcolor.data());
  }

  fini_leds();
}
