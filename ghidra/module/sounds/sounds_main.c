#include "sounds_data.h"
#include <kernel.h>
#include <sif.h>
#include <sifrpc.h>
#include <thread.h>
#include <sys/types.h>

ModuleInfo Module = {"Sesq_Module", 0x0101 };

extern void SesqInit(void*); // sounds_cmd.c
extern void SesqTerm(void*); // sounds_cmd.c
extern void SesqReset(void*); // sounds_cmd.c
extern void SesqSet(uint*); // sounds_cmd.c
extern void SesqFree(uint*); // sounds_cmd.c
extern void SesqUpdate(void*); // sounds_cmd.c
extern void SesqPlay(SoundData*); // sounds_cmd.c
extern void SesqStop(SoundData*); // sounds_cmd.c
extern void SesqStopId(SoundData*); // sounds_cmd.c
extern uint SesqCheck(uint*); // sounds_cmd.c
extern uint SesqCheckId(uint*); // sounds_cmd.c
extern void SesqMute(uint*); // sounds_cmd.c
extern int SesqOutput(uint*); // sounds_cmd.c
extern void SesqStopAll(uint*); // sounds_cmd.c
extern void SesqSetVolume(uint*); // sounds_cmd.c
extern uint SesqCheckVoicesStat(uint*); // sounds_cmd.c
extern void SesqDsidbPrint(void*); // sounds_cmd.c
 // sounds_cmd.c

int start (int argc, char *argv []){
    if (!sceSifCheckInit())
        sceSifInit();
    sceSifInitRpc(0);
    struct ThreadParam thread;
    thread.attr = TH_C;
    thread.entry = RegisterHandler;
    thread.initPriority = BASE_priority;
    thread.stackSize = 0x1000;
    thread.option = 0;
    int thid = CreateThread(&thread);
    if (thid > 0){
        StartThread(thid, 0);
        return FALSE;
    }else {
        return TRUE;
    }
}

uint* Handler(int fno, void* data, int size){
    switch (fno)
    {
    case 0:
        SesqInit(data);
        break;
    case 1:
        SesqTerm(data);
        break;
    case 2:
        SesqReset(data);
        break;
    case 3:
        SesqSet((uint*)data);
        break;
    case 4:
        SesqFree((uint*)data);
        break;
    case 5:
        SesqOutput((uint*)data);
        break;
    case 6:
        SesqMute((uint*)data);
        break;
    case 7:
        SesqSetVolume((uint*)data);
        break;
    case 8:
        ret = SesqCheckVoicesStat((uint*)data);
        break;
    case 9:
        SesqUpdate(data);
        break;
    case 10:
        SesqStopAll((uint*)data);
        break;
    case 11:
        ret = SesqCheck((uint*)data);
        break;
    case 12:
        ret = SesqCheckId((uint*)data);
        break;
    case 13:
        SoundData *sound = ((SoundDataList*)(data))->items;
        for (int counter = 0; counter < ((SoundDataList*)(data))->count; counter++)
        {
            if (sound->action == SOUND_ACTION_PLAY)
                SesqPlay(sound);
            else if (sound->action == SOUND_ACTION_STOP)
                SesqStop(sound);
            else if (sound->action == SOUND_ACTION_STOP_ID)
                SesqStopId(sound);
            sound++;
        }
        break;
    case 14:
        SesqDsidbPrint(data);
        break;
    case 15:
        SesqLoopCompare((ushort *)data);
        break;
    default:
        Kprintf("[SESQ ASSERT]  : ILLIGAL COMMAND : %08x\n",fno);
    }
    return &ret;
}

int RegisterHandler(void){
    sceSifInitRpc(0);
    int thid = GetThreadId();
    sceSifQueueData queue;
    sceSifSetRpcQueue(&queue, thid);
    sceSifServeData server;
    sceSifRegisterRpc(&server, FUNCTION_CALL, Handler, (void*)&buffer[0], 0, 0, &queue);
    sceSifRpcLoop(&queue);
    return 0;
}

uint GetPortNum(uint port_bit){
    int num = 0;
    while (TRUE){
        if (num > 7) return 0;
        if ((1 << (num) & port_bit) != 0) break;
        num++;
    }
    return num;
}