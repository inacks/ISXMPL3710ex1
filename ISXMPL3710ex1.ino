#include <Wire.h>

// -----------------------------------------------------------------------------
// This example is intended to be used with the Kappa3710 DMX shield.
// It demonstrates how to read DMX Channel 1, Channel 2, and Channel 3
// from the IS3710 chip via I2C, and map these DMX values to an RGB LED
// using the Arduino PWM outputs.
// -----------------------------------------------------------------------------

// IS3710 Memory Map Registers:
// DMX channels are mapped sequentially from address 1 to 512.
#define DMX_CH1   1
#define DMX_CH2   2
#define DMX_CH3   3
// ...
#define DMX_CH512 512

// IS3710 ID Register:
#define CHIP_ID   513

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

// I2C address of the IS3710
#define I2C_DEVICE_ADDRESS  16

// Expected value of the CHIP_ID register
#define CHIP_ID_VALUE       16

// -----------------------------------------------------------------------------
// Read one byte from a given IS3710 holding register
// The IS3710 uses a 16-bit register address over I2C.
// -----------------------------------------------------------------------------
uint8_t readIS3710Register(uint16_t holdingRegisterAddress) {
  uint8_t result; // Variable to store the read data.
  
  // Start I2C transmission to the IS3710 device
  Wire.beginTransmission(I2C_DEVICE_ADDRESS); 
  
  // Send the 16-bit register address (MSB first, then LSB)
  Wire.write((holdingRegisterAddress >> 8) & 0xFF); 
  Wire.write(holdingRegisterAddress & 0xFF);        
  
  // End transmission but keep the bus active (repeated START)
  Wire.endTransmission(false); 
  
  // Request one byte from the specified register
  Wire.requestFrom(I2C_DEVICE_ADDRESS, 1);  
  
  // Read the received byte
  result = Wire.read();
  
  return result; 
}

void setup() {
  uint16_t chipID;

  // Initialize the I2C interface
  Wire.begin();           
  
  // Initialize the serial port for debug messages
  Serial.begin(9600);     

  // ---------------------------------------------------------------------------
  // Detect the IS3710 chip by reading its CHIP_ID register
  // ---------------------------------------------------------------------------
  chipID = readIS3710Register(CHIP_ID);
  
  if (chipID == CHIP_ID_VALUE) {
    Serial.println("IS3710 Chip detected on I2C!");
    Serial.print("Chip ID: ");
    Serial.println(chipID);
  }
  else {
    Serial.print("\nERROR: IS3710 Chip NOT detected on I2C!");
    
    // Halt execution if the chip is not detected
    while(1);
  }
}

void loop() {
  uint8_t DMX_Channel_1, DMX_Channel_2, DMX_Channel_3;
  
  // Read DMX values from channels 1, 2, and 3
  DMX_Channel_1 = readIS3710Register(1);
  DMX_Channel_2 = readIS3710Register(2);
  DMX_Channel_3 = readIS3710Register(3);

  // Print DMX channel values to the serial monitor
  Serial.print("\n\nDMX_Channel_1: ");
  Serial.println(DMX_Channel_1);
  Serial.print("DMX_Channel_2: ");
  Serial.println(DMX_Channel_2);
  Serial.print("DMX_Channel_3: ");
  Serial.println(DMX_Channel_3); 

  // Map DMX channel values to PWM outputs
  analogWrite(9, DMX_Channel_1);
  analogWrite(10, DMX_Channel_2);
  analogWrite(11, DMX_Channel_3);

  delay(100);
}
