# ESP32_MPU6050
Send Accelerometer and gyroscope readings from mpu6050 sensor over UDP and store values in InfluxDB 


#### 1) Wiring ESP32 and MPU6050
Connect the VCC and ground pins of the esp32 to that of mpu6050
Connect the SCL pin of mpu6050 to the SCL pin of ESP32
Connect the SDA pin of mpu6050 to the pin21 of ESP32

#### 2) Sending sensor values using UDP
Include the ESP32 board in to the arduino IDE 
Write the program to the ESP32

#### 3) Read UDP packets 
Run the python program on the same port to capture the udp packets

#### 4) Store sensor value in InfluxDB
Install InfluxDB and change configuration file (influxdb.conf) to enable udp 
Create a database 'mpu' to store the sensor values

#### 5) Add InfluxDB Datasource into Grafana 
Install Grafana and add the influxDB 'mpu' 
Write a query to view the values in the grafana dashboard 

![img](https://github.com/deepakravibabu/ESP32_MPU6050/blob/master/mpu6050.gif)
