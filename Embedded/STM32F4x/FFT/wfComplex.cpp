//
//  wfComplex.c
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#include "wfComplex.h"

Complex ComplexMake(Real re, Real im)
{
    Complex complex;
    complex.re = re; complex.im = im;
    return complex;
}
