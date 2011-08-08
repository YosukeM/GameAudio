#include "Sound.h"
#include <cassert>
#include "../../include/gameaudio/IFileReader.h"
#include "../../include/gameaudio/Error.h"

#include "WavDecoder.h"
#include "OggVorbisDecoder.h"

#include <boost/scoped_ptr.hpp>

using namespace gameaudio;

Sound::Sound(boost::shared_ptr<IFileReader> reader, bool loop, bool stream, unsigned looppos, encoding_type encoding)
	: _isStream(stream), _isLooped(loop), _loopPosition(looppos)
{
	assert(reader != 0);

	alGenBuffers(1, &_alBuffer);
	alGenSources(1, &_alSource);

	if (!stream) {
		boost::scoped_ptr<IDecoder> decoder;

		switch (encoding) {
		case ENCODING_OGG_VORBIS:
			decoder.reset(new OggVorbisDecoder());
			break;
		case ENCODING_WAV:
			decoder.reset(new WavDecoder());
			break;
		default:
			throw Error("This file format is not supported.");
		}
		if (!decoder->init(reader)) {
			throw Error("Unkown wav format");
		}

		ALenum format = -1;
		if (decoder->getChannelsNum() == 1) {
			if (decoder->getBitNum() == 8) {
				format = AL_FORMAT_MONO8;
			} else if (decoder->getBitNum() == 16) {
				format = AL_FORMAT_MONO16;
			}
		} else if (decoder->getChannelsNum() == 2) {
			if (decoder->getBitNum() == 8) {
				format = AL_FORMAT_STEREO8;
			} else if (decoder->getBitNum() == 16) {
				format = AL_FORMAT_STEREO16;
			}
		}
		if (format == -1) throw Error("Format which OpenAL does not support");

		unsigned size = decoder->getSizeByBytes();
		char* data = new char[size];
		decoder->read(data, 0, size);
		alBufferData(_alBuffer, format, data, size, decoder->getFrequency());
		delete[] data;
		
		alSourcei(_alSource, AL_BUFFER, _alBuffer);
	}
}

Sound::~Sound() {
	alDeleteSources(1, &_alSource);
	alDeleteBuffers(1, &_alBuffer);
}

void Sound::update() {

}

void Sound::play() {
	alSourcePlay(_alSource);
}

bool Sound::isPlaying() const {
	ALint value;
	alGetSourcei(_alSource, AL_SOURCE_STATE, &value);
	return value == AL_PLAYING;
}

void Sound::pause() {
	alSourcePause(_alSource);
}

void Sound::stop() {
	alSourceStop(_alSource);
}

void Sound::rewind() {
	alSourceRewind(_alSource);
}

unsigned Sound::getPlayPositionBySamples() const {
	ALint value;
	alGetSourcei(_alSource, AL_SAMPLE_OFFSET, &value);
	return value;
}

void Sound::setPlayPositionBySamples(unsigned v) {
	alSourcei(_alSource, AL_SAMPLE_OFFSET, v);
}

float Sound::getPlayPositionBySecs() const {
	ALfloat value;
	alGetSourcef(_alSource, AL_SEC_OFFSET, &value);
	return value;
}

void Sound::setPlayPositionBySecs(float v) {
	alSourcef(_alSource, AL_SEC_OFFSET, v);
}

#define PARAM_F(camel_name, al_name) \
	float Sound::get##camel_name () const {\
		ALfloat value;\
		alGetSourcef(_alSource, AL_##al_name, &value);\
		return value;\
	}\
	void Sound::set##camel_name (float v) {\
		alSourcef(_alSource, AL_##al_name, v);\
	}

#define PARAM_3F(camel_name, al_name) \
	Vector3 Sound::get##camel_name () const {\
		Vector3 v;\
		alGetSource3f(_alSource, AL_##al_name, &(v.x), &(v.y), &(v.z));\
		return v;\
	}\
	void Sound::set##camel_name (const Vector3& v) {\
		alSource3f(_alSource, AL_##al_name, v.x, v.y, v.z);\
	}

PARAM_F(Pitch, PITCH)
PARAM_F(Gain, GAIN)
PARAM_F(RolloffFactor, ROLLOFF_FACTOR)
PARAM_F(MinGain, MIN_GAIN)
PARAM_F(MaxGain, MAX_GAIN)
PARAM_F(ConeOuterGain, CONE_OUTER_GAIN)
PARAM_F(ConeInnerAngle, CONE_INNER_ANGLE)
PARAM_F(ConeOuterAngle, CONE_OUTER_ANGLE)

PARAM_3F(Position, POSITION)
PARAM_3F(Velocity, VELOCITY)
PARAM_3F(Direction, DIRECTION)