#ifndef GAMEAUDIO_FILEREADER_H_
#define GAMEAUDIO_FILEREADER_H_

#include "../../include/gameaudio/IFileReader.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace gameaudio {

class FileReader : public IFileReader {
	boost::filesystem::ifstream* _ifstream;
	uint64 _size;
public:
	FileReader(boost::filesystem::ifstream* ifstream, uint64 size);
	~FileReader();
	
	virtual unsigned read(void* buffer, unsigned size_to_read);
	virtual bool seek(uint64 pos, seek_type whence = E_SEEK_BEG);
	virtual uint64 getSize();
	virtual uint64 tell() const;
};

};

#endif