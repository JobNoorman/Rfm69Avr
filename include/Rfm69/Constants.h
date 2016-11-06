#ifndef RFM69_CONSTANTS_H
#define RFM69_CONSTANTS_H

#include <stdint.h>

namespace Rfm69
{

enum class Reg : uint8_t
{
    Fifo          = 0x00,
    OpMode        = 0x01,
    DataModul     = 0x02,
    BitrateMsb    = 0x03,
    BitrateLsb    = 0x04,
    FdevMsb       = 0x05,
    FdevLsb       = 0x06,
    FrfMsb        = 0x07,
    FrfMid        = 0x08,
    FrfLsb        = 0x09,
    Osc1          = 0x0a,
    AfcCtrl       = 0x0b,
    Listen1       = 0x0d,
    Listen2       = 0x0e,
    Listen3       = 0x0f,
    Version       = 0x10,
    PaLevel       = 0x11,
    PaRamp        = 0x12,
    Ocp           = 0x13,
    Lna           = 0x18,
    RxBw          = 0x19,
    AfcBw         = 0x1a,
    OokPeak       = 0x1b,
    OokAvg        = 0x1c,
    OokFix        = 0x1d,
    AfcFei        = 0x1e,
    AfcMsb        = 0x1f,
    AfcLsb        = 0x20,
    FeiMsb        = 0x21,
    FeiLsb        = 0x22,
    RssiConfig    = 0x23,
    RssiValue     = 0x24,
    DioMapping1   = 0x25,
    DioMapping2   = 0x26,
    IrqFlags1     = 0x27,
    IrqFlags2     = 0x28,
    RssiThresh    = 0x29,
    RxTimeout1    = 0x2a,
    RxTimeout2    = 0x2b,
    PreambleMsb   = 0x2c,
    PreambleLsb   = 0x2d,
    SyncConfig    = 0x2e,
    SyncValue1    = 0x2f,
    SyncValue2    = 0x30,
    SyncValue3    = 0x31,
    SyncValue4    = 0x32,
    SyncValue5    = 0x33,
    SyncValue6    = 0x34,
    SyncValue7    = 0x35,
    SyncValue8    = 0x36,
    PacketConfig1 = 0x37,
    PayloadLength = 0x38,
    NodeAdrs      = 0x39,
    BroadcastAdrs = 0x3a,
    AutoModes     = 0x3b,
    FifoThresh    = 0x3c,
    PacketConfig2 = 0x3d,
    AesKey1       = 0x3e,
    AesKey2       = 0x3f,
    AesKey3       = 0x40,
    AesKey4       = 0x41,
    AesKey5       = 0x42,
    AesKey6       = 0x43,
    AesKey7       = 0x44,
    AesKey8       = 0x45,
    AesKey9       = 0x46,
    AesKey10      = 0x47,
    AesKey11      = 0x48,
    AesKey12      = 0x49,
    AesKey13      = 0x4a,
    AesKey14      = 0x4b,
    AesKey15      = 0x4c,
    AesKey16      = 0x4d,
    Temp1         = 0x4e,
    Temp2         = 0x4f,
    TestLna       = 0x58,
    TestPa1       = 0x5a,
    TestPa2       = 0x5c,
    TestDagc      = 0x6f,
    TestAfc       = 0x71
};

namespace Detail
{
    template<uint8_t N, uint8_t Shift, uint8_t Min, uint8_t Max,
             int8_t Adjust=0>
    struct IntBitmask
    {
        static_assert(N >= Min && N <= Max, "Integer out of range");

        enum
        {
            Mask = (N + Adjust) << Shift
        };
    };

    #define INT_BITMASK(name, ...) \
        template<uint8_t N>        \
        uint8_t name = Detail::IntBitmask<N, __VA_ARGS__>::Mask
}

namespace OpMode
{
    enum
    {
        SequencerOff = (1 << 7),
        ListenOn     = (1 << 6),
        ListenAbort  = (1 << 5)
    };

    namespace Mode
    {
        enum
        {
            Mask  = (0b111 << 2),
            Sleep = (0b000 << 2),
            Stdby = (0b001 << 2),
            Fs    = (0b010 << 2),
            Tx    = (0b011 << 2),
            Rx    = (0b100 << 2)
        };
    }
}

namespace SyncConfig
{
    enum
    {
        SyncOn            = (1 << 7),
        FifoFillCondition = (1 << 6)
    };

    INT_BITMASK(SyncSize, 3, 1, 8, -1);
    INT_BITMASK(SyncTol, 0, 0, 7);
}

namespace PacketConfig1
{
    enum
    {
        VariableLength  = (1 << 7),
        CrcOn           = (1 << 4),
        CrcAutoClearOff = (1 << 3)
    };

    namespace DcFree
    {
        enum
        {
            None       = (0b00 << 5),
            Manchester = (0b01 << 5),
            Whitening  = (0b10 << 5)
        };
    }

    namespace AddressFiltering
    {
        enum
        {
            None                   = (0b00 << 1),
            NodeAddress            = (0b01 << 1),
            NodeOrBroadcatsAddress = (0b10 << 1)
        };
    }
}

namespace IrqFlags1
{
    enum
    {
        ModeReady        = (1 << 7),
        RxReady          = (1 << 6),
        TxReady          = (1 << 5),
        PllLock          = (1 << 4),
        Rssi             = (1 << 3),
        Timeout          = (1 << 2),
        AutoMode         = (1 << 1),
        SyncAddressMatch = (1 << 0)
    };
}

namespace IrqFlags2
{
    enum
    {
        FifoFull     = (1 << 7),
        FifoNotEmpty = (1 << 6),
        FifoLevel    = (1 << 5),
        FifoOverrun  = (1 << 4),
        PacketSent   = (1 << 3),
        PayloadReady = (1 << 2),
        CrcOk        = (1 << 1)
    };
}

namespace FifoThresh
{
    namespace TxStartCondition
    {
        enum
        {
            FifoNotEmpty = (1 << 7)
        };
    }
}

enum
{
    RegWriteFlag = 0x80
};

constexpr float Fxosc = 32e6;
constexpr float Fstep = Fxosc / 524288;

}

#undef INT_BITMASK

#endif
