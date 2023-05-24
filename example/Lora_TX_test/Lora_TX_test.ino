#include <RadioLib.h>
#include <SPI.h>

#define IO_5V_ENABLE 18  // RS485 POWER
#define IO_3V3_ENABLE 19 // OLED POWER
#define IO_LED 21

#define DIO0 25
#define DIO1 26

#define LORA_RST 33
#define LORA_CS 32

#define SPI_MOSI 13
#define SPI_MISO 12
#define SPI_SCK 14

#define FREQUENCY 868.0 // 868.0 // 433.0 915.0
#define BANDWIDTH 125.0
#define SPREADING_FACTOR 9
#define CODING_RATE 7
#define OUTPUT_POWER 10
#define PREAMBLE_LEN 8
#define GAIN 0

SX1276 radio = new Module(LORA_CS, DIO0, LORA_RST, DIO1, SPI, SPISettings());

void setup()
{
  Serial.begin(115200);

  pinMode(IO_LED, OUTPUT);
  digitalWrite(IO_LED, LOW);

  pinMode(IO_5V_ENABLE, OUTPUT);
  digitalWrite(IO_5V_ENABLE, HIGH);
  delay(200);
  pinMode(IO_3V3_ENABLE, OUTPUT);
  digitalWrite(IO_3V3_ENABLE, HIGH);
  delay(200);
  digitalWrite(IO_LED, HIGH);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  Serial.print(F("[SX1276] Initializing ... "));

  int state = radio.begin(FREQUENCY, BANDWIDTH, SPREADING_FACTOR, CODING_RATE, SX127X_SYNC_WORD, OUTPUT_POWER, PREAMBLE_LEN, GAIN);
  if (state == ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}

void loop()
{

  int state = radio.transmit("Lora send test");
  if (state == ERR_NONE)
  {
    // the packet was successfully transmitted
    Serial.println(F(" success!"));

    // print measured data rate
    Serial.print(F("[SX1276] Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(F(" bps"));
  }
  else if (state == ERR_PACKET_TOO_LONG)
  {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));
  }
  else if (state == ERR_TX_TIMEOUT)
  {
    // timeout occurred while transmitting packet
    Serial.println(F("timeout!"));
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  delay(5000);
}
