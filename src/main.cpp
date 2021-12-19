#include <Arduino.h>
#include <SoftwareSerial.h>

//pin definitions
const int Tx_pin = 13;
const int Rx_pin = 12;
const int keyPin = 14;

//Bluetooth serial interface
SoftwareSerial bluetoothSerial(Tx_pin,Rx_pin);


//global varibles
char c = ' ';
int keyState=1;

//Keyboard buffer variables
const int bufferSize = 30;
char buffer[bufferSize];
int bufferPos = 0;

//------------------------------------------------------- vvv
//SETUP 

void setup() {

//key pin setup block (AT COMMAND configuration)
pinMode(keyPin,OUTPUT);
digitalWrite(keyPin,HIGH);


//serial interfaces initialization
Serial.begin(115200);
//bluetoothSerial.begin(9600);//Active mode baud rate
bluetoothSerial.begin(38400);//AT mode baud rate
}
//------------------------------------------------------- ^^^

// LOOP
void loop() {

  //receive
  if(bluetoothSerial.available()>0){
  
    char character=bluetoothSerial.read();
    Serial.print(character);
    
  }
  
  //Keyboard-------------------------------------------------------------------- vvv
  if(Serial.available()>0){//Character received
    
    c=Serial.read();//save the character

    //what character?
    if(c=='\n'){//\n (enter)

      //Key pin state switch -------------------------------
      if(buffer[0]=='k' & buffer[1]=='e' & buffer[2]=='y'){
        
        Serial.println(" ");
        if(keyState==1){
          Serial.println("Switching to ACTIVE mode");
          
          bluetoothSerial.end();
          delay(10);
          bluetoothSerial.begin(9600);//ACTIVE mode baud rate

          keyState=0;
          digitalWrite(keyPin,LOW);
        }
        else{
          Serial.println("Switching to AT COMMANDS mode");

          bluetoothSerial.end();
          delay(10);
          bluetoothSerial.begin(38400);//AT mode baud rate

          keyState=1;
          digitalWrite(keyPin,HIGH);
          }
          Serial.println("-----------------------------------");
      }//---------------------------------------------------


      //enter command
      Serial.print("Entering command: ");
      Serial.println(buffer);//print buffer
      Serial.println("-----------------------------------");
      Serial.println(" ");

      //write to device
      bluetoothSerial.write(buffer); // <------------------- WRITE TO DEVICE
      bluetoothSerial.write('\n');
      
      //clean the buffer
      for(int i=0;i<bufferSize;i++){buffer[i]=NULL;}//fill buffer with nulls
      bufferPos=0;//reset buffer pos
    }
    else{//any other character

      //enter character
      if(bufferPos<bufferSize){//if the buffer is NOT full
        buffer[bufferPos]=c;//push back
        bufferPos++;//increase pos
      }
      else{// buffer FULL
        Serial.println("the buffer is FULL!!");
        Serial.println("Emptying the buffer");
        for(int i=0;i<bufferSize;i++){buffer[i]=NULL;}
        Serial.println("The buffer is empty, consider increasing BUFFER SIZE");
        Serial.println("----------------------------------");
        Serial.println(" ");

      }
    }
  }
  //------------------------------------------------------------------------------ ^^^
}


