#include <RHReliableDatagram.h>
#include <RH_ASK.h>
#include <SPI.h>

#define TRANSMITTER_ADDRESS 1
#define RECEIVER_ADDRESS 2
RH_ASK driver;
RHReliableDatagram manager(driver, RECEIVER_ADDRESS);

const int maxSize = 10;
const int LED = 13;

int pending = 0;
int writeTo = 0;
int readFrom = 0;
uint8_t data[10][32];

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  if (!manager.init())
    Serial.println("init failed");
}

void loop()
{
  readIt();
  sendIt();
}

void readIt() {
  if (manager.available())
  {
	digitalWrite(LED, HIGH);
    uint8_t len = sizeof(data[writeTo]);
    uint8_t from;
    manager.recvfromAck(data[writeTo], &len, &from);
    writeTo = next(writeTo);
    pending = pending + 1;
  }
}

void sendIt() {
  if (pending > 0) {
    Serial.write(data[readFrom], sizeof(data[readFrom]));
    memset (data[readFrom], 0, 32);
    readFrom = next(readFrom);
    pending = pending - 1;
  }
}

int next(int current) {
  if (current < maxSize) {
    return current + 1;
  } else {
    return 0;
  }
}
