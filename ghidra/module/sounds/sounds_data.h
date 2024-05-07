#ifndef SOUNDS_DATA_H
#define SOUNDS_DATA_H

#include <sys/types.h>
#include <csl.h>
#include <modsesq2.h>
#include <modhsyn.h>

#define FUNCTION_CALL 0x2000004
#define BASE_priority 29
#define TERM_priority 28
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 12288
#define STOP_RET 0x903e
#define TIME 4167
#define SESQ_COUNT 96
#define PORTS_COUNT 8

typedef struct _Timer_Handler_Data_
{
    uint TermThid;
    int Timid;
    uint End;
    uint pad;
} TimerHandlerData; 

typedef struct _Port_Data_{
    void *hd_addr;
    void *sq_addr;
    uint vol;
    uint pad;
}PortData;

typedef struct _Sesq_{
    char play;
    char port;
    char sesq;
    char loop;
    uint cpu_id;
    int iop_id;
}Sesq;

#define SOUND_ACTION_STOP 1
#define SOUND_ACTION_PLAY 0
#define SOUND_ACTION_STOP_ID 2

typedef struct _Sound_Data_{
    char action;
    char loop;
    short cpu_id;
    char port_bit;
    char sesq;
    char vol;
    char pan;
    int pitch;
}SoundData;

typedef struct _Sound_Data_List_{
    int count;
    SoundData items[32];
}SoundDataList;

uint ret = 0;
char buffer[BUFFER_SIZE];
sceCslCtx sesqCtx;
sceCslCtx synthCtx;
sceCslBuffGrp sesqBuffGrp[2];
sceCslBuffGrp synthGrp;
sceCslBuffCtx sesqInBuffCtx[PORTS_COUNT * 2];
sceCslBuffCtx sesqOutBuffCtx[PORTS_COUNT];
sceSESq2Env sesqSESqEnv[PORTS_COUNT];
sceCslBuffCtx sInBfCtx[PORTS_COUNT * 2];
char synthEnv[PORTS_COUNT][1368];
char streamBf[PORTS_COUNT][2056];
PortData ports[PORTS_COUNT];
TimerHandlerData timerData;
sceHSyn_VoiceStat pVstat;
sceCslIdMonitor idCtx;
Sesq sesqList[SESQ_COUNT];
uint termthid;

#endif
