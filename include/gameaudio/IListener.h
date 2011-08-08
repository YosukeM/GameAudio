#ifndef GAMEAUDIO_ILISTENER_H_
#define GAMEAUDIO_ILISTENER_H_

#include "Vector3.h"

namespace gameaudio {
	class IListener {
	protected:
		IListener() {};
		IListener(const IListener&) {};
	public:
		virtual ~IListener() {};

		virtual void setPosition(const Vector3&) = 0;
		virtual Vector3 getPosition() const = 0;

		virtual void setVelocity(const Vector3&) = 0;
		virtual Vector3 getVelocity() const = 0;

		virtual void setGain(float) = 0;
		virtual float getGain() const = 0;

		virtual void setDirection(const Vector3&) = 0;
		virtual Vector3 getDirection() const = 0;

		virtual void setUpVector(const Vector3&) = 0;
		virtual Vector3 getUpVector() const = 0;
	};
};

#endif