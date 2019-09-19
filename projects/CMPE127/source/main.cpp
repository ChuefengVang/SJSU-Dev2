#include <cstdint>
#include "lab1.hpp"
#include "utility/log.hpp"
#include "utility/time.hpp"

int main()
{
  LOG_INFO("Staring Hello World Application");
	Bus::ControlType kControl = Bus::ControlType::kMemory;
	uint8_t kBusAddress = 0b00110011, kBusData = 0b10101110;
	while(true) {		
	Bus bus;
	bus.Initialize();
	bus.Write(kControl, kBusAddress, kBusData);
}
  return -1;
}
