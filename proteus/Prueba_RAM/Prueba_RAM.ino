#define DATA_PIN  13   // pin conectado a la entrada D de los flip-flops
#define CLK_PIN   12   // pin conectado a la entrada CLK común

void setup() {
  Serial.begin(9600);
  delay(2000);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
}

void loop() {
  if (Serial.available() >= 2) {
    byte ram_data[2];
    ram_data[0] = Serial.read(); // LSB (bits 0-7)
    ram_data[1] = Serial.read(); // MSB (bits 8-15)

    Serial.println("Datos recibidos:");
    Serial.print("Byte 0: ");
    Serial.println(ram_data[0], BIN);
    Serial.print("Byte 1: ");
    Serial.println(ram_data[1], BIN);
    
    // Enviar bits a flip-flops
    writeToRAM(ram_data);
  }
}

void pulseClock() {
  digitalWrite(CLK_PIN, HIGH);
  delay(100);  // suficiente para que el flip-flop lo registre
  digitalWrite(CLK_PIN, LOW);
  delay(100);
}

void writeToRAM(byte data[]) {
  // Enviar de bit 15 a bit 0 (MSB primero)
  for (int i = 15; i >= 0; i--) {
    byte currentByte = data[i / 8];
    byte bitValue = (currentByte >> (i % 8)) & 0x01;

    digitalWrite(DATA_PIN, bitValue);
    pulseClock();
  }
}
