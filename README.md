# ISXMPL3710ex1

# Example: Using the IS3710 I2C DMX Receiver Chip with Arduino IDE

## Objective
Demonstrate how to write a program for the **IS3710 I2C DMX Receiver** using the **Arduino IDE**.

## Required Material
- **Kappa3710Ard Board** (INACKS evaluation board featuring the IS3710)  
- **Arduino UNO Board**  
- **DMX Controller**  

## Setup
1. Flash the Arduino UNO with the provided code.  
2. Connect the Kappa3710 to the Arduino UNO.  
3. Connect a DMX controller to the Kappa3710.

**All set!** Now, moving the slider for **DMX Channel 1** should change the **red** color of the RGB LED on the **Kappa3710**. The same applies to **Channels 2 and 3**, which control the **green** and **blue** colors respectively.

## How it works
The Arduino code reads the IS3710 memory map registers 1, 2, and 3, which correspond to DMX channels 1, 2, and 3.  
It then maps these values to the PWM outputs on pins 9, 10, and 11.  
As a result, controlling these three DMX channels with a DMX controller will change the color of the RGB LED. 

## Other
- Buy the Kappa3710Ard board and download its documentation: [Kappa3710Ard Documentation](https://inacks.com/kappa3710ard)
- Buy the IS3710 chip and download its documentation: [IS3710 Documentation](https://inacks.com/is3710)


