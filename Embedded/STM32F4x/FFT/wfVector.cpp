//
//  wfVector.c
//  wavelet-fft
//
//  Created by Helloworld Park. on 2017. 2. 25..
//  Copyright © 2017년 Helloworld Park. All rights reserved.
//

#include "mbed.h" 
#include "wfVector.h"

vecRef vecAlloc(unsigned long long length) {
    vecRef vector = (vecRef)malloc(sizeof(vec) * 1);
    vector->length = length;
    vector->arr = (Complex*)calloc(length, sizeof(Complex));
    return vector;
}

void vecRelease(vecRef* arr) {
    if (*arr)
    {
        if ((*arr)->arr) free((*arr)->arr);
        free(*arr);
    }
}

void vadd(vecRef a, vecRef b, vecRef* output) {
    assert(a->length == b->length);
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cAdd(a->arr[i], b->arr[i]);
    }
}

void vsubtract(vecRef a, vecRef b, vecRef* output) {
    assert(a->length == b->length);
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cSubtract(a->arr[i], b->arr[i]);
    }
}

void vmultiply(vecRef a, vecRef b, vecRef* output) {
    assert(a->length == b->length);
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cMultiply(a->arr[i], b->arr[i]);
    }
}

void vdivide(vecRef a, vecRef b, vecRef* output) {
    assert(a->length == b->length);
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cDivide(a->arr[i], b->arr[i]);
    }
}

void vaddScalar(vecRef a, Complex c, vecRef* output) {
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cAdd(a->arr[i], c);
    }
}

void vsubScalar(vecRef a, Complex c, vecRef* output) {
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cSubtract(a->arr[i], c);
    }
}

void vmulScalar(vecRef a, Complex c, vecRef* output) {
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cMultiply(a->arr[i], c);
    }
}

void vdivScalar(vecRef a, Complex c, vecRef* output) {
    (*output)->length = a->length;
    for (int i = 0; i < a->length; i++) {
        (*output)->arr[i] = cDivide(a->arr[i], c);
    }
}
