/*
 * main_.h
 *
 *  Created on: 11.01.2019
 *      Author: piotr
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "usbh_core.h"
#include "stm32f7xx_hal.h"
#include "stm32f723e_discovery.h"
#include "stm32f723e_discovery_audio.h"
#include "stm32f723e_discovery_ts.h"
#include "lcd_log.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"
#include "arm_math.h"

/* Exported Defines ----------------------------------------------------------*/
#define AUDIO_IN_BUFFER_SIZE     512  //buffer size in half-word

/* Exported types ------------------------------------------------------------*/
/* Application State Machine Structure */
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_DISCONNECT,
}AppStateMachine;

typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_WAIT,
  AUDIO_STATE_INIT,
  AUDIO_STATE_PLAY,
  AUDIO_STATE_PRERECORD,
  AUDIO_STATE_RECORD,
  AUDIO_STATE_BEFORE_PRERECORD,
  AUDIO_STATE_PRERECORD_CHOOSE,
  AUDIO_STATE_NEXT,
  AUDIO_STATE_PREVIOUS,
  AUDIO_STATE_FORWARD,
  AUDIO_STATE_BACKWARD,
  AUDIO_STATE_STOP,
  AUDIO_STATE_PAUSE,
  AUDIO_STATE_RESUME,
  AUDIO_STATE_VOLUME_UP,
  AUDIO_STATE_VOLUME_DOWN,
  AUDIO_STATE_ERROR,
}AUDIO_StateTypeDef;

typedef enum {
  BUFFER_EMPTY = 0,
  BUFFER_FULL,
}WR_BUFFER_StateTypeDef;

typedef struct {
  uint16_t pcm_buff[AUDIO_IN_BUFFER_SIZE];
  uint32_t pcm_ptr;
  BufferCtl fptr;
  WR_BUFFER_StateTypeDef wr_state;
  uint32_t offset;
}AUDIO_IN_BufferTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0,
  AUDIO_ERROR_IO,
  AUDIO_ERROR_EOF,
  AUDIO_ERROR_INVALID_VALUE,
}AUDIO_ErrorTypeDef;

/*Public variables*/
extern AppStateMachine appli_state;
extern AUDIO_StateTypeDef AudioState;
extern AUDIO_IN_BufferTypeDef BufferCtl;
/* Public functions ------------------------------------------------------- */
uint8_t  Touchscreen_Calibration (void);
uint16_t TouchScreen_Get_Calibrated_X(uint16_t x);
uint16_t TouchScreen_Get_Calibrated_Y(uint16_t y);
uint8_t  TouchScreen_IsCalibrationDone(void);
/* Menu API */
void AUDIO_MenuProcess(void);


#endif /* MAIN_H_ */
