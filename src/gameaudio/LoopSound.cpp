#include "LoopSound.h"

#include "IDecoder.h"
#include "../../include/gameaudio/Error.h"
#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>

using namespace gameaudio;

LoopSound::LoopSound(boost::shared_ptr<IFileReader> reader, encoding_type encoding, unsigned looppos)
	: _loopPosition(looppos), _initialBufferProcessed(false), _mutex(0)
{
	assert(reader != 0);

	_mutex = new boost::mutex();

	alGenBuffers(1, &_initialBuffer);
	alGenBuffers(1, &_loopBuffer);

	boost::scoped_ptr<IDecoder> decoder(createDecoder(reader, encoding));

	ALenum format = decoder->getFormat();
	if (format == -1) throw Error("Format which OpenAL does not support");
	if (decoder->getLengthBySamples() <= looppos) {
		throw Error("Too large loop position");
	}

	_frequency = decoder->getFrequency();

	uint64 realsize = looppos * (decoder->getBitNum() / 8) * decoder->getChannelsNum();
	if (realsize > 0xFFFFFFFF) throw Error("Too large sound buffer");
	unsigned size = (unsigned)realsize;

	char* data = new char[size];
	unsigned read_size = decoder->read(data, 0, size);
	alBufferData(_initialBuffer, format, data, read_size, decoder->getFrequency());
	delete[] data;
	
	size = (decoder->getLengthBySamples() - looppos) * (decoder->getBitNum() / 8) * decoder->getChannelsNum();
	data = new char[size];
	read_size = decoder->read(data, looppos, size);
	alBufferData(_loopBuffer, format, data, read_size, decoder->getFrequency());
	delete[] data;

	_initBuffers();
}

LoopSound::~LoopSound() {
	alDeleteBuffers(1, &_initialBuffer);
	alDeleteBuffers(1, &_loopBuffer);

	delete _mutex;
}

void LoopSound::_initBuffers() {
	alSourceQueueBuffers(_alSource, 1, &_initialBuffer);
	alSourceQueueBuffers(_alSource, 1, &_loopBuffer);
	_initialBufferProcessed = false;
	alSourcei(_alSource, AL_LOOPING, 0);
}

void LoopSound::_resetBuffers() {
	ALint queued;
	alGetSourcei(_alSource, AL_BUFFERS_QUEUED, &queued);
	
	ALuint dummy[2];
	alSourceUnqueueBuffers(_alSource, queued, dummy);

	_initBuffers();
}

void LoopSound::update() {
	boost::mutex::scoped_lock lock(*_mutex);

	ALint queued = 0, processed = 0;
	alGetSourcei(_alSource, AL_BUFFERS_QUEUED, &queued);
	alGetSourcei(_alSource, AL_BUFFERS_PROCESSED, &processed);

	if (processed && !_initialBufferProcessed) {
		ALuint dummy;
		alSourceUnqueueBuffers(_alSource, processed, &dummy);
		alSourcei(_alSource, AL_LOOPING, 1);
		_initialBufferProcessed = true;
	}
}

void LoopSound::stop() {
	pause();
	setPlayPositionBySamples(0);
}

void LoopSound::rewind() {
	setPlayPositionBySamples(0);
}

void LoopSound::setPlayPositionBySamples(uint64 v) {
	boost::mutex::scoped_lock lock(*_mutex);
	if (_initialBufferProcessed) {
		if (v >= _loopPosition) {
			alSourcei(_alSource, AL_SAMPLE_OFFSET, (ALint)(v - _loopPosition));
		} else {
			_resetBuffers();
			alSourcei(_alSource, AL_SAMPLE_OFFSET, (ALint)v);
		}
	} else {
		if (v >= _loopPosition) {
			alSourceUnqueueBuffers(_alSource, 1, 0);
			alSourcei(_alSource, AL_SAMPLE_OFFSET, (ALint)(v - _loopPosition));
		} else {
			alSourcei(_alSource, AL_SAMPLE_OFFSET, (ALint)v);
		}
	}
}

void LoopSound::setPlayPositionBySecs(float t) {
	setPlayPositionBySamples((uint64)(t * _frequency));
}

uint64 LoopSound::getPlayPositionBySamples() const {
	boost::mutex::scoped_lock lock(*_mutex);
	ALint pos;
	alGetSourcei(_alSource, AL_SAMPLE_OFFSET, &pos);
	if (_initialBufferProcessed) {
		pos += _loopPosition;
	}
	return pos;
}

float LoopSound::getPlayPositionBySecs() const {
	return getPlayPositionBySamples() / (float)_frequency;
}