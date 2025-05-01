export declare class AppService {
    private puertoSerial;
    constructor();
    getHello(): string;
    ajustarModo(texto: string): void;
    enviarNumero(numero: string): void;
    procesarTexto(texto: string): {
        resultado: string;
    };
}
