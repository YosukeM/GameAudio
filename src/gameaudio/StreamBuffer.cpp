#include "StreamBuffer.h"

using namespace gameaudio;

Sound *_sound;
IDecoder *_decoder;
boost::circular_buffer<ALuint> _buffers;

StreamBuffer::StreamBuffer(Sound *sound, IDecoder *decoder)
: _sound(sound), _decoder(decoder)
{
}

StreamBuffer::~StreamBuffer() {
}

void StreamBuffer::update() {
	
}