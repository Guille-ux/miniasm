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

int resizeStream(ByteStream *stream, size_t needed) {
	if (stream==NULL || stream->data==NULL) return -1;
	
	while (1) {
		if (stream->size+needed < stream->cap) break;
		size_t newSize = stream->cap*STREAM_BUFFER_GROW_FACTOR;

		uint8_t *tmp_buffer = (uint8_t*)malloc(newSize);
		memset(tmp_buffer, 0, newSize)
		memcpy(tmp_buffer, stream->data, stream->size);
		free(stream->data);
		stream->data=tmp_buffer;
		stream->cap = newSize;
	}
	
	return 0;
}

int streamAppendByte(ByteStream *stream, uint8_t byte) {
	if (resizeStream(stream, 1) != 0) return -1;
	stream->data[size++]=byte;
	return 0;
}

int streamAppendBytes(ByteStream *stream, uint8_t *bytes, size_t n) {
	if (resizeStream(stream, n)!=0 || bytes==NULL || n==0) return -1;
	memcpy(&stream->data[size], bytes, n);
	stream->size+=n;
	return 0;
}


