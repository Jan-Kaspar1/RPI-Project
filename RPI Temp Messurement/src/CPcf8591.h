/*
 * CPcf8591.h
 *
 *  Created on: 31.05.2020
 *      Author: esy
 */

#ifndef SRC_CPCF8591_H_
#define SRC_CPCF8591_H_

#include "CI2cDevice.h"

class CPcf8591: public CI2cDevice {		//basis-Klasse CI2cDevice
public:
	explicit CPcf8591(uint8_t slaveAddress);
	float acquireValue(uint8_t CommandCode);
private:

};

#endif /* SRC_CPCF8591_H_ */
