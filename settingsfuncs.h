using namespace std;

extern int led_count;
extern int defaultsetting;
extern bool refreshstatics;

extern std::vector<uint8_t> ledsetting;

extern string input(const string com);

int avsets[] = {0,11,12,13,14,15,21,31,51};

void ledresize(bool manual, int setting, int from, int to, int n)
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
