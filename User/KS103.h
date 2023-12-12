#ifndef __KS103_H__
#define __KS103_H__

#include "i2c.h"

uint8_t KS103_ReadOneByte(uint8_t address, uint8_t reg);
void KS103_WriteOneByte(uint8_t address, uint8_t reg, uint8_t command);

#endif
