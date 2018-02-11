#ifndef AS5048B_CHIP_H
#define AS5048B_CHIP_H

#include <Arduino.h>
#include <Wire.h>

// DEBUGING
// #define AS5048B_SERIAL_DEBUG

#ifdef AS5048B_SERIAL_DEBUG
#include <Debug.h>
#define AS5048B_DPRINT(...) DPRINT(__VA_ARGS__)
#define AS5048B_DPRINTLN(...) DPRINTLN(__VA_ARGS__)
#else
#define AS5048B_DPRINT(...)
#define AS5048B_DPRINTLN(...)
#endif

// Addresses AS5048B
#define AS5048B_DEFAULT_ADDRESS 0x40 // 0b10000
#define AS5048B_PROG_REG 0x03
#define AS5048B_ADDR_REG 0x15
#define AS5048B_ZEROMSB_REG 0x16 // bits 0..7
#define AS5048B_ZEROLSB_REG 0x17 // bits 0..5
#define AS5048B_GAIN_REG 0xFA
#define AS5048B_DIAG_REG 0xFB
#define AS5048B_MAGNMSB_REG 0xFC // bits 0..7
#define AS5048B_MAGNLSB_REG 0xFD // bits 0..5
#define AS5048B_ANGLMSB_REG 0xFE // bits 0..7
#define AS5048B_ANGLLSB_REG 0xFF // bits 0..5
#define AS5048B_RESOLUTION 16384 // 14 bits

class AS5048B_Chip {
public:
  AS5048B_Chip(unsigned char i2c_address = AS5048B_DEFAULT_ADDRESS);

  void writeAddressRegister(unsigned char value);
  unsigned char readAddressRegister(void);

  void writeZeroRegister(unsigned short value);
  unsigned short readZeroRegister(void);

  unsigned char readDiagnosticRegister(void);
  unsigned char readGainRegister(void);
  unsigned short readMagnitudeRegister(void);

  unsigned short readAngleRegister(void);

private:
  unsigned char deviceAddress;

  void writeByte(unsigned char address, unsigned char value);
  unsigned char readByte(unsigned char address);
  unsigned short read2Bytes(unsigned char address);
};

#endif /* end of include guard: AS5048B_CHIP_H */
