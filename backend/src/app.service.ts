import { Injectable } from '@nestjs/common';
import { SerialPort } from 'serialport';

@Injectable()
export class AppService {
  private puertoSerial: SerialPort;

  constructor() {
    this.puertoSerial = new SerialPort({
      path: 'COM2', // Asegúrate de que sea el correcto
      baudRate: 9600,
    });   

    this.puertoSerial.on('open', () => {
      console.log('Puerto serial abierto');
    });

    this.puertoSerial.on('error', (err) => {
      console.error('Error en el puerto serial:', err);
    });
  }

  getHello(): string {
    return 'Hello World!';
  }

  ajustarModo(texto: string): void {
    try {
      // Convertir el texto a un Buffer
      const bytes = Buffer.from(texto, 'utf-8');
  
      // Enviar los bytes al puerto serial
      this.puertoSerial.write(bytes, (err) => {
        if (err) {
          console.error("Error al escribir en el puerto serial:", err.message);
        } else {
          console.log(`Modo "${texto}" enviado correctamente al puerto serial.`);
        }
      });
    } catch (error) {
      console.error("Error al ajustar el modo:", error.message);
    }
  }

  enviarNumero(numero: string): void {
    try {
      // Convertir el string a un Buffer
      const buffer = Buffer.from(numero, 'utf-8');
  
      // Enviar el string al puerto serial
      this.puertoSerial.write(buffer, (err) => {
        if (err) {
          console.error("Error al escribir en el puerto serial:", err.message);
        } else {
          console.log(`String "${numero}" enviado correctamente al puerto serial.`);
        }
      });
    } catch (error) {
      console.error("Error al enviar el string:", error.message);
    }
  }

  procesarTexto(texto: string): { resultado: string } {
    const bytes = configToBytes(texto);
    const bin_str = bytes[1].toString(2).padStart(8, '0') + bytes[0].toString(2).padStart(8, '0');
    
    this.puertoSerial.write(bytes, (err) => {
      if (err) {
        console.error("Error al escribir en el puerto serial:", err.message);
      } else {
        console.log("Datos enviados correctamente al Arduino.");
      }
    });

    return { resultado: `Bits enviados: ${bin_str}` };
  }
  
}


function configToBytes(texto: string): Buffer {
  let ram = 0;

  const lineas = texto.split('\n');
  for (let linea of lineas) {
    linea = linea.trim();
    if (linea.startsWith("ADD")) {
      try {
        let clean = linea.split("//")[0]
          .replace("ADD", "")
          .replace("x:", "")
          .replace("y:", "")
          .trim();

        const [xStr, yStr] = clean.split(",");
        const x = parseInt(xStr.trim());
        const y = parseInt(yStr.trim());

        if (x >= 1 && x <= 4 && y >= 1 && y <= 4) {
          const pos = (y - 1) * 4 + (x - 1);
          ram |= (1 << pos);
        }
      } catch (e) {
        continue;
      }
    }
  }

  const byte0 = ram & 0xFF;
  const byte1 = (ram >> 8) & 0xFF;

  return Buffer.from([byte0, byte1]);
}

