/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-one-to-many-esp32-esp8266/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

/**
   UPLOAD THIS CODE TO THE SENDER
*/

#include <esp_now.h>
#include <WiFi.h>

/*
    REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
    ADD OR DELETE LINES DEPENDING ON NUMBER OF RECEIVER BOARDS
*/
uint8_t broadcastAddress1[] = {0x24, 0xA1, 0x60, 0x75, 0xB8, 0xE0};

String deviceBData = "";


//type struct with two integer variables
typedef struct data_struct {
  String wifiData;
} data_struct;

data_struct test; //store variable values

// callback when data is sent
// executed when data is sent, prints if message was successfully delivered to know if board received message
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  //Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(9600);

  //initialize device as wifi station
  WiFi.mode(WIFI_STA);

  //initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //register callback function to be called when a message is sent
  esp_now_register_send_cb(OnDataSent);

  // register peer
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}
char deviceBdata;
char input[43];
String interpretData(String data) {
  data.toCharArray(input, 43);
  //Serial.println(data);
  //A button is index 0

  //make D pad into integer 0-7
  String dpad = "";
  int dpadval = 0;
  for (int i = 0; i <= 3; i++) {
    dpadval *= 2;
    if (data[i] == '1') dpadval++;
  }
  //make right joystick int 0-255 (128 is nothing)
//  String rjoy1 = "";
//  int rjoyval = 0;
//  for (int i = 34; i <= 42; i++) {
//    rjoyval *= 2;
//    if (data[i] == '1') rjoyval++;
//  }
  //Serial.print("rjoy: ");
  //Serial.println(rjoyval);
  //Serial.print("dpad: ");
  //Serial.println(dpad);


  //check buttons
  if (data[4] == '1') { //y button
    return "1";
  }
  if (data[5] == '1') { //x button
    return "2";
  }
  if (data[6] == '1') { //B button
    return "3";
  }
  if (data[7] == '1') { //A button
    return "4";
  }
  if (data[8] == '1') { //right bumper
    return "8";
  }
  if (data[9] == '1') { //left bumper
    return "9";
  }
//  if (rjoyval !=128){  // < 130 && rjoyval > 125) { //stick has been moved 
//    if (rjoyval < 10) {
//      return "10";
//    }
//    else {
//      return String(rjoyval);
//    }
//  }
  else {
    return "0";
  }

  //
}
String newData="";



void loop() {
  if (Serial.available() > 0) {
    if (Serial.peek() != '\n') //if we press enter in the serial monitor and sent data
    {
      deviceBData += (char) Serial.read(); //add read string into a data cache
    }
    else { //end of aline
      Serial.read();
      //now need to interpret deviceBData
      Serial.print("You said: ");
      newData = interpretData(deviceBData);
      Serial.println(newData);
      
      //sends the actual data
      test.wifiData = newData; //convert data to an integer
      //send the message - first argument is mac address, if you pass 0 then it sends the same message to all registered peers
      esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(data_struct));
      if (result == ESP_OK) {
        //Serial.println("Sent with success");
      }
      else {
        //Serial.println("Error sending the data");
      }
      deviceBData = "";
    }
  }

  delay(5);
}


/**
   to send data to inidivudal boards
   esp_err_t result1 = esp_now_send(
  broadcastAddress1,
  (uint8_t *) &test,
  sizeof(test_struct));

  if (result1 == ESP_OK) {
  Serial.println("Sent with success");
  }
  else {
  Serial.println("Error sending the data");
  }

*/