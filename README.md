# Juego con Arduino, memoria simulada y aplicación web

Integración académica de Arduino y Proteus con un backend NestJS que transforma instrucciones y envía datos por puerto serial. Un frontend Flask presenta las pantallas del juego y su configuración.

## Requisitos

Node.js 22 o compatible con NestJS 11, Python 3, un puerto serial conectado al dispositivo/simulador y Proteus para abrir los circuitos. SERIAL_PORT selecciona el puerto; el valor predeterminado es COM5.

## Ejecución

Los comandos parten de la raíz del repositorio, salvo donde se indica otra terminal.

```bash
# Desde la raíz: dependencia serial compartida por el backend
npm ci
cd backend
npm ci
npm run start:dev

# Otra terminal, desde la raíz:
python -m pip install -r frontend/requirements.txt
python frontend/app.py
```

## Recorrido de demostración

Abrir http://127.0.0.1:5000; el backend escucha en http://localhost:3000. Configurar el puerto serial antes de enviar instrucciones. Consultar el manual para emparejar los sketches y el circuito de Proteus.

## Organización

Carpetas de la entrega: [`arduino/`](arduino/), [`backend/`](backend/), [`frontend/`](frontend/), [`informe/`](informe/), [`manuales/`](manuales/), [`proteus/`](proteus/).

## Documentación

- [manualTecnico](manuales/manualTecnico.mkd)
- [manualUsuario](manuales/manualUsuario.mkd)
- [Documentacion](informe/Documentacion.md)

## Alcance y límites

El flujo de hardware requiere Arduino/Proteus y cableado o puerto virtual. El servidor web por sí solo no sustituye esa simulación; las rutas Flask de procesamiento no representan todas las operaciones del backend.

## Contexto y licencia

Trabajo académico o de aprendizaje conservado en el portafolio de [Enner Mendizabal](https://github.com/Ennero). Los créditos de los trabajos en grupo se conservan en sus entregas y manuales.

Consultar [LICENSE](LICENSE) para la licencia de la raíz. Las dependencias y los componentes con avisos o licencias propios conservan sus condiciones.

### Licencias declaradas por componente

La licencia de la raíz no sustituye las declaraciones de estos manifiestos:

- `backend/package.json`: `UNLICENSED`.
