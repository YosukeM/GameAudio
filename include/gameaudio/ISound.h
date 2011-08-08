#ifndef GAMEAUDIO_ISOUND_H_
#define GAMEAUDIO_ISOUND_H_

#include "Vector3.h"

namespace gameaudio {
	class ISound {
	protected:
		ISound() {};
		ISound(const ISound&) {};
	public:
		typedef enum {
			ENCODING_WAV,
			ENCODING_OGG_VORBIS,
			ENCODING_AUTO,
		} encoding_type;

		virtual ~ISound() {};
		
		virtual void play() = 0;
		virtual bool isPlaying() const = 0;
		virtual void pause() = 0;
		virtual void stop() = 0;

		virtual void rewind() = 0;

		virtual void setPlayPositionBySamples(unsigned) = 0;
		virtual void setPlayPositionBySecs(float) = 0;

		virtual unsigned getPlayPositionBySamples() const = 0;
		virtual float getPlayPositionBySecs() const = 0;

		virtual void setPitch(float) = 0;
		virtual float getPitch() const = 0;

		virtual void setGain(float) = 0;
		virtual float getGain() const = 0;

		virtual void setRolloffFactor(float) = 0;
		virtual float getRolloffFactor() const = 0;

		virtual void setMinGain(float) = 0;
		virtual float getMinGain() const = 0;
		
		virtual void setMaxGain(float) = 0;
		virtual float getMaxGain() const = 0;

		virtual void setConeOuterGain(float) = 0;
		virtual float getConeOuterGain() const = 0;

		virtual void setConeInnerAngle(float) = 0;
		virtual float getConeInnerAngle() const = 0;

		virtual void setConeOuterAngle(float) = 0;
		virtual float getConeOuterAngle() const = 0;
		
		virtual void setPosition(const Vector3&) = 0;
		virtual Vector3 getPosition() const = 0;

		virtual void setVelocity(const Vector3&) = 0;
		virtual Vector3 getVelocity() const = 0;

		virtual void setDirection(const Vector3&) = 0;
		virtual Vector3 getDirection() const = 0;
	};
};

#endif