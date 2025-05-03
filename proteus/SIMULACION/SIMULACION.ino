#include <LiquidCrystal_I2C.h>

const int DATA_PIN = 13;
const int CLK_PIN = 12;
// Define los pines de entrada para cada celda del buscaminas (4x4)
// Ajusta estos números de pin según tu conexión física
const int pos[4][4]={{23, 25, 27, 29}, {31, 33, 35, 37}, {39, 41, 43, 45}, {47, 49, 51, 53}};
const int configLed = 6;
const int gameLed = 5;
const int winLed = 4;
const int looseLed = 3;
LiquidCrystal_I2C lcd(0x27,16,2); // Dirección I2C 0x27, LCD 16x2

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.display();
  print("<- BUSCAMINAS ->", 0);
  Serial.begin(9600);
  Serial1.begin(9600); // Asume conexión Bluetooth en Serial1
  delay(1000);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);

  pinMode(configLed, OUTPUT);
  pinMode(gameLed, OUTPUT);
  pinMode(winLed, OUTPUT); // Asegúrate de tener este LED conectado
  pinMode(looseLed, OUTPUT);
  digitalWrite(configLed, LOW);
  digitalWrite(gameLed, LOW);
  digitalWrite(winLed, LOW);
  digitalWrite(looseLed, LOW);

  // Configura los pines de entrada para leer el estado de las celdas
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      pinMode(pos[i][j], INPUT_PULLUP); // Usar INPUT_PULLUP es común si lees estados HIGH/LOW directamente
                                        // Si usas otro método, ajusta aquí. Asumo que HIGH = bomba.
    }
  }
}

void loop() {
  if (Serial.available()) {
    handleInput(Serial); // PC por USB
  }

  if (Serial1.available()) {
    handleInput(Serial1); // Bluetooth
  }
}

void handleInput(Stream &port) {
  String mode = "";

  if (port.available()) {
    mode = port.readStringUntil('\n'); // Leer hasta nueva línea para mayor robustez
    mode.trim(); // Quitar espacios en blanco

    if (mode == "configuration") {
      turnOnConfigLed();
      lcd.clear();
      print("MODO", 0);
      print("CONFIGURACION", 1);
      while(true) {
          String configCmd = "";
          if (port.available()) {
              configCmd = port.readStringUntil('\n');
              configCmd.trim();

              if (configCmd == "X") {
                  lcd.clear();
                  print("Salida del", 0);
                  print("modo config.", 1);
                  turnOffAll();
                  delay(1000); // Pequeña pausa para leer el mensaje
                  lcd.clear();
                  print("<- BUSCAMINAS ->", 0); // Volver al mensaje inicial
                  mode = "";
                  break;
              } else if (configCmd.length() >= 2) { // Asumiendo que envías 2 bytes como antes
                  // --- Tu lógica para recibir y escribir en RAM ---
                  // (Asegúrate que la lectura de bytes sea robusta aquí)
                  // Ejemplo simplificado si envías los bytes directamente:
                  if (port.available() >= 2) { // Verificar si realmente llegaron los bytes
                     byte ram_data[2];
                     port.readBytes(ram_data, 2); // Leer 2 bytes
                     lcd.clear();
                     print("Datos", 0);
                     print("recibidos", 1);
                     delay(500); // Aumentar delay para leer
                     port.print("Byte 0: ");
                     port.println(ram_data[0], BIN);
                     port.print("Byte 1: ");
                     port.println(ram_data[1], BIN);
                     writeToRAM(ram_data);
                     lcd.clear();
                     print("Datos escritos", 0);
                     print("en RAM.", 1);
                     delay(1000);
                     lcd.clear();
                     print("MODO", 0);
                     print("CONFIGURACION", 1); // Volver a mostrar el modo
                  }
              }
          }
      }
    } else if (mode == "game"){
      lcd.clear();
      print("MODO", 0);
      print("JUEGO", 1);
      turnOnGameLed();
      delay(1000); // Pausa para ver el modo

      int num_bombs_total = getNumBombs(); // Calcula una sola vez
      int pos_without_bombs = 16 - num_bombs_total;
      int verified_pos_ok = 0; // Contador de posiciones seguras verificadas
      int score = 0; // <<<--- Inicializa el puntaje a 0 para esta partida

      lcd.clear();
      print("Ingresa pos (1-16)", 0);
      print("o X para salir", 1);


      while (true) {
          String gameCmd = "";
          if (port.available()) {
              gameCmd = port.readStringUntil('\n');
              gameCmd.trim();

              if (gameCmd == "X") {
                  port.read(); // Consumir el caracter 'X'
                  lcd.clear();
                  print("Salida del", 0); // Mensaje corregido
                  print("modo juego.", 1);
                  turnOffAll();
                  delay(1500);
                  lcd.clear();
                  print("<- BUSCAMINAS ->", 0); // Volver al mensaje inicial
                  mode = "";
                  break; // Salir del bucle while(true) del juego
              } else {
                  int num = gameCmd.toInt(); // Convierte el string a entero

                  lcd.clear(); // Limpiar para mostrar resultado

                  if (num < 1 || num > 16) { // Validar rango 1-16
                      print("Posicion", 0);
                      print("invalida (1-16)", 1);
                      delay(1500);
                      lcd.clear();
                      print("Ingresa pos (1-16)", 0); // Volver a pedir
                      print("o X para salir", 1);
                      continue; // Saltar al siguiente ciclo del while
                  }

                  // Convertir número (1-16) a índice de matriz (0-15)
                  num = num - 1;
                  int row = num / 4;
                  int column = num % 4;

                  // Leer el estado del pin correspondiente a la celda
                  int estado = digitalRead(pos[row][column]);

                  String posX = String(row + 1);
                  String posY = String(column + 1);

                  if (estado == HIGH) { // Asumiendo HIGH = Bomba
                      lcd.clear();
                      print("! BOMBA !", 0);
                      print("F:" + posX + " C:" + posY, 1); // Más corto
                      turnOnLooseLed(); // Encender LED de perder inmediatamente
                      delay(2000);
                      lcd.clear();
                      print("-- PERDISTE --", 0);
                      // El puntaje se reiniciará automáticamente la próxima vez que se entre a "game"
                      delay(2000);
                      lcd.clear();
                      print("<- BUSCAMINAS ->", 0); // Volver al mensaje inicial
                      turnOffAll(); // Apagar LEDs antes de salir
                      mode = "";
                      break; // Salir del bucle while(true) del juego
                  } else { // NO hay bomba
                      score++; // <<<--- Incrementar el puntaje
                      verified_pos_ok++; // Incrementar contador de posiciones seguras

                      lcd.clear();
                      print("Puntaje: " + String(score), 0); // <<<--- Mostrar puntaje
                      print("F:" + posX + " C:" + posY + " OK", 1); // Indicar posición segura
                      delay(2000); // Tiempo para ver el puntaje

                      // Verificar si se ganó después de encontrar una casilla segura
                      if(verified_pos_ok >= pos_without_bombs) {
                          lcd.clear();
                          print("-- GANASTE --", 0);
                          print("Puntaje: " + String(score), 1); // Mostrar puntaje final
                          turnOnWinLed(); // Encender LED de ganar
                          // El puntaje se reiniciará automáticamente la próxima vez
                          delay(3000);
                          lcd.clear();
                          print("<- BUSCAMINAS ->", 0); // Volver al mensaje inicial
                          turnOffAll(); // Apagar LEDs antes de salir
                          mode = "";
                          break; // Salir del bucle while(true) del juego
                      } else {
                          // Si no ha ganado, volver a pedir posición
                          lcd.clear();
                          print("Ingresa pos (1-16)", 0);
                          print("Puntaje: " + String(score), 1); // Mostrar puntaje actual mientras se espera
                      }
                  }
              }
          }
          // Pequeña pausa para no saturar el bucle si no hay datos
          delay(50);
      } // Fin while(true) del modo juego
    } else {
        // Si el comando no es "configuration" ni "game", podría ser un error
        // o simplemente esperar al siguiente comando.
        // Podrías añadir un mensaje de error si lo deseas.
        // lcd.clear();
        // print("Comando", 0);
        // print("desconocido", 1);
        // delay(1000);
        // lcd.clear();
        // print("<- BUSCAMINAS ->", 0);
    }
  } // Fin if (port.available())
}

void pulseClock() {
  digitalWrite(CLK_PIN, HIGH);
  delayMicroseconds(100); // Usar microsegundos para pulsos rápidos si es necesario
  digitalWrite(CLK_PIN, LOW);
  delayMicroseconds(100);
}

void writeToRAM(byte data[]) {
  // Asegúrate que la lógica de escritura sea correcta para tu RAM/Shift Register
  for (int i = 15; i >= 0; i--) {
    // Determina qué byte (data[0] o data[1]) y qué bit dentro de ese byte usar
    byte currentByte = data[i / 8]; // byte 1 para i=15..8, byte 0 para i=7..0
    byte bitIndexInByte = i % 8;    // bit 7..0 dentro del byte correspondiente
    byte bitValue = (currentByte >> bitIndexInByte) & 0x01;

    digitalWrite(DATA_PIN, bitValue);
    pulseClock(); // Envía el bit
  }
  // Podrías necesitar un pulso extra para "latch" (guardar) los datos
  // dependiendo del chip que uses (ej. un 74HC595 necesitaría un pulso en ST_CP)
}

int getNumBombs() {
  int num_bombs = 0;
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(digitalRead(pos[i][j]) == HIGH) { // Asume HIGH = Bomba
         num_bombs++;
      }
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
  digitalWrite(gameLed, LOW); // Apagar LED de juego al ganar
  digitalWrite(looseLed, LOW);
  digitalWrite(winLed, HIGH);
}

void turnOnLooseLed() {
  digitalWrite(configLed, LOW);
  digitalWrite(gameLed, LOW); // Apagar LED de juego al perder
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
  if (textLength >= 16) { // Si es igual o mayor, empezar en 0
      return 0;
  }
  return (16 - textLength) / 2;
}

void print(String text, int row) {
  lcd.setCursor(centerText(text), row);
  lcd.print(text);
}
