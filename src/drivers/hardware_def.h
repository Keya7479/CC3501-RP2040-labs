// All hardcoded definitions

// pin definitions 
#define LED_PIN 14
#define BUTTON_PIN 15 

// LED specific definitions
#define LED_PIO pio0
#define LED_SM 0
#define MAX_NUM_LED 12

// Accelerometer specific definitions
#define ACCEL_SDA_PIN 16
#define ACCEL_SCL_PIN 17
#define ACCEL_I2C_INSTANCE i2c0

#define ACCEL_I2C_ADDRESS 0x19  // slave address

#define ACCEL_WHO_AM_I 0x33  // expected register value
#define ACCEL_WHO_AM_I_ADDRESS 0x0F 

#define ACCEL_CTRL_REG_1 0x20 // sampling (data) rate: 1 Hz to 5.3 kHz. Default value: 0000 (power-down mode).
#define ACCEL_CTRL_REG_4 0x23 // accel range:  ±2g/±4g/±8g/±16g. Default is ±2g. 

