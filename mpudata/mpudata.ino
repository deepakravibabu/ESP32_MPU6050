/*
 *  This sketch sends MPU6050 data over UDP on a ESP32 device
 *
 */
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

// WiFi network name and password:
const char * networkName = "Naruto";
const char * networkPswd = "barcelona";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "192.168.0.100";
const int udpPort = 3333;
IPAddress ip(192, 168, 0, 117);
IPAddress dns(192, 168, 0, 1);
IPAddress gateway(192, 168, 0, 1);

int cnt = 07;
String gyr_val, acc_val, val;

const int MPU_addr=0x68;  //I2C address of the MPU
float AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); //0x6B is power management register by default device will be in SLEEP mode
  Wire.write(0); //so we set the register to ensure we are NOT in sleep mode
  Wire.endTransmission(true); 

  mpu_setup();
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  //only send data when connected
  if(connected){
    //Initialize UDP transmission
    udp.beginPacket(udpAddress,udpPort);
    
    mpu_read();
  
    acc_val = String(AcX) + "," + String(AcY) + "," + String(AcZ); 
    gyr_val = String(GyX)+ "," + String(GyY) + "," +  String(GyZ); //GyX,GyY,GyZ
    val = acc_val + "," + gyr_val;
    //send the mpu values by UDP
    udp.print(val);
    
    udp.endPacket();
  }
  delay(100);
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

 
  //Initiate connection
  WiFi.begin(ssid, pwd);

  //config WiFi network
  WiFi.config(ip, dns, gateway);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}

void mpu_setup(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C);
  Wire.write(0b00000000); //set the bits(bit4 and  bit3) in the accelerometer setting register 0x1C to (b'00) for sensitivity of (+/-)2g 
  
  Wire.write(0x1B);
  Wire.write(0b00000000); //set the bits(bit4 and  bit3) in the gyroscope setting register 0x1C to (b'00) (+/-) 250 deg/s

  Wire.endTransmission();
}

void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 consequtive registers
 AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
 AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 //Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
 GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
 GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

// divide the Acc and Gyr readings by the sensitivy value for that range

 AcX = AcX / 16384.0; 
 AcY = AcY / 16384.0;
 AcZ = AcZ / 16384.0;

 GyX = GyX / 131.0;
 GyY = GyY / 131.0;
 GyZ = GyZ / 131.0;
 
 Serial.print("Accelerometer Values: \n");
 Serial.print("AcX: "); Serial.print(AcX); Serial.print("\nAcY: "); Serial.print(AcY); Serial.print("\nAcZ: "); Serial.print(AcZ);   
 //Serial.print("\nTemperature: " );  Serial.print(Tmp);
 Serial.print("\nGyroscope Values: \n");
 Serial.print("GyX: "); Serial.print(GyX); Serial.print("\nGyY: "); Serial.print(GyY); Serial.print("\nGyZ: "); Serial.print(GyZ);
 Serial.print("\n");
 delay(30);
 }
