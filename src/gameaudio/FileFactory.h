#ifndef GAMEAUDIO_FILEFACTORY_H_
#define GAMEAUDIO_FILEFACTORY_H_

#include "../../include/gameaudio/IFileFactory.h"
#include "FileReader.h"
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>

namespace gameaudio {
	class FileFactory : public IFileFactory {
		template <typename T> IFileReader* _create(const T* filepath) {
			namespace fs = boost::filesystem;
			fs::ifstream* stream;
			uint64 size;
			try {
				size = fs::file_size(filepath);
				stream = new fs::ifstream(filepath, std::ios::binary);
			} catch (...) {
				return NULL;
			}
			return new FileReader(stream, size);
		};
	public:
		IFileReader* createFileReader(const char* filepath) {
			return _create(filepath);
		}
		IFileReader* createFileReader(const wchar_t* filepath) {
			return _create(filepath);
		}
	};
};

#endif