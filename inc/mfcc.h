/*
 * mfcc.h
 *
 *  Created on: 24.01.2019
 *      Author: piotr
 */

#ifndef MFCC_H_
#define MFCC_H_

#define BLOCK_SIZE	256
#define CEPLENGTH	20
#define pi 3.14159265358979323846264338

#include"arm_math.h"
#include"stdio.h"

void mfcc_init();
void mfcc(uint16_t *input1,float32_t *output);
void DCT_custom(float32_t *input,float32_t *output,uint16_t length);

#endif /* MFCC_H_ */
