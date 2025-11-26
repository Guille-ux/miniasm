#include "../include/stream.h"
#include "../include/compat.h"

int initStream(ByteStream *stream) {
	if (stream==NULL) return -1;
	stream->size=0;
	stream->cap=STREAM_BUFFER_INITIAL_CAP;
	stream->data=(uint8_t*)malloc(STREAM_BUFFER_INITIAL_CAP);
	return 0;
}

int freeStream(ByteStream *stream) {
	if (stream==NULL || stream->data==NULL) return -1;
	free(stream->data);
	stream->cap = 0;
	stream->size = 0;

	return 0;
}

int resizeStream(ByteStream *stream) {
	if (stream==NULL || stream->data==NULL) return -1;
	if (stream->size < stream->cap) return 0;

	size_t newSize = stream->cap*STREAM_BUFFER_GROW_FACTOR;

	uint8_t *tmp_buffer = (uint8_t*)malloc(newSize);
	memset(tmp_buffer, 0, newSize)
	memcpy(tmp_buffer, stream->data, stream->size);
	free(stream->data);
	stream->data=tmp_buffer;
	stream->cap = newSize;

	return 0;
}
