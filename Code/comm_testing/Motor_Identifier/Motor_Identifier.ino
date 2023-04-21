/**
 * CLARA MQP 2023
 * Author: Derik Pignone
 * Upload to mainboard to handle I2C communication between smart motor driver boards
 * and help identify motor addresses.
 */

 #include <Wire.h> // Include Wire.h library
 #include <WiFi.h>
 #include <math.h>

void findDevices() {
  byte error, address; //variable for error and I2C address
  int nDevices; //number of devices found on I2C bus

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address); //begin transmission with each possible address
    error = Wire.endTransmission(); //returns 0 for success, 1,2,3,4 for other errors


    //if we receive a successful transaction then print out the device's address in hex format
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  Serial.print("Number of devices found: ");
  Serial.println(nDevices);
}

void sendMsg(int address, char message){
  Wire.beginTransmission(address); //open I2C bus
  Wire.write(message); //write message
  int error = Wire.endTransmission();
  if(error != 0){
    Serial.println("Error sending command:");
    Serial.println(error);
  }
  else{
    //Serial.println("Message sent");
  }
}

void driveMotor(int address){
  
}

void setup() {
  //Begin I2C communication
  Wire.begin();
  Serial.begin(9600);
  while(!Serial); // wait for serial initialization
  Serial.println("\nI2C Scanner");

  findDevices();

}

void loop(){

}
