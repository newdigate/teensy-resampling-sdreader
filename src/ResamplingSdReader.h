//
// Created by Nicholas Newdigate on 10/02/2019.
//

#ifndef TEENSYAUDIOLIBRARY_RESAMPLINGSDREADER_H
#define TEENSYAUDIOLIBRARY_RESAMPLINGSDREADER_H

#include "SD.h"
#include <cstdint>
#include "spi_interrupt.h"
#include "loop_type.h"
#include <list>

#ifndef AUDIO_BLOCK_SAMPLES
#define AUDIO_BLOCK_SAMPLES 256
#endif

#define RESAMPLE_BUFFER_SAMPLE_SIZE 256

struct IntepolationData
{
    double x, y;
};

class ResamplingSdReader {
public:
    ResamplingSdReader() {
    }

    void begin(void);
    bool play(const char *filename);
    bool play();
    void stop(void);
    bool isPlaying(void) { return _playing; }

    unsigned int read(void *buf, uint16_t nbyte);
    bool readNextValue(int16_t *value);

    void setPlaybackRate(double f) {
        _playbackRate = f;
        if (f < 0 && _file_offset == _header_size) {
            //_file.seek(_file_size);
            _file_offset = _file_size;
        }
    }

    double playbackRate() {
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

    void setHeaderSize(uint32_t headerSize) {
        _header_size = headerSize;
    }

    void setLoopStart(uint32_t loop_start) {
        _loop_start = loop_start;
    }

    void setLoopFinish(uint32_t loop_finish) {
        // sample number, (NOT byte number)
        _loop_finish = loop_finish;
    }

    void enableInterpolation(bool enabled) {
        _enable_interpolation = enabled;
    }

private:
    volatile bool _playing = false;
    volatile int32_t _file_offset;
    volatile int32_t _last_read_offset = 0;

    int32_t _file_size;
    int32_t _header_size = 0;
    double _playbackRate = 1.0;
    double _remainder = 0.0;
    loop_type _loopType = looptype_none;
    char *_filename = "";
    int _bufferPosition = 0;
    int32_t _loop_start = 0;
    int32_t _loop_finish = 0;

    int16_t _buffer[RESAMPLE_BUFFER_SAMPLE_SIZE];
    unsigned int _bufferLength = 0;

    File _file;

    bool _enable_interpolation = false;
    unsigned int _numInterpolationPoints = 0;
    IntepolationData _interpolationPoints[4] = { 0.0f, 0.0f };

    bool updateBuffers(void);

    void StartUsingSPI(){
        //Serial.printf("start spi: %s\n", _filename);

#if defined(HAS_KINETIS_SDHC)
        if (!(SIM_SCGC3 & SIM_SCGC3_SDHC)) AudioStartUsingSPI();
#else
        AudioStartUsingSPI();
#endif
    }

    void StopUsingSPI() {
        //Serial.printf("stop spi: %s\n", _filename);
#if defined(HAS_KINETIS_SDHC)
        if (!(SIM_SCGC3 & SIM_SCGC3_SDHC)) AudioStopUsingSPI();
#else
        AudioStopUsingSPI();
#endif
    }
};


#endif //PAULSTOFFREGEN_RESAMPLINGSDREADER_H
