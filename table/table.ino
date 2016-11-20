#include <OctoWS2811.h>

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF
#define BLACK  0x000000

#define TABLE_NB_LED 240

const int ledPin = 13;

const int ledsPerStrip = 48;
DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup() {
  leds.begin();
  delay(200);
  setColor(WHITE);
  leds.show();
  delay(1000);
  setColor(BLACK);
  Serial.begin(115200);
  Serial.setTimeout(50);
}

void loop()
{
  int startChar = Serial.read();

  if (startChar == '*')
  {
      int count = Serial.readBytes((char *)drawingMemory, sizeof(drawingMemory));
      leds.show();
  }
  else if (startChar == '#')
  {
      int color = 0;
      int count = Serial.readBytes((char *)&color, sizeof(int));
      setColor(color);
  }
}

void setColor(int color)
{
    for (int i=0; i < leds.numPixels(); i++)
    {
        leds.setPixel(i, color);
    }
    leds.show();
}
