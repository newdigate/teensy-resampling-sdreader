#ifndef TEENSYAUDIOLIBRARY_RESAMPLINGARRAYREADER_H
#define TEENSYAUDIOLIBRARY_RESAMPLINGARRAYREADER_H

#include <SD.h>
#include <cstdint>
#include "spi_interrupt.h"
#include "loop_type.h"

#ifndef AUDIO_BLOCK_SAMPLES
#define AUDIO_BLOCK_SAMPLES 256
#endif

class ResamplingArrayReader {
public:
    ResamplingArrayReader() {
    }

    void begin(void);
    bool play(int16_t *array, uint32_t length);
    bool play();
    void stop(void);
    bool isPlaying(void) { return _playing; }

    unsigned int read(void *buf, uint16_t nbyte);
    bool readNextValue(int16_t *value);

    void setPlaybackRate(double f) {
        _playbackRate = f;
        if (f < 0 && _file_offset == 0) {
            //_file.seek(_file_size);
            _file_offset = _file_size;
        }
    }

    float playbackRate() {
        return _playbackRate;
    }

    void setLoopType(loop_type loopType)
    {
        _loopType = loopType;
    }

    loop_type getLoopType(){
        return _loopType;    
    }

    int available(void);
    void reset(void);
    void close(void);

    void setLoopStart(uint32_t loop_start) {
        _loop_start = loop_start;
    }

    void setLoopFinish(uint32_t loop_finish) {
        // sample number, (NOT byte number)
        _loop_finish = loop_finish;
    }

private:
    volatile bool _playing = false;
    volatile int32_t _file_offset;
    volatile int32_t _last_read_offset = 0;

    int32_t _file_size;
    double _playbackRate = 1.0;
    double _remainder = 0.0;
    loop_type _loopType = looptype_none;
    int _bufferPosition = 0;
    int32_t _loop_start = 0;
    int32_t _loop_finish = 0;

    int16_t _destinationBuffer[AUDIO_BLOCK_SAMPLES];
    int16_t *_sourceBuffer;
    unsigned int _bufferLength = 0;

    bool updateBuffers(void);

};


#endif //TEENSYAUDIOLIBRARY_RESAMPLINGARRAYREADER_H