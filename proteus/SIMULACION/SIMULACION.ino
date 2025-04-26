const int DATA_PIN = 13;
const int CLK_PIN = 12;
const int pos[4][4]={{23, 25, 27, 29}, {31, 33, 35, 37}, {39, 41, 43, 45}, {47, 49, 51, 53}};
const int setUpLed = 6;
const int gameLed = 5;
const int looseLed = 4;

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
  
  pinMode(setUpLed, OUTPUT);
  pinMode(gameLed, OUTPUT);
  pinMode(looseLed, OUTPUT);
  digitalWrite(setUpLed, LOW);
  digitalWrite(gameLed, LOW);
  digitalWrite(looseLed, LOW);
  
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      pinMode(pos[i][j], INPUT);
    }
  }
}

void loop() {
  String mode = "";
  if (Serial.available()) {
    String mode = Serial.readString();
  }

  if (mode == "set up") {
    Serial.println("==== MODO CONFIGURACION ====");
    turnOnSetUpLed();
    byte ram_data[2];
    while(true) {
      if (Serial.available() >= 2) {
        ram_data[0] = Serial.read(); // LSB (bits 0-7)
        ram_data[1] = Serial.read(); // MSB (bits 8-15)
    
        Serial.println("Datos recibidos:");
        Serial.print("Byte 0: ");
        Serial.println(ram_data[0], BIN);
        Serial.print("Byte 1: ");
        Serial.println(ram_data[1], BIN);
        
        writeToRAM(ram_data);
        mode = "";
        break;
      }
    }
  } else if (mode == "game"){
    Serial.println("==== MODO JUEGO ====");
    turnOnGameLed();
    int pos_whitout_bombs = 16 - getNumBombs();
    int verified_pos = 0;
    while (true) {
      if (Serial.available() >= 1) {
        String text = Serial.readString();

        if (text == 'X') {
          mode = "";
          break;
        }

        int num = text.toInt();

        int row = num / 4;
        int column = num % 4;
  
        int estado = digitalRead(pos[row][column]);
  
        if (estado == HIGH) {
          Serial.print("Hay una bomba en la");
           Serial.print(" fila: ");
          Serial.print(row + 1);
          Serial.print(" columna: ");
          Serial.println(column +1 );
          Serial.println("---- ¡Perdiste! ----");
          mode = "";
          turnOnLooseLed();
          break;
        } else {
          Serial.print("No hay bomba en la");
          Serial.print(" fila: ");
          Serial.print(row + 1);
          Serial.print(" columna: ");
          Serial.println(column +1 );
        }
        
        verified_pos++;
      }

      if(verified_pos >= pos_whitout_bombs) {
        Serial.println("---- ¡Ganaste! ----");
        mode = "";
        break;
      }
    }
  }
}

void pulseClock() {
  digitalWrite(CLK_PIN, HIGH);
  delay(50);
  digitalWrite(CLK_PIN, LOW);
  delay(50);
}

void writeToRAM(byte data[]) {
  for (int i = 15; i >= 0; i--) {
    byte currentByte = data[i / 8];
    byte bitValue = (currentByte >> (i % 8)) & 0x01;

    digitalWrite(DATA_PIN, bitValue);
    pulseClock();
  }
}

int getNumBombs() {
  int num_bombs = 0;
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(digitalRead(pos[i][j]) == HIGH)
        num_bombs++;
    }
  }
  return num_bombs;
}

void turnOnSetUpLed() {
  digitalWrite(setUpLed, HIGH);
  digitalWrite(gameLed, LOW);
  digitalWrite(looseLed, LOW);
}

void turnOnGameLed() {
  digitalWrite(setUpLed, LOW);
  digitalWrite(gameLed, HIGH);
  digitalWrite(looseLed, LOW);
}

void turnOnLooseLed() {
  digitalWrite(setUpLed, LOW);
  digitalWrite(gameLed, LOW);
  digitalWrite(looseLed, HIGH);
}
