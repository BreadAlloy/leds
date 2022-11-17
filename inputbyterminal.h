using namespace std;

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
extern string input(const string com);

string input(const string com)
    {
    string in;
    cout << com << endl;
    cin >> in;
    return in;
    }

void inputbyterminal(){
    string op;
    while (running) {
    op = input("Operation");
#if 0
    if (op == "resize"){
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "b") {
        brightness = std::stoi(input("Brightness"));
        cout << brightness << endl;
        refreshstatics = true;
        } else {

    if (op == "rainbow") {
    lenght = std::stoi(input("lenght"));
    cout << lenght << endl;
    getspectrum();
    } else {

        if (op == "off") {
        sleep(std::stoi(input("delay(min)"))*60);
        getrgb();
        ledresize(false, 41);
        } else {

    if (op == "resizeall") {
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "static") {
        int n = std::stoi(input("index")) - 1;
        r[n] = std::stoi(input("red"));
        g[n] = std::stoi(input("green"));
        b[n] = std::stoi(input("blue"));
        refreshstatics = true;
        } else {

    if (op == "resizeeveryn") {
    ledresize(true);
    refreshstatics = true;
    } else {

        if (op == "mod") {
        specmod = std::stoi(input("modify mod/1000"))/1000.;
        cout << specmod << endl;
        getspectrum();
        } else {

    if (op == "slowness") {
    slowness = std::stoi(input("modify slowness/1000"));
    cout << slowness << endl;
    } else {

        if (op == "help") {
        cout << "slowness, mod, resizeeveryn, static, resizeall, off, rainbow, resize, b, speed" << endl;
        } else {

    if (op == "speed") {
    speed = std::stoi(input("speed"));
    getspectrum();
    } else {

        if (op == "on") {
        sleep(std::stoi(input("delay(min)"))*60);
        sleeping = false;
        ledresize(false);
        } else {

    if (op == "sleep") {
    sleeping = true;
    } else {

        if (op == "timeron") {
        timer();
        } else {

    if (op == "remoteoff") {
    remotecontrol = false;
    close(new_socket);
    } else {

        if (op == "settings") {
        for(int i = 0; i < led_count; i++){
        cout << ledsetting[i] << endl;
        }} else {

}}} }}} }}} }}} }}} }
#endif
}}
