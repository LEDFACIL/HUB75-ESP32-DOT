//#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 1

MatrixPanel_I2S_DMA *dma_display = nullptr;

void setup() {
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,
    PANEL_RES_Y,
    PANEL_CHAIN
  );

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(255);
  dma_display->clearScreen(); // Limpiamos la pantalla
}

void loop() {
  static int x = 0; // Coordenada X del punto
  static int y = 0; // Coordenada Y del punto
  static int directionX = 1; // Dirección del movimiento horizontal (1 derecha, -1 izquierda)
  static int directionY = 1; // Dirección del movimiento vertical (1 abajo, -1 arriba)
  static int colorIndex = 0; // Índice del color actual

  // Secuencia de colores para el punto
  const uint16_t colors[] = {
    dma_display->color565(255, 0, 0),   // Rojo
    dma_display->color565(0, 255, 0),   // Verde
    dma_display->color565(0, 0, 255),   // Azul
  };

  // Apagamos el punto en su posición anterior
  dma_display->drawPixel(x, y, dma_display->color565(0, 0, 0));

  // Movemos el punto en ambas direcciones
  x += directionX;
  y += directionY;

  // Verificamos si llegamos al final de una fila
  if (x >= PANEL_RES_X || x < 0) {
    directionX = -directionX; // Cambiamos la dirección horizontal
    x += directionX * 2; // Ajustamos la posición para evitar quedarse en el borde
    colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0])); // Cambiamos el color
  }

  // Verificamos si llegamos al borde superior o inferior
  if (y >= PANEL_RES_Y || y < 0) {
    directionY = -directionY; // Cambiamos la dirección vertical
    y += directionY * 2; // Ajustamos la posición para evitar quedarse en el borde
    colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0])); // Cambiamos el color
  }

  // Encendemos el punto en su nueva posición y color
  dma_display->drawPixel(x, y, colors[colorIndex]);
  delay(50); // Añade un retraso para controlar la velocidad de movimiento
}
