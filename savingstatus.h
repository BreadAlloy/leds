#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <assert.h>

using namespace std;

extern std::vector<unsigned char> settings;
extern int led_count;
extern std::vector<uint32_t> ledcolor;
extern std::vector<uint8_t> ledsetting;
extern bool stats;
extern bool sleeping;
extern int lenght;
extern int speed;
extern int rainbowphase;
extern int brightness;
extern int onanim;
extern int offanim;
extern std::vector<uint8_t> r;
extern std::vector<uint8_t> g;
extern std::vector<uint8_t> b;

extern std::vector<std::vector<uint8_t>> remoteled;

//extern uint_32t color(uint16_t r, uint32_t g, uint32_t b, bool brightcorrect);
extern void getspectrum();

void Save(int fd){
int temp;
settings[0] = led_count;
settings[1] = led_count >> 8;
settings[2] = brightness;
settings[3] = stats;
settings[4] = sleeping;
settings[5] = lenght;
settings[6] = lenght >> 8;
settings[7] = speed;
settings[8] = rainbowphase;
settings[9] = rainbowphase >> 8;
settings[10] = onanim;
settings[11] = offanim;
temp = 3*ledcolor.size()+12;
for(int i = 12; i < temp; i++){
   int n = (i-12)%3;
   settings[i] = ledcolor[i-(i%3)/3] >> (8*(n));
}
for(int i = ledsetting.size() - 1; i >= 0; i--){
   settings[temp+i] = ledsetting[i];
}
temp = temp + ledsetting.size();
for(int i = r.size() - 1; i >= 0; i--){
settings[temp+i] = r[i];
}
temp = temp + r.size();
for(int i = g.size() - 1; i >= 0; i--){
settings[temp+i] = g[i];
}
temp = temp + g.size();
for(int i = b.size() - 1; i >= 0; i--){
settings[temp+i] = b[i];
}
temp = temp + b.size();

lseek(fd, 0, SEEK_SET);
write(fd, settings.data(), settings.size());
}

					//ledcolor save and load probaly does not work
void Load(int fd){
int temp;
lseek(fd, 0, SEEK_SET);
read(fd, settings.data(), settings.size());

led_count = settings[1] << 8 | settings[0];
led_count = 406;

brightness = settings[2];
stats = settings[3];
sleeping = settings[4];
//sleeping = false;
lenght = settings[5] | settings[6] << 8;
//lenght = 30;
getspectrum();
speed = settings[7];
//speed = 1;
rainbowphase = settings[8] | settings[9] << 8;
//rainbowpoint = 0;
onanim = settings[10];
//onanim = 0;
offanim = settings[11];
//offanim = 0;
temp = 3*ledcolor.size()+12;
for(int i = 12; i < temp; i=i+3){
  ledcolor[(i-12)/3] = settings[i] | settings[i+1] >> 8 | settings[i+2] >> 16;
//   ledcolor[i] = 0xaaaaaa;
}

for(int i = 0; i < ledsetting.size(); i++){
   ledsetting[i] = settings[temp+i];
//   ledsetting[i] = 21;
}
temp = temp + ledsetting.size();
for(int i = r.size() - 1; i >= 0; i--){
r[i] = settings[temp+i];
}

temp = temp + r.size();
for(int i = g.size() - 1; i >= 0; i--){
g[i] = settings[temp+i];
}

temp = temp + g.size();
for(int i = b.size() - 1; i >= 0; i--){
b[i] = settings[temp+i];
}
temp = temp + b.size();

}


