import { AppService } from './app.service';
export declare class AppController {
    private readonly appService;
    constructor(appService: AppService);
    getHello(): string;
    procesarTexto(bombas: string): {
        resultado: string;
    };
    ajustarModo(mode: string): void;
    enviarNumero(numero: any): void;
}
