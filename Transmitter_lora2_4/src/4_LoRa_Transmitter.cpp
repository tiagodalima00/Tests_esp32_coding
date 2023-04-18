#include <Arduino.h>
#include <SPI.h>                                               //the SX128X device is SPI based so load the SPI library                                         
#include <SX128XLT.h>                                          //include the appropriate library  
#include "Settings.h"                                          //include the setiings file, frequencies, LoRa settings etc   

SX128XLT LT;                                                   //create a library class instance called LT

uint8_t TXPacketL;
uint32_t TXPacketCount, startmS, endmS;

uint8_t buff[] = {0x01, 0x24, 0x16, 0x23,0x01 ,0x01 ,0x01};

void packet_is_OK();
void packet_is_Error();
//void led_Flash(uint16_t flashes, uint16_t delaymS);
void printHEXPacket(uint8_t *buffer,uint8_t size);

void setup()
{
  //pinMode(LED1, OUTPUT);                                   //setup pin as output for indicator LED
  //led_Flash(2, 125);                                       //two quick LED flashes to indicate program start

  Serial.begin(115200);
  Serial.println();
  Serial.println(F("3_LoRa_Transmitter Starting"));

  SPI.begin();

  //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE))
  {
    Serial.println(F("LoRa Device found"));
    //led_Flash(2, 125);                                   //two further quick LED flashes to indicate device found
    delay(1000);
  }
  else
  {
    Serial.println(F("No device responding"));
    while (1)
    {
      //led_Flash(50, 50);                                 //long fast speed LED flash indicates device error
      if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE))
      {
        Serial.println(F("LoRa Device found"));
        //led_Flash(2, 125);                                   //two further quick LED flashes to indicate device found
        delay(1000);
        break;
      }
    }
  }

  LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate);

  Serial.print(F("Transmitter ready"));
  Serial.println();
}

void loop()
{
  Serial.print(TXpower);                                       //print the transmit power defined
  Serial.print(F("dBm "));
  Serial.print(F("Packet> "));
  Serial.flush();

  TXPacketL = sizeof(buff) / sizeof(buff[0]);                                    //set TXPacketL to length of array
  //buff[TXPacketL - 1] = '*';                                   //replace null character at buffer end so its visible on reciver

  //LT.printASCIIPacket(buff, TXPacketL);                        //print the buffer (the sent packet) as ASCII

  printHEXPacket(buff, TXPacketL);
  
  //digitalWrite(LED1, HIGH);
  startmS =  millis();                                         //start transmit timer
  if (LT.transmit(buff, TXPacketL, 10000, TXpower, WAIT_TX))   //will return packet length sent if OK, otherwise 0 if transmit, timeout 10 seconds
  {
    endmS = millis();                                          //packet sent, note end time
    TXPacketCount++;
    packet_is_OK();
  }
  else
  {
    packet_is_Error();                                 //transmit packet returned 0, there was an error
  }

  //digitalWrite(LED1, LOW);
  Serial.println();
  delay(packet_delay);                                 //have a delay between packets
}

/////////////
void printHEXPacket(uint8_t *buffer, uint8_t size){

  for ( int i = 0; i < size; i++ )
  {
    if (buffer[i] < 10) Serial.print(F("0"));
    Serial.print( buffer[i], HEX);
    Serial.print(" ");
  }

}

void packet_is_OK()
{
  //if here packet has been sent OK
  uint16_t localCRC;

  Serial.print(F("  BytesSent,"));
  Serial.print(TXPacketL);                             //print transmitted packet length
  //localCRC = LT.CRCCCITT(buff, TXPacketL, 0xFFFF);
  //Serial.print(F("  CRC,"));
  //Serial.print(localCRC, HEX);                              //print CRC of sent packet
  Serial.print(F("  TransmitTime,"));
  Serial.print(endmS - startmS);                       //print transmit time of packet
  Serial.print(F("mS"));
  Serial.print(F("  PacketsSent,"));
  Serial.print(TXPacketCount);                         //print total of packets sent OK
}

void packet_is_Error()
{
  //if here there was an error transmitting packet
  uint16_t IRQStatus;
  IRQStatus = LT.readIrqStatus();                  //read the the interrupt register
  Serial.print(F(" SendError,"));
  Serial.print(F("Length,"));
  Serial.print(TXPacketL);                         //print transmitted packet length
  Serial.print(F(",IRQreg,"));
  Serial.print(IRQStatus, HEX);                    //print IRQ status
  LT.printIrqStatus();                             //prints the text of which IRQs set
}

/*void led_Flash(uint16_t flashes, uint16_t delaymS)
{
  uint16_t index;
  for (index = 1; index <= flashes; index++)
  {
    digitalWrite(LED1, HIGH);
    delay(delaymS);
    digitalWrite(LED1, LOW);
    delay(delaymS);
  }
}*/

