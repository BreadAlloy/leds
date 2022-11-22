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

extern std::vector<uint8_t> r;
extern std::vector<uint8_t> g;
extern std::vector<uint8_t> b;
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
//extern void off(int delay, bool night, int offanim);
//extern void on(bool now, int hour, int minute, int onanim, int b);
extern int chset(int prevset, bool norp);
extern uint32_t secondsto(int sec = 0, int min = 0, int hour = 0, int days = 0);

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
//int r[5] = {0,0,255,1,0};
//int g[5] = {255,255,255,1,1};
//int b[5] = {0,255,255,1,0};

int booltocolor(bool status){
  if(status){
    return color(0,255,0,false); }
  return color(0,0,255,false);
}

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

#if 0
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
#endif

extern double now();
double temptime = 0;
int targetbrightness = brightness;
extern bool turningon;
extern bool turningoff;
extern bool offrepeat;
extern int onanim;
extern int offanim;

void on(){
  if(sleeping == true && turningon == true){
    sleeping = false;
    targetbrightness = brightness;
    brightness = 0;
  }

  if(onanim == 0){
    turningon = false;
    brightness = targetbrightness;
  } else {

  if(onanim == 1){
    if(abs(temptime - now()) >= 50000){
      brightness++;
      refreshstatics = true;
      temptime = now();
    }

    if(targetbrightness <= brightness){
      turningon = false;
    }

  } else {

  if(onanim == 2){
    
  } else {

  if(onanim == 3){
    
  } else {

  if(onanim == 4){
    
  } else {

}}}}}}

void off(){
  if(turningoff){
    turningoff = false;
    offrepeat = true;
    targetbrightness = brightness;
  }

  if(offanim == 0){
    if(brightness <= 0){
      offrepeat = false;
      allempty();
      brightness = targetbrightness;
    } else {brightness = 0;}
    sleeping = true;
  } else {

  if(offanim == 1){
    if(brightness <= 0){
      allempty();
      brightness = targetbrightness;
      offrepeat = false;
    } else {

    if(abs(temptime - now()) >= 50000){
        brightness--;
        refreshstatics = true;
        temptime = now();
      }

      if(brightness <= 0){
        sleeping = true;
      }
    }
  } else {

  if(offanim == 2){
    
  } else {

  if(offanim == 3){
    
  } else {

  if(offanim == 4){
    
  } else {

}}}}}}

void rainbow(int i){
  rainbowpoint = i*20+rainbowphase;
  while (rainbowpoint > 20 * led_count - 1){
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
