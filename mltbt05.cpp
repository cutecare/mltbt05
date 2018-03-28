/*
Part of http://cutecare.ru project
Author: evgeny.savitsky@gmail.com
*/
#include "mltbt05.h"

/**
 * Constructor
 */
MLTBT05::MLTBT05(int rxPin, int txPin, int resetPin, long baud)
{
	bleRxPin = rxPin;
	bleTxPin = txPin;
	bleResetPin = resetPin;
	bleBaud = baud;
}

void MLTBT05::configure(const char * bleName)
{
	pinMode(bleRxPin, OUTPUT);
	
	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);
	
	char text[32] = "";
	sprintf(text, "AT+NAME%s", bleName);
	sendCommand(&bleSerial, text);

	sendCommand(&bleSerial, "AT+ROLE3");	// slave mode
	sendCommand(&bleSerial, "AT+TYPE0");	// unsecure, no pin required
	sendCommand(&bleSerial, "AT+POWE3");	// max RF power
	sendCommand(&bleSerial, "AT+ADVI3");	// advertising interval 300ms
	sendCommand(&bleSerial, "AT+MARJ0x0000");    
	sendCommand(&bleSerial, "AT+MINO0x0000");
	sendCommand(&bleSerial, "AT+RESET");    // sleep
	
	pinMode(bleRxPin, INPUT);
}

void MLTBT05::setData(unsigned int minor, unsigned int major, bool autosleep)
{
	pinMode(bleRxPin, OUTPUT);
	wakeUpBLE();

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);

	char buff[32] = "";
	sprintf(buff, "AT+MARJ0x%04X", major);
	sendCommand(&bleSerial, buff);
	sprintf(buff, "AT+MINO0x%04X", minor);
	sendCommand(&bleSerial, buff);
	if (autosleep) {
		sendCommand(&bleSerial, "AT+RESET");
	}

	pinMode(bleRxPin, INPUT);
}

void MLTBT05::major(unsigned int value, bool autosleep)
{
	pinMode(bleRxPin, OUTPUT);
	wakeUpBLE();

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);

	char buff[32] = "";
	sprintf(buff, "AT+MARJ0x%04X", value);
	sendCommand(&bleSerial, buff);
	if (autosleep) {
		sendCommand(&bleSerial, "AT+RESET");
	}

	pinMode(bleRxPin, INPUT);
}

void MLTBT05::minor(unsigned int value, bool autosleep)
{
	pinMode(bleRxPin, OUTPUT);
	wakeUpBLE();

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);

	char buff[32] = "";
	sprintf(buff, "AT+MINO0x%04X", value);
	sendCommand(&bleSerial, buff);
	if (autosleep) {
		sendCommand(&bleSerial, "AT+RESET");
	}

	pinMode(bleRxPin, INPUT);
}

void MLTBT05::wakeUpBLE()
{
	pinMode(bleResetPin, OUTPUT);
	delay(300);
	digitalWrite(bleResetPin, HIGH);
	delay(300);
	digitalWrite(bleResetPin, LOW);
	pinMode(bleResetPin, INPUT);
}

void MLTBT05::sendCommand(SoftwareSerial * bleSerial, const char * data) {
	delay(250);
	bleSerial->println(data);
}
