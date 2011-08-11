project(libGameAudio)

add_library(GameAudio STATIC
    src/gameaudio/FileReader.cpp
    src/gameaudio/IDecoder.cpp
    src/gameaudio/Listener.cpp
    src/gameaudio/LoopSound.cpp
    src/gameaudio/OggVorbisDecoder.cpp
    src/gameaudio/SoundAbstract.cpp
    src/gameaudio/SoundManager.cpp
    src/gameaudio/StaticSound.cpp
    src/gameaudio/StreamSound.cpp
    src/gameaudio/WavDecoder.cpp
)
add_executable(test test/main.cpp)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
link_directories("${PROJECT_SOURCE_DIR}" /opt/local/lib /usr/local/lib /usr/lib)

find_path(BOOST_INCLUDE_DIR boost/version.hpp)
find_path(LIBOGG_INCLUDE_DIR ogg/ogg.h)
find_path(LIBVORBIS_INCLUDE_DIR vorbis/vorbisfile.h)
find_path(OPENAL_INCLUDE_DIR al.h)

find_library(BOOST_SYSTEM_LIB boost_system-mt)
find_library(BOOST_FILESYSTEM_LIB boost_filesystem-mt)
find_library(BOOST_THREAD_LIB boost_thread-mt)
find_library(LIBOGG_LIB ogg)
find_library(LIBVORBIS_LIB vorbis)
find_library(LIBVORBISFILE_LIB vorbisfile)
if(APPLE)
    find_library(OPENAL_LIBRARY openal)
    mark_as_advanced(OPENAL_LIBRARY)
else(APPLE)
    find_library(OPENAL_LIBRARY OpenAL32)
endif(APPLE)

include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${BOOST_INCLUDE_DIR}"
    "${LIBOGG_INCLUDE_DIR}"
    "${LIBVORBIS_INCLUDE_DIR}"
    "${OPENAL_INCLUDE_DIR}"
)

target_link_libraries(
    test GameAudio
    ${BOOST_SYSTEM_LIB} ${BOOST_FILESYSTEM_LIB} ${BOOST_THREAD_LIB}
    ${LIBOGG_LIB} ${LIBVORBIS_LIB} ${LIBVORBISFILE_LIB} ${OPENAL_LIBRARY}
)