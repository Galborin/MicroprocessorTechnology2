/*
 * menu_.c
 *
 *  Created on: 11.01.2019
 *      Author: piotr
 */

#include <record.h>

// Private define
#define TOUCH_RECORD_XMIN       100
#define TOUCH_RECORD_XMAX       130
#define TOUCH_RECORD_YMIN       200
#define TOUCH_RECORD_YMAX       230

/* Selected State Structure */
typedef enum {
  AUDIO_IDLE = 0,
  AUDIO_WAIT,
  AUDIO_EXPLORE,
  AUDIO_PLAYBACK,
  AUDIO_IN,
}SelectedState;

/* Selected State Machine Structure */
typedef struct SelectedStateMachine {
  __IO SelectedState state;
  __IO uint8_t select;
}SelectedStateMachine;

//Public variables
TS_StateTypeDef  TS_State = {0};

// Private variables
static SelectedStateMachine  AppSelectedState;

// Exported functions
void LCD_ClearTextZone(void);

/*
 * @brief Menu display and state machine
 */
void AUDIO_MenuProcess(void)
{
  AUDIO_ErrorTypeDef  status;
  TS_StateTypeDef  TS_State;
  uint8_t x,y;
  
  if(appli_state == APPLICATION_READY)
  { 
    switch(AppSelectedState.state)
    {
    case AUDIO_IDLE:
/*AUDIO_IDLE*/
      AppSelectedState.state = AUDIO_WAIT;
      
      //Start_Display();
      BSP_LCD_SetFont(&LCD_LOG_HEADER_FONT);
      LCD_ClearTextZone();
      BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
      BSP_LCD_FillCircle((TOUCH_RECORD_XMAX+TOUCH_RECORD_XMIN)/2, /* Record circle */
                         (TOUCH_RECORD_YMAX+TOUCH_RECORD_YMIN)/2,
                         (TOUCH_RECORD_XMAX-TOUCH_RECORD_XMIN)/2);
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
      BSP_LCD_SetFont(&LCD_LOG_TEXT_FONT);
      BSP_LCD_DisplayStringAtLine(14, (uint8_t *)"Use touch screen to enter");
      break;    
/*AUDIO_IDLE*/
    case AUDIO_WAIT:
/*AUDIO_WAIT*/
      BSP_TS_GetState(&TS_State);
      x=TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
      y=TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);
      if(TS_State.touchDetected == 1)
      {
        if ((x > (TOUCH_RECORD_XMIN-10)) && (x < (TOUCH_RECORD_XMAX+10)) &&
            (y > (TOUCH_RECORD_YMIN-10)) && (y < (TOUCH_RECORD_YMAX+10)))
        {
          AppSelectedState.state = AUDIO_IN;
        }
        else
        {
          AppSelectedState.state = AUDIO_WAIT;
        }
        
        /* Wait for touch released */
        do
        {
          BSP_TS_GetState(&TS_State);
        }while(TS_State.touchDetected > 0);
      }
      break;
/*AUDIO_WAIT*/
    case AUDIO_EXPLORE:
/*AUDIO_EXPLORE*/
      break;
/*AUDIO_EXPLORE*/
    case AUDIO_PLAYBACK:
/*AUDIO_PLAYBACK*/

      break; 
/*AUDIO_PLAYBACK*/
    case AUDIO_IN:
/*AUDIO_IN*/
      if(appli_state == APPLICATION_READY)
      {

        if(AudioState == AUDIO_STATE_IDLE)
        {
          /* Start Playing */
          AudioState = AUDIO_STATE_INIT;
          
          /* Clear the LCD */
          LCD_ClearTextZone();

          /* Configure the audio recorder: sampling frequency, bits-depth, number of channels */
          if(AUDIO_REC_Start() == AUDIO_ERROR_IO)
          {
            AppSelectedState.state = AUDIO_IDLE;
          }
        }
        else /* Not idle */
        {
          status = AUDIO_REC_Process();
          if((status == AUDIO_ERROR_IO) || (status == AUDIO_ERROR_EOF))
          {
            AppSelectedState.state = AUDIO_IDLE;
          }
        }
      }
      else
      {
        AppSelectedState.state = AUDIO_WAIT;
      }
      break;
/*AUDIO_IN*/
    default:
      break;
    }//end switch
  }//end if(APPLICATION_READY)
  
  if(appli_state == APPLICATION_DISCONNECT)
  {
	  LCD_ClearTextZone();
	  AppSelectedState.state = AUDIO_IDLE;
  }
}

/*
 * @brief Clears lines from 3 to 15
 */
void LCD_ClearTextZone(void)
{
  uint8_t i = 0;
  
  for(i= 0; i < 13; i++)
  {
    BSP_LCD_ClearStringLine(i + 3);
  }
}

/**************************************end*/
