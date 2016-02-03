#include "config.h"
#include "communication.h"
#include "mmd.h"
#include "string.h"

enum
{
	CMD_SET_MODEL = 0X80
};

UINT8 APP_comCallBack( far UINT8 *rxPacket,  far UINT8* txCode, far UINT8** txPacket);
void APP_init(void);
void APP_task(void);

