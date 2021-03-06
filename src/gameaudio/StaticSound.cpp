#include "StaticSound.h"

#include <cassert>
#include <boost/scoped_ptr.hpp>

#include "../../include/gameaudio/IFileReader.h"
#include "../../include/gameaudio/Error.h"

#include "WavDecoder.h"
#include "OggVorbisDecoder.h"

using namespace gameaudio;

StaticSound::StaticSound(boost::shared_ptr<IFileReader> reader, encoding_type encoding) {
	assert(reader != 0);

	alGenBuffers(1, &_alBuffer);

	boost::scoped_ptr<IDecoder> decoder(createDecoder(reader, encoding));

	ALenum format = decoder->getFormat();
	if (format == -1) throw Error("Format which OpenAL does not support");

	uint64 size = decoder->getSizeByBytes();
	if (size > 0xFFFFFFFF) throw Error("Too large file for non-streaming sound");
	char* data = new char[size];
	unsigned read_size = decoder->read(data, 0, size);
	alBufferData(_alBuffer, format, data, read_size, decoder->getFrequency());
	delete[] data;
	
	alSourcei(_alSource, AL_BUFFER, _alBuffer);
	
	_frequency = decoder->getFrequency();
	_size = read_size / (decoder->getBitNum() / 8 * decoder->getChannelsNum());
}

uint64 StaticSound::getLengthBySamples() const {
	return _size;
}

float StaticSound::getLengthBySecs() const {
	return (float)((double)_size / (double)_frequency);
}

void StaticSound::update() {
	if (getPlayPositionBySamples() >= _size) {
		setPlayPositionBySamples(0);
	}
}

float StaticSound::getPlayPositionBySecs() const {
	return getPlayPositionBySamples() / (float)_frequency;
}

void StaticSound::setPlayPositionBySecs(float v) {
	setPlayPositionBySamples(v * _frequency);
}

unsigned StaticSound::getFrequency() const {
	return _frequency;
}