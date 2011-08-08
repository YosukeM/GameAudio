#ifndef GAMEAUDIO_LISTENER_H_
#define GAMEAUDIO_LISTENER_H_

#include "../../include/gameaudio/IListener.h"

namespace gameaudio {
	class Listener : public IListener {
		Vector3 _position;
		Vector3 _direction;
	public:
		Listener();

		virtual void setPosition(const Vector3&);
		virtual Vector3 getPosition() const;

		virtual void setVelocity(const Vector3&);
		virtual Vector3 getVelocity() const;

		virtual void setGain(float);
		virtual float getGain() const;

		virtual void setDirection(const Vector3&);
		virtual Vector3 getDirection() const;

		virtual void setUpVector(const Vector3&);
		virtual Vector3 getUpVector() const;
	};
};

#endif