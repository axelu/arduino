#include <DFMiniMp3.h>
#include <Servo.h>

// using v1.0.5 of DFMiniMp3 lib
class Mp3Notify {
public:
  static void OnError(uint16_t errorCode);
  static void OnPlayFinished(uint16_t globalTrack);
  static void OnCardOnline(uint16_t code);
  static void OnUsbOnline(uint16_t code);
  static void OnCardInserted(uint16_t code);
  static void OnUsbInserted(uint16_t code);
  static void OnCardRemoved(uint16_t code);
  static void OnUsbRemoved(uint16_t code);
};

// DFPlayer Mini
DFMiniMp3<HardwareSerial,Mp3Notify> mp3(Serial);

// Servo
Servo myservo1; // gate1
Servo myservo2; // gate2


// LEDs
int LED1 = 9;   // gate1
int LED2 = 10;  // gate1
int LED3 = 11;  // gate2
int LED4 = 12;  // gate2


int state = 0;

int pinButton = 2;

void setup() {
  state = 0; // 0 open, 1 closed

  pinMode(pinButton, INPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);

  // init DFPlayer Mini
  Serial.begin(9600); 
  mp3.begin();
  mp3.setVolume(25);

  // init servos
  myservo1.attach(5,600,2400);  // closed position (if left at exactly 0 servo makes constant noise) 
  myservo1.write(33);           // open position
  myservo2.attach(4,600,2400);  // closed position (if left at exactly 0 servo makes constant noise) 
  myservo2.write(33);           // open position
  delay(500);
  // Note: if power loss in closed position, it will jump to open
  // assumption is we turn power off in open position under normal operation

}

void loop() {
  int stateButton = digitalRead(pinButton); //read the state of the button
  if( stateButton == 1 && state == 0 ) {
      // gate was open, close the gate
      state = 1;
      
      // start the bell
      mp3.setVolume(25);
      mp3.loopGlobalTrack(1);
      
      // start the flasher
      digitalWrite(LED1,LOW);
      digitalWrite(LED3,LOW);
      delay(600);
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,LOW);
      delay(600);
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,HIGH);

      // close the gate
      // start is 33deg -> 33x60ms = 1980ms
      int cpos = myservo1.read();
      for(; cpos >= 0; cpos -= 1) {
        myservo1.write(cpos);
        myservo2.write(cpos);
        if( cpos == 22 ) {
          digitalWrite(LED1,HIGH);
          digitalWrite(LED2,LOW);  
          digitalWrite(LED3,HIGH);
          digitalWrite(LED4,LOW);  
        }
        if( cpos == 11 ) {
          digitalWrite(LED1,LOW);
          digitalWrite(LED2,HIGH);  
          digitalWrite(LED3,LOW);
          digitalWrite(LED4,HIGH);  
        }
        delay(60); // waits ms for the servo to reach the position
      }
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,LOW);
      delay(600);
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,HIGH);

      // stop the bell
      delay(600);
      mp3.stop();
  } else if( stateButton == 1 && state == 1 ) {
      // gate was closed, keep it closed
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,LOW);
      delay(600);
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,HIGH);
      delay(600);
  } else if( stateButton == 0 && state == 1 ) {
      // gate was closed, open it
      state = 0;

      int cpos = myservo1.read();
      for(; cpos <= 33; cpos += 1) {
        myservo1.write(cpos);
        myservo2.write(cpos);
        if( cpos == 11 ) {
          digitalWrite(LED1,HIGH);
          digitalWrite(LED2,LOW);  
          digitalWrite(LED3,HIGH);
          digitalWrite(LED4,LOW);  
        }
        if( cpos == 22 ) {
          digitalWrite(LED1,LOW);
          digitalWrite(LED2,HIGH);  
          digitalWrite(LED3,LOW);
          digitalWrite(LED4,HIGH);  
        }
        delay(60);
      }
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,HIGH);
      digitalWrite(LED4,LOW);
      delay(600);
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,HIGH);
      digitalWrite(LED3,LOW);
      digitalWrite(LED4,HIGH);
      delay(600);
      digitalWrite(LED1,HIGH);
      digitalWrite(LED3,HIGH);

  } else {
    // gate is open and should be open
    // do nothing
  }
}



void Mp3Notify::OnError(uint16_t errorCode) {
  // see DfMp3_Error for code meaning
  Serial.println();
  Serial.print("Com Error ");
  Serial.println(errorCode);
}

void Mp3Notify::OnPlayFinished(uint16_t globalTrack) {
  Serial.println();
  Serial.print("Play finished for #");
  Serial.println(globalTrack);  
}

void Mp3Notify::OnCardOnline(uint16_t code) {
  Serial.println();
  Serial.print("Card online ");
  Serial.println(code);    
}

void Mp3Notify::OnUsbOnline(uint16_t code) {
  Serial.println();
  Serial.print("USB Disk online ");
  Serial.println(code);    
}

void Mp3Notify::OnCardInserted(uint16_t code) {
  Serial.println();
  Serial.print("Card inserted ");
  Serial.println(code);
}

void Mp3Notify::OnUsbInserted(uint16_t code) {
  Serial.println();
  Serial.print("USB Disk inserted ");
  Serial.println(code);
}

void Mp3Notify::OnCardRemoved(uint16_t code) {
  Serial.println();
  Serial.print("Card removed ");
  Serial.println(code);  
}

void Mp3Notify::OnUsbRemoved(uint16_t code) {
  Serial.println();
  Serial.print("USB Disk removed ");
  Serial.println(code);  
}
