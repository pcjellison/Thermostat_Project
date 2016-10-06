#include <LiquidCrystal.h>
LiquidCrystal LcdDriver(11,9,5,6,7,8);
int inputA = 2;
int inputB = 3;
int encPos = 0;   //encoder position
int currEncPos = 0;
unsigned long Timer = millis(); //timer
const int Interval = 1000;  //1 second length
int tempPin = 0;  //analgo pin 0;
int Hours = 0;
int Minutes = 0;
int Seconds = 0;
int setTemp = 78;
int fanPin = 12;

//material from lab 6
void monitorA()
{
  if(digitalRead(inputA)==digitalRead(inputB))  //reads input from encoder button
  {
    currEncPos ++;  //increment current encoder position
  }  
  else
  {
    currEncPos --;  //decrement current encoder position
  }
}
//material from lab 6
void monitorB()//reads input from encoder button
{
  if(digitalRead(inputA) == digitalRead(inputB))
  {
    currEncPos--;  //decrement current encoder position
  } 
  else
  {
    currEncPos++;  //increment current encoder position
  } 
}

//material from lab 4
void UpdateClock()  //updates clock's values every second
{
  if(Seconds < 59)
  {
    Seconds++;  //increments seconds
  }
  else
  {
    Seconds = 0;  //resets seconds every 59 seconds
    if(Minutes<59)
    {
      Minutes++;  //increments Minutes
    }  
    else
    {
      Minutes=0;  //resets minutes every 59 minutes
      if(Hours<23)
      {
        Hours++;  //increments hours
      }
      else
      {
          Hours = 0; //resets hours every 24 hours
          //reset
      } 
    }
  }
}//end of UpdateClock

//material from lab 4
void SendClock()
{
  LcdDriver.setCursor(0,0); //sets cursor position
  if(Hours<10)
  {
    LcdDriver.print("0"); //prints 0 if hours is <10
  } 
  LcdDriver.print(Hours);//prints hours
  LcdDriver.print(":");
  if(Minutes<10)
  {
    LcdDriver.print("0");  
  } 
  LcdDriver.print(Minutes); //prints minutes
  LcdDriver.print(":");
  if(Seconds<10)
  {
    LcdDriver.print("0");
  }
  LcdDriver.print(Seconds); //prints seconds
}

//used to set initial set temperature based on hours.
void UseTemp() 
{
  if(Hours>6 && Hours <18)
  {
    setTemp = 78; //initializes set temperature to 78
  }  
  else if (Hours >18 || Hours<6)
  {
    setTemp = 70; //initializes set temperature to 70
  }
}
void setup() 
{
  LcdDriver.begin(16,2);
  LcdDriver.setCursor(0,0);
  LcdDriver.clear();

  attachInterrupt(0,monitorA,CHANGE);   //used to read encoder button
  attachInterrupt(1,monitorB,CHANGE);   //used to read encoder button

Serial.begin(9600);
  pinMode(fanPin, OUTPUT);  //sets fanPin (digital pin 12) as an output
  Hours = 7;  //initializes Hours at 7am. Must be changed to test hours function
  UseTemp();  //used to call method to initialize set temperature.
}

void loop() 
{
   if(millis()-Timer>=Interval) //if 1 second has passed
   {
     UpdateClock(); //changes clock time
     SendClock(); //display clock time

     float fahr = (analogRead(A0))*(5.0/1023)*100;  //takes in voltage from LM34 and converts to degrees farenheit
     if(fahr>=setTemp)    //if room temperature is greater than set temperature
     {
      digitalWrite(fanPin, HIGH); //turn fan on
     }
     else if (fahr<setTemp) //if room temperature is less than set temperature
     {
      digitalWrite(fanPin, LOW); //turn fan off
     }

     
     while (currEncPos - encPos >4) //while encoder turns right
     {
      setTemp++;    //increase set temperature
      currEncPos = 0; //reset current encoder position
     }
     while (currEncPos - encPos <-4)    //while encoder turns left
     {
      setTemp--;  //decrease set temperature
      currEncPos = 0;   //reset current encoder position
     }

     
     LcdDriver.print("Room ");  //display room temperature in upper right hand corner (screen is too small to display full length)
     LcdDriver.print(fahr);
     LcdDriver.setCursor(0,1);  //change place of printing
     LcdDriver.print("Set Temp: "); //displays set temperature in lower left hand corner.
     LcdDriver.print(setTemp);  
   }
}
