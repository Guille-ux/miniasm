#ifndef MINIASM_STREAM_H
#define MINIASM_STREAM_H

/*
 *
 * Archivo encargado de lo Buffers Dinámicos para almacenar el binario
 *
 */

#include <stdint.h>
#include <stddef.h>

#define STREAM_BUFFER_INITIAL_CAP 4096
#define STREAM_BUFFER_GROW_FACTOR 2

typedef struct {
	uint8_t *data;
	size_t size;
	size_t cap;
} ByteStream;

int initStream(ByteStream *stream); // se encargara de asignar memoria para los datos 
int freeStream(ByteStream *stream); // para liberar el stream
int resizeStream(ByteStream *stream, int needed); // para la gestión del tamaño
int streamAppendByte(ByteStream *stream)

#endif
