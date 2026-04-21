#include <Wire.h>

// -----------------------------------------------------------------------------
// Example for Kappa3710Ard DMX Shield
//
// This example reads DMX Channel 1, 2, and 3 from the IS3710 via I2C,
// and maps these values to RGB outputs using Arduino PWM pins.
//
// - DMX CH1 → Red   (Pin 9)
// - DMX CH2 → Green (Pin 10)
// - DMX CH3 → Blue  (Pin 11)
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// IS3710 Register Map
// -----------------------------------------------------------------------------
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

// Buffer for DMX data (index 1..512 used, index 0 unused for clarity)
uint8_t dmxData[513]; 

// Flag to indicate I2C read errors
bool i2cReadError;

// -----------------------------------------------------------------------------
// Read multiple consecutive registers from IS3710
//
// Parameters:
//   holdingRegisterAddress : Start register address (16-bit)
//   buffer                 : Destination buffer
//   length                 : Number of bytes to read
//
// Returns:
//   false → success
//   true  → error
// -----------------------------------------------------------------------------
bool readIS3710Registers(uint16_t holdingRegisterAddress, uint8_t *buffer, uint16_t length) {
  
  // Start transmission
  Wire.beginTransmission(I2C_DEVICE_ADDRESS);

  // Send 16-bit register address
  Wire.write((holdingRegisterAddress >> 8) & 0xFF);
  Wire.write(holdingRegisterAddress & 0xFF);

  // End transmission with repeated start
  uint8_t txStatus = Wire.endTransmission(false);

  // Check if address phase failed
  if (txStatus != 0) {
    return true; // error
  }

  // Request bytes
  uint16_t received = Wire.requestFrom(I2C_DEVICE_ADDRESS, length);

  // Check if we got all requested bytes
  if (received != length) {
    return true; // error
  }

  // Read data
  for (uint16_t i = 0; i < length; i++) {
    if (Wire.available()) {
      buffer[i] = Wire.read();
    } else {
      return true; // error (unexpected)
    }
  }

  return 0; // success
}

void setup() {
  uint8_t chipID;

  // Initialize the I2C interface
  Wire.begin();           
  
  // Initialize the serial port for debug messages
  Serial.begin(9600);     

  // ---------------------------------------------------------------------------
  // Detect the IS3710 chip by reading its CHIP_ID register
  // ---------------------------------------------------------------------------
  readIS3710Registers(CHIP_ID, &chipID, 1);
  
  if (chipID == CHIP_ID_VALUE) {
    Serial.println("IS3710 Chip detected on I2C!");
    Serial.print("Chip ID: ");
    Serial.println(chipID);
  }
  else {
    Serial.print("\nERROR: IS3710 Chip NOT detected on I2C!");
    Serial.println(chipID);
    // Halt execution if the chip is not detected
    while(1);
  }
}

void loop() {

  // Read 3 DMX channels starting from channel 1
  // Data is stored at dmxData[1], dmxData[2], dmxData[3]
  i2cReadError = readIS3710Registers(1, &dmxData[1], 3);

  if (i2cReadError == false) {

    // Print DMX values
    Serial.print("\nDMX_Channel_1: "); Serial.println(dmxData[DMX_CH1]);
    Serial.print("DMX_Channel_2: "); Serial.println(dmxData[DMX_CH2]);
    Serial.print("DMX_Channel_3: "); Serial.println(dmxData[DMX_CH3]);

    // Map DMX values (0–255) directly to PWM outputs
    analogWrite(9,  dmxData[DMX_CH1]); // Red
    analogWrite(10, dmxData[DMX_CH2]); // Green
    analogWrite(11, dmxData[DMX_CH3]); // Blue

    // Optional delay for easier serial monitoring
    // Remove for maximum refresh rate
    delay(100);
  }
}
