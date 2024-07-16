#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "WavLoader.h"


bool WavLoader::LoadWavFile(std::string filename,WavFile* wfile) {
	//Local Declarations
    FILE* soundFile = NULL;
 
	try {
	soundFile=fopen(filename.c_str(), "rb");
	if (!soundFile)
		throw (filename);
 
	// Read in the first chunk into the struct
    fread(&(wfile->Rheader), sizeof(RIFF_Header), 1, soundFile);
 
	//check for RIFF and WAVE tag in memeory
    if ((wfile->Rheader.chunkID[0] != 'R' ||
            wfile->Rheader.chunkID[1] != 'I' ||
            wfile->Rheader.chunkID[2] != 'F' ||
            wfile->Rheader.chunkID[3] != 'F') ||
        (wfile->Rheader.format[0] != 'W' ||
            wfile->Rheader.format[1] != 'A' ||
            wfile->Rheader.format[2] != 'V' ||
            wfile->Rheader.format[3] != 'E')){
            return false;
    }
 
	//Read in the 2nd chunk for the wave info
    fread(&(wfile->Wformat), sizeof(WAVE_Format), 1, soundFile);
	//check for fmt tag in memory
    if (wfile->Wformat.subChunkID[0] != 'f' ||
        wfile->Wformat.subChunkID[1] != 'm' ||
        wfile->Wformat.subChunkID[2] != 't' ||
        wfile->Wformat.subChunkID[3] != ' '){
                return false;
    }
	//check for extra parameters;
    if (wfile->Wformat.subChunkSize > 16)
        fseek(soundFile, sizeof(short), SEEK_CUR);
	//Read in the the last byte of data before the sound file
    fread(&(wfile->Wdata), sizeof(WAVE_Data), 1, soundFile);
	//check for data tag in memory
    if (wfile->Wdata.subChunkID[0] != 'd' ||
        wfile->Wdata.subChunkID[1] != 'a' ||
        wfile->Wdata.subChunkID[2] != 't' ||
        wfile->Wdata.subChunkID[3] != 'a'){
                return false;
    }
	//Allocate memory for data
    (wfile->data) = new unsigned char[wfile->Wdata.subChunk2Size]();
	// Read in the sound data into the soundData variable
    if (!fread(wfile->data, wfile->Wdata.subChunk2Size, 1, soundFile))
        throw ("error loading WAVE data into struct!");


	//clean up and return true if successful
	fclose(soundFile);
	return true;
    } catch(std::string error) {
	if (soundFile != NULL)
		fclose(soundFile);
	//return false to indicate the failure to load wave
	return false;
	}
}
