#include <AirwellIR.h>

AirwellIR::AirwellIR() : HeatpumpIR()
{
  static const char model[] PROGMEM = "Airwell";
  static const char info[]  PROGMEM = "{\"mdl\":\"airwell\",\"dn\":\"Airwell\",\"mT\":16,\"xT\":30,\"fs\":4,\"maint\":[10,11,12,13,14,15,16,17]}}}";

  _model = model;
  _info = info;
}

uint8_t AirwellIR::reverseByte(uint8_t myByte)
{   uint8_t myByteReversed = 0;
    for (int i = 0; i<8; i++){
        if (myByte & (1 << i)) myByteReversed |= 1 << (7-i);
    }
    return  myByteReversed;
}

// Airwell numeric values to command bytes
void AirwellIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd)
{
  // Sensible defaults
  uint8_t powerMode     = AIRWELL_AIRCON_MODE_OFF;
  uint8_t operatingMode = AIRWELL_AIRCON_MODE_AUTO;
  uint8_t fanSpeed      = AIRWELL_AIRCON_FAN_AUTO;
  uint8_t temperature   = AIRWELL_AIRCON_TEMP23;

  switch (powerModeCmd)
  {
    case POWER_ON:
      powerMode = AIRWELL_AIRCON_MODE_ON;
      break;
    case POWER_OFF:
      powerMode = AIRWELL_AIRCON_MODE_OFF;
      temperatureCmd = 16; // 16°C
      fanSpeedCmd = FAN_1; // Fan Low
      break;
  }

  switch (operatingModeCmd)
  {
    case MODE_AUTO:
      operatingMode = AIRWELL_AIRCON_MODE_AUTO;
      break;
    case MODE_HEAT:
      operatingMode = AIRWELL_AIRCON_MODE_HEAT;
      if (temperatureCmd<20) temperatureCmd = 20; // 20°C
      break;
    case MODE_COOL:
      operatingMode = AIRWELL_AIRCON_MODE_COOL;
      break;
    case MODE_DRY:
      operatingMode = AIRWELL_AIRCON_MODE_DRY;
      break;
    case MODE_FAN:
      operatingMode = AIRWELL_AIRCON_MODE_FAN;
      if (fanSpeedCmd < FAN_1) fanSpeedCmd = FAN_1; // Fan Low
      break;
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = AIRWELL_AIRCON_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = AIRWELL_AIRCON_FAN1;
      break;
    case FAN_2:
      fanSpeed = AIRWELL_AIRCON_FAN2;
      break;
    case FAN_3:
      fanSpeed = AIRWELL_AIRCON_FAN3;
      break;
    case FAN_4:
      fanSpeed = AIRWELL_AIRCON_FAN4;
      break;
    case FAN_5:
      fanSpeed = AIRWELL_AIRCON_FAN5;
      break;
  }

  if ((operatingModeCmd == MODE_HEAT && temperatureCmd >= 20 && temperatureCmd <= 30) ||
      (temperatureCmd >= 16 && temperatureCmd <= 30))
  { 
    switch (temperatureCmd)
    {
        case 16:
            temperature = AIRWELL_AIRCON_TEMP16;
            break;
        case 17:
            temperature = AIRWELL_AIRCON_TEMP17;
            break;
        case 18:
            temperature = AIRWELL_AIRCON_TEMP18;
            break;
        case 19:
            temperature = AIRWELL_AIRCON_TEMP19;
            break;
        case 20:
            temperature = AIRWELL_AIRCON_TEMP20;
            break;
        case 21:
            temperature = AIRWELL_AIRCON_TEMP21;
            break;
        case 22:
            temperature = AIRWELL_AIRCON_TEMP22;
            break;
        case 23:
            temperature = AIRWELL_AIRCON_TEMP23;
            break;
        case 24:
            temperature = AIRWELL_AIRCON_TEMP24;
            break;
        case 25:
            temperature = AIRWELL_AIRCON_TEMP25;
            break;
        case 26:
            temperature = AIRWELL_AIRCON_TEMP26;
            break;
        case 27:
            temperature = AIRWELL_AIRCON_TEMP27;
            break;
        case 28:
            temperature = AIRWELL_AIRCON_TEMP28;
            break;
        case 29:
            temperature = AIRWELL_AIRCON_TEMP29;
            break;
        case 30:
            temperature = AIRWELL_AIRCON_TEMP30;
            break;
    }
  }

  sendAirwell(IR, powerMode, operatingMode, fanSpeed, temperature);
}


// Send the Airwell code
void AirwellIR::sendAirwell(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature)
{
  // Default command
  uint8_t airwellTemplate[14] = {
    0xC4, 0xD3, 0x64, 0x80, 0x00, 0xE4, 0xC0,
   //  0    1     2     3     4     5     6
    0xE0, 0x00, 0x00, 0x00, 0x00, 0x08, 0x6A};
   //  7    8     9    10    11    12    13
   
  airwellTemplate[5] = powerModeCmd;   // Byte 5
  airwellTemplate[6] = operatingMode; // Byte 6
  airwellTemplate[7] = temperature;   // Byte 7
  airwellTemplate[8] = fanSpeed;      // Byte 8 include fan velocity and direction (for now just controlling velocity)
  
  // Checksum calculation in byte 13
  // Parece que byte 6 y 8 no importan en el checkSum
  // Corrección byte 6 y 7 si importan en el checkSum valores diferentes pueden dar el mismo checkSum
  // * Checksums at bytes 7 and 15 are calculated the same way
  uint8_t checksum = 0x00;

  for (int i=0; i<13; i++) {
    checksum += reverseByte(airwellTemplate[i]);
  }

  airwellTemplate[13] = reverseByte(checksum);

  for (int i=0; i<14; i++) {
    Serial.print(airwellTemplate[i], HEX); Serial.print(", ");
  }
  Serial.println();

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  IR.mark(AIRWELL_AIRCON_HDR_MARK);
  IR.space(AIRWELL_AIRCON_HDR_SPACE);

  // Send commands
  for (int i=0; i<14; i++) {
    // send data from MSB to LSB until mask bit is shifted out
    for (uint32_t tMask = 1UL << (8 - 1); tMask; tMask >>= 1) {
        if (airwellTemplate[i] & tMask) {
            IR.mark(AIRWELL_AIRCON_BIT_MARK);
            IR.space(AIRWELL_AIRCON_ONE_SPACE);
        } else {
            IR.mark(AIRWELL_AIRCON_BIT_MARK);
            IR.space(AIRWELL_AIRCON_ZERO_SPACE);
        }
    }
  }
  // Finish the transmission
  IR.mark(AIRWELL_AIRCON_BIT_MARK);
  IR.space(0);
  // IR.IRLedOff();

  /*
  IrSender.mark(HEADER);      // Send header
  IrSender.space(START_BIT);  // Start bit
  for(int i = 0; i<14; i++) IrSender.sendPulseDistanceWidthData(MARK, ONE_BIT, MARK, ZERO_BIT, commands[i], 8, true, false);
  IrSender.mark(MARK);        // Send header
  IrSender.IRLedOff();        // Always end with the LED off
  */
}