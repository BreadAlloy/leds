using namespace std;

extern int transmitter;

extern int speed;
extern int lenght;
extern int led_count;
extern int rainbowpoint;
extern int rainbowphase;
extern int colorphase;
extern int colorcyclecolor;
extern bool refreshstatics;
extern bool sleeping;
extern bool running;

extern std::vector<std::vector<uint8_t>> ledrgb;
extern std::vector<std::vector<uint16_t>> ledrainbow;
extern std::vector<std::vector<uint8_t>> remoteled;
extern std::vector<uint8_t> ledsetting;
extern std::vector<uint32_t> ledcolor;

extern uint32_t color(uint16_t r, uint16_t g, uint16_t b, bool brightcorrect);
extern void rainbow(int i);
extern void colorcycle(int i);
extern int constant(int i);

bool stats = true;
bool turningon = false;
bool turningoff = false;
bool offrepeat = false;
bool fpscounter = true;
extern void off();
extern void on();
extern int booltocolor(bool status);
//int onanim = 1;
//int temptime = 0;

void renderstats(){
  for(int i = 230; i < 301; i++){ ledcolor[i] = 0; }
  ledcolor[230] = ledcolor[231] = ledcolor[299] = ledcolor[300] = color(255, 0, 0, false);
  ledcolor[233] = booltocolor(running);
  ledcolor[234] = booltocolor(turningon);
  ledcolor[235] = booltocolor(turningoff || offrepeat);
  ledcolor[236] = booltocolor(sleeping);
  ledcolor[238] = ledcolor[242] = ledcolor[299] = ledcolor[300] = color(255, 0, 0, false);
  ledcolor[240] = booltocolor(fpscounter);
  fpscounter = !fpscounter;
}

void repeatingfuncs() {
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
        ledcolor[i] = color(remoteled[i][0], remoteled[i][1], remoteled[i][2], true);
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

        if(ledsetting[i] >= 11 && ledsetting[i] <= 20)
        {
        ledcolor[i] = constant(ledsetting[i]-11);
        } else {

}}}}

void ledcolorset() {
  if (sleeping) {
    sleep(1);

    if (turningoff || offrepeat){
      off();
    } else {
        if(turningon) {
          on();
    }}
  } else {

  if (turningoff || offrepeat){
    off();
  } else {
      if(turningon) {
        on();
  }}

  if (refreshstatics) {
    onetimefuncs();
    refreshstatics = false;
  }
  repeatingfuncs();
  ledsetting[transmitter] = 0;
}
  if(stats) {
  renderstats();
}}
