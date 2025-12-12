// mouse.c
#include "mouse.h"
#include "io.h"
#include "cursor.h"

#define MOUSE_DATA 0x60
#define MOUSE_STAT 0x64
#define MOUSE_CMD  0x64

static uint8_t cycle=0;
static int8_t packet[3];

static void mouse_write(uint8_t val){
  // Espera que el controlador esté listo
  while(inb(MOUSE_STAT) & 0x02);
  outb(MOUSE_CMD, 0xD4); // Comando para el mouse
  while(inb(MOUSE_STAT) & 0x02);
  outb(MOUSE_DATA, val);
}

static uint8_t mouse_read_ack(){
  while(!(inb(MOUSE_STAT) & 0x01));
  return inb(MOUSE_DATA);
}

void mouse_init(void){
  // Habilitar dispositivo
  outb(0x64, 0xA8);       // habilitar IRQ12
  mouse_write(0xF4);      // habilitar envío de datos
  (void)mouse_read_ack(); // debería ser 0xFA (ACK)
}

void mouse_irq_handler(void){
  // Leer paquete de 3 bytes
  while(!(inb(MOUSE_STAT) & 0x01)); packet[cycle++] = inb(MOUSE_DATA);
  if(cycle<3) return;
  cycle=0;

  int8_t flags = packet[0];
  int dx = packet[1];
  int dy = packet[2];
  // Convertir con signo extendido si es necesario
  if(flags & 0x10) dx |= -256;
  if(flags & 0x20) dy |= -256;

  cursor_move(dx, -dy); // Y invertido
  // Botones: left = flags&1, right = flags&2, middle = flags&4
}
