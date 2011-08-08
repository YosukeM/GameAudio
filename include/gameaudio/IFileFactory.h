#ifndef GAMEAUDIO_IFILEFACTORY_H_
#define GAMEAUDIO_IFILEFACTORY_H_

namespace gameaudio {
	class IFileReader;

	class IFileFactory {
	public:
		virtual ~IFileFactory() {};

		virtual IFileReader* createFileReader(const char* filepath) { return 0; };
		virtual IFileReader* createFileReader(const wchar_t* filepath) { return 0; };
	};
};

#endif