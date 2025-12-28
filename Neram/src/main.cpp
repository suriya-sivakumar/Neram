#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <time.h>

#define SHARP_SCLK PIN_113
#define SHARP_MOSI PIN_111
#define SHARP_SS PIN_009
#define VCC_ENABLE PIN_013

#define BLACK 0
#define WHITE 1

Adafruit_SharpMem display(SHARP_SCLK, SHARP_MOSI, SHARP_SS, 128, 128);

SoftwareTimer secondTimer;
volatile struct tm t;

void timerCallback(TimerHandle_t xTimer)
{
  (void)xTimer;
  // update the global time struct 't' once per second
  t.tm_sec++;
  if (t.tm_sec >= 60)
  {
    t.tm_sec = 0;
    t.tm_min++;
    if (t.tm_min >= 60)
    {
      t.tm_min = 0;
      t.tm_hour++;
      if (t.tm_hour >= 24)
      {
        t.tm_hour = 0;
        t.tm_mday++;
        if (t.tm_mday > 31)
        {
          t.tm_mday = 1;
          t.tm_mon++;
          if (t.tm_mon > 11)
          {
            t.tm_mon = 0;
            t.tm_year++;
          }
        }
      }
    }
  }
}

void setup()
{
  pinMode(VCC_ENABLE, OUTPUT);
  digitalWrite(VCC_ENABLE, HIGH);
  delay(100);

  // 2. ENABLE HIGH DRIVE for SPI pins to prevent voltage sag
  pinMode(SHARP_SCLK, OUTPUT);
  pinMode(SHARP_MOSI, OUTPUT);
  pinMode(SHARP_SS, OUTPUT);
  NRF_P1->PIN_CNF[13] |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);
  NRF_P1->PIN_CNF[11] |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);
  NRF_P0->PIN_CNF[9] |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);

  // 3. Start Serial for debugging
  Serial.begin(115200);

  // 4. Initialize Display
  display.begin();
  display.clearDisplay();
  display.refresh();

  t.tm_year = 2025 - 1900; // Years since 1900
  t.tm_year = 2025 - 1900; // Years since 1900
  t.tm_mon = 11;           // 0-11 (Jan = 0)
  t.tm_mday = 7;
  t.tm_hour = 7;
  t.tm_min = 45;
  t.tm_sec = 0;
  secondTimer.begin(1000, timerCallback);
  secondTimer.start();
}

void loop()
{
  // Format the time string: HH:MM:SS

  char timeStr[10];
  sprintf(timeStr, "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);

  // Update LCD
  display.clearDisplay();
  display.setTextColor(0); // BLACK
  display.setTextSize(2);
  display.setCursor(10, 50);
  display.print(timeStr);

  display.refresh();
  delay(500); // Update every 500 ms
}