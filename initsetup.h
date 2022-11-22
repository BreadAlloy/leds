using namespace std;

//#include "variables.h"

//global_leds leds;


extern	int led_count;

extern	std::vector<uint8_t> r;
extern	std::vector<uint8_t> g;
extern	std::vector<uint8_t> b;

 extern int file;

extern	std::vector<uint8_t> ledsetting;
extern	std::vector<uint32_t> ledcolor;
extern	std::vector<std::vector<uint8_t>> ledrgb;
extern	std::vector<std::vector<uint16_t>> ledrainbow;
extern	std::vector<std::vector<uint8_t>> remoteled;
extern	std::vector<unsigned char> settings;



extern void getspectrum();

extern void Load(int fd);

void setup(){
   ledsetting.resize(led_count);
   ledcolor.resize(led_count);

   remoteled.resize(led_count);
   for (int i = 0; i < led_count; i++){
      remoteled[i].resize(3);
      remoteled[i][0] = 100;
      remoteled[i][1] = 0;
      remoteled[i][2] = 0;
   }

r.resize(10);
g.resize(10);
b.resize(10);
for(int i = 0; i < 10; i++){
r[i] = 0;
g[i] = 0;
b[i] = 0;
}
   ledrainbow.resize(led_count * 20);
   for (int i = 0; i < led_count * 20; i++){
      ledrainbow[i].resize(3); }


   ledrgb.resize(led_count);
   for (int i = 0; i < led_count; i++){
      ledrgb[i].resize(3); }

   settings.resize(ledsetting.size()+3*ledcolor.size()+12+r.size()+g.size()+b.size());


   file = open("save.txt", O_RDWR);

						led_count = 406;
   Load(file);

						led_count = 406;
   init_leds(led_count);

   //ledsetall();
   getspectrum();
}
