#ifndef INCLUDED_SOUNDABSTRACT_H_
#define INCLUDED_SOUNDABSTRACT_H_

#include "../../include/gameaudio/ISound.h"
#include <alc.h>
#include <al.h>

namespace gameaudio {
	class IFileReader;
	class StreamBuffer;

	class SoundAbstract : public ISound {
	protected:
		ALuint _alSource;
	public:
		SoundAbstract();
		~SoundAbstract();

		virtual void update();

		// external methods:
		virtual void play();
		virtual bool isPlaying() const;
		virtual void pause();
		virtual void stop();

		virtual void rewind();

		virtual void setPlayPositionBySamples(uint64);
		virtual void setPlayPositionBySecs(float);

		virtual uint64 getPlayPositionBySamples() const;
		virtual float getPlayPositionBySecs() const;

		virtual void setPitch(float);
		virtual float getPitch() const;

		virtual void setGain(float);
		virtual float getGain() const;

		virtual void setRolloffFactor(float);
		virtual float getRolloffFactor() const;

		virtual void setMinGain(float);
		virtual float getMinGain() const;
		
		virtual void setMaxGain(float);
		virtual float getMaxGain() const;

		virtual void setConeOuterGain(float);
		virtual float getConeOuterGain() const;

		virtual void setConeInnerAngle(float);
		virtual float getConeInnerAngle() const;

		virtual void setConeOuterAngle(float);
		virtual float getConeOuterAngle() const;
		
		virtual void setPosition(const Vector3&);
		virtual Vector3 getPosition() const;

		virtual void setVelocity(const Vector3&);
		virtual Vector3 getVelocity() const;

		virtual void setDirection(const Vector3&);
		virtual Vector3 getDirection() const;

		virtual void setIsRelative(bool);
		virtual bool getIsRelative() const;
	};
};

#endif