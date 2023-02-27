//
//  fourierTransform.c
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//
#define M_PI 3.14

#include "mbed.h" 
#include "fourierTransform.h"  

typedef unsigned long long Long;

int checkPowerOfTwo(const unsigned long long n);
void vecSwap(vecRef *a, vecRef *b);
Long reverse(const unsigned long long n, const unsigned long long k);
void permutate(const int q, const vecRef input, vecRef *output);
void compute(const int q, int sign, const vecRef input, vecRef *output);
void computeFFT(const int q, const vecRef input, vecRef *output);
void computeIFFT(const int q, const vecRef input, vecRef *output);

void fft(const vecRef input, vecRef *output)
{
    assert(input->length >= 2);
    Long N = input->length;
    if (checkPowerOfTwo(N) != 1)
    {
        N = 1 << (int)(round(log2(N))+1);
    }
    int rounds = (int)log2(N);
    
    // If output is null, we make it
    // If output is not null and if it is not power of 2, we reallocate it
    // Else, use output
    if (*output == NULL || (*output)->length != N)
    {
        vecRelease(output);
        *output = vecAlloc(N);
    }
    
    permutate(rounds, input, output);
    vecRef tmp = vecAlloc(N);
    memcpy(tmp->arr, (*output)->arr, sizeof(Complex) * tmp->length);
    for (int q = rounds-1; q >= 0; q--)
    {
        vecSwap(output, &tmp);
        computeFFT(q, tmp, output);
    }
    vecRelease(&tmp);
    vdivScalar(*output, ComplexMake(sqrt(N), 0.0), output);
}

void ifft(const vecRef input, vecRef *output)
{
    assert(input->length >= 2);
    Long N = input->length;
    if (checkPowerOfTwo(N) != 1)
    {
        N = 1 << (int)(round(log2(N))+1);
    }
    int rounds = (int)log2(N);
    
    // If output is null, we make it
    // If output is not null and if it is not power of 2, we reallocate it
    // Else, use output
    if (*output == NULL || (*output)->length != N)
    {
        vecRelease(output);
        *output = vecAlloc(N);
    }
    
    permutate(rounds, input, output);
    vecRef tmp = vecAlloc(N);
    memcpy(tmp->arr, (*output)->arr, sizeof(Complex) * tmp->length);
    for (int q = rounds-1; q >= 0; q--)
    {
        vecSwap(output, &tmp);
        computeIFFT(q, tmp, output);
    }
    vecRelease(&tmp);
    vdivScalar(*output, ComplexMake(sqrt(N), 0.0), output);
}

void vecSwap(vecRef *a, vecRef *b)
{
    vecRef tmp = *a;
    *a = *b;
    *b = tmp;
}

int checkPowerOfTwo(const Long n)
{
    Long k = n;
    while(k > 1)
    {
        if (k % 2 == 1) return 0;
        k /= 2;
    }
    return 1;
}

// http://stackoverflow.com/a/34236981
Long reverse(const Long n, const Long k)
{
    Long r, i;
    for (r = 0, i = 0; i < k; ++i)
        r |= ((n >> i) & 1) << (k - i - 1);
    return r;
}

void permutate(const int q, const vecRef input, vecRef *output)
{
    for (Long i = 0; i < (1 << q); i++)
    {
        (*output)->arr[i] = input->arr[reverse(i, q)];
    }
}

void compute(const int q, int sign, const vecRef input, vecRef *output)
{
    const Long countBlock = 1 << q;
    const Long lenBlock = (*output)->length / countBlock;
    if (sign >= 0) sign = 1;
    if (sign < 0) sign = -1;
    
    for (Long baseIdx = 0; baseIdx < lenBlock/2; baseIdx++)
    {
        const Complex w = cPolarToComplex(1.0, sign * (M_PI * baseIdx)/(lenBlock/2));
        for (Long block = 0; block < countBlock; block++)
        {
            const Long idx = baseIdx + block * lenBlock;
            const Complex a = input->arr[idx];
            const Complex b = input->arr[idx + lenBlock/2];
            const Complex wB = cMultiply(w, b);
            // Upper part
            (*output)->arr[idx] = cAdd(a, wB);
            // Lower part
            (*output)->arr[idx + lenBlock/2] = cSubtract(a, wB);
        }
    }
}

void computeFFT(const int q, const vecRef input, vecRef *output)
{
    compute(q, -1, input, output);
}

void computeIFFT(const int q, const vecRef input, vecRef *output)
{
    compute(q, 1, input, output);
}
