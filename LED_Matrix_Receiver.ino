#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <TimerOne.h>
#include <EEPROM.h>

byte dataPin  = 2;    // White wire on my Grid
byte clockPin = 3;    // Green wire on my Grid

const byte nLEDs = 49;

Adafruit_WS2801 strip = Adafruit_WS2801(nLEDs, dataPin, clockPin);

// NETWORK SETUP

byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

unsigned int localPort = 8888;      // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char remoteIPAddy[50];
char addyPart[8];

// END NETWORK SETUP

byte digit_data(byte digit, byte row);
void clear_strip(Adafruit_WS2801 &a_strip);
void fill_strip(Adafruit_WS2801 &a_strip, byte r=80, byte g=80, byte b=80);

void display_number(byte number, Adafruit_WS2801 &a_strip);
void display_line(byte number_line, Adafruit_WS2801 &a_strip);
void display_block(byte number, Adafruit_WS2801 &a_strip);

void display_IP_address(IPAddress &address,  Adafruit_WS2801 &a_strip);

void setup() {
  
  strip.begin();
  strip.show();

// Start the Ethernet and UDP
  Ethernet.begin(mac);
  Udp.begin(localPort);

  Serial.begin(9600);
  while(true){
    display_IP_address(Ethernet.localIP(),strip);
  }
  
  
  Timer1.initialize();
  Timer1.attachInterrupt(callback, 1000000 / 60); // 60 = 60 frames/second


}

void loop() {
  // put your main code here, to run repeatedly:

}

// Timer1 interrupt handler.  Called at equal intervals; 60 Hz by default.
void callback() {
  // Very first thing here is to issue the strip data generated from the
  // *previous* callback.  It's done this way on purpose because show() is
  // roughly constant-time, so the refresh will always occur on a uniform
  // beat with respect to the Timer1 interrupt.  The various effects
  // rendering and compositing code is not constant-time, and that
  // unevenness would be apparent if show() were called at the end.
  strip.show();

  // strip.setPixelColor(iMessage, rMessage, gMessage, bMessage);

}

void clear_strip(Adafruit_WS2801 &a_strip){
  fill_strip(a_strip, 0,0,0);
}

void fill_strip(Adafruit_WS2801 &a_strip, byte r, byte g, byte b){
  for(int i = 0; i < nLEDs ; i++) {
    a_strip.setPixelColor(i,r,g,b);
  }
}

void display_IP_address(IPAddress address,  Adafruit_WS2801 &a_strip){
  char remote_IP_Addy[50] = {0};
  char addyPart[8]; 
  
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    sprintf(addyPart,"%d.",Ethernet.localIP()[thisByte]);
    sprintf(remote_IP_Addy,"%s%s",remote_IP_Addy,addyPart);
  }
  Serial.println(remote_IP_Addy);
  Serial.println(strlen(remote_IP_Addy));
    
  clear_strip(a_strip);
  for(byte counter = 0; counter < strlen(remote_IP_Addy) ; counter++) {
    if(remote_IP_Addy[counter] == '.') {
      fill_strip(a_strip);
      a_strip.show();
      delay(1000);
    } else {
      clear_strip(a_strip);
      a_strip.show();
      delay(500);
      display_number((int)(remote_IP_Addy[counter] - '0'), a_strip);
      delay (1000);
    }
    
  }

  delay(4000);
  
}

void display_number(byte number, Adafruit_WS2801 &a_strip){
    clear_strip(a_strip);
    display_block(number, a_strip);
    a_strip.show();
}

void display_block(byte number, Adafruit_WS2801 &a_strip){
    for (int line_no = 0; line_no < 7; line_no++) {
        display_line(digit_data(number,line_no), line_no, a_strip);
    }
}

void display_line(byte row_of_number_grid, byte line_no, Adafruit_WS2801 &a_strip){
    String line = "";
    byte column = 0;
    while (row_of_number_grid) {
        row_of_number_grid >>= 1; // gets rid of useless 8th bit since this is 7 bit LED array
        bool pixel = row_of_number_grid & 0x0001;
        if (pixel) {
            a_strip.setPixelColor(convert_to_linear(line_no,column),80,80,80);
        } else {
            // do nothing
        }
        column++;
    }
    //cout << endl;
}

byte convert_to_linear(byte line_no, byte column) {

    if(line_no % 2 == 0) {
      return (line_no * 7) + (6 - column);
    } else {
      return (line_no * 7) + column;
    }

}

byte digit_data(byte digit, byte row){
  return EEPROM.read((digit * 7) + row);
}

