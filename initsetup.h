using namespace std;

extern int desk[];
extern int monitor[];
extern int abovedesk[];
extern int transmitter;
extern int flowers[];
extern int bed[];

extern int led_count;

extern std::vector<std::vector<uint8_t>> ledrgb;
extern std::vector<std::vector<uint16_t>> ledrainbow;
extern std::vector<std::vector<uint8_t>> remoteled;
extern std::vector<uint8_t> ledsetting;
extern std::vector<uint32_t> ledcolor;

extern void getspectrum();

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
    }
}

void setup(){
   init_leds(led_count);
   ledsetting.resize(led_count);
   ledcolor.resize(led_count);

   remoteled.resize(led_count);
   for (int i = 0; i < led_count; i++){
      remoteled[i].resize(3);
      remoteled[i][0] = 0;
      remoteled[i][1] = 0;
      remoteled[i][2] = 0;
   }

   ledrainbow.resize(led_count * 20);
   for (int i = 0; i < led_count * 20; i++){
      ledrainbow[i].resize(3); }

   ledrgb.resize(led_count);
   for (int i = 0; i < led_count; i++){
      ledrgb[i].resize(3); }

   ledsetall();
   getspectrum();
}
