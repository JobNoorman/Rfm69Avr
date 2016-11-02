#include "Rfm69.h"

#include "Spi.h"
#include "Debug.h"

#ifndef RFM69_FREQUENCY
#define RFM69_FREQUENCY 868
#endif

#ifndef RFM69_NETWORK_ID
#define RFM69_NETWORK_ID 0x0101
#endif

#ifndef RFM69_NODE_ADDRESS
#define RFM69_NODE_ADDRESS 1
#endif

namespace
{
    constexpr uint32_t Frf    = RFM69_FREQUENCY * 1'000'000 / Rfm69::Fstep;
    constexpr uint8_t  FrfMsb = (Frf >> 16) & 0xff;
    constexpr uint8_t  FrfMid = (Frf >>  8) & 0xff;
    constexpr uint8_t  FrfLsb =  Frf        & 0xff;

    constexpr uint8_t SyncValue1 = (RFM69_NETWORK_ID >> 8) & 0xff;
    constexpr uint8_t SyncValue2 =  RFM69_NETWORK_ID       & 0xff;
    static_assert(SyncValue1 != 0x00 && SyncValue2 != 0x00,
                  "Network ID cannot contain 0-bytes");
}

namespace Rfm69
{

Rfm69::Rfm69()
{
    SpiInit();

    auto version = readRegister(Reg::Version);

    if (version != 0x24)
    {
        DBG_PRINTF("Radio reports wrong version 0x%x\n", version);
        return;
    }

    setMode(Mode::Standby);

    // Configure the radio

    // Common configuration
    // Mode: Standby (default)
    // DataMode: Packet (default)
    // ModulationType: FSK (default)
    // ModulationShaping: No shaping (default)
    // BitRate: 4.8 kb/s (default)
    // Fdev (frequency deviation): 5 kHz (default)
    // Frf (carrier frequency)
    writeRegister(Reg::FrfMsb, FrfMsb);
    writeRegister(Reg::FrfMid, FrfMid);
    writeRegister(Reg::FrfLsb, FrfLsb);

    // Packet engine configuration
    // PreambleSize: 3 (default)
    // SyncConfig: Enable 1 byte sync word detection and no error tolerance
    writeRegister(Reg::SyncConfig,
                  SyncConfig::SyncOn      |
                  SyncConfig::SyncSize<2> |
                  SyncConfig::SyncTol<0>);
    // SyncValue: Network ID
    writeRegister(Reg::SyncValue1, SyncValue1);
    writeRegister(Reg::SyncValue2, SyncValue2);
    // PacketConfig1: Variable length packets with CRC, whitening encoding and
    //                filter incoming packets on node address
    writeRegister(Reg::PacketConfig1,
                  PacketConfig1::VariableLength    |
                  PacketConfig1::CrcOn             |
                  PacketConfig1::DcFree::Whitening |
                  PacketConfig1::AddressFiltering::NodeAddress);
    // PayloadLength: Maximum size of incoming packets, 64 (default)
    // NodeAdrs
    setNodeAddress(RFM69_NODE_ADDRESS);
    // BroadcastAdrs: 0x00 (default, not used)
}

void Rfm69::setNodeAddress(NodeAddress address)
{
    writeRegister(Reg::NodeAdrs, address);
}

NodeAddress Rfm69::nodeAddress()
{
    return readRegister(Reg::NodeAdrs);
}

void Rfm69::send(const Frame& frame)
{
    // Read this first since we cannot read register while writing the FIFO
    auto source = nodeAddress();

    SpiSelect();
    SpiTransfer(static_cast<uint8_t>(Reg::Fifo) | RegWriteFlag);
    SpiTransfer(frame.length + 2);
    SpiTransfer(frame.destination);
    SpiTransfer(source);

    for (size_t i = 0; i < frame.length; i++)
        SpiTransfer(frame.payload[i]);

    SpiDeselect();

    setMode(Mode::Tx);

    while (!(readRegister(Reg::IrqFlags2) & IrqFlags2::PacketSent)) {}

    setMode(Mode::Standby);
}

void Rfm69::receive(Frame& frame)
{
    setMode(Mode::Rx);

    while (!(readRegister(Reg::IrqFlags2) & IrqFlags2::PayloadReady)) {}

    setMode(Mode::Standby);

    SpiSelect();
    SpiTransfer(static_cast<uint8_t>(Reg::Fifo));
    auto length = SpiTransfer(0) - 2;

    if (length > frame.length)
    {
        DBG_PRINTF("Dropping frame of length %u\n", length);
        frame.length = 0;
        return;
    }

    frame.length = length;
    frame.destination = SpiTransfer(0);
    frame.source = SpiTransfer(0);

    for (uint8_t i = 0; i < length; i++)
        frame.payload[i] = SpiTransfer(0);

    SpiDeselect();
}

void Rfm69::setMode(Mode mode)
{
    // We have to make sure that the other bits in RegOpMode are not affected
    auto opmode = readRegister(Reg::OpMode);
    opmode &= ~OpMode::Mode::Mask;
    opmode |= static_cast<uint8_t>(mode);
    writeRegister(Reg::OpMode, opmode);

    // Make sure the mode is ready before returning
    while (!(readRegister(Reg::IrqFlags1) & IrqFlags1::ModeReady)) {}
}

uint8_t Rfm69::readRegister(Reg reg)
{
    SpiSelect();
    SpiTransfer(static_cast<uint8_t>(reg));
    auto value = SpiTransfer(0);
    SpiDeselect();
    return value;
}

void Rfm69::writeRegister(Reg reg, uint8_t value)
{
    SpiSelect();
    SpiTransfer(static_cast<uint8_t>(reg) | RegWriteFlag);
    SpiTransfer(value);
    SpiDeselect();
}

void Rfm69::printRegister(Reg reg)
{
    DBG_PRINTF("0x%02x: 0x%02x\n",
               static_cast<uint8_t>(reg), readRegister(reg));
}

void Rfm69::printRegisters()
{
    for (uint8_t i = 0; i < 0x50; i++)
        printRegister(static_cast<Reg>(i));

    Reg testRegs[] = {Reg::TestLna, Reg::TestPa1, Reg::TestPa2,
                      Reg::TestDagc, Reg::TestAfc};

    for (size_t i = 0; i < sizeof(testRegs); i++)
        printRegister(testRegs[i]);
}

}
