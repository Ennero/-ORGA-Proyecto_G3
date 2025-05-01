"use strict";
var __decorate = (this && this.__decorate) || function (decorators, target, key, desc) {
    var c = arguments.length, r = c < 3 ? target : desc === null ? desc = Object.getOwnPropertyDescriptor(target, key) : desc, d;
    if (typeof Reflect === "object" && typeof Reflect.decorate === "function") r = Reflect.decorate(decorators, target, key, desc);
    else for (var i = decorators.length - 1; i >= 0; i--) if (d = decorators[i]) r = (c < 3 ? d(r) : c > 3 ? d(target, key, r) : d(target, key)) || r;
    return c > 3 && r && Object.defineProperty(target, key, r), r;
};
var __metadata = (this && this.__metadata) || function (k, v) {
    if (typeof Reflect === "object" && typeof Reflect.metadata === "function") return Reflect.metadata(k, v);
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.AppService = void 0;
const common_1 = require("@nestjs/common");
const serialport_1 = require("serialport");
let AppService = class AppService {
    puertoSerial;
    constructor() {
        this.puertoSerial = new serialport_1.SerialPort({
            path: 'COM2',
            baudRate: 9600,
        });
        this.puertoSerial.on('open', () => {
            console.log('Puerto serial abierto');
        });
        this.puertoSerial.on('error', (err) => {
            console.error('Error en el puerto serial:', err);
        });
    }
    getHello() {
        return 'Hello World!';
    }
    ajustarModo(texto) {
        try {
            const bytes = Buffer.from(texto, 'utf-8');
            this.puertoSerial.write(bytes, (err) => {
                if (err) {
                    console.error("Error al escribir en el puerto serial:", err.message);
                }
                else {
                    console.log(`Modo "${texto}" enviado correctamente al puerto serial.`);
                }
            });
        }
        catch (error) {
            console.error("Error al ajustar el modo:", error.message);
        }
    }
    enviarNumero(numero) {
        try {
            const buffer = Buffer.from(numero, 'utf-8');
            this.puertoSerial.write(buffer, (err) => {
                if (err) {
                    console.error("Error al escribir en el puerto serial:", err.message);
                }
                else {
                    console.log(`String "${numero}" enviado correctamente al puerto serial.`);
                }
            });
        }
        catch (error) {
            console.error("Error al enviar el string:", error.message);
        }
    }
    procesarTexto(texto) {
        const bytes = configToBytes(texto);
        const bin_str = bytes[1].toString(2).padStart(8, '0') + bytes[0].toString(2).padStart(8, '0');
        this.puertoSerial.write(bytes, (err) => {
            if (err) {
                console.error("Error al escribir en el puerto serial:", err.message);
            }
            else {
                console.log("Datos enviados correctamente al Arduino.");
            }
        });
        return { resultado: `Bits enviados: ${bin_str}` };
    }
};
exports.AppService = AppService;
exports.AppService = AppService = __decorate([
    (0, common_1.Injectable)(),
    __metadata("design:paramtypes", [])
], AppService);
function configToBytes(texto) {
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
            }
            catch (e) {
                continue;
            }
        }
    }
    const byte0 = ram & 0xFF;
    const byte1 = (ram >> 8) & 0xFF;
    return Buffer.from([byte0, byte1]);
}
//# sourceMappingURL=app.service.js.map