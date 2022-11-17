
using namespace std;

extern int desk[];
extern int monitor[];
extern int abovedesk[];
extern int transmitter;
extern int flowers[];
extern int bed[];

extern int brightness;
extern int speed;
extern int lenght;
extern int led_count;
extern int rainbowpoint;
extern int rainbowphase;
extern int colorphase;
extern int colorcyclecolor;
extern int defaultsetting;
extern float specmod;
extern bool refreshstatics;
extern bool sleeping;
extern bool running;
extern bool turningon;
extern int startbrightness;
extern int offdelay;
extern int onbrightness;

extern std::vector<std::vector<uint8_t>> ledrgb;
extern std::vector<std::vector<uint16_t>> ledrainbow;
extern std::vector<std::vector<uint8_t>> remoteled;
extern std::vector<uint8_t> ledsetting;
extern std::vector<uint32_t> ledcolor;

extern uint32_t color(uint16_t r, uint16_t g, uint16_t b, bool brightcorrect);
extern void rainbow(int i);
extern void colorcycle(int i);
extern int constant(int i);
extern void ledresize(bool manual, int setting, int from, int to, int n);
extern void off(int delay, bool night, int offanim);
extern void on(bool now, int hour, int minute, int onanim, int b);
extern int chset(int prevset, bool norp);

double now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec*1000000 + tv.tv_usec;
}

uint32_t secondsto(int sec, int min, int hour, int days){
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

void timer(std::function<void()> func, int wait = 0){
  cout << wait << endl;
  sleep(wait);
  func();
}

void delay(std::function<void()> func, int wait = 0){
  std::thread thread4(timer, func, wait);
  thread4.detach();
}


