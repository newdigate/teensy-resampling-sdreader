//
// Created by Nicholas Newdigate on 18/07/2020.
//

#ifndef TEENSY_RESAMPLING_WAVSDREADER_READER_MONO_NOLOOP_BACKWARD_TESTS_CPP
#define TEENSY_RESAMPLING_WAVSDREADER_READER_MONO_NOLOOP_BACKWARD_TESTS_CPP
#include <boost/test/unit_test.hpp>
#include "ResamplingReaderFixture.h"

BOOST_AUTO_TEST_SUITE(test_wav_mono_noloop_backward_playback)

    uint16_t test_sndhdrdata_sndhdr_wav[] = {
            0x4952, 0x4646, 0x0038, 0x0000, 0x4157, 0x4556,
            0x6d66, 0x2074, 0x0010, 0x0000, 0x0001, 0x0002,
            0xac44, 0x0000, 0xb110, 0x0002, 0x0004, 0x0010,
            0x6164, 0x6174, 0x0014, 0x0000
    };
    unsigned int test_sndhdrdata_sndhdr_wav_len = 22; // 22 int16_t = 44 bytes = size of wave header

    BOOST_FIXTURE_TEST_CASE(ReadBackwardAtRegularPlaybackRate, ResamplingReaderFixture) {

        const uint32_t expectedDataSize = 258;
        printf("test_wav_mono_noloop_forward_playback::ReadForwardAtRegularPlaybackRate(%d)\n", expectedDataSize);
        int16_t mockFileBytes[expectedDataSize + test_sndhdrdata_sndhdr_wav_len];
        int16_t expected[expectedDataSize];
        for (int16_t i = 0; i < test_sndhdrdata_sndhdr_wav_len; i++) {
            mockFileBytes[i] = test_sndhdrdata_sndhdr_wav[i];
        }
        for (int16_t i = 0; i < expectedDataSize; i++) {
            mockFileBytes[i + test_sndhdrdata_sndhdr_wav_len] = i;
            expected[i] = expectedDataSize - i - 1;
        }
        SD.setSDCardFileData((char*) mockFileBytes, (expectedDataSize + test_sndhdrdata_sndhdr_wav_len) * 2);

        resamplingSdReader->setHeaderSize(44);
        resamplingSdReader->begin();
        resamplingSdReader->setPlaybackRate(-1.0);
        resamplingSdReader->play("test2.wav");
        resamplingSdReader->setLoopType(looptype_none);
        int16_t actual[1024];

        int j = 0, bytesRead = 0, total_bytes_read = 0;
        do {
            bytesRead = resamplingSdReader->read(&actual[j * 256], 512 );
            total_bytes_read += bytesRead;
            printf("j:%d bytesRead: %d \n", j, bytesRead);

            for (int i=0; i < bytesRead/2; i++) {
                printf("\t\t[%x]:%x", expected[j * 256 + i], actual[j * 256 + i]);
            }
            printf("\n");
            j++;
        } while (bytesRead > 0);
        printf("total_bytes_read: %d \n", total_bytes_read);
        BOOST_CHECK_EQUAL(resamplingSdReader->isPlaying(), false);
        resamplingSdReader->close();

        BOOST_CHECK_EQUAL_COLLECTIONS(&expected[0], &expected[expectedDataSize-1], &actual[0], &actual[(total_bytes_read / 2)-1]);
    }

    BOOST_FIXTURE_TEST_CASE(ReadBackwardAtHalfPlaybackRate, ResamplingReaderFixture) {

        const uint32_t expectedDataSize = 258;
        printf("test_wav_mono_noloop_forward_playback::ReadForwardAtRegularPlaybackRate(%d)\n", expectedDataSize);
        int16_t mockFileBytes[expectedDataSize + test_sndhdrdata_sndhdr_wav_len];
        int16_t expected[expectedDataSize * 2];
        for (int16_t i = 0; i < test_sndhdrdata_sndhdr_wav_len; i++) {
            mockFileBytes[i] = test_sndhdrdata_sndhdr_wav[i];
        }
        for (int16_t i = 0; i < expectedDataSize; i++) {
            mockFileBytes[i + test_sndhdrdata_sndhdr_wav_len] = i;
        }
        for (int16_t i = 0; i < expectedDataSize * 2; i++) {
            expected[i] = expectedDataSize - (i/2) - 1;
        }

        SD.setSDCardFileData((char*) mockFileBytes, (expectedDataSize + test_sndhdrdata_sndhdr_wav_len) * 2);

        resamplingSdReader->begin();
        resamplingSdReader->setHeaderSize(44);
        resamplingSdReader->setPlaybackRate(-0.5);
        resamplingSdReader->play("test2.bin");
        resamplingSdReader->setLoopType(looptype_none);
        int16_t actual[expectedDataSize*2];

        int j = 0, bytesRead = 0, total_bytes_read = 0;
        do {
            bytesRead = resamplingSdReader->read(&actual[j * 256], 512 );
            total_bytes_read += bytesRead;
            printf("j:%d bytesRead: %d: ", j, bytesRead);
            for (int i=0; i < bytesRead/2; i++) {
                printf("\t\t[%x]:%x", expected[j * 256 + i], actual[j * 256 + i]);
            }
            printf("\n");
            j++;
        } while (bytesRead > 0);
        printf("total_bytes_read: %d \n", total_bytes_read);
        resamplingSdReader->close();

        BOOST_CHECK_EQUAL_COLLECTIONS(&expected[0], &expected[(expectedDataSize * 2) - 1], &actual[0], &actual[(total_bytes_read / 2) - 1]);
    }

BOOST_AUTO_TEST_SUITE_END()

#endif //TEENSY_RESAMPLING_WAVSDREADER_READER_MONO_NOLOOP_BACKWARD_TESTS_CPP
