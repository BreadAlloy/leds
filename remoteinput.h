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
extern const int motionblur;
extern bool refreshstatics;
extern bool sleeping;
extern bool running;
extern bool remotecontrol;

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
extern void ConnectionSetup();
extern void Connection();

int chartoint(char ch){
  return (unsigned char)ch;
}

void reciever() {
  ConnectionSetup();
  int valread;
  int op[1024] = {0};
  char buffer[1024];
  int new_socket;
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
        for(int j = 0; j <= 2; j++) {
            remoteled[i][j] = op[j+1];
        }
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
//    std::thread thread3(off, op[1], op[2], op[3]);
 //   thread3.detach();
    } else {

    if (op[0] == 5) {
 //   std::thread thread4(on, op[1], op[2], op[3], op[4], op[5]);
  //  thread4.detach();
    } else {

    if (op[0] == 6) {
        for(int i = 1; i < 341; i ++) {
            for(int j = 0; j <= 2; j++) {
            remoteled[i][j] = (motionblur * remoteled[i][j] + op[3*i-j])/(motionblur+1); }
        }
    valread = recv(new_socket, buffer, 1024, 0);
    for(int i = 0; i < 1024; i ++) {
    op[i] = chartoint(buffer[i]);
    }
        for(int i = 341; i < led_count; i ++) {
            for(int j = 0; j <= 2; j++) {
            remoteled[i][j] = 0;}//(motionblur * remoteled[i][j] + op[3*i-j])/(motionblur+1); }
        }
    } else {

}}}}}}}
  } else {
  Connection(new_socket);
  remotecontrol = true;
}}}
