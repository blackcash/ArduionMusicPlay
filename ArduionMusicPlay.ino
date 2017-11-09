/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>
#include "Utility.h"
#include <TM1637Display.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>


#define CLK 9
#define DIO 10
#define BUSY 13

TM1637Display display(CLK, DIO);
SoftwareSerial mySerial(11, 12); // RX, TX

const byte ROWS = 2; //four rows
const byte COLS = 5; //four columns
const byte SONG_BUTTON = 6;
//define the cymbos on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3','4'},
  {'5','6','7','8','9'},
};
char oldKey = 0xff;
byte rowPins[ROWS] = {2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7, 8 }; //connect to the column pinouts of the keypad
byte state_Light = 0;
byte mp3_song = 1;
byte time_song = 0;
boolean isStop = true;
boolean isPause = true;
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  
  Serial.begin (9600);
  mySerial.begin (9600);
  mp3_set_serial (mySerial);	//set softwareSerial for DFPlayer-mini mp3 module 
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (15);
  
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(BUSY, INPUT);
  displayNumAll();
  lightControl();
  stop_Music();
}
  
void loop(){
  char customKey = customKeypad.getKey();
/*  
  int val = analogRead(2);
  val = map(val, 0, 15, 0, 255);
  mp3_set_volume(val);
*/  
  if (customKey){
    Serial.println(customKey);
    if(getKeyAction(customKey) == select_song){
      if(oldKey == customKey){
          state_Light++;
      }else{
          state_Light = 0;
      }
      oldKey = customKey;
    }
    lightControl();
    func_KeyAction(customKey);
  }
  displayNumAll();
  mp3Player();
}

void lightControl(){
   switch(state_Light){
     case 1:
         digitalWrite(A0, HIGH);
         digitalWrite(A1, LOW);
         break;       
     case 2:
         digitalWrite(A0, LOW);
         digitalWrite(A1, HIGH);
         break;
     case 3:
         digitalWrite(A0, HIGH);
         digitalWrite(A1, HIGH);
         break;
     case 0:
     default:
         digitalWrite(A0, LOW);
         digitalWrite(A1, LOW);
         state_Light = 0;
         break;
   }
}

keyAction getKeyAction(char key){
  keyAction action = unknow;
  switch(key){
    case '0': action = select_song; break;
    case '1': action = select_song; break;
    case '2': action = select_song; break;
    case '3': action = select_song; break;
    case '4': action = select_song; break;
    case '5': action = select_song; break;
    case '6': action = play_song; break;
    case '7': action = stop_song;  break;
    case '8': action = increase_play;  break;
    case '9': action = decrease_play;  break;
    default:  break;
  }  
  return action;
}

void func_KeyAction(char key){
  byte data = 0xff;
  switch(key){
    case '0': data = 1;  break;
    case '1': data = 2;  break;
    case '2': data = 3;  break;
    case '3': data = 4;  break;
    case '4': data = 5;  break;
    case '5': data = 6;  break;
    case '6': data = 7;  break;
    case '7': data = 8;  break;
    case '8': data = 9;  break;
    case '9': data = 10;  break;
    default:  break;
  }
  if(getKeyAction(key) == select_song){
    mp3_song = ( state_Light * SONG_BUTTON )+data;
    stop_Music();
  }else if(getKeyAction(key) == play_song){
    if(isPause)
        play_Music();
    else
        pause_Music();    
  }else if(getKeyAction(key) == stop_song){
    stop_Music();
  }else if(getKeyAction(key) == increase_play){
    time_song++;
    stop_Music();
  }else if(getKeyAction(key) == decrease_play){  
    if(time_song != 0){
      time_song--;
    }
    stop_Music();
  } 
}

boolean check_Busy(){
  boolean isCheck = false;
  for(int i=0;i<10;i++){
    if(digitalRead(BUSY)){
      isCheck = true;
    }else{
      isCheck = false;
      break;
    }
    delay(10);
  }
  return isCheck;
}

void play_Music(){
  static int song;  
  if(time_song == 0){
    return;
  }
  isStop = false;
  isPause = false;
  if(song == mp3_song){
    mp3_play();
    delay(100); 
    mp3_play();
    delay(100); 
    Serial.println("play");  
  }else{
    mp3_play(mp3_song);
    delay(100); 
    mp3_play(mp3_song);
    delay(100);     
    Serial.print("play");      
    Serial.println(mp3_song);          
  }
  song = mp3_song;
  digitalWrite(12, LOW);
}

void pause_Music(){
  if(time_song == 0){
    return;
  }
  isStop = false;
  isPause = true;
  mp3_pause();
  delay(100); 
  mp3_pause();
  delay(100); 
  digitalWrite(12, LOW);
  Serial.println("pause");  
}

void stop_Music(){
   isStop = true;
   isPause = true;
   mp3_stop ();
   delay(100); 
   mp3_stop ();
   delay(100); 
   digitalWrite(12, LOW);
   Serial.println("stop");
}

void mp3Player(){
   if((!isStop)&&(!isPause)){
     if(check_Busy()){
       play_Music();
       if(time_song != 0){
        time_song--;
        Serial.print("count");      
        Serial.println(time_song);         
       }
     }
   }
   if(time_song == 0){
     if(!isStop){
       stop_Music();
     }
   }
   delay(100);
}

void displayNumber(int num){
  display.setBrightness(0x0f);
  display.showNumberDec(num, true);
}

void displayNumAll(){
  int num = (mp3_song*100)+time_song;
  //int num = time_song;
  displayNumber(num);
}
