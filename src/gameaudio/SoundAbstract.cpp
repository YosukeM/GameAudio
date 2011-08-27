#include "SoundAbstract.h"

using namespace gameaudio;

SoundAbstract::SoundAbstract() {
	alGenSources(1, &_alSource);
}

SoundAbstract::~SoundAbstract() {
	alDeleteSources(1, &_alSource);
}

void SoundAbstract::update() {

}

void SoundAbstract::play() {
	alSourcePlay(_alSource);
}

bool SoundAbstract::isPlaying() const {
	ALint value;
	alGetSourcei(_alSource, AL_SOURCE_STATE, &value);
	return value == AL_PLAYING;
}

void SoundAbstract::pause() {
	alSourcePause(_alSource);
}

void SoundAbstract::stop() {
	alSourceStop(_alSource);
}

void SoundAbstract::rewind() {
	alSourceRewind(_alSource);
}

uint64 SoundAbstract::getPlayPositionBySamples() const {
	ALint value;
	alGetSourcei(_alSource, AL_SAMPLE_OFFSET, &value);
	return value;
}

void SoundAbstract::setPlayPositionBySamples(uint64 v) {
	alSourcei(_alSource, AL_SAMPLE_OFFSET, v);
}

float SoundAbstract::getPlayPositionBySecs() const {
	ALfloat value;
	alGetSourcef(_alSource, AL_SEC_OFFSET, &value);
	return value;
}

void SoundAbstract::setPlayPositionBySecs(float v) {
	alSourcef(_alSource, AL_SEC_OFFSET, v);
}

#define PARAM_F(camel_name, al_name) \
	float SoundAbstract::get##camel_name () const {\
		ALfloat value;\
		alGetSourcef(_alSource, AL_##al_name, &value);\
		return value;\
	}\
	void SoundAbstract::set##camel_name (float v) {\
		alSourcef(_alSource, AL_##al_name, v);\
	}

#define PARAM_3F(camel_name, al_name) \
	Vector3 SoundAbstract::get##camel_name () const {\
		Vector3 v;\
		alGetSource3f(_alSource, AL_##al_name, &(v.x), &(v.y), &(v.z));\
		return v;\
	}\
	void SoundAbstract::set##camel_name (const Vector3& v) {\
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

bool SoundAbstract::getIsRelative() const {
	ALint value;
	alGetSourcei(_alSource, AL_SOURCE_RELATIVE, &value);
	return value != 0;
}

void SoundAbstract::setIsRelative(bool v) {
	alSourcei(_alSource, AL_SOURCE_RELATIVE, v);
}