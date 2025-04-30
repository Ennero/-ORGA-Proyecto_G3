  #include <LiquidCrystal_I2C.h>
  
  const int DATA_PIN = 13;
  const int CLK_PIN = 12;
  const int pos[4][4]={{23, 25, 27, 29}, {31, 33, 35, 37}, {39, 41, 43, 45}, {47, 49, 51, 53}};
  const int configLed = 6;
  const int gameLed = 5;
  const int winLed = 4;
  const int looseLed = 3;
  LiquidCrystal_I2C lcd(0x27,16,2);
  
  void setup() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.display();
    print("<- BUSCAMINAS ->", 0);
    Serial.begin(9600);
    delay(1000);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLK_PIN, OUTPUT);
    digitalWrite(DATA_PIN, LOW);
    digitalWrite(CLK_PIN, LOW);
    
    pinMode(configLed, OUTPUT);
    pinMode(gameLed, OUTPUT);
    pinMode(looseLed, OUTPUT);
    digitalWrite(configLed, LOW);
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
      mode = Serial.readString();
    
      if (mode == "configuration") {
        turnOnConfigLed();
        lcd.clear();
        print("MODO", 0);
        print("CONFIGURACION", 1);
        while(true) {
            if (Serial.peek() == 'X') {
              Serial.read();
              lcd.clear();
              print("Salida del", 0);
              print("modo config.", 1);
              turnOffAll();
              mode = "";
              break;
            } else if (Serial.available() >= 2) {
            byte ram_data[2];        
            ram_data[0] = Serial.read(); // LSB (bits 0-7)
            ram_data[1] = Serial.read(); // MSB (bits 8-15)
            lcd.clear();
            print("Datos", 0);
            print("recibidos", 1);
            delay(200);
            Serial.print("Byte 0: ");
            Serial.println(ram_data[0], BIN);
            Serial.print("Byte 1: ");
            Serial.println(ram_data[1], BIN);
            
            writeToRAM(ram_data);
          }
        }
      } else if (mode == "game"){
        lcd.clear();
        print("MODO", 0);
        print("JUEGO", 1);
        turnOnGameLed();
        int pos_whitout_bombs = 16 - getNumBombs();
        int verified_pos = 0;
        while (true) {
            if (Serial.peek() == 'X') {
              Serial.read();
              lcd.clear();
              print("X - salida juego.", 1);
              turnOffAll();
              mode = "";
              break;
            } else if (Serial.available() >= 1) {
            String text = Serial.readString();
            int num = text.toInt() - 1;

            lcd.clear();

            if (num > 15 || num < 0) {
              lcd.clear();
              print("Numero de", 0);
              print("posicion invalida.", 1);
              continue;
            }
            
            int row = num / 4;
            int column = num % 4;
      
            int estado = digitalRead(pos[row][column]);

            String posX = String(row + 1);
            String posY = String(column + 1);
            
            if (estado == HIGH) {
              lcd.clear();
              print("HAY BOMBA", 0); 
              print("Fila: " + posX + " Col: " + posY, 1);
              delay(2000);
              lcd.clear();
              print("-- PERDISTE --", 0);
              print("Salida juego", 1);
              mode = "";
              turnOnLooseLed();
              break;
            } else {
              lcd.clear();
              print("NO HAY BOMBA", 0);
              print("Fila: " + posX + " Col: " + posY, 1);
              delay(2000);
            }
            
            verified_pos++;
          }
    
          if(verified_pos >= pos_whitout_bombs) {
            lcd.clear();
            print("-- GANASTE --", 0);
            print("Salida juego", 1);
            turnOnWinLed();
            mode = "";
            break;
          }
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
  
  void turnOnConfigLed() {
    digitalWrite(configLed, HIGH);
    digitalWrite(gameLed, LOW);
    digitalWrite(looseLed, LOW);
    digitalWrite(winLed, LOW);
  }
  
  void turnOnGameLed() {
    digitalWrite(configLed, LOW);
    digitalWrite(gameLed, HIGH);
    digitalWrite(looseLed, LOW);
    digitalWrite(winLed, LOW);
  }

  void turnOnWinLed() {
    digitalWrite(configLed, LOW);
    digitalWrite(gameLed, LOW);
    digitalWrite(looseLed, LOW);
    digitalWrite(winLed, HIGH);
  }
  
  void turnOnLooseLed() {
    digitalWrite(configLed, LOW);
    digitalWrite(gameLed, LOW);
    digitalWrite(looseLed, HIGH);
    digitalWrite(winLed, LOW);
  }

  void turnOffAll() {
    digitalWrite(configLed, LOW);
    digitalWrite(gameLed, LOW);
    digitalWrite(looseLed, LOW);
    digitalWrite(winLed, LOW);
  }

  int centerText(String text) {
    int textLength = text.length();

    if (textLength > 16)
      return 0;

    return (16 - textLength) / 2;
  }

  void print(String text, int row) {
    lcd.setCursor(centerText(text),row);
    lcd.print(text);
  }
