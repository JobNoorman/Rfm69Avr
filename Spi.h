#ifndef RFM69_SPI_H
#define RFM69_SPI_H

#include <stdint.h>

void SpiInit();
uint8_t SpiTransfer(uint8_t b);
void SpiSelect();
void SpiDeselect();

#endif
