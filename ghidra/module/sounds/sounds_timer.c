#include "sounds_data.h"
#include <thread.h>
#include <modhsyn.h>
#include <modsesq2.h>
#include <timerman.h>

void Term(){
    do
    {
        SleepThread();
        sceHSyn_ATick(&synthCtx);
        sceSESq2ATick(&sesqCtx);
    } while (TRUE);
}

int CreateTermThread(){
    struct ThreadParam thread;
    thread.attr = TH_C;
    thread.entry = Term;
    thread.initPriority = TERM_priority;
    thread.stackSize = 0x1000;
    thread.option = 0;
    return CreateThread(&thread);
}

uint TimerHandler(TimerHandlerData *data){
    iWakeupThread(data->TermThid);
    return data->End;
}

void SetTimer(TimerHandlerData *data){
    struct SysClock clock;
    USec2SysClock(TIME, &clock);
    data->End = clock.low;
    int timid = AllocHardTimer(1, 32, 1);
    data->Timid = timid;
    if (1 > timid){
        Kprintf("[SESQ ASSERT]  : AllocHardTimer()  faild. [ret=%d]\n",timid);
    }
    timid = SetTimerHandler(timid, data->End, TimerHandler, data);
    if (timid != 0){
        Kprintf("[SESQ ASSERT]  : SetTimerHandler() faild. [ret=%d]\n",timid);
    }
    timid = SetupHardTimer(timid, 1, 0, 1);
    if (timid != 0){
        Kprintf("[SESQ ASSERT]  : SetupHardTimer()  faild. [ret=%d]\n",timid);
    }
    timid = StartHardTimer(timid);
    if (timid != 0){
        Kprintf("[SESQ ASSERT]  : StartHardTimer()  faild. [ret=%d]\n",timid);
    }
}

void StopTimer(TimerHandlerData *data){
    ret = StopHardTimer(data->Timid);
    if (ret != 0 && ret != -0x9c){
        Kprintf("[SESQ ASSERT]  : StopHardTimer() faild. [ret=%d]\n",ret);
    }
    ret = FreeHardTimer(data->Timid);
    if (ret != 0){
        Kprintf("[SESQ ASSERT]  : FreeHardTimer() faild. [ret=%d]\n",ret);
    }
}
