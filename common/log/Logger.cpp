/*
 Copyright (C) 2013 by Anton Gaenko
 Mail anton.gaenko@yahoo.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */


#include "Logger.h"
#import <iostream>

// use to notify reader about a program state
void info(char const *txt, ...) {
  #ifdef DEBUG
    printf("INFO: ");
    va_list args;
    va_start(args, txt);
    vprintf(txt, args);
    va_end(args);
    printf("\n");
  #endif
}

// use to print for your needs
void debug(char const *txt, ...) {
  #ifdef DEBUG
    printf("DEBUG: ");
    va_list args;
    va_start(args, txt);
    vprintf(txt, args);
    va_end(args);
    printf("\n");
  #endif
}

// use to print float array or matrix
void debugArray(char const *txt, float arr[], int size, short matrixRowSize) {
  #ifdef DEBUG
    printf("DEBUG: %s \n", txt);
    for (int i=0; i<size; i+= matrixRowSize) {
        // if this array is matrix
        if (matrixRowSize >0) {
            // print each row on new line
            for (int j=0; j< matrixRowSize; j++) {
                printf(" %f ", arr[i+j]);
            }
            printf("\n");
        } else {
            printf("%f, ", arr[i]);
        }
    }
    printf("\n");
  #endif
}

// use to notify about wrong state of a program
void error(char const *txt, ...) {
  #ifdef DEBUG
    fprintf(stderr, "ERROR: ");
    va_list args;
    va_start(args, txt);
    vfprintf(stderr, txt, args);
    va_end(args);
    fprintf(stderr, "\n");
  #endif
}

