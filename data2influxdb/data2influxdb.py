from influxdb import *
import socket
import sys

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = ('192.168.0.100', 3333)
print('starting up on {} port {}'.format(*server_address))
# Bind the socket to the port
sock.bind(server_address)

# create client to connect to influxDB running at 8086
client = InfluxDBClient(host='localhost', port=8086)
# list the databases present
databases = client.get_list_database()
print(databases)
# connect to mpu database
client.switch_database('mpu')

while True:
    print('\nwaiting to receive message')
    # sock.recv from listens for the packets with buffer size of 1024
    data, address = sock.recvfrom(1024)

    print('received {} bytes from {}'.format(len(data), address))
    mpu_data = str(data.decode())
    print(mpu_data)
    # split accelerometer, gyroscope values with ','
    split_data = mpu_data.split(',')
    print(split_data)
    # influx db takes values in key value pairs
    # measurement is a grouping with each field holding its value
    json_body = [
        {
            "measurement": "Acc",
            "fields": {
                    "AcX": split_data[0],
                    "AcY": split_data[1],
                    "AcZ": split_data[2],
            }
        },
        {
            "measurement": "Gyr",
            "fields": {
                "GyX": split_data[3],
                "GyY": split_data[4],
                "GyZ": split_data[5]
            }
        }
    ]
    # client writes the json_body to the influxDB
    client.write_points(json_body)
    #file object file1 is created to write the values to a file out.txt
    #second argument 'a' indicates that values gets appended to the same file
    file1 = open("/home/deepak/Projects/Pycharm/bluetooth/out.txt", "a")
    file1.writelines(mpu_data)
    file1.write("\n")