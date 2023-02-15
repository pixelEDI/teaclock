/*


        _          _ ______ _____ _____
       (_)        | |  ____|  __ \_   _|
  _ __  ___  _____| | |__  | |  | || |
 | '_ \| \ \/ / _ \ |  __| | |  | || |
 | |_) | |>  <  __/ | |____| |__| || |_
 | .__/|_/_/\_\___|_|______|_____/_____|
 | |
 |_|

www.pixeledi.eu | www.linktr.ee/pixeledi
TeaClock | V1.0 | 01.2023

*/

#include <Arduino.h>
#include "button.h"
#include <TM1637Display.h>

// 7 segment display
const byte CLK = 0;
const byte DIO = 1;
TM1637Display display(CLK, DIO);

// button
const byte buzzer = 3;
Button btn(4);

// time
int minutes = 7;
int seconds = 0;
unsigned long previousTime = 0;
bool startCountDown = false;

void setDisplay()
{
  display.showNumberDec(minutes, true, 2, 0);
  display.showNumberDec(seconds, true, 2, 2);
}

void setTime()
{
  switch (btn.checkBTN())
  {
  case 1:
    if (seconds == 0)
    {
      seconds += 30;
    }
    else
    {
      seconds = 0;
      minutes++;
    }
    setDisplay();
    break;

  case 2:
    if (seconds == 0)
    {
      seconds = 30;
      minutes--;
    }
    else
    {
      seconds = 0;
    }
    setDisplay();
    break;

  case 3:
    startCountDown = true;
    break;
  }
}

void countDown()
{
  unsigned long timeNow = millis();

  if (timeNow - previousTime >= 1000)
  {
    previousTime = timeNow;
    if (seconds == 0 && minutes != 0)
    {
      minutes--;
      seconds = 59;
    }

    else if (seconds == 0 && minutes == 0)
    {
      for (int i = 0; i < 10; i++)
      {
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        delay(500);
      }

      startCountDown = false;
    }
    else
    {
      seconds--;
    }
    setDisplay();
  }
}

void setup()
{
  display.setBrightness(0x02); // 0-7
  btn.start();
  pinMode(buzzer, OUTPUT);
  setDisplay();
}

void loop()
{
  setTime();
  if (startCountDown)
  {
    countDown();
  }
}