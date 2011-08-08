#include "Listener.h"

#include <al.h>

using namespace gameaudio;

Listener::Listener() {

}

void Listener::setPosition(const Vector3& v) {
	alListener3f(AL_POSITION, v.x, v.y, v.z);
}

Vector3 Listener::getPosition() const {
	Vector3 v;
	alGetListener3f(AL_POSITION, &(v.x), &(v.x), &(v.x));
	return v;
}

void Listener::setVelocity(const Vector3& v) {
	alListener3f(AL_VELOCITY, v.x, v.y, v.z);
}

Vector3 Listener::getVelocity() const {
	Vector3 v;
	alGetListener3f(AL_VELOCITY, &(v.x), &(v.x), &(v.x));
	return v;
}

void Listener::setGain(float v) {
	alListenerf(AL_GAIN, v);
}

float Listener::getGain() const {
	float v;
	alGetListenerf(AL_GAIN, &v);
	return v;
}

void Listener::setDirection(const Vector3& v) {
	Vector3 up = getUpVector();
	float x[6] = {v.x, v.y, v.z, up.x, up.y, up.z};
	alListenerfv(AL_ORIENTATION, x);
}

Vector3 Listener::getDirection() const {
	float x[6];
	alGetListenerfv(AL_ORIENTATION, x);
	return Vector3(x[0], x[1], x[2]);
}

void Listener::setUpVector(const Vector3& v) {
	Vector3 dir = getDirection();
	float x[6] = {dir.x, dir.y, dir.z, v.x, v.y, v.z};
	alListenerfv(AL_ORIENTATION, x);
}

Vector3 Listener::getUpVector() const {
	float x[6];
	alGetListenerfv(AL_ORIENTATION, x);
	return Vector3(x[3], x[4], x[5]);
}