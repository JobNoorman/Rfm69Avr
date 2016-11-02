#include <avr/io.h>

void SpiInit()
{
    // The MISO pin (B4) is automatically configured as input in SPI master mode
    // Set SS (B2), MOSI (B3) and SCK (B5) as output
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);

    // Configure SPI
    // The default mode (CPOL=0, CPHA=0) is what the RF69 expects
    SPCR = (1 << SPE)  | // Enable
           (1 << MSTR);  // Master mode
}

uint8_t SpiTransfer(uint8_t b)
{
    SPDR = b;

    while (!(SPSR & (1 << SPIF))) {}

    return SPDR;
}

void SpiSelect()
{
    PORTB &= ~(1 << PORTB2);
}

void SpiDeselect()
{
    PORTB |= (1 << PORTB2);
}
