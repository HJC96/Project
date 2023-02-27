//
//  fourierTransform.h
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#ifndef fourierTransform_h
#define fourierTransform_h

#include "wfPrimitiveOperation.h"
#include "wfVector.h"

void fft(const vecRef input, vecRef *output);
void ifft(const vecRef input, vecRef *output);

#endif /* fourierTransform_h */
