#include <RadioLib.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define Display_power 19
#define RS485_power 18

//esp32
#define DIO0 25
#define DIO1 27

#define LORA_RST 33
#define LORA_CS 32

#define SPI_MOSI 13
#define SPI_MISO 12
#define SPI_SCK 14

#define ESP32_SDA 4
#define ESP32_SCL 5

#define FREQUENCY 434.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

#define MYSerialRX 23
#define MYSerialTX 22

HardwareSerial MySerial(1);



void setup() {
    // put your setup code here, to run once:
    Serial.begin(4800);

    MySerial.begin(4800, SERIAL_8N1, 23, 22);
    
    pinMode(Display_power,OUTPUT);
    pinMode(RS485_power,OUTPUT);
    delay(50);
    digitalWrite(Display_power, HIGH);
    digitalWrite(RS485_power, HIGH);
    delay(1000);

    Serial.println(" Test Begin!");

    Wire.begin(ESP32_SDA, ESP32_SCL);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
      for (;;)
        ; // Don't proceed, loop forever
    }
    Serial.println("SSD1306 found");

    logo_show();
    
}

unsigned char testcode[8] = {0X01,0X03,0X00,0X00,0X00,0X02,0XC4,0X0B};
unsigned char responsecode[9] = {};
int moisture;
int tem;

float moisture_value;
float tem_value;



void loop() {
  
    delay(3000);
    MySerial.write(testcode,8);
    int i = 0;
    while (MySerial.available() > 0 && i < 9)
    {
        responsecode[i] = MySerial.read();
        i++;
        yield();
    }
    for(int j=0;j<9;j++)
      {Serial.print((int)responsecode[j]);
      Serial.print("  ");}
    Serial.print("\n");

    moisture = caculatevalue((int)responsecode[3],(int)responsecode[4]);
    moisture_value = moisture*0.1 ;
    tem = caculatevalue((int)responsecode[5],(int)responsecode[6]);
    tem_value = tem*0.1;


    Serial.println(moisture);
    Serial.println(moisture_value);
    Serial.println(tem_value);
    value_show(moisture_value, tem_value);
            
}

void logo_show()
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(10, 0);
  display.println(F("Makerfabs"));
  display.setTextSize(1);
  display.setCursor(10, 16);
  display.println(F("RS485-LoRa"));
  display.display(); // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x01);
  delay(4000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}


int caculatevalue(int x, int y)
{
  int t = 0;
  t = x*256;
  t = t+y;
  return t;
}

void value_show(float h, float t)
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(2, 0);
  display.print(F("T:"));
  display.print(t,1);
  display.print(F("C"));
  
  display.setCursor(2, 16);
  display.print(F("H:"));
  display.print(h,1);
  display.print(F("%"));


  display.display(); // Show initial text
  delay(100);
}
/*
 * unsigned char testcode[236] = {0XFD,0XFD,0X30,0X3,...
 * Serial.write(testcode,236);
 * 
 */
