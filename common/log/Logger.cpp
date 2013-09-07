/*
 Copyright (C) 2013 by Anton Gaenko
 
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

// TODO Place here this code if iOS to print to device log
/*int printf(const char * __restrict format, ...)
{
    va_list args;
    va_start(args,format);
    NSLogv([NSString stringWithUTF8String:format], args) ;
    va_end(args);
    return 1;
}*/

void info(const char *txt, ...) {
    fprintf(stdout, "INFO: ");
    va_list args;
    va_start(args, txt);
    vfprintf(stdout, txt, args);
    va_end(args);
    fprintf(stdout, "\n");
}

void debug(const char *txt, ...) {
    fprintf(stdout, "DEBUG: ");
    va_list args;
    va_start(args, txt);
    vfprintf(stdout, txt, args);
    va_end(args);
    fprintf(stdout, "\n");
}

void error(const char *txt, ...) {
    fprintf(stderr, "ERROR: ");
    va_list args;
    va_start(args, txt);
    vfprintf(stderr, txt, args);
    va_end(args);
    fprintf(stderr, "\n");
}