/*
 * CI2cDevice.h
 *
 *  Created on: 31.05.2020
 *      Author: esy
 */

#ifndef SRC_CI2CDEVICE_H_
#define SRC_CI2CDEVICE_H_


#include <linux/i2c-dev.h>
#include "stdint.h"
#include <string>
#include <sys/types.h>		//open
#include <sys/stat.h>		//open
#include <fcntl.h>			//open
#include <sys/ioctl.h>		//input/output control
#include <unistd.h>			//read/write/close

using namespace std;

class CI2cDevice {
public:
	CI2cDevice(const string &i2CBus, uint8_t slaveAddress);
	virtual ~CI2cDevice();
	void sendByte(uint8_t data);
	uint8_t receiveByte();
	void writeByte(uint8_t commandCode, uint8_t data);
	uint8_t readByte(uint8_t commandCode);
	void writeWord(uint8_t commandCode, uint16_t data);
	uint16_t readWord(uint8_t commandCode);

private:
	uint8_t m_file; /**< uint8_t speichert den file */
	uint8_t m_slave_adress; /**< uint8_t Adresse des ausgewähten Slavebausteins */
};

#endif /* SRC_CI2CDEVICE_H_ */
