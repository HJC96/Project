//
//  wfComplex.h
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#ifndef wfComplex_h
#define wfComplex_h

#include "wfReal.h"

typedef struct _complex
{
    Real re;
    Real im;
} Complex;

Complex ComplexMake(Real re, Real im);

#endif /* wfComplex_h */
