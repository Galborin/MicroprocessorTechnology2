/*
 * process_sections.h
 *
 *  Created on: 24.01.2019
 *      Author: piotr
 */

#ifndef PROCESS_SECTIONS_H_
#define PROCESS_SECTIONS_H_

#define SAMPLING_FREQ 	8000
#define BLOCK_SIZE 	  	256
#define CEP				19
#define NCEPVEC			65

#include "stdio.h"
#include "arm_math.h"

float32_t classify(uint16_t ref_ncepvec,float32_t reference[CEP][ref_ncepvec],float32_t input[CEP][NCEPVEC]);

#endif /* PROCESS_SECTIONS_H_ */
