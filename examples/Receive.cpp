#include "UartStdio/UartStdio.h"

#include "Rfm69/Rfm69.h"

#include <stdio.h>

int main()
{
    UartStdio::Init();

    Rfm69::Rfm69 radio;
    radio.setFrequency(868000000);
    radio.setNodeAddress(RECEIVER_ADDRESS);

    uint8_t buf[64];
    Rfm69::Frame frame;
    frame.payload = buf;

    while (true)
    {
        frame.length = sizeof(buf);
        radio.receive(frame);
        printf("Received '%s'\n", frame.payload);
    }
}
