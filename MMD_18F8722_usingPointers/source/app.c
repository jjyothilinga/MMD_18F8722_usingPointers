//#include "communication.h"
#include "mmd.h"
#include "string.h"
#include "app.h"
//#include "uart.h"
#include "typedefs.h"
#include "eep.h"


typedef struct _APP
{
	UINT8 model[MSG_LENGTH];
	UINT8 eepUpdate;

}APP;

#pragma idata APP_DATA
APP app = {{0},0};
MMD_Config mmdConfig = {0};
#pragma idata


void APP_init(void)
{
	UINT8 i = 0;

	do
	{
			app.model[i] = Read_b_eep(EEPROM_ADDRESS+i);
			Busy_eep();	
	}while(app.model[i++]);

	MMD_clearSegment(0);
	mmdConfig.startAddress = 0;
	mmdConfig.length = MMD_MAX_CHARS;
	mmdConfig.symbolCount = strlen(app.model);
	mmdConfig.symbolBuffer = app.model;
	mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
	MMD_configSegment( 0 , &mmdConfig);

}

void APP_task(void)
{

UINT8 i;

	if(app.eepUpdate == TRUE)
	{
		app.eepUpdate = FALSE;
		for( i = 0; i <= (strlen(app.model)); i++ )
			{
				Write_b_eep( EEPROM_ADDRESS+i , app.model[i] );
				Busy_eep( );
			}

		
	}	
}

UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{

	UINT8 i;

	UINT8 rxCode = rxPacket[0];
	UINT8 length = 0;

	switch( rxCode )
	{
		case CMD_SET_MODEL:
			app.eepUpdate = TRUE;
			strcpy(app.model,&rxPacket[1]);
			MMD_clearSegment(0);
			mmdConfig.startAddress = 0;
			mmdConfig.length = MMD_MAX_CHARS;
			mmdConfig.symbolCount = strlen(app.model);
			mmdConfig.symbolBuffer = app.model;
			mmdConfig.scrollSpeed = 0;//SCROLL_SPEED_LOW;
			MMD_configSegment( 0 , &mmdConfig);

			*txCode = CMD_SET_MODEL;
			break;

		default:
		break;

	}

	return length;

}
