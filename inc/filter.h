/*
 * filter.h
 *
 *  Created on: 11.12.2018
 *      Author: piotr
 */

#ifndef FILTER_H_
#define FILTER_H_
#include "arm_math.h"
#include "math_helper.h"
#include "stdio.h"
/* ----------------------------------------------------------------------
** Macro Defines
** ------------------------------------------------------------------- */
#define AUDIO_BLOCK_SIZE   ((uint32_t)512)
#define TEST_LENGTH_SAMPLES  AUDIO_BLOCK_SIZE//320
#define SNR_THRESHOLD_F32    140.0f
#define BLOCK_SIZE            32
#define NUM_TAPS              29

/* -------------------------------------------------------------------
 * The input signal and reference output (computed with MATLAB)
 * are defined externally in arm_fir_lpf_data.c.
 * ------------------------------------------------------------------- */
//extern float32_t testInput_f32_1kHz_15kHz[TEST_LENGTH_SAMPLES];
//extern float32_t refOutput[TEST_LENGTH_SAMPLES];

float32_t * filter(float32_t * buf);

#endif /* FILTER_H_ */


