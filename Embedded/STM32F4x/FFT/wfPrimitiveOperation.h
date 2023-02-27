//
//  wfPrimitiveOperation.h
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#ifndef wfPrimitiveOperation_h
#define wfPrimitiveOperation_h

#include "wfPrimitive.h"

Real cAbsolute(Complex a);
Real cArgument(Complex a);
Complex cAdd(Complex a, Complex b);
Complex cSubtract(Complex a, Complex b);
Complex cMultiply(Complex a, Complex b);
Complex cDivide(Complex a, Complex b);
Complex cNegate(Complex a);
Complex cPolarToComplex(Real length, Real angle);


#endif /* wfPrimitiveOperation_h */
