#ifndef GAMEAUDIO_ERROR_H_
#define GAMEAUDIO_ERROR_H_

#include <stdexcept>

namespace gameaudio {
	class Error : public std::runtime_error {
	public:
		Error(const char* message)
			: std::runtime_error(message)
		{
		};
	};
};

#endif