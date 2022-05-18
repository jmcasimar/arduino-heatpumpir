/*
    Airwell air conditioner control (Airwell remote control P/N ARC433B50)
*/
#ifndef AirwellIR_h
#define AirwellIR_h

#include <HeatpumpIR.h>


// Airwell timing constants
#define AIRWELL_AIRCON_HDR_MARK   3360 // 3344 
#define AIRWELL_AIRCON_HDR_SPACE  1260 // 1260
#define AIRWELL_AIRCON_BIT_MARK   430  // 430
#define AIRWELL_AIRCON_ONE_SPACE  1030 // 1030
#define AIRWELL_AIRCON_ZERO_SPACE 350  // 350

// Airwell codes
#define AIRWELL_AIRCON_MODE_OFF   0xC4 // Power OFF
#define AIRWELL_AIRCON_MODE_ON    0xE4
#define AIRWELL_AIRCON_MODE_AUTO  0x10
#define AIRWELL_AIRCON_MODE_HEAT  0x80 // Operating mode
#define AIRWELL_AIRCON_MODE_COOL  0xC0
#define AIRWELL_AIRCON_MODE_DRY   0x40
#define AIRWELL_AIRCON_MODE_FAN   0xE0
#define AIRWELL_AIRCON_FAN_AUTO   0x00 // Fan speed
#define AIRWELL_AIRCON_FAN1       0x40
#define AIRWELL_AIRCON_FAN2       0xC0
#define AIRWELL_AIRCON_FAN3       0xA0
#define AIRWELL_AIRCON_FAN4       AIRWELL_AIRCON_FAN_AUTO
#define AIRWELL_AIRCON_FAN5       AIRWELL_AIRCON_FAN_AUTO
#define AIRWELL_AIRCON_TEMP16     0xF0 // Temperature
#define AIRWELL_AIRCON_TEMP17     0x70
#define AIRWELL_AIRCON_TEMP18     0xB0
#define AIRWELL_AIRCON_TEMP19     0x30
#define AIRWELL_AIRCON_TEMP20     0xD0
#define AIRWELL_AIRCON_TEMP21     0x50
#define AIRWELL_AIRCON_TEMP22     0x90
#define AIRWELL_AIRCON_TEMP23     0x10
#define AIRWELL_AIRCON_TEMP24     0xE0
#define AIRWELL_AIRCON_TEMP25     0x60
#define AIRWELL_AIRCON_TEMP26     0xA0
#define AIRWELL_AIRCON_TEMP27     0x20
#define AIRWELL_AIRCON_TEMP28     0xC0
#define AIRWELL_AIRCON_TEMP29     0x40
#define AIRWELL_AIRCON_TEMP30     0x80
// CheckSum manual
#define AIRWELL_AIRCON_CHECKSUM_OFF   0x49 // Checksum

class AirwellIR : public HeatpumpIR
{
  public:
    AirwellIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd);

  private:
    uint8_t reverseByte(uint8_t myByte);
    void sendAirwell(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature);
};

#endif