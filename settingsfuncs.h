using namespace std;

extern int led_count;
extern int defaultsetting;
extern bool refreshstatics;

extern std::vector<uint8_t> ledsetting;

extern string input(const string com);

std::vector<uint8_t> avsets = {0,11,12,13,14,15,16,17,18,19,20,21,31,51};

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
  int temp = 1;
  for(unsigned int i = 0; i < avsets.size(); i++){
    if(avsets[i] == prevset)temp = i;
  }
  if(avsets[temp] != prevset){
    temp = 1;
    cout << "bruh" << endl;
  }
  if(norp){
    if(temp == 0){ temp = avsets.size()-1; } else { temp--; }
    return avsets[temp];
  } else {
    if(temp == avsets.size()-1){ temp = 0; } else { temp++; }
    return avsets[temp];
  }
}



#if 0

struct moje_zmienne_t
{
int i = 0;
int j = 0;
};


moje_zmienne_t xxx;




xxx.i = 


int i;

#endif
