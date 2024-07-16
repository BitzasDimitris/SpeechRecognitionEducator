#ifndef WAVESTRUCTS_H
#define WAVESTRUCTS_H

#endif // WAVESTRUCTS_H

struct RIFF_Header {
    char chunkID[4];
    long chunkSize;//size not including chunkSize or chunkID
    char format[4];
};

struct WAVE_Format {
    char subChunkID[4];
    long subChunkSize;
    short audioFormat;
    short numChannels;
    long sampleRate;
    long byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct WAVE_Data {
    char subChunkID[4]; //should contain the word data
    long subChunk2Size; //Stores the size of the data block
};
