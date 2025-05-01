import os
import serial
import time

serialPort = serial.Serial("COM2", 9600)
time.sleep(2)


def serialTest():

    while True:
        inputText = input("Seleccionar modo: ")

        if inputText == "configuration":
            time.sleep(1)
            serialPort.write(inputText.encode())

            while True:
                inputText = input("Desea enviar las bombas a la RAM? ")

                if inputText == "S":
                    print("Enviando bombas a la RAM...")
                    with open("./config.txt", "r") as file:
                        content = file.read()

                    ram = config_to_bytes(content)

                    bin_str = format(ram[1], "08b") + format(ram[0], "08b")
                    print("Bits enviados: ", bin_str)

                    time.sleep(2)
                    serialPort.write(ram)
                elif inputText == "N":
                    serialPort.write("X".encode())
                    break
        elif inputText == "game":
            time.sleep(1)
            serialPort.write(inputText.encode())

            while True:
                inputText = input("Bomba que desea verificar: ")

                time.sleep(1)
                serialPort.write(inputText.encode())

                if inputText == "X":
                    break
        else:
            print("Opcion no valida.")


def config_to_bytes(text):
    ram = 0

    lines = text.splitlines()
    for line in lines:
        line = line.strip()
        if line.startswith("ADD"):
            try:
                parts = line.split("//")[0]
                parts = parts.replace("ADD", "").replace("x:", "").replace("y:", "")
                values = parts.split(",")

                x = int(values[0].strip())
                y = int(values[1].strip())

                if 1 <= x <= 4 and 1 <= y <= 4:
                    pos = (y - 1) * 4 + (x - 1)
                    ram |= 1 << pos
            except:
                continue

    byte0 = ram & 0xFF
    byte1 = (ram >> 8) & 0xFF

    return bytes([byte0, byte1])


if __name__ == "__main__":
    serialTest()
