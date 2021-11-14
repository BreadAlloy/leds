#!/usr/bin/env python3 rpi_ws281x library strandtest example Author: Tony DiCola (tony@tonydicola.com)
#
# Direct port of the Arduino NeoPixel library strandtest example.  Showcases
# various animations on a strip of NeoPixels.

import time
from rpi_ws281x import *
from neopixel import *
from timeit import default_timer as timerx
import argparse
import threading
import array

# LED strip configuration:
LED_COUNT      = 640      # Number of LED pixels.
LED_PIN        = 18      # GPIO pin connected to the pixels (18 uses PWM!).
#LED_PIN        = 10      # GPIO pin connected to the pixels (10 uses SPI /dev/spidev0.0).
LED_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA        = 10      # DMA channel to use for generating signal (try 10)
LED_BRIGHTNESS = 255     # Set to 0 for darkest and 255 for brightest
LED_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL    = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53


brightness = 100
speed_wave = 2
lenght_wave = 50




rainbowswitch = True
staticswitch = False
stresstestswitch = False
timerswitch1 = True
inputswitch = False

def operator():
    global brightness
    global speed_wave
    global lenght_wave

    global rainbowswitch
    global staticswitch
    global inputswitch
    global stresstestswitch
    global timerswitch1

    try:
        while True:
            dddd = timerx()
            print("timer"+str(dddd))
            if inputswitch:
                op = input("Operation")
                if op == "b":
                   # switch()
                    brightness = int(input("Brightness"))
                    inputswitch = True
                elif op == "static":
                    switch()
                    staticswitch = True
                elif op == "rainbow":
                    switch()
                    time.sleep(0.1)
                    rainbowswitch = True
                elif op == "stresstest":
                    switch()
                    stresstestswitch = True
                elif op == "off":
                    offanim()
                    inputswitch = True
                elif op == "timeron":
                    timerswitch1 = True
                    print("Timer in on")
                op = "stop"
            time.sleep(1)
    except KeyboardInterrupt:
         print("aaa")

def switch():
    global rainbowswitch
    global staticswitch
    global inputswitch
    global stresstestswitch

    inputswitch = False
    rainbowswitch = False
    staticswitch = False
    stresstestswitch = False

def staticinput():
    col1 = int(input("red"))
    col2 = int(input("green"))
    col3 = int(input("blue"))
    for j in range(0, strip.numPixels()):
        strip.setPixelColor(j, Color(col1, col2, col3))
    strip.show()

def staticset(col1, col2 ,col3):
    for j in range(0, strip.numPixels()):
        strip.setPixelColor(j, Color(col1, col2, col3))
    strip.show()

def offanim():
    time.sleep(int(input("Delay(min)"))*60)
    switch()
    time.sleep(1)
    colorr = array.array('i', [])
    colorg = array.array('i', [])
    colorb = array.array('i', [])
    for j in range(0, strip.numPixels()):
         colorr.append(j)
         colorg.append(j)
         colorb.append(j)
         color = bin(strip.getPixelColor(j))
         color = color.replace("0b", "00000000000000000000000", 1)
         colorl = len(color)
         colorr[j] = int(color[colorl-24:colorl-16], 2)
         colorg[j] = int(color[colorl-16:colorl-8], 2)
         colorb[j] = int(color[colorl-8:colorl], 2)
    for i in range(0, brightness):
        for j in range(0, strip.numPixels()):
             if colorr[j] - 1 >= 0:
                 colorr[j] = colorr[j] - 1
             if colorg[j] - 1 >= 0:
                 colorg[j] = colorg[j] - 1
             if colorb[j] - 1 >= 0:
                 colorb[j] = colorb[j] - 1
             strip.setPixelColor(j, Color(colorr[j], colorg[j], colorb[j]))
        strip.show()

def rainbow2(phase, lenght):
    aa = timerx()
    i = 0
    point = (phase)/lenght
    xxxx=strip.numPixels()
    while(i <= xxxx):
        while(point > 1):
            point = point - 1
        strip.setPixelColor(i, Color(spectrum2(point), spectrum2(point+0.3333), spectrum2(point+0.6666)))
        point = point + 1/lenght
        i = i + 1
    bb = timerx();
    #print("calc=" + str(bb-aa))
    a = timerx();
    strip.show()
    b = timerx();
    #print("show=" + str(b-a));

def spectrum2(point):
     global brightness
     mod = 0.29  #0.25782
     if(point > 1):
         point = point - 1
     if(point < mod*2):
         return round((-(point * 1/mod - 1)**2 + 1) * brightness)
     else:
         return 0

def framess():
        print(str(time.localtime()))
        for i in range(0, strip.numPixels()):
            strip.setPixelColor(strip.numPixels() - i, Color(i, 0, 0))
            strip.show()
        print(str(time.localtime()))

def wave(man):
    global brightness
    global speed_wave
    global lenght_wave

    global rainbowswitch
    global staticswitch
    global inputswitch
    global stresstestswitch
    global timerswitch1

    if speed_wave == 0:
        rainbowswitch = False
        speed_wave = 0.1
    if(man):
        speed_wave = float(input("speed"))
        lenght_wave = int(input("lenght"))
        inputswitch = True
    else:
        rainbowswitch = True
    i = 0
    while rainbowswitch:
        if i > lenght_wave:
            i = i - lenght_wave
        rainbow2(i, lenght_wave)
        i = i + 0.1*speed_wave

def glob():
    global brightness
    global speed_wave
    global lenght_wave

    global rainbowswitch
    global staticswitch
    global inputswitch
    global stresstestswitch
    global timerswitch1


def setting():
    global brightness
    global speed_wave
    global lenght_wave

    global rainbowswitch
    global staticswitch
    global inputswitch
    global stresstestswitch
    global timerswitch1

    try:
         while True:
             if rainbowswitch:
                 wave(True)
             elif staticswitch:
                 staticinput()
                 staticswitch = False
                 inputswitch = True
             elif stresstestswitch:
                 stresstest()
                 inputswitch = True
             time.sleep(1)
         print("Exiting control")
    except KeyboardInterrupt:
        if args.clear:
            colorWipe(strip, Color(0,0,0), 10)


def timer():
    global brightness
    global speed_wave
    global lenght_wave

    global rainbowswitch
    global staticswitch
    global inputswitch
    global stresstestswitch
    global timerswitch1

    while True:
        if(str(time.localtime()).find("tm_hour=3") != -1 and timerswitch1):
            switch()
            timerswitch = False
            time.sleep(0.1)
            wave(False)
        time.sleep(20)

def stresstest():
    staticset(255, 255, 255)
    time.sleep(1)

# Main program logic follows:
if __name__ == '__main__':
    # Process arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--clear', action='store_true', help='clear the display on exit')
    args = parser.parse_args()

    # Create NeoPixel object with appropriate configuration.
    strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL)
    # Intialize the library (must be called once before other functions).
    strip.begin()

    print ('Press Ctrl-C to quit.')
    if not args.clear:
        print('Use "-c" argument to clear LEDs on exit')

#    rainbow2(strip, 0, 50)


    #framess()


    thread1 = threading.Thread(target=setting)
    thread2 = threading.Thread(target=operator)
    thread3 = threading.Thread(target=timer)
    thread1.start()
    thread2.start()
    thread3.start()
    print("Exiting Main")
