#include <stdlib.h>	
#include <stdio.h>
#include <cstdio>
#include <string>
#include "WavFile.h"
#ifndef WAVESTRUCTS_H
#define WAVESTRUCTS_H
#include "wavestructs.h"
#endif // WAVESTRUCTS_H


class WavLoader
{
	
public:
    static bool LoadWavFile(std::string filename,WavFile* wfile);
};

