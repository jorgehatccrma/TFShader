//
//  utils.h
//  TFShader
//
//  Created by Jorge Herrera on 3/20/12.
//  Copyright (c) 2012 Stanford. All rights reserved.
//

#ifndef TFShader_utils_h
#define TFShader_utils_h

#include <sys/time.h>

/*******************************
 * function prototypes
 *******************************/

float ** createTFmat(int nT, int nF);
double current_time_in_ms();




/*******************************
 * implementations
 *******************************/

float ** createTFmat(int nT, int nF)
{
    // declare it
    float ** mat;
    
    // allocate it
    mat = new float*[nT];
    for (int t = 0; t < nT; t++)
        mat[t] = new float[nF];
    
    // initialize to 0
    for (int t = 0; t < nT; t++)
        for (int f = 0; f < nF; f++)
            mat[t][f] = 0; //(float)rand()/RAND_MAX - 0.5;
    
    return mat;
}


double current_time_in_ms()
{
    timeval timer;
    gettimeofday(&timer,NULL);
    double ms = 1e-3 * timer.tv_usec + 1e+3 * timer.tv_sec;
    return ms;
}


#endif
