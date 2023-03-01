// Vincent Fix 2023/3/1

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

// esp32
#define DIO0 25
#define DIO1 27

#define ESP32_SDA 4
#define ESP32_SCL 5

#define LORA_RST 33
#define LORA_CS 32

#define SPI_MOSI 13
#define SPI_MISO 12
#define SPI_SCK 14

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

// unsigned char test_command[8] = {0X01, 0X03, 0X00, 0X00, 0X00, 0X07, 0X04, 0X08};
// New Sensor 2023/3/1
unsigned char test_command[8] =
    {0X01, 0X04, 0X00, 0X00,
     0X00, 0X07, 0XB1, 0XC8}; 
unsigned char test_response[80] = {0};

int moisture;
int tem;
int ph;
float moisture_value;
float tem_value;
float ph_value;
int P_value;
int N_value;
int K_value;
float P_float_value;
float N_float_value;
float K_float_value;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(4800);

  MySerial.begin(4800, SERIAL_8N1, 23, 22);

  pinMode(Display_power, OUTPUT);
  pinMode(RS485_power, OUTPUT);
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

  MySerial.write(test_command, 8);

  delay(1000);

  MySerial.write(test_command, 8);

  delay(1000);
}

void loop()
{
  Serial.println("Send loop");
  //********************GET Temperature, Moisture and PH*******************
  MySerial.write(test_command, 8);
  int i = 0;

  while (MySerial.available() > 0 && i < 80)
  {
    test_response[i] = MySerial.read();
    i++;

    yield();
  }

  Serial.print("Answer Length:");
  Serial.println(i);

  for (int j = 0; j < 19; j++)
  {
    Serial.print((int)test_response[j]);
    Serial.print("  ");
  }
  Serial.print("\n");

  moisture = CaculateValue((int)test_response[3], (int)test_response[4]);
  moisture_value = moisture * 0.1;
  tem = CaculateValue((int)test_response[5], (int)test_response[6]);
  tem_value = tem * 0.1;
  ph = CaculateValue((int)test_response[9], (int)test_response[10]);
  ph_value = ph * 0.1;

  Serial.print("moisture:");
  Serial.println(moisture);
  Serial.print("moisture_value:");
  Serial.println(moisture_value);
  Serial.print("tem_value:");
  Serial.println(tem_value);
  Serial.print("ph_value:");
  Serial.println(ph_value);
  value_show(moisture_value, tem_value, ph_value);
  delay(1000);

  //****************GET Nitrogen, Phosphorus and Potassium

  N_value = CaculateValue((int)test_response[11], (int)test_response[12]);
  P_value = CaculateValue((int)test_response[13], (int)test_response[14]);
  K_value = CaculateValue((int)test_response[15], (int)test_response[16]);

  Serial.print("N= ");
  Serial.print(N_value);
  Serial.println(" mg/kg");
  Serial.print("P= ");
  Serial.print(P_value);
  Serial.println(" mg/kg");
  Serial.print("K= ");
  Serial.print(K_value);
  Serial.println(" mg/kg");

  NPK_Show(N_value, P_value, K_value);
  delay(1000);
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

int CaculateValue(int x, int y)
{
  int t = 0;
  t = x * 256;
  t = t + y;
  return t;
}

void value_show(float h, float t, float ph_f)
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); // Draw 2X-scale text
  display.setCursor(2, 0);
  display.print(F("T:"));
  display.print(t, 1);
  display.print(F("C"));

  display.setCursor(66, 0);
  display.print(F("H:"));
  display.print(h, 1);
  display.print(F("%"));

  display.setCursor(2, 16);
  display.print(F("PH:"));
  display.print(ph_f, 1);

  display.display(); // Show initial text
  delay(3000);
}

void NPK_Show(int N, int P, int K)
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1); // Draw 2X-scale text
  display.setCursor(2, 0);
  display.print(F("N:"));
  display.print(N);
  // display.print(F("C"));

  display.setCursor(66, 0);
  display.print(F("P:"));
  display.print(P);
  // display.print(F("%"));

  display.setCursor(2, 16);
  display.print(F("K:"));
  display.print(K);
  display.print(F(" mg/kg"));

  display.display(); // Show initial text
  delay(3000);
}