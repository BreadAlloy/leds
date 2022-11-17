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
extern void ledresize(bool manual, int setting = 21, int from = 0, int to = led_count, int n = 1);
extern void off(int delay, bool night, int offanim);
extern void on(bool now, int hour, int minute, int onanim, int b);
extern int chset(int prevset, bool norp);

extern bool turningon;
extern bool turningoff;
extern bool offrepeat;
extern bool stats;

char prevpress ='N';
int presses = 0;
int xpresses = 0;
const char *devkb = "/dev/input/by-path/platform-3f980000.usb-usb-0\:1.4\:1.0-event-kbd";
const char *devm = "/dev/input/by-path/platform-3f980000.usb-usb-0\:1.2\:1.1-event-mouse";
struct input_event evm, evkb;
ssize_t nm, nkb;
int fdm, fdkb;

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

bool readkey(int &op, struct input_event ev, ssize_t n, int fd) {
    while(true){
    n = read(fd, &ev, sizeof ev);
      if (n == (ssize_t)-1) {
        if (errno == EINTR){
          //continue;
        } else {
          cout << "Nope" << endl;
        }
      } else {
      if (n != sizeof ev) {
        errno = EIO;
        cout << "Nope" << endl;
      }}

      if (ev.type == EV_KEY && ev.value >= 1 && ev.value <= 2){
        //cout << ("%s 0x%04x (%d)\n", (int)ev.value, (int)ev.code, (int)ev.code);
        op = (int)ev.code;
        //op = 0;
        cout << (int)op << endl;

        if (op == 1) {
          xpresses ++;
        if(xpresses > 6) {
          running = false;
          fflush(stdout);
        }
        } else { xpresses = 0; }
      if(op == 14) {
        cout << "Back" << endl;
        return false;
        }
      return true;
      break;
      }
}}

void inputbymouse(){
  fdm = open(devm, O_RDONLY);

  if (fdm == -1) {
    fprintf(stderr, "Cannot open %s: %s.\n", devm, strerror(errno));
    cout << "Nope" << endl;
  }

  int op = 0;
  while(running) {
    readkey(op, evm, nm, fdm);

    if (op == 0) {///1
      cout << "emptey" << endl;
    } else {

    if (op == 272 || op == 273) {///right/left click
      if(sleeping == true){
        turningon = true;
        } else {
          if(!turningon){
          turningoff = true;
        }}
    } else {

    if (op == 274) {///1
      if(stats){
        stats = false;
        if(sleeping && !turningon && !turningoff && !offrepeat){
          for(int i = 230; i < 301; i++) {
            ledcolor[i] = 0;
        }}
      } else {
        stats = true;
      }
    } else {

    if (op == 0) {///1
      cout << "emptey" << endl;
    } else {
}}}}

}}

extern void delay(std::function<void()> func, int wait);

      void oning(){
        turningon = true;
      }


void inputbykeyboard(){
  fdkb = open(devkb, O_RDONLY);

  if (fdkb == -1) {
    fprintf(stderr, "Cannot open %s: %s.\n", devkb, strerror(errno));
    cout << "Nope" << endl;
  }

  int op = 0;
  while(running) {
    readkey(op, evkb, nkb, fdkb);

    if (op == 0) {///1
      cout << "emptey" << endl;
    } else {

    if (op == 41) { //`/2
      cout << "on" << endl;
      delay([](){turningon = true;}, 0);
      //turningon = true;
      //std::thread thread4(timer, [](){turningon = true;}, 0);
      //thread4.detach();
    } else {

    if (op <= 11 && op >= 2) { //1234567890/3
      cout << "off" << endl;
      if(op == 11) {
        op = 0;
          } else {
        op = op - 1; }
      turningoff = true;
      delay([](){turningoff = true;},op);
      //std::thread thread3(off, op, false, 1);
      //thread3.detach();
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
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) ledresize(false, chset(ledsetting[10], true));
        if (op == 27) ledresize(false, chset(ledsetting[10], false));
      }
    } else {

    if (op == 16) { //q/7
      cout << "setting change 1" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) ledresize(false, chset(ledsetting[desk[1]-10], true), desk[0], desk[1]);
        if (op == 27) ledresize(false, chset(ledsetting[desk[1]-10], false), desk[0], desk[1]);
      }
    } else {

    if (op == 17) { //w/8
      cout << "setting change 2" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) ledresize(false, chset(ledsetting[monitor[1]-10], true), monitor[0], monitor[1]);
        if (op == 27) ledresize(false, chset(ledsetting[monitor[1]-10], false), monitor[0], monitor[1]);
      }
    } else {

    if (op == 18) { //e/9
      cout << "setting change 3" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) ledresize(false, chset(ledsetting[abovedesk[1]-10], true), abovedesk[0], abovedesk[1]);
        if (op == 27) ledresize(false, chset(ledsetting[abovedesk[1]-10], false), abovedesk[0], abovedesk[1]);
      }
    } else {

    if (op == 19) { //r/10
      cout << "setting change 4" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) ledresize(false, chset(ledsetting[flowers[1]-10], true), flowers[0], flowers[1]);
        if (op == 27) ledresize(false, chset(ledsetting[flowers[1]-10], false), flowers[0], flowers[1]);
      }
    } else {

    if (op == 20) { //t/11
      cout << "setting change 5" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) ledresize(false, chset(ledsetting[bed[1]-10], true), bed[0], bed[1]);
        if (op == 27) ledresize(false, chset(ledsetting[bed[1]-10], false), bed[0], bed[1]);
      }
    } else {

    if (op == 31) { //s/12
      cout << "Speed" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
        if (op == 26) speed --;
        if (op == 27) speed ++;
        if (speed < 0) speed = 0;
      }
    } else {

    if (op == 30) { //a/13
      cout << "Length" << endl;
      while(readkey(op, evkb, nkb, fdkb)){
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
