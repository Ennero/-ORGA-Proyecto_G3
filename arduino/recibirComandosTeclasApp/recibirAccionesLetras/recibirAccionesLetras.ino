// Declaración de pines para salida de informacion
int salida1x1 = 22;
int salida1x2 = 24;
int salida1x3 = 26;
int salida1x4 = 28;
int salida2x1 = 30;
int salida2x2 = 32;
int salida2x3 = 34;
int salida2x4 = 36;
int salida3x1 = 38;
int salida3x2 = 40;
int salida3x3 = 42;
int salida3x4 = 44;
int salida4x1 = 46;
int salida4x2 = 48;
int salida4x3 = 50;
int salida4x4 = 52;

String entradaHC05 = "";


void setup() {
  // Codigo inicial
  Serial.begin(9600);
  
  // Inicializar pines
  pinMode(salida1x1, OUTPUT);
  pinMode(salida1x2, OUTPUT);
  pinMode(salida1x3, OUTPUT);
  pinMode(salida1x4, OUTPUT);
  pinMode(salida2x1, OUTPUT);
  pinMode(salida2x2, OUTPUT);
  pinMode(salida2x3, OUTPUT);
  pinMode(salida2x4, OUTPUT);
  pinMode(salida3x1, OUTPUT);
  pinMode(salida3x2, OUTPUT);
  pinMode(salida3x3, OUTPUT);
  pinMode(salida3x4, OUTPUT);
  pinMode(salida4x1, OUTPUT);
  pinMode(salida4x2, OUTPUT);
  pinMode(salida4x3, OUTPUT);
  pinMode(salida4x4, OUTPUT);
}

void loop() {
  // Código a ejecutar
  while(Serial.available()){
    entradaHC05 = Serial.readString();
  }

  delay(400);

  if(entradaHC05 == "1"){
    // codigo a ejecutar
    digitalWrite(salida1x1, HIGH);
  }else if(entradaHC05 == "2"){
    // codigo a ejecutar
    digitalWrite(salida1x2, HIGH);
  }else if(entradaHC05 == "3"){
    // codigo a ejecutar
    digitalWrite(salida1x3, HIGH);
  }else if(entradaHC05 == "4"){
    // codigo a ejecutar
    digitalWrite(salida1x4, HIGH);
  }else if(entradaHC05 == "5"){
    // codigo a ejecutar
    digitalWrite(salida2x1, HIGH);
  }else if(entradaHC05 == "6"){
    // codigo a ejecutar
    digitalWrite(salida2x2, HIGH);
  }else if(entradaHC05 == "7"){
    // codigo a ejecutar
    digitalWrite(salida2x3, HIGH);
  }else if(entradaHC05 == "8"){
    // codigo a ejecutar
    digitalWrite(salida2x4, HIGH);
  }else if(entradaHC05 == "9"){
    // codigo a ejecutar
    digitalWrite(salida3x1, HIGH);
  }else if(entradaHC05 == "10"){
    // codigo a ejecutar
    digitalWrite(salida3x2, HIGH);
  }else if(entradaHC05 == "11"){
    // codigo a ejecutar
    digitalWrite(salida3x3 HIGH);
  }else if(entradaHC05 == "12"){
    // codigo a ejecutar
    digitalWrite(salida3x4, HIGH);
  }else if(entradaHC05 == "13"){
    // codigo a ejecutar
    digitalWrite(salida4x1, HIGH);
  }else if(entradaHC05 == "14"){
    // codigo a ejecutar
    digitalWrite(salida4x2, HIGH);
  }else if(entradaHC05 == "15"){
    // codigo a ejecutar
    digitalWrite(salida4x3, HIGH);
  }else if(entradaHC05 == "16"){
    // codigo a ejecutar
    digitalWrite(salida4x4, HIGH);
  }

  if(entradaHC05 != ""){
    entradaHC05 = "";
  }
}
