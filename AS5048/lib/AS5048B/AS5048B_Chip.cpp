#include <AS5048B_Chip.h>

AS5048B_Chip::AS5048B_Chip(unsigned char i2c_address)
    : deviceAddress(i2c_address) {}

void AS5048B_Chip::writeAddressRegister(unsigned char value) {
  this->writeByte(AS5048B_ADDR_REG, value);
  deviceAddress = ((value << 2) | (deviceAddress & 0b11)) ^ (1 << 6);
}

unsigned char AS5048B_Chip::readAddressRegister(void) {
  return this->readByte(AS5048B_ADDR_REG);
}

void AS5048B_Chip::writeZeroRegister(unsigned short value) {
  this->writeByte(AS5048B_ZEROMSB_REG, (uint8_t)(value >> 6));
  this->writeByte(AS5048B_ZEROLSB_REG, (uint8_t)(value & 0x3F));
}
unsigned short AS5048B_Chip::readZeroRegister(void) {
  return this->read2Bytes(AS5048B_ZEROMSB_REG);
}

unsigned char AS5048B_Chip::readDiagnosticRegister(void) {
  return this->readByte(AS5048B_DIAG_REG);
}
unsigned char AS5048B_Chip::readGainRegister(void) {
  return this->readByte(AS5048B_GAIN_REG);
}
unsigned short AS5048B_Chip::readMagnitudeRegister(void) {
  return this->read2Bytes(AS5048B_MAGNMSB_REG);
}

unsigned short AS5048B_Chip::readAngleRegister(void) {
  return this->read2Bytes(AS5048B_ANGLMSB_REG);
}

// PRIVATE

unsigned char AS5048B_Chip::readByte(unsigned char address) {

  Wire.beginTransmission(deviceAddress);
  Wire.write(address);

#ifdef AS5048B_SERIAL_DEBUG
  byte transmissionResult = Wire.endTransmission(false);
  if (transmissionResult) {
    AS5048B_DPRINT(
        "[ERROR]<<AS5048B_Chip::read2Bytes>> I2C Transmission result: ");
    AS5048B_DPRINTLN((String)transmissionResult);
  }
#else
  Wire.endTransmission(false);
#endif

  Wire.requestFrom(deviceAddress, (uint8_t)1);
  return (unsigned char)Wire.read();
}

unsigned short AS5048B_Chip::read2Bytes(unsigned char address) {
  unsigned short bytes[2];

  Wire.beginTransmission(deviceAddress);
  Wire.write(address);

#ifdef AS5048B_SERIAL_DEBUG
  byte transmissionResult = Wire.endTransmission(false);
  if (transmissionResult) {
    AS5048B_DPRINT(
        "[ERROR]<<AS5048B_Chip::read2Bytes>> I2C Transmission result: ");
    AS5048B_DPRINTLN((String)transmissionResult);
  }
#else
  Wire.endTransmission(false);
#endif

  Wire.requestFrom(deviceAddress, (uint8_t)2);

  bytes[0] = Wire.read();
  bytes[1] = Wire.read();

  return (((unsigned short)bytes[0] << 6) | (bytes[1] & 0x3F));
}

void AS5048B_Chip::writeByte(unsigned char address, unsigned char value) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(address);
  Wire.write(value);

#ifdef AS5048B_SERIAL_DEBUG
  byte transmissionResult = Wire.endTransmission();
  if (transmissionResult) {
    AS5048B_DPRINT(
        "[ERROR]<<AS5048B_Chip::writeByte>> I2C Transmission result: ");
    AS5048B_DPRINTLN((String)transmissionResult);
  }
#else
  Wire.endTransmission();
#endif
}
