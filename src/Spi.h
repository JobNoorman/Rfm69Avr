#ifndef RFM69_SPI_H
#define RFM69_SPI_H

#include <stdint.h>

void SpiInit();
uint8_t SpiTransfer(uint8_t b);
void SpiSelect();
void SpiDeselect();

class SpiSession
{
public:

    SpiSession();
    ~SpiSession();

    uint8_t transfer(uint8_t b=0);
};

#endif
