#ifndef INCLUDED_IFILEREADER_H_
#define INCLUDED_IFILEREADER_H_

namespace gameaudio {
	#if defined(_MSC_VER) || defined(__BORLANDC__)
		typedef unsigned __int64 uint64;
	#else
		typedef unsigned long long uint64;
	#endif

	class IFileReader {
	public:
		typedef enum {
			E_SEEK_BEG,
			E_SEEK_CUR,
			E_SEEK_END,
		} seek_type;
		virtual ~IFileReader() {};
		virtual unsigned read(void* buffer, unsigned size_to_read) = 0;
		virtual bool seek(uint64 pos, seek_type whence = E_SEEK_BEG) = 0;
		virtual uint64 getSize() const = 0;
		virtual uint64 tell() const = 0;
	};
};

#endif