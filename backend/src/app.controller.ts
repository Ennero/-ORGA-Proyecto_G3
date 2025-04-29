import { Body, Controller, Get, Post } from '@nestjs/common';
import { AppService } from './app.service';

@Controller()
export class AppController {
  constructor(private readonly appService: AppService) {}

  @Get()
  getHello(): string {
    return this.appService.getHello();
  }

  @Post('procesar')
  procesarTexto(@Body('content') bombas: string) {
    return this.appService.procesarTexto(bombas);
  }

  @Post('ajustar-modo')
  ajustarModo(@Body('mode') mode: string) {
    this.appService.ajustarModo(mode);
  }

  @Post('/enviar-numero')
  enviarNumero(@Body('numero') numero: any) {
    this.appService.enviarNumero(numero);
  }
}
