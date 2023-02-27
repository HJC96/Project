//
//  wfVector.h
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#ifndef wfVector_h
#define wfVector_h

#include "wfPrimitiveOperation.h"
typedef struct _vec {
    unsigned long long length;
    Complex* arr;
} vec;

typedef vec* vecRef;

vecRef vecAlloc(unsigned long long length);
void   vecRelease(vecRef* arr);

void      vadd(vecRef a, vecRef b, vecRef* output);
void vsubtract(vecRef a, vecRef b, vecRef* output);
void vmultiply(vecRef a, vecRef b, vecRef* output);
void   vdivide(vecRef a, vecRef b, vecRef* output);

void vaddScalar(vecRef a, Complex c, vecRef* output);
void vsubScalar(vecRef a, Complex c, vecRef* output);
void vmulScalar(vecRef a, Complex c, vecRef* output);
void vdivScalar(vecRef a, Complex c, vecRef* output);

#endif /* wfVector_h */
