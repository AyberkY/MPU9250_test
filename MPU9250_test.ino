#include "MPU9250.h"

int ledPin = 13;

MPU9250 IMU1;

void setup() 
{
  Wire.begin();

  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);

  byte c = IMU1.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250: "); Serial.print("I AM "); Serial.print(c, HEX);

  if(c == 0x73) //Sparkfun breakouts return 0x68, but I found that HiLetgo breakouts return 0x73 for some reason.
  {
    Serial.println("MPU9250 is online.");

    IMU1.initMPU9250();
    Serial.println("MPU9250 initialized for active data mode.");
  }
  else
  {
    Serial.print("Could not connect to MPU9250 at 0x"); Serial.println(c, HEX);
    while(1); // Loop forever if communication doesn't happen
  }
}

void loop() 
{
  //The least significant bit of int INT_STATUS register shows wheter or not raw data is ready to be read.
  if (IMU1.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {  
    IMU1.readAccelData(IMU1.accelCount);  // Read the x/y/z adc values
    IMU1.getAres();

    // Now we'll calculate the accleration value into actual g's
    // This depends on scale being set
    IMU1.ax = (float)IMU1.accelCount[0]*IMU1.aRes; // - accelBias[0];
    IMU1.ay = (float)IMU1.accelCount[1]*IMU1.aRes; // - accelBias[1];
    IMU1.az = (float)IMU1.accelCount[2]*IMU1.aRes; // - accelBias[2];

    IMU1.readGyroData(IMU1.gyroCount);  // Read the x/y/z adc values
    IMU1.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    IMU1.gx = (float)IMU1.gyroCount[0]*IMU1.gRes;
    IMU1.gy = (float)IMU1.gyroCount[1]*IMU1.gRes;
    IMU1.gz = (float)IMU1.gyroCount[2]*IMU1.gRes;
    
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

  Serial.print("Ax: "); Serial.print(IMU1.ax);
  Serial.print("g \t");
  Serial.print("Ay: "); Serial.print(IMU1.ay);
  Serial.print("g \t");
  Serial.print("Az: "); Serial.print(IMU1.az);
  Serial.print("g \t");

  Serial.print("Gx: "); Serial.print(IMU1.gx);
  Serial.print("d/s \t");
  Serial.print("Gy: "); Serial.print(IMU1.gy);
  Serial.print("d/s \t");
  Serial.print("Gz: "); Serial.print(IMU1.gz);
  Serial.print("d/s \t\n");
  
}
