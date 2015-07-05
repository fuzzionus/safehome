/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>

/*
 * Analog input example
 *
 * Demonstrate how to read an analog voltage value from an input pin using the
 * MRAA library, any sensor that outputs a variable voltage can be used with
 * this example code.
 * Suitable ones in the Grove Starter Kit are the Rotary Angle Sensor, Light
 * Sensor, Sound Sensor, Temperature Sensor.
 *
 * - analog in: analog sensor connected to pin A0 (Grove Base Shield Port A0)
 *
 * Additional linker flags: none
 */

/*
//1st
  float sensor_volt;
  float RS_air; //  Get the value of RS via in a clear air
  float R0;  // Get the value of R0 via in H2
  float sensorValue;

*--- Get a average data by testing 100 times ---
    for(int x = 0 ; x < 100 ; x++)
  {
    sensorValue = sensorValue + analogRead(A0);
  }
  sensorValue = sensorValue/100.0;
-----------------------------------------------*

  sensor_volt = sensorValue/1024*5.0;
  RS_air = (5.0-sensor_volt)/sensor_volt; // omit *RL
  R0 = RS_air/6.5; // The ratio of RS/R0 is 6.5 in a clear air

  Serial.print("sensor_volt = ");
  Serial.print(sensor_volt);
  Serial.println("V");

  Serial.print("R0 = ");
  Serial.println(R0);
  delay(1000);

 //2nd

  float sensor_volt;
  float RS_gas; // Get value of RS in a GAS
  float ratio; // Get ratio RS_GAS/RS_air
  int sensorValue = analogRead(A0);
  sensor_volt=(float)sensorValue/1024*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL

  *-Replace the name "R0" with the value of R0 in the demo of First Test -
  ratio = RS_gas/R0;  // ratio = RS/R0
  -----------------------------------------------------------------------*

  Serial.print("sensor_volt = ");
  Serial.println(sensor_volt);
  Serial.print("RS_ratio = ");
  Serial.println(RS_gas);
  Serial.print("Rs/R0 = ");
  Serial.println(ratio);

  Serial.print("\n\n");

  delay(1000);

*/

// return AI from A1
// -1 error
// 0 no gas detected
// 1 gas detected
int has_fire()
{
	static bool first_call = true;
	static mraa::Aio* a_pin;
	uint32_t pin_value = 0;

	if (first_call) {
		first_call = !first_call;
		// create an analog input object from MRAA using pin A1
		a_pin = new mraa::Aio(1);
		if (a_pin == NULL) {
			std::cerr << "Can't create mraa::Aio object, exiting" << std::endl;
			return -1; // MRAA_ERROR_UNSPECIFIED;
		}
	}

	for(int x = 0 ; x < 100 ; x++)
	{
	    pin_value = pin_value + a_pin->read();
	}

	std::cout << "fire analog input value " << pin_value << std::endl;

	return (pin_value < 4000) ? 1 : 0;
}

// return AI from A0
// -1 error
// 0 no gas detected
// 1 gas detected
int has_gas()
{
	static bool first_call = true;
	static mraa::Aio* a_pin;
	uint32_t pin_value = 0;

	if (first_call) {
		first_call = !first_call;
		// create an analog input object from MRAA using pin A0
		a_pin = new mraa::Aio(0);
		if (a_pin == NULL) {
			std::cerr << "Can't create mraa::Aio object, exiting" << std::endl;
			return -1; // MRAA_ERROR_UNSPECIFIED;
		}
	}

	for(int x = 0 ; x < 100 ; x++)
	{
	    pin_value = pin_value + a_pin->read();
	}

	std::cout << "gas analog input value " << pin_value << std::endl;

	return (pin_value > 6000) ? 1 : 0;
}

// return 0 if no error
int buz_alert(bool alert)
{
	static bool first_call = true;
	static mraa::Gpio* d_pin;

	if (first_call) {
		first_call = !first_call;

		// create a gpio object from MRAA using pin 2
		d_pin = new mraa::Gpio(2);
		if (d_pin == NULL) {
			std::cerr << "Can't create mraa::Gpio object, exiting" << std::endl;
			return -1; //MRAA_ERROR_UNSPECIFIED;
		}

		// set the pin as output
		if (d_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
			std::cerr << "Can't set digital pin as output, exiting"
					<< std::endl;
			return -1; //MRAA_ERROR_UNSPECIFIED;
		}
	}

	d_pin->write(alert ? 1 : 0);

	return 0;
}

// return 0 if no error
int led_alert(bool alert)
{
	static bool first_call = true;
	static mraa::Gpio* d_pin;

	if (first_call) {
		first_call = !first_call;

		// create a gpio object from MRAA using pin 4
		d_pin = new mraa::Gpio(4);
		if (d_pin == NULL) {
			std::cerr << "Can't create mraa::Gpio object, exiting" << std::endl;
			return -1; //MRAA_ERROR_UNSPECIFIED;
		}

		// set the pin as output
		if (d_pin->dir(mraa::DIR_OUT) != MRAA_SUCCESS) {
			std::cerr << "Can't set digital pin as output, exiting"
					<< std::endl;
			return -1; //MRAA_ERROR_UNSPECIFIED;
		}
	}

	d_pin->write(alert ? 1 : 0);

	return 0;
}

// return DI from pin 8 (Grove Base Shield Port D8)
// -1 error
// 0 no water detected
// 1 water detected
int has_water()
{
	static bool first_call = true;
	static mraa::Gpio* d_pin;

	if (first_call) {
			first_call = !first_call;

			// create a GPIO object from MRAA using pin 8
			d_pin = new mraa::Gpio(8);
			if (d_pin == NULL) {
				std::cerr << "Can't create mraa::Gpio object, exiting" << std::endl;
				return -1; // MRAA_ERROR_UNSPECIFIED;
			}

			// set the pin as input
			if (d_pin->dir(mraa::DIR_IN) != MRAA_SUCCESS) {
				std::cerr << "Can't set digital pin as input, exiting" << std::endl;
				return -1; //MRAA_ERROR_UNSPECIFIED;
			}
	}

	return d_pin->read() ? 0 : 1;
}

int main()
{
	// check that we are running on Galileo or Edison
	mraa_platform_t platform = mraa_get_platform_type();
	if ((platform != MRAA_INTEL_GALILEO_GEN1) &&
			(platform != MRAA_INTEL_GALILEO_GEN2) &&
			(platform != MRAA_INTEL_EDISON_FAB_C)) {
		std::cerr << "Unsupported platform, exiting" << std::endl;
		return MRAA_ERROR_INVALID_PLATFORM;
	}


	// loop forever
	for (;;) {
		bool water_leak = has_water() ? true : false;
		bool gas_leak   = has_gas() ? true : false;
		bool fire   = has_fire() ? true : false;
		bool alert  = water_leak || gas_leak || fire;

		(void) led_alert(alert);
		(void) buz_alert(alert);


		std::cout << "water  " << water_leak << std::endl;
		std::cout << "gas    " << gas_leak << std::endl;
		std::cout << "fire    " << fire << std::endl;

		sleep(1);
	}

	return MRAA_SUCCESS;
}
