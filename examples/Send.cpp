#include "UartStdio/UartStdio.h"

#include "Rfm69/Rfm69.h"

#include <stdio.h>
#include <assert.h>

#include <util/delay.h>

int main()
{
    UartStdio::Init();

    Rfm69::Rfm69 radio;
    radio.setFrequency(868);
    radio.setNodeAddress(SENDER_ADDRESS);

    uint8_t buf[64];
    Rfm69::Frame frame;
    frame.destination = RECEIVER_ADDRESS;
    frame.payload = buf;

    unsigned counter  = 0;

    while (true)
    {
        auto result = snprintf(reinterpret_cast<char*>(buf), sizeof(buf),
                               "Hello times %u", ++counter);
        assert(result >= 0);
        unsigned len = result;
        assert(len < sizeof(buf));

        printf("Sending '%s'\n", buf);
        frame.length = len + 1;
        radio.send(frame);

        _delay_ms(1000);
    }
}
