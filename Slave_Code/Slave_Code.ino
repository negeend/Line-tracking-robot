//-----------------------------------------------------------------------------------------------------------//
//                                                                                                           //
//  Slave_ELEC1601_Student_2019_v3                                                                           //
//  The Instructor version of this code is identical to this version EXCEPT it also sets PIN codes           //
//  20191008 Peter Jones                                                                                     //
//                                                                                                           //
//  Bi-directional passing of serial inputs via Bluetooth                                                    //
//  Note: the void loop() contents differ from "capitalise and return" code                                  //
//                                                                                                           //
//  This version was initially based on the 2011 Steve Chang code but has been substantially revised         //
//  and heavily documented throughout.                                                                       //
//                                                                                                           //
//  20190927 Ross Hutton                                                                                     //
//  Identified that opening the Arduino IDE Serial Monitor asserts a DTR signal which resets the Arduino,    //
//  causing it to re-execute the full connection setup routine. If this reset happens on the Slave system,   //
//  re-running the setup routine appears to drop the connection. The Master is unaware of this loss and      //
//  makes no attempt to re-connect. Code has been added to check if the Bluetooth connection remains         //
//  established and, if so, the setup process is bypassed.                                                   //
//                                                                                                           //
//-----------------------------------------------------------------------------------------------------------//

#include <SoftwareSerial.h>   //Software Serial Port

#define RxD 7
#define TxD 6
#define ConnStatus A1

#define DEBUG_ENABLED  1

// ##################################################################################
// ### EDIT THE LINES BELOW TO MATCH YOUR SHIELD NUMBER AND CONNECTION PIN OPTION ###
// ##################################################################################

int shieldPairNumber = 15;

// CAUTION: If ConnStatusSupported = true you MUST NOT use pin A1 otherwise "random" reboots will occur
// CAUTION: If ConnStatusSupported = true you MUST set the PIO[1] switch to A1 (not NC)

boolean ConnStatusSupported = true;   // Set to "true" when digital connection status is available on Arduino pin

// #######################################################

// The following two string variable are used to simplify adaptation of code to different shield pairs

String slaveNameCmd = "\r\n+STNA=Slave";   // This is concatenated with shieldPairNumber later

SoftwareSerial blueToothSerial(RxD,TxD);

#include <Servo.h>
Servo servoRight;
Servo servoLeft;

int if_returning = false;
int if_line_tracking = false;

// Warning: the number of input command greater than 200 will cause IndexOutOfRange
String reverseCommand[50] = {""}; // uninitialised array containing elements with default value 0
int count = 0;  // count for the number of command



void setup()
{
    Serial.begin(9600);
    blueToothSerial.begin(38400);                    // Set Bluetooth module to default baud rate 38400
    
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    pinMode(ConnStatus, INPUT);

    //  Check whether Master and Slave are already connected by polling the ConnStatus pin (A1 on SeeedStudio v1 shield)
    //  This prevents running the full connection setup routine if not necessary.

    if(ConnStatusSupported) Serial.println("Checking Slave-Master connection status.");

    if(ConnStatusSupported && digitalRead(ConnStatus)==1)
    {
        Serial.println("Already connected to Master - remove USB cable if reboot of Master Bluetooth required.");
    }
    else
    {
        Serial.println("Not connected to Master.");
        
        setupBlueToothConnection();   // Set up the local (slave) Bluetooth module

        delay(1000);                  // Wait one second and flush the serial buffers
        Serial.flush();
        blueToothSerial.flush();
    }

  //ir setup
  servoLeft.attach(13);  // Attach left signal to pin 13
  servoRight.attach(12); // Attach right signal to pin 12

  pinMode(10,INPUT);
  pinMode(9,OUTPUT);
  pinMode(3,INPUT);
  pinMode(2,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  

}

// IR Object Detection Function

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}  


void loop()
{
    char recvChar;
    
    while(1)
    {
      if(blueToothSerial.available())   // Check if there's any data sent from the remote Bluetooth shield
      {
          recvChar = blueToothSerial.read();
          Serial.print(recvChar);
      }
      
      //moving forward
      if (recvChar == 'w'){
      
        servoLeft.writeMicroseconds(1700);
        servoRight.writeMicroseconds(1300);
        Serial.println("moving forward");
        delay(1000);

        // everytime receive a command, store its reversed command in reverseCommand
        reverseCommand[count] = "s";
        count = count + 1;

        delay(100);


        }
      
      
      
      //turing left
      else if (recvChar == 'a'){
      
      servoLeft.writeMicroseconds(1530);
      servoRight.writeMicroseconds(1300);
      Serial.println("turing left");
      delay(1000);

      reverseCommand[count] = "d";
      count = count + 1;
      
      delay(100);
      }
      
      
      //turning right
      else if (recvChar == 'd'){
      
      servoLeft.writeMicroseconds(1700);
      servoRight.writeMicroseconds(1460);
      Serial.println("turing right");
      delay(500);

      reverseCommand[count] = "a";
      count = count + 1;
      
      
      }
      
      //returning
      else if (recvChar == 'r'){
      
      if_returning = true; 
      Serial.println("returning");
      }
      
      //moving backward
      else if (recvChar == 's'){
      
      servoLeft.writeMicroseconds(1300);
      servoRight.writeMicroseconds(1700);
      delay(500);
      Serial.println("moving backward");

      reverseCommand[count] = "w";
      count = count + 1;
      
      //delay(100);
      }
      
      //left backward
      else if (recvChar == 'z'){
      
      servoLeft.writeMicroseconds(1300);
      servoRight.writeMicroseconds(1530);
      delay(500);
      Serial.println("left backward");

      reverseCommand[count] = "x";
      count = count + 1;
      
     // delay(100);
      }
      
      //right backward
      else if (recvChar == 'x'){
      
      servoLeft.writeMicroseconds(1460);
      servoRight.writeMicroseconds(1700);
      delay(500);
      Serial.println("right backward");

      reverseCommand[count] = "z";
      count = count + 1;
      
      //delay(100);
      }

      //stop
      else if (recvChar == 'n'){
      
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
      delay(500);
      Serial.println("stop");

      // Do not reverse "stop" and "start" command
      reverseCommand[count] = "n";
      count = count + 1;
      
     // delay(100);
      }
      
      //start line tracking
      else if (recvChar == 'b'){
      Serial.println("start");
      
      if_line_tracking = true;

      }
      
     
      //turn around
      else if (recvChar == 't'){
      Serial.println("Found the table tennis ball");
      servoLeft.writeMicroseconds(1700);
      servoRight.writeMicroseconds(1300);
      delay(1000);
      
        

      }
      

      if(Serial.available())            // Check if there's any data sent from the local serial terminal. You can add the other applications here.
      {
          recvChar  = Serial.read();
          Serial.print(recvChar);
          blueToothSerial.print(recvChar);
        
      }
      
      
    //irrobot code
    int irLeft = irDetect(9,10,38000);
    Serial.print(irLeft);
    int irRight = irDetect(2,3,38000);
    Serial.println(irRight);   
    delay(100);     


    servoLeft.attach(13);  // Attach left signal to pin 13
    servoRight.attach(12); // Attach right signal to pin 12

    //while if_line_tracking is True start line tracking
    while (if_line_tracking == true){
      // && if_returning == false)

      if (irLeft == 1 && irRight == 1){

        //moving forward

        servoLeft.writeMicroseconds(1525);
        servoRight.writeMicroseconds(1475);
        Serial.println("moving forward");
        delay(10); 
      }

      else if (irLeft == 1 && irRight == 0){

        //turing left

        servoLeft.writeMicroseconds(1500);
        servoRight.writeMicroseconds(1300);
        Serial.println("turning left");
        delay(110);
      }

      else if (irLeft == 0 && irRight == 1){

        //turning right

        servoLeft.writeMicroseconds(1700);
        servoRight.writeMicroseconds(1500);
        Serial.println("turning right");
        delay(110);
      }

      else if (irLeft == 0 && irRight == 0){

        servoLeft.writeMicroseconds(1500);
        servoRight.writeMicroseconds(1500);
        Serial.println("stopping");
        delay(10);
      }
      

     // if (irLeft == 0 && irRight == 0 && if_returning == false){

        // reaches the object
        
        //Serial.println("Found table tennis ball");
        

        //if_returning = true;

      //}



      //if (irLeft == 0 && irRight == 0 && if_returning == true){

        //when returning after reaching the end of the track, find some way to return to the start point by reversing the commands

     

      //}

    //}    


        // if the robot reaches an end, then break the loop, and go backwards

         // ------------------------------------------------------ going backwards -----------------------------------------------

 
  
    // ----------------------------------------------------------------------------------------------------------------------

    }
    

      if (if_returning) {

        Serial.println("############### GOING BACK ###################");
    
      // counting backwards
      for (int count1 = count; count1 >= 0; count1--) {
                
        if (recvChar == 'w'){
        
        servoLeft.writeMicroseconds(1700);
        servoRight.writeMicroseconds(1300);
        Serial.println("moving forward");
        delay(500);
        }
        
        
        if (recvChar == 'a'){
        
        servoLeft.writeMicroseconds(1530);
        servoRight.writeMicroseconds(1300);
        Serial.println("turing left");
        delay(500);
        }
        
        
        if (recvChar == 'd'){
        
        servoLeft.writeMicroseconds(1700);
        servoRight.writeMicroseconds(1460);
        Serial.println("turing right");
        delay(500);
        }
        
        if (recvChar == 's'){
        
        servoLeft.writeMicroseconds(1300);
        servoRight.writeMicroseconds(1700);
        Serial.println("moving backward");
        delay(500);
        }
      
        if (recvChar == 'z'){
        
        servoLeft.writeMicroseconds(1300);
        servoRight.writeMicroseconds(1530);
        Serial.println("left backward");
        delay(500);
        }
        
        //right backward
        if (recvChar == 'x'){
        
        servoLeft.writeMicroseconds(1460);
        servoRight.writeMicroseconds(1700);
        Serial.println("right backward");
        delay(500);
        }
  
        //stop
       // if (recvChar == 'n'){
        
        //servoLeft.writeMicroseconds(1500);
        //servoRight.writeMicroseconds(1500);
        //Serial.println("stop");
        //delay(500);
        }
        
       // if (recvChar == 'b'){
       // Serial.println("start");
        //if_line_tracking = true;
        }
      }
      
      
    }

    
      

   


  

  
// 1 - black (receiver did not receive the infared signal)
// 0 - white (receiver received the signal)
void setupBlueToothConnection()
{
    Serial.println("Setting up the local (slave) Bluetooth module.");

    slaveNameCmd += shieldPairNumber;
    slaveNameCmd += "\r\n";

    blueToothSerial.print("\r\n+STWMOD=0\r\n");      // Set the Bluetooth to work in slave mode
    blueToothSerial.print(slaveNameCmd);             // Set the Bluetooth name using slaveNameCmd
    blueToothSerial.print("\r\n+STAUTO=0\r\n");      // Auto-connection should be forbidden here
    blueToothSerial.print("\r\n+STOAUT=1\r\n");      // Permit paired device to connect me
    
    //  print() sets up a transmit/outgoing buffer for the string which is then transmitted via interrupts one character at a time.
    //  This allows the program to keep running, with the transmitting happening in the background.
    //  Serial.flush() does not empty this buffer, instead it pauses the program until all Serial.print()ing is done.
    //  This is useful if there is critical timing mixed in with Serial.print()s.
    //  To clear an "incoming" serial buffer, use while(Serial.available()){Serial.read();}

    blueToothSerial.flush();
    delay(2000);                                     // This delay is required

    blueToothSerial.print("\r\n+INQ=1\r\n");         // Make the slave Bluetooth inquirable
    
    blueToothSerial.flush();
    delay(2000);                                     // This delay is required
    
    Serial.println("The slave bluetooth is inquirable!");
    
}
