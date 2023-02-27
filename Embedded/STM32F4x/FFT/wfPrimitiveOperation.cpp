//
//  wfPrimitiveOperation.c
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#include "mbed.h" 
#include "wfPrimitiveOperation.h"

Real cAbsolute(Complex a)
{
    return sqrt(a.re * a.re + a.im * a.im);
}

Real cArgument(Complex a)
{
    return atan2(a.im, a.re);
}

Complex cAdd(Complex a, Complex b)
{
    return ComplexMake(a.re + b.re, a.im + b.im);
}

Complex cSubtract(Complex a, Complex b)
{
    return ComplexMake(a.re - b.re, a.im - b.im);
}

Complex cMultiply(Complex a, Complex b)
{
    return ComplexMake(a.re * b.re - a.im * b.im, a.re * b.im + b.re * a.im);
}

Complex cDivide(Complex a, Complex b)
{
    Real abs = cAbsolute(b);
    abs *= abs;
    if (abs < 1.e-30) assert(0);
    return ComplexMake((a.re * b.re + a.im * b.im)/abs, (-a.re * b.im + b.re * a.im)/abs);
}

Complex cNegate(Complex a)
{
    return ComplexMake(-a.re, -a.im);
}

Complex cPolarToComplex(Real length, Real angle)
{
    return ComplexMake(length * cos(angle), length * sin(angle));
}
