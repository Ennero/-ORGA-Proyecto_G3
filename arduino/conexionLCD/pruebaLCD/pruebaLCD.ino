#include <LiquidCrystal.h> // incluimos la libreria de la pantalla lcd
LiquidCrystal lcd (8,9,     4,5,6,7); //reconocemos los pines de uso
void setup() {
  lcd.begin(16,2); // Reconocemos y empezamos el LCD
}
void loop() {
lcd.setCursor (0,0); // (Columna fila) 
lcd.print("ELECTROALL"); // imprimimos en la pantalla la palabra electroall en la primera fila
lcd.setCursor (0,1);     // (columna fila) 
lcd.print ("ELECTROALL"); // imprimimos en la pantalla la palabra electroallm en la seguna fila
}
