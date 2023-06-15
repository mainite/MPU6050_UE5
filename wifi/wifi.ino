#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<Wire.h>
const int MPU=0x68;  // I2C地址


int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


const char* ssid = "hotMonk";
const char* password = "88888888";

ESP8266WebServer server(80);

void handleRoot() {
    //String message = " GyX=" + String(GyX) + ", GyY=" + String(GyY) + ", GyZ=" + String(GyZ);

    
    //String message ="{\"GyX\":\" String(GyX)\",\"GyY\":\" String(GyY)\",\"GyZ\":\" String(GyZ)\"}";

    String message ="{\"AcX\":\" "+String(AcX)+"\",\"AcY\":\" "+String(AcY)+"\",\"AcZ\":\" "+String(AcZ)+"\"}";

    String message2 ="{\"GyX\":\" "+String(GyX)+"\",\"GyY\":\" "+String(GyY)+"\",\"GyZ\":\" "+String(GyZ)+"\"}";


  server.send(200, "text/plain", message);
}

void setup() {
   Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1寄存器
  Wire.write(0);     // 唤醒MPU-6050
  Wire.endTransmission(true);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  server.on("/", handleRoot);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();


  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // 从寄存器0x3B（ACCEL_XOUT_H）开始
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);  // 请求6个字节的数据，true表示停止I2C传输

  AcX=Wire.read()<<8|Wire.read();    // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read();    // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();    // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();    // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();    // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();    // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();    // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // Serial.print("AcX = ");
  // Serial.print(AcX);
  // Serial.print(" | AcY = ");
  // Serial.print(AcY);
  // Serial.print(" | AcZ = ");
  // Serial.println(AcZ);

}