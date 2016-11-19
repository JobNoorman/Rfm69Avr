#ifndef RFM69_RFM69_H
#define RFM69_RFM69_H

#include "Constants.h"

#include <stddef.h>

namespace Rfm69
{

using NodeAddress = uint8_t;
using Frequency   = uint32_t;
using BitRate     = uint32_t;

struct Frame
{
    uint8_t length;
    NodeAddress source;
    NodeAddress destination;
    uint8_t* payload;
};

class Rfm69
{
public:

    Rfm69();

    void setFrequency(Frequency frequency);
    void setBitRate(BitRate bitRate);
    void setNodeAddress(NodeAddress address);
    NodeAddress nodeAddress();

    void send(const Frame& frame);
    void receive(Frame& frame);

private:

    enum class Mode
    {
        Sleep   = OpMode::Mode::Sleep,
        Standby = OpMode::Mode::Stdby,
        Fs      = OpMode::Mode::Fs,
        Tx      = OpMode::Mode::Tx,
        Rx      = OpMode::Mode::Rx
    };

    void setMode(Mode mode);
    void writeFrame(const Frame& frame);

    uint8_t readRegister(Reg reg);
    void writeRegister(Reg reg, uint8_t value);

    void printRegister(Reg reg);
    void printRegisters();
};

}

#endif
