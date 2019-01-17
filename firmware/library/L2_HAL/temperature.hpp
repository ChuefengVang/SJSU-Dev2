#pragma once

#include <cstdint>
#include <cstdio>
#include "L1_Drivers/i2c.hpp"

class TemperatureInterface
{
 public:
  virtual bool Initialize()                             = 0;
  virtual int GetCelsius(I2c TemperatureDevice)         = 0;
  virtual int GetFahrenheit(int TemperatureData)        = 0;
};

class Temperature : public TemperatureInterface
{
 public:
  bool Initialize() override
  {
   Activate.initialize();
   uint8_t Device_id = 0x00;
   // Checks to see if the value read by I2c for the Temperature Chip ID register
   // is the same as the Base ID (0x14).
   Activate.WriteThenRead(kTemperatureAddress, &kTemperatureId, 1, &Device_Id, 1);
   return(Device_Id == kMemoryMapAddress);
  }

 int GetCelsius(I2c TemperatureDevice) override
 {
  uint8_t const OneBurst [2]   = { 0xC4, 0x04 };
  uint8_t const AutoincBit [2] = { 0xC5, 0x01 };
  // The register will enable the device to collect data once
  // and automatically sets the stop bit to 0 (2nd bit).
  TemperatureDevice.Write(kTemperatureAddress, &OneBurst);
  // Auto increments I2c register address pointer.
  TemperatureDevice.Write(kTemperatureAddress, &AutoincBit);
  uint8_t LeastSignificantRegister = 0xC2;
  uint8_t MostSignificantRegister  = 0xC1;
  TemperatureDevice.WriteThenRead(kTemperatureAddress, &MostSignificantRegister, 1,
                                  &MostSignificantRegister, 1);
  TemperatureDevice.Read(kTemperatureAddress, &LeastSignificantRegister, 1);

  int data = 0;
  data = ((MostSignificantRegister & 0x7F) << 8) | LeastSignificantRegister;
  // The required computation after bit shifting.
  return (((data - 16384) / 160 ) + 55)
 }

 int GetFahrenheit(int TemperatureData)
 {
  // Formula used to convert Celsius to Fahrenheit
  return (((TemperatureData - 32) * 5) / 9 )
 }

 private:
  I2c Activate;
  uint8_t const kTemperatureAddress = 0x31;
  uint8_t const kTemperatureId      = 0xC0;
  uint8_t const kMemoryMapAddress   = 0x14;
};
