import os
import serial
import time

serialPort = serial.Serial('COM2', 9600)
time.sleep(2)

def serialTest():
    while True:
        os.system('cls')

        inputText = input("Transmite algo al arduino: ")

        if inputText == "x":
            print("Saliendo del programa...")
            break

        serialPort.write(inputText.encode())
        time.sleep(1)

if __name__ == "__main__":
    serialTest()