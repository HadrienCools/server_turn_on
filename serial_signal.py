import serial
from serial.tools import list_ports

# pip install pyserial

serial_port = serial.tools.list_ports.comports()
for port in serial_port:
    print(f"{port}")
# COM1 - Port de communication (COM1)
# COM3 - USB-SERIAL CH340 (COM3)

# 115200
ser = serial.Serial("COM3", baudrate=115200)

#Envoi d’un caractère 
ser.write(b'a')

# Envoi d’une chaine de caractères
# TURN ON YELLOW LED
ser.write(b'TURN ON YELLOW LED')
ser.write(b'TURN OFF YELLOW LED')

# Envoi d’octets
dataL = [1, 2, 3, 4, 5]
dataBytes = bytes(dataL)
ser.write(dataBytes)

print(ser)
ser.close()