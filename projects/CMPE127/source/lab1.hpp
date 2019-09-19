
#include <cstdint>
#include "L1_Peripheral/lpc40xx/gpio.hpp"
class Bus
{
public:
enum class ControlType
{
kMemory = 0,
kIO
};
void Initialize()
{
// Setting each pin as Open Drain
ad[0].GetPin().SetAsOpenDrain();
ad[1].GetPin().SetAsOpenDrain();
ad[2].GetPin().SetAsOpenDrain();
ad[3].GetPin().SetAsOpenDrain();
ad[4].GetPin().SetAsOpenDrain();
ad[5].GetPin().SetAsOpenDrain();
ad[6].GetPin().SetAsOpenDrain();
ad[7].GetPin().SetAsOpenDrain();

// Do the rest yourself ...
// To disable Open drain do the following:
// ad[1].GetPin().SetAsOpenDrain(false);
// Do the rest of the setup here ...
//
write_enable.SetAsOutput();
ale.SetAsOutput();
m_io.SetAsOutput();
interrupt.SetAsInput();
}

void Write(ControlType control, uint8_t address, uint8_t data)
	{	
		uint8_t i = 0x00;
		uint8_t check = 0x00;
		write_enable.SetHigh();
		while (i < 8) {
			ad[i].SetAsOutput();
			check = address & 1;
		if (check) {
			ad[i].SetHigh();
			}
		else {
			ad[i].SetLow();
			}
			address >>= 1;
			i++;
		}
		
		ale.SetHigh();
		sjsu::Delay(500ms);
		ale.SetLow();
		
		check = 0x00;
		
		while (i < 8) {
			check = data & 1;
		if (check) {
			ad[i].SetHigh();
			}
		else {
			ad[i].SetLow();
			}
			data >>= 1;
			i++;
		}
	}

uint8_t Read(ControlType control, uint8_t address)
{
// Fill out Read Bus cycle here...
}
	private:
	sjsu::lpc40xx::Gpio ad[8] = {
	sjsu::lpc40xx::Gpio(2, 2),
	sjsu::lpc40xx::Gpio(2, 5),
	sjsu::lpc40xx::Gpio(2, 7),
	sjsu::lpc40xx::Gpio(2, 9),
	sjsu::lpc40xx::Gpio(0, 15),
	sjsu::lpc40xx::Gpio(0, 18),
	sjsu::lpc40xx::Gpio(0, 1),
	sjsu::lpc40xx::Gpio(0, 10),
	};
	// The gpio below will be used in Lab 4
	sjsu::lpc40xx::Gpio write_enable = sjsu::lpc40xx::Gpio(0, 17);
	sjsu::lpc40xx::Gpio ale = sjsu::lpc40xx::Gpio(0, 22);
	sjsu::lpc40xx::Gpio m_io = sjsu::lpc40xx::Gpio(0, 0);
	sjsu::lpc40xx::Gpio interrupt = sjsu::lpc40xx::Gpio(0, 11);
	};
