//
//  Codegen.h
//
//  Created by Brian on 02/16/2010
//  Copyright 2010 The Echo Nest. All rights reserved.
//

#ifndef CODEGEN_H
#define CODEGEN_H

// Entry point for generating codes from PCM data.
#define VERSION 3.15

#include <memory>
#include <string>

#include "Common.h"
using namespace std;

#ifdef _MSC_VER
    #ifdef CODEGEN_EXPORTS
        #define CODEGEN_API __declspec(dllexport)
        #pragma message("Exporting codegen.dll")
    #else
        #define CODEGEN_API __declspec(dllimport)
        #pragma message("Importing codegen.dll")
    #endif
#else
    #define CODEGEN_API
#endif

class Fingerprint;
class CODEGEN_API Codegen
{
public:
    Codegen(const float* pcm, uint numSamples, int start_offset);

    string getCodeString(){return _CodeString;}
    int getNumCodes(){return _NumCodes;}
    float getVersion() { return VERSION; }
private:
    string _CodeString;
    int _NumCodes;
};

#endif
