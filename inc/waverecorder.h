/*
 * waverecorder_.h
 *
 *  Created on: 11.01.2019
 *      Author: piotr
 */

#ifndef __WAVERECORDER_H
#define __WAVERECORDER_H

// Includes
#include "main.h"

// Defines for the Audio recording process
#define DEFAULT_TIME_REC                      30  /* Recording time in second (default: 30s) */

#define REC_WAVE_NAME "Wave.wav"

#define REC_SAMPLE_LENGTH   (DEFAULT_TIME_REC * DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * 2)

// Exported functions
AUDIO_ErrorTypeDef AUDIO_REC_Process(void);
AUDIO_ErrorTypeDef AUDIO_REC_Start(void);
AUDIO_ErrorTypeDef AUDIO_PLAYER_Init(void);

#endif /* __WAVERECORDER_H */
