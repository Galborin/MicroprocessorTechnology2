/*
 * recordd.c
 *
 *  Created on: 11.01.2019
 *      Author: piotr
 */

#include "waverecorder.h"
#include "filter.h"

//zmienne do lcd - filter/nofilter
#define TOUCH_RECORD_XMIN       180
#define TOUCH_RECORD_XMAX       210
#define TOUCH_RECORD_YMIN       210
#define TOUCH_RECORD_YMAX       239

#define TOUCH_STOP_XMIN         100
#define TOUCH_STOP_XMAX         125
#define TOUCH_STOP_YMIN         210
#define TOUCH_STOP_YMAX         239

#define TOUCH_PAUSE_XMIN        20
#define TOUCH_PAUSE_XMAX        60
#define TOUCH_PAUSE_YMIN        210
#define TOUCH_PAUSE_YMAX        239

#define TOUCH_CHOOSE1_XMIN 	    20
#define TOUCH_CHOOSE1_XMAX 		123
#define TOUCH_CHOOSE1_YMIN 		100
#define TOUCH_CHOOSE1_YMAX 		239

#define TOUCH_CHOOSE2_XMAX 		230
#define TOUCH_CHOOSE2_XMIN 		127
#define TOUCH_CHOOSE2_YMIN 		100
#define TOUCH_CHOOSE2_YMAX 		239
//Private variables
uint8_t Header_Buffer[44];
static AUDIO_IN_BufferTypeDef  BufferCtl;
static __IO uint32_t uwVolume = 100;
extern WAVE_FormatTypeDef WaveFormat;
extern FIL WavFile;
static uint32_t  display_update = 1;
static uint32_t filtration = 0;

//FIR Coefficients buffer generated using fir1() MATLAB function.
//fir1(28, 6/24)
const float32_t firCoeffs32[NUM_TAPS] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};//*/
/*const float32_t firCoeffs32[NUM_TAPS] = {
		1, -0.9735
};//*/
// Declare State buffer of size (numTaps + blockSize - 1)
static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];
arm_fir_instance_f32 S;
//Private functions
static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t* pHeader);
static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct);
static void AUDIO_REC_DisplayButtons(void);
static void DisplayChoose(void);
static void LCD_ClearZone(void);

AUDIO_ErrorTypeDef AUDIO_REC_Start(void)
{
  uint32_t byteswritten = 0;
  uwVolume = 100;

  //init filter
  arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], BLOCK_SIZE);
  /* Create a new file system */
  if(f_open(&WavFile, REC_WAVE_NAME, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
  {
    /* Initialize header file */
    WavProcess_EncInit(DEFAULT_AUDIO_IN_FREQ, Header_Buffer);
    /* Write header file */
    if(f_write(&WavFile, Header_Buffer, 44, (void*)&byteswritten) == FR_OK)
    {
      AudioState = AUDIO_STATE_PRERECORD_CHOOSE;
      DisplayChoose();
      {
        if(byteswritten != 0)
        {
          BSP_AUDIO_IN_InitEx(INPUT_DEVICE_DIGITAL_MICROPHONE_2, BSP_AUDIO_FREQUENCY_16K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, DEFAULT_AUDIO_IN_CHANNEL_NBR);
          BSP_AUDIO_IN_Record((uint16_t*)&BufferCtl.pcm_buff[0], AUDIO_IN_BUFFER_SIZE);
          BufferCtl.fptr = byteswritten;
          BufferCtl.pcm_ptr = 0;
          BufferCtl.offset = 0;
          BufferCtl.wr_state = BUFFER_EMPTY;
          return AUDIO_ERROR_NONE;
        }
      }
    }
  }
  return AUDIO_ERROR_IO;
}

AUDIO_ErrorTypeDef AUDIO_REC_Process(void)
{
  uint32_t byteswritten = 0;
  uint16_t * temp;
  static float32_t testOutput[BLOCK_SIZE];
  float32_t  *inputF32;
  float32_t *outputF32=&testOutput[0];
  AUDIO_ErrorTypeDef audio_error = AUDIO_ERROR_NONE;
  uint32_t elapsed_time;
  static uint32_t prev_elapsed_time = 0xFFFFFFFF;
  uint8_t str[FILEMGR_FILE_NAME_SIZE + 20];
  uint8_t x,y;
  static TS_StateTypeDef  TS_State={0};

  switch(AudioState)
  {
  case AUDIO_STATE_PRERECORD_CHOOSE:
 	 if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
     {
 		BSP_TS_GetState(&TS_State);
     }
 	 else
 	 {
 		 BSP_TS_GetState(&TS_State);
 		 x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
 		 y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
 		 if(TS_State.touchDetected == 1)
 		 {
 			 if ((x > TOUCH_CHOOSE1_XMIN) && (x < TOUCH_CHOOSE1_XMAX) &&
 					 (y > TOUCH_CHOOSE1_YMIN) && (y < TOUCH_CHOOSE1_YMAX))
 	         {
 				 filtration = 1;
 				 AudioState = AUDIO_STATE_BEFORE_PRERECORD;
 	         }
 			 else if ((x > TOUCH_CHOOSE2_XMIN) && (x < TOUCH_CHOOSE2_XMAX) &&
 					 (y > TOUCH_CHOOSE2_YMIN) && (y < TOUCH_CHOOSE2_YMAX))
 	         {
 				 filtration = 0;
 				 AudioState = AUDIO_STATE_BEFORE_PRERECORD;
 	         }
 		 }
 		 else
 		 {
 			 AudioState = AUDIO_STATE_PRERECORD_CHOOSE;
 		 }
 	 }
   	  break;

  case AUDIO_STATE_BEFORE_PRERECORD:
		LCD_ClearZone();
	    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	    sprintf((char *)str, "Recording file: %s",
	            (char *)REC_WAVE_NAME);
	    BSP_LCD_DisplayStringAtLine(4, str);


	    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	    sprintf((char *)str,  "Sample rate : %d Hz", (int)DEFAULT_AUDIO_IN_FREQ);
	    BSP_LCD_DisplayStringAtLine(6, str);

	    sprintf((char *)str,  "Channels number : %d", (int)DEFAULT_AUDIO_IN_CHANNEL_NBR);
	    BSP_LCD_DisplayStringAtLine(7, str);

	    sprintf((char *)str,  "Volume : %d ", (int)uwVolume);
	    BSP_LCD_DisplayStringAtLine(8, str);

	    sprintf((char *)str, "File Size :");
	    BSP_LCD_DisplayStringAtLine(9, str);

	    BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [     ]", RIGHT_MODE);

	    AUDIO_REC_DisplayButtons();
	  AudioState = AUDIO_STATE_PRERECORD;
	break;

  case AUDIO_STATE_PRERECORD:
    if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
    {
      BSP_TS_GetState(&TS_State);
    }
    else
    {
      BSP_TS_GetState(&TS_State);
      x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > TOUCH_STOP_XMIN) && (x < TOUCH_STOP_XMAX) &&
            (y > TOUCH_STOP_YMIN) && (y < TOUCH_STOP_YMAX))
        {
          AudioState = AUDIO_STATE_STOP;
        }
        else if ((x > TOUCH_RECORD_XMIN) && (x < TOUCH_RECORD_XMAX) &&
                 (y > TOUCH_RECORD_YMIN) && (y < TOUCH_RECORD_YMAX))
        {
          display_update = 1;
          AudioState = AUDIO_STATE_RECORD;
        }
      }
      else
      {
        AudioState = AUDIO_STATE_PRERECORD;
      }
    }
    break;
    break;

  case AUDIO_STATE_RECORD:
    if (display_update)
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);    /* Display red record circle */
      BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                         (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                         (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
      BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [RECORD]", RIGHT_MODE);
      display_update = 0;
    }

    if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
    {
      BSP_TS_GetState(&TS_State);
    }
    else
    {
      BSP_TS_GetState(&TS_State);
      x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > TOUCH_STOP_XMIN) && (x < TOUCH_STOP_XMAX) &&
            (y > TOUCH_STOP_YMIN) && (y < TOUCH_STOP_YMAX))
        {
          AudioState = AUDIO_STATE_STOP;
        }
        else if ((x > TOUCH_PAUSE_XMIN) && (x < TOUCH_PAUSE_XMAX) &&
                 (y > TOUCH_PAUSE_YMIN) && (y < TOUCH_PAUSE_YMAX))
        {
          AudioState = AUDIO_STATE_PAUSE;
        }
      }
    }

    /* MAX Recording time reached, so stop audio interface and close file */
    if(BufferCtl.fptr >= REC_SAMPLE_LENGTH)
    {
      display_update = 1;
      AudioState = AUDIO_STATE_STOP;
      break;
    }

    /* Check if there are Data to write to USB Key */
    if(BufferCtl.wr_state == BUFFER_FULL)
    {
      /* write buffer in file */ //+ filtracja
    if(filtration == 1)
	{
    	arm_fir_f32(&S, (uint8_t*)(BufferCtl.pcm_buff + BufferCtl.offset), outputF32, BLOCK_SIZE);
    	if(f_write(&WavFile, outputF32,
               AUDIO_IN_BUFFER_SIZE,
               (void*)&byteswritten) != FR_OK)
   		{
       		BSP_LCD_SetTextColor(LCD_COLOR_RED);
           	BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
           	return AUDIO_ERROR_IO;
   		}
	}
	else
	{
    	if(f_write(&WavFile, (uint8_t*)(BufferCtl.pcm_buff + BufferCtl.offset),
                 AUDIO_IN_BUFFER_SIZE,
                 (void*)&byteswritten) != FR_OK)
    	{
    		BSP_LCD_SetTextColor(LCD_COLOR_RED);
        	BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
        	return AUDIO_ERROR_IO;
    	}
      }
      BufferCtl.fptr += byteswritten;
      BufferCtl.wr_state =  BUFFER_EMPTY;
    }

    /* Display elapsed time */
    elapsed_time = BufferCtl.fptr / (DEFAULT_AUDIO_IN_FREQ * DEFAULT_AUDIO_IN_CHANNEL_NBR * 2);
    if(prev_elapsed_time != elapsed_time)
    {
      prev_elapsed_time = elapsed_time;
      sprintf((char *)str, "[%02d:%02d]", (int)(elapsed_time /60), (int)(elapsed_time%60));
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
      BSP_LCD_DisplayStringAt(31, LINE(9), str, RIGHT_MODE);
      sprintf((char *)str, "%4d KB", (int)((int32_t)BufferCtl.fptr/1024));
      BSP_LCD_DisplayStringAt(83, LINE(9), str, LEFT_MODE);
    }
    break;

  case AUDIO_STATE_STOP:
    /* Stop recorder */
    BSP_AUDIO_IN_Stop(CODEC_PDWN_SW);
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);   /* Display blue cyan record circle */
    BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                       (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                       (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(TOUCH_STOP_XMIN, TOUCH_STOP_YMIN , /* Stop rectangle */
                     TOUCH_STOP_XMAX - TOUCH_STOP_XMIN,
                     TOUCH_STOP_YMAX - TOUCH_STOP_YMIN);
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    display_update = 1;
    HAL_Delay(150);
    if(f_lseek(&WavFile, 0) == FR_OK)
    {
      /* Update the wav file header save it into wav file */
      WavProcess_HeaderUpdate(Header_Buffer, &WaveFormat);
      if(f_write(&WavFile, Header_Buffer, sizeof(WAVE_FormatTypeDef), (void*)&byteswritten) == FR_OK)
      {
        audio_error = AUDIO_ERROR_EOF;
      }
      else
      {
        audio_error = AUDIO_ERROR_IO;
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
      }
    }
    else
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
      BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"RECORD FAIL");
      audio_error = AUDIO_ERROR_IO;
    }
    AudioState = AUDIO_STATE_IDLE;
    /* Close file */
    f_close(&WavFile);
    break;

  case AUDIO_STATE_PAUSE:
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    /* Displays red pause rectangles */
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);   /* Display blue cyan record circle */
    BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                       (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                       (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [PAUSE] ", RIGHT_MODE);
    BSP_AUDIO_IN_Pause();
    AudioState = AUDIO_STATE_WAIT;
    break;

  case AUDIO_STATE_RESUME:
    BSP_LCD_SetTextColor(LCD_COLOR_CYAN);    /* Displays blue cyan pause rectangles */
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);    /* Display red record circle */
    BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2,
                       (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                       (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_DisplayStringAt(15, LINE(6), (uint8_t *)"  [RECORD]", RIGHT_MODE);
    BSP_AUDIO_IN_Resume();
    AudioState = AUDIO_STATE_RECORD;
    break;

  case AUDIO_STATE_NEXT:
  case AUDIO_STATE_PREVIOUS:
    AudioState = AUDIO_STATE_RECORD;
    break;

  case AUDIO_STATE_WAIT:
    if(TS_State.touchDetected == 1)   /* If previous touch has not been released, we don't proceed any touch command */
    {
      BSP_TS_GetState(&TS_State);
    }
    else
    {
      BSP_TS_GetState(&TS_State);
      x = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > TOUCH_RECORD_XMIN) && (x < TOUCH_RECORD_XMAX) &&
            (y > TOUCH_RECORD_YMIN) && (y < TOUCH_RECORD_YMAX))
        {
          AudioState = AUDIO_STATE_RESUME;
        }
        else if ((x > TOUCH_PAUSE_XMIN) && (x < TOUCH_PAUSE_XMAX) &&
                 (y > TOUCH_PAUSE_YMIN) && (y < TOUCH_PAUSE_YMAX))
        {
          AudioState = AUDIO_STATE_RESUME;
        }
      }
    }
    break;

  case AUDIO_STATE_IDLE:
  case AUDIO_STATE_INIT:
  default:
    /* Do Nothing */
    break;
  }
  return audio_error;
}

void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  BufferCtl.pcm_ptr+= AUDIO_IN_BUFFER_SIZE/2;
  if(BufferCtl.pcm_ptr == AUDIO_IN_BUFFER_SIZE/2)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = 0;
  }

  if(BufferCtl.pcm_ptr >= AUDIO_IN_BUFFER_SIZE)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = AUDIO_IN_BUFFER_SIZE/2;
    BufferCtl.pcm_ptr = 0;
  }
}

void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  BufferCtl.pcm_ptr+= AUDIO_IN_BUFFER_SIZE/2;
  if(BufferCtl.pcm_ptr == AUDIO_IN_BUFFER_SIZE/2)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = 0;
  }

  if(BufferCtl.pcm_ptr >= AUDIO_IN_BUFFER_SIZE)
  {
    BufferCtl.wr_state   =  BUFFER_FULL;
    BufferCtl.offset  = AUDIO_IN_BUFFER_SIZE/2;
    BufferCtl.pcm_ptr = 0;
  }
}

static uint32_t WavProcess_EncInit(uint32_t Freq, uint8_t *pHeader)
{
  /* Initialize the encoder structure */
  WaveFormat.SampleRate = Freq;        /* Audio sampling frequency */
  WaveFormat.NbrChannels = 2;          /* Number of channels: 1:Mono or 2:Stereo */
  WaveFormat.BitPerSample = 16;        /* Number of bits per sample (16, 24 or 32) */
  WaveFormat.FileSize = 0x001D4C00;    /* Total length of useful audio data (payload) */
  WaveFormat.SubChunk1Size = 44;       /* The file header chunk size */
  WaveFormat.ByteRate = (WaveFormat.SampleRate * \
                        (WaveFormat.BitPerSample/8) * \
                         WaveFormat.NbrChannels);     /* Number of bytes per second  (sample rate * block align)  */
  WaveFormat.BlockAlign = WaveFormat.NbrChannels * \
                         (WaveFormat.BitPerSample/8); /* channels * bits/sample / 8 */

  /* Parse the wav file header and extract required information */
  if(WavProcess_HeaderInit(pHeader, &WaveFormat))
  {
    return 1;
  }
  return 0;
}

static uint32_t WavProcess_HeaderInit(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* Write chunkID, must be 'RIFF'  ------------------------------------------*/
  pHeader[0] = 'R';
  pHeader[1] = 'I';
  pHeader[2] = 'F';
  pHeader[3] = 'F';

  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = 0x00;
  pHeader[5] = 0x4C;
  pHeader[6] = 0x1D;
  pHeader[7] = 0x00;
  /* Write the file format, must be 'WAVE' -----------------------------------*/
  pHeader[8]  = 'W';
  pHeader[9]  = 'A';
  pHeader[10] = 'V';
  pHeader[11] = 'E';

  /* Write the format chunk, must be'fmt ' -----------------------------------*/
  pHeader[12]  = 'f';
  pHeader[13]  = 'm';
  pHeader[14]  = 't';
  pHeader[15]  = ' ';

  /* Write the length of the 'fmt' data, must be 0x10 ------------------------*/
  pHeader[16]  = 0x10;
  pHeader[17]  = 0x00;
  pHeader[18]  = 0x00;
  pHeader[19]  = 0x00;

  /* Write the audio format, must be 0x01 (PCM) ------------------------------*/
  pHeader[20]  = 0x01;
  pHeader[21]  = 0x00;

  /* Write the number of channels, ie. 0x01 (Mono) ---------------------------*/
  pHeader[22]  = pWaveFormatStruct->NbrChannels;
  pHeader[23]  = 0x00;

  /* Write the Sample Rate in Hz ---------------------------------------------*/
  /* Write Little Endian ie. 8000 = 0x00001F40 => byte[24]=0x40, byte[27]=0x00*/
  pHeader[24]  = (uint8_t)((pWaveFormatStruct->SampleRate & 0xFF));
  pHeader[25]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 8) & 0xFF);
  pHeader[26]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 16) & 0xFF);
  pHeader[27]  = (uint8_t)((pWaveFormatStruct->SampleRate >> 24) & 0xFF);

  /* Write the Byte Rate -----------------------------------------------------*/
  pHeader[28]  = (uint8_t)((pWaveFormatStruct->ByteRate & 0xFF));
  pHeader[29]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 8) & 0xFF);
  pHeader[30]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 16) & 0xFF);
  pHeader[31]  = (uint8_t)((pWaveFormatStruct->ByteRate >> 24) & 0xFF);

  /* Write the block alignment -----------------------------------------------*/
  pHeader[32]  = pWaveFormatStruct->BlockAlign;
  pHeader[33]  = 0x00;

  /* Write the number of bits per sample -------------------------------------*/
  pHeader[34]  = pWaveFormatStruct->BitPerSample;
  pHeader[35]  = 0x00;

  /* Write the Data chunk, must be 'data' ------------------------------------*/
  pHeader[36]  = 'd';
  pHeader[37]  = 'a';
  pHeader[38]  = 't';
  pHeader[39]  = 'a';

  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  pHeader[40]  = 0x00;
  pHeader[41]  = 0x4C;
  pHeader[42]  = 0x1D;
  pHeader[43]  = 0x00;

  /* Return 0 if all operations are OK */
  return 0;
}

static uint32_t WavProcess_HeaderUpdate(uint8_t* pHeader, WAVE_FormatTypeDef* pWaveFormatStruct)
{
  /* Write the file length ---------------------------------------------------*/
  /* The sampling time: this value will be written back at the end of the
     recording operation.  Example: 661500 Btyes = 0x000A17FC, byte[7]=0x00, byte[4]=0xFC */
  pHeader[4] = (uint8_t)(BufferCtl.fptr);
  pHeader[5] = (uint8_t)(BufferCtl.fptr >> 8);
  pHeader[6] = (uint8_t)(BufferCtl.fptr >> 16);
  pHeader[7] = (uint8_t)(BufferCtl.fptr >> 24);
  /* Write the number of sample data -----------------------------------------*/
  /* This variable will be written back at the end of the recording operation */
  BufferCtl.fptr -=44;
  pHeader[40] = (uint8_t)(BufferCtl.fptr);
  pHeader[41] = (uint8_t)(BufferCtl.fptr >> 8);
  pHeader[42] = (uint8_t)(BufferCtl.fptr >> 16);
  pHeader[43] = (uint8_t)(BufferCtl.fptr >> 24);

  /* Return 0 if all operations are OK */
  return 0;
}

static void AUDIO_REC_DisplayButtons(void)
{
  BSP_LCD_SetFont(&LCD_LOG_HEADER_FONT);
  BSP_LCD_ClearStringLine(12);            /* Clear dedicated zone */
  BSP_LCD_ClearStringLine(13);
  BSP_LCD_ClearStringLine(14);
  BSP_LCD_ClearStringLine(15);

  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
  BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2, /* Record circle */
                     (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                     (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
  BSP_LCD_FillRect(TOUCH_PAUSE_XMIN, TOUCH_PAUSE_YMIN , 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);    /* Pause rectangles */
  BSP_LCD_FillRect(TOUCH_PAUSE_XMIN + 20, TOUCH_PAUSE_YMIN, 15, TOUCH_PAUSE_YMAX - TOUCH_PAUSE_YMIN);
  BSP_LCD_FillRect(TOUCH_STOP_XMIN, TOUCH_STOP_YMIN , /* Stop rectangle */
                   TOUCH_STOP_XMAX - TOUCH_STOP_XMIN,
                   TOUCH_STOP_YMAX - TOUCH_STOP_YMIN);
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
  BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"Use record button to start record,");
  BSP_LCD_DisplayStringAtLine(15, (uint8_t *)"stop to exit");
  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
}

static void DisplayChoose(void)
{
	  LCD_ClearZone();
	  BSP_LCD_SetFont(&LCD_LOG_HEADER_FONT);
	  BSP_LCD_ClearStringLine(12);            /* Clear dedicated zone */
	  BSP_LCD_ClearStringLine(13);
	  BSP_LCD_ClearStringLine(14);
	  BSP_LCD_ClearStringLine(15);

	  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
	  BSP_LCD_FillRect(TOUCH_CHOOSE1_XMIN, TOUCH_CHOOSE1_YMIN , /* Stop rectangle */
			  TOUCH_CHOOSE1_XMAX - TOUCH_CHOOSE1_XMIN,
			  TOUCH_CHOOSE1_YMAX - TOUCH_CHOOSE1_YMIN);

	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_FillRect(TOUCH_CHOOSE2_XMIN, TOUCH_CHOOSE2_YMIN , /* Stop rectangle */
			  TOUCH_CHOOSE2_XMAX - TOUCH_CHOOSE2_XMIN,
			  TOUCH_CHOOSE2_YMAX - TOUCH_CHOOSE2_YMIN);

	  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	  BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
	  BSP_LCD_DisplayStringAt(0,80,(uint8_t *)"FILTER", LEFT_MODE);
	  BSP_LCD_DisplayStringAt(0,80,(uint8_t *)"NO FILTER", RIGHT_MODE);
	  BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
}


static void LCD_ClearZone(void)
{
  uint8_t i = 0;

  for(i= 0; i < 13; i++)
  {
    BSP_LCD_ClearStringLine(i + 3);
  }
}



