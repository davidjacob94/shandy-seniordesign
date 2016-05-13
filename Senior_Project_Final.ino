#include <Bridge.h>
#include <Process.h>
#include <FileIO.h>

int pwr1, pwr2, pwr3;

Process socket;
bool socketAvailable = false;

String msg, message;
int sckt1Floor, sckt2Floor, sckt3Floor;
String sData1, sData2, sData3;

int lastUpdate = 0;

void setup() {
  pinMode(13, OUTPUT);

  //Outlet 1
  pinMode(5, OUTPUT);
  //Outlet 2
  pinMode(6, OUTPUT);
  //Outlet 3
  pinMode(7, OUTPUT);
  
  // Initialize Bridge
  Bridge.begin();
  
  // Initialize Serial
  //Serial.begin(9600);
  //while (!Serial);
  
  // Waits a preset amount of time until the Linux side is fully Booted and connected to the internet
  // Possibly update this to check wifi status instead?
  delay(90000);

  // Runs the client.js file using node on the Linux side
  socket.runShellCommandAsynchronously("node /mnt/sda1/arduino/node/client.js");
  while (!socket.available()) { }
  socketAvailable = true;

}

void loop() {
  // Reads in the cosole output of the client.js process one character at a time
  char c = socket.read();
  // Turn On L13 once socket is connected
  if(c != NULL) {
    // Turns on the LED at pin 13 once the process can be read
    digitalWrite(13, HIGH);
  }

// Since we weren't succesfully able to read in strings we assigned the on/off commands to numbers
switch (c) {
  case '1':
    digitalWrite(5, HIGH);
  break;
  case '2':
    digitalWrite(5, LOW);
  break;
  case '3':
    digitalWrite(6, HIGH);
  break;
  case '4':
    digitalWrite(6, LOW);
  break;
  case '5':
    digitalWrite(7, HIGH);
  break;
  case '6':
    digitalWrite(7, LOW);
  break;
  default:
  break;
}
/* // Attempt to read chars into a string
  // Read process until newline
  if(c == '\n') {
      message = msg;
      processMsg(message);
      msg = "";
  } else {
      msg += c;
  }
*/

  // Reads the data from the LEM sensor every 2.5 seconds
  if(millis() / 2500 > lastUpdate){
    lastUpdate = millis() / 2500;

    sckt1Floor = 545;
    sckt2Floor = 559;
    sckt3Floor = 558;    
    
    pwr1 = readCurrent(A1);
    if(pwr1 <= sckt1Floor){
      pwr1 = 0; 
    } else {
      pwr1 = (3.01 * (pwr1 - sckt1Floor));
    }

    sData1 = "sckt_1 " + String(pwr1);
    sendToSocket(sData1);

    pwr2 = readCurrent(A2);

    if(pwr2 <= sckt2Floor){
      pwr2 = 0; 
    } else {
      pwr2 = (3.02 * (pwr2 - sckt2Floor));
    }
    sData2 = "sckt_2 " + String(pwr2);
    sendToSocket(sData2);

    pwr3 = readCurrent(A3);
    if(pwr3 <= sckt3Floor){
      pwr3 = 0; 
    } else {
      pwr3 = (3.11 * (pwr3 - sckt3Floor));
    }
    sData3 = "sckt_3 " + String(pwr3);
    sendToSocket(sData3);
  }
}

// Sends the string to the socket one character at a time
// Then sends a new line to complete the transmission
void sendToSocket(String str) {
  for(int i = 0; i<str.length(); i++){
    socket.write(str.charAt(i));
  }
  socket.write('\n');
  //Serial.println("Sent to Socket: " + str);
}

// Samples the Sin wave from the LEM sensor for the MAX value
int readCurrent(int pin){
  int value, maxValue;
  for(int i = 0; i < 390; i++) {
    value = analogRead(pin);
    if(value > maxValue) {
      maxValue = value;
    }
  }
  //Serial.println("Read Value" + pin);
  return maxValue;
}

// Attempt to proccess string if we were able to read data in as a string
void processMsg(String data) {
     char sckt = data.charAt(0);
     char toggle = data.charAt(1);

     String tempSckt = sckt + "";
     String tempToggle = toggle + "";

     Serial.print("processMsg");
     Serial.println(data);
     Serial.println(sckt);
     Serial.println(toggle);

     int pin;
     switch (sckt) {
     case '1':
          pin = 5;
          if(toggle == 'H') {
               digitalWrite(pin, HIGH);
          } else {
               digitalWrite(pin, LOW);
          }
     break;
     case '2':
          pin = 6;
          if(toggle == 'H') {
               digitalWrite(pin, HIGH);
          } else {
               digitalWrite(pin, LOW);
          }
     break;
     case '3':
          pin = 7;
          if(toggle == 'H') {
               digitalWrite(pin, HIGH);
          } else {
               digitalWrite(pin, LOW);
          }
     break;
     default:
     break;
     }
}
