#include "sounds_data.h"
#include <kernel.h>
#include <memory.h>
#include <cslse.h>

extern uint GetPortNum(uint); // sounds_main.c
extern int CreateTermThread(void); // sounds_timer.c
extern void StopTimer(TimerHandlerData*); // sounds_timer.c
extern uint SESq2InitErrCheck(int); // sounds_sesq2_err.c
extern uint SESq2LoadErrCheck(int); // sounds_sesq2_err.c
extern uint SESq2PlayErrCheck(int); // sounds_sesq_err.c
extern uint SESq2StopErrCheck(int); // sounds_sesq_err.c

void SesqInit(void *data){
    Kprintf("[SESQ] : SesqInit()       : \n");
    memset(sesqList, 0, sizeof(Sesq) * SESQ_COUNT);
    termthid = CreateTermThread();
    sesqCtx.buffGrpNum = 2;
    sesqCtx.buffGrp = sesqBuffGrp;
    sesqCtx.conf = NULL;
    sesqCtx.extmod = NULL;
    sesqCtx.callBack = NULL;
    sesqBuffGrp[0].buffNum = PORTS_COUNT * 2;
    sesqBuffGrp[0].buffCtx = sesqInBuffCtx;
    timerData.TermThid = termthid;
    for (int port = 0; port < PORTS_COUNT; port += 1) {
        sesqInBuffCtx[port * 2].sema = 0;
        sesqInBuffCtx[port * 2].buff = NULL;
        sesqInBuffCtx[port * 2 + 1].sema = 0;
        sesqInBuffCtx[port * 2 + 1].buff = sesqSESqEnv + port;
    }
    sesqBuffGrp[1].buffNum = PORTS_COUNT;
    sesqBuffGrp[1].buffCtx = sesqOutBuffCtx;
    for (int port = 0; port < PORTS_COUNT; port += 1) {
        sesqOutBuffCtx[port].sema = 0;
        sesqOutBuffCtx[port].buff = streamBf + port;
    }
    synthCtx.buffGrpNum = 1;
    synthCtx.buffGrp = &synthGrp;
    synthCtx.conf = NULL;
    synthCtx.extmod = NULL;
    synthCtx.callBack = NULL;
    synthGrp.buffNum = PORTS_COUNT * 2;
    synthGrp.buffCtx = sInBfCtx;
    for (int port = 0; port < PORTS_COUNT; port += 1) {
        sInBfCtx[port * 2].sema = 0;
        sInBfCtx[port * 2].buff = streamBf + port;
        sInBfCtx[port * 2 + 1].sema = 0;
        sInBfCtx[port * 2 + 1].buff = synthEnv + port;
    }
    for (int port = 0; port < PORTS_COUNT; port += 1) {
        ((sceCslSeStream *)&(streamBf[port]))->buffsize  = 0x808;
        ((sceCslSeStream *)&(streamBf[port]))->validsize  = 0x808;
    }
    int reserv[2];
    reserv[0] = 0;
    reserv[1] = 0x100;
    sceHSyn_SetReservVoice(&reserv);
    int ret = sceSESq2Init(&sesqCtx,TIME);
    SESq2InitErrCheck(ret);
    ret = sceHSyn_Init(&synthCtx,TIME);
    if (ret != 0)
        Kprintf("[SESQ ASSERT]  : sceHSyn_Init() faild. [ret=%d]\n",ret);
    sceHSyn_SetVoiceStatBuffer(&pVstat);
    sceHSyn_SetIdMonitor(&idCtx);
    sceSESq2SetIdMonitor(&idCtx);
    StartThread(termthid,NULL);
    SetTimer(&timerData);
}

void SesqTerm(void *data){
    Kprintf("[SESQ] : SesqTerm()       : \n");
    StopTimer(&timerData);
}

void SesqReset(void *data){
    Kprintf("[SESQ] : SesqReset()      : \n");
    for (int port = 0; port < PORTS_COUNT; port++){
        sceHSyn_SEAllSoundOff(&synthCtx, port);
        sceHSyn_SetVolume(&synthCtx, port, (ushort)ports[port].vol);
    }
    memset(sesqList, 0, sizeof(Sesq) * SESQ_COUNT);
}

void SesqSet(uint *data){
    Kprintf("[SESQ] : SesqSet()        : [port_num=%d spu_addr=%x hd_addr=%x sq_addr=%d]\n", GetPortNum(data[0]),
            data[1], data[2], data[3]);
    uint port_num = GetPortNum(data[0]);
    ports[port_num].hd_addr = data[2];
    ports[port_num].sq_addr = data[3];
    ports[port_num].vol = 127;
    sesqInBuffCtx[port_num * 2].buff = ports[port_num].sq_addr;
    int ret = sceSESq2Load(&sesqCtx, port_num);
    SESq2LoadErrCheck(ret);
    *(uint *)(sesqSESqEnv[port_num].system + 0xc) = port_num;
    sesqSESqEnv[port_num].system[17] = 0xff;
    if (port_num == 2 || ((port_num > 1 && port_num < 6) && port_num > 3)){
        sesqSESqEnv[port_num].system[4] = 0xff;
    }else{
        sesqSESqEnv[port_num].system[4] = 0x7f;
    }
    sesqSESqEnv[port_num].system[5] = 0x40;
    *(ushort*)(sesqSESqEnv[port_num].system + 6) = 1000;
    ret = sceHSyn_Load(&synthCtx, port_num, data[1], ports[port_num].hd_addr, 0);
    if (ret)
        Kprintf("[SESQ ASSERT]  : sceHSyn_Load() faild. [ret=%d]\n",ret);
    synthEnv[port_num][0] = 0;
    synthEnv[port_num][2] = 0;
    synthEnv[port_num][3] = 0;
    *(uint*)(synthEnv[port_num] + 4) = 0;
    *(uint*)(synthEnv[port_num] + 8) = 0;
    *(uint*)(synthEnv[port_num] + 0xc) = 0;
    *(uint*)(synthEnv[port_num] + 0x10) = 0;
}

void SesqFree(uint *data){
    Kprintf("[SESQ] : SesqFree()       : [port_num=%d]\n", GetPortNum(data[0]));
    uint port_num = GetPortNum(data[0]);
    ports[port_num].hd_addr = NULL;
    ports[port_num].sq_addr = NULL;
    ports[port_num].vol = 127;
    sesqInBuffCtx[port_num * 2].buff = NULL;
}

void SesqUpdate(void *data){
    Sesq *sesq = sesqList;
    for (int sesq_i; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play && sceSESq2IsDataEnd(&sesqCtx, (uint)sesq->port, (int)sesq->iop_id)){
            Kprintf("[SESQ] : SesqUpdate()     : se end. [port_num=%d, sesq=%3d, cpu_id=%8d iop_id=%8x]\n",
                    sesq->port, sesq->sesq, sesq->cpu_id, sesq->iop_id);
            sceSESq2Stop(&sesqCtx,sesq->iop_id);
            sesq->play = FALSE;
        }
        sesq++;
    }
    uint condition = FALSE;
    sesq = sesqList;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play && sesq->port == 2){
            condition = TRUE;
            break;
        }
        sesq++;
    }
    if (condition){
        for (int core = 0; core < sceHSyn_NumCore; core++){
            for (int voice = 0; voice < sceHSyn_NumVoice; voice++){
                if (pVstat.voice_state[core][voice] & sceHSyn_VoiceStat_KeyOn &&
                    (pVstat.voice_state[core][voice] & 0xfU) == 2){
                    Kprintf("[SESQ ASSERT]  : SesqUpdate() : unknown battle se. will STOP!\n");
                    sceHSyn_SEAllSoundOff(&synthCtx,2);
                    break;
                }
            }
        }
    }
}

void SesqPlay(SoundData *data){
    Kprintf("[SESQ] : SesqPlay()       : [port_num=%d sesq=%3d, loop=%d, vol=%4d, pan=%3d, pitch=%4d cpu_id=%8d]\n",
            GetPortNum(data->port_bit), data->sesq, data->loop, data->vol, data->pan, data->pitch, data->cpu_id);
    uint port_num = GetPortNum(data->port_bit);
    int ret = sceSESq2Play(&sesqCtx, port_num, 0, data->sesq, data->vol, data->pan, data->pitch);
    if (SESq2PlayErrCheck(ret)){
        Sesq *resultSesq = NULL;
        Sesq *sesq = sesqList;
        for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
            if (!sesq->play){
                resultSesq = sesq;
                sesq->play = TRUE;
                sesq->port = (char)port_num;
                sesq->loop = data->loop;
                sesq->sesq = data->sesq;
                sesq->cpu_id = data->cpu_id;
                sesq->iop_id = ret;
                break;
            }
            sesq++;
        }
        if (resultSesq == NULL){
            Kprintf("[SESQ WARNING] : SesqPlay() : sesq id overflow\n");
        }
    }
}

void SesqStop(SoundData *data){
    Kprintf("[SESQ] : SesqStop()       : [port_num=%d sesq=%3d]\n", GetPortNum(data->port_bit), data->sesq);
    uint port_num = GetPortNum(data->port_bit);
    Sesq *resultsesq = NULL;
    Sesq *sesq = sesqList;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play && sesq->port == port_num && sesq->sesq == data->sesq){
            resultsesq = sesq;
            int ret = sceSESq2Stop(&sesqCtx, sesq->iop_id);
            SESq2StopErrCheck(ret);
        }
        sesq++;
    }
    if (resultsesq == NULL){
        Kprintf("[SESQ] : SesqStop()       : not play port_num=%d sesq=%3d]\n", GetPortNum(data->port_bit), 
                sesq->sesq);
    }
}

void SesqStopId(SoundData *data){
    Kprintf("[SESQ] : SesqStopId()     : [cpu_id=%8d]\n",data->cpu_id);
    Sesq *resultSesq = NULL;
    Sesq *sesq = sesqList;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play && sesq->cpu_id == data->cpu_id){
            resultSesq = sesq;
            break;
        }
        sesq++;
    }
    if (resultSesq == NULL){
        Kprintf("[SESQ WARNING] : SesqStopId() : not play [id=%d]\n", sesq->cpu_id);
    }else{
        int ret = sceSESq2Stop(&sesqCtx, sesq->iop_id);
        SESq2StopErrCheck(ret);
    }
}

uint SesqCheck(uint *data){
    uint port_num = GetPortNum(data[0]);
    Sesq *sesq = sesqList;
    int ret = FALSE;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play && sesq->port == port_num && sesq->sesq == data[1]){
            ret = TRUE;
            break;
        }
        sesq++;
    }
    Kprintf("[SESQ] : SesqCheck()      : [port_num=%d sesq=%3d ret=%d]\n", GetPortNum(data[0]), data[1], ret);
    return ret;
}

uint SesqCheckId(uint *data){
    uint ret = FALSE;
    Sesq *sesq = sesqList;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++)
    {
        if (sesq->play && sesq->cpu_id == data[0]){
            ret = TRUE;
            break;
        }
        sesq++;
    }
    Kprintf("[SESQ] : SesqCheckId()    : [cpu_id=%8d ret=%d]\n",data[0],ret);
}

void SesqMute(uint *data){
    Kprintf("[SESQ] : SesqMute()       : [port_bit=%x flg=%d]\n",data[0],data[1]);
    for (int port = 0; port < PORTS_COUNT; port++){
        if ((1 << port) & data[0]){
            if (data[1])
                sceHSyn_SetVolume(&synthCtx, port, (ushort)ports[port].vol);
            else
                sceHSyn_SetVolume(&synthCtx, port, 0);
        }
    }
}

int SesqOutput(uint *data){
    Kprintf("[SESQ] : SesqOutput()     : [mono=%d]\n",data[0]);
    int ret;
    if (!data[0])
        ret = sceHSyn_SetOutputMode(1);
    else
        ret = sceHSyn_SetOutputMode(0);
    return ret;
}

void SesqStopAll(uint *data){
    Kprintf("[SESQ] : SesqStopAll()    : [port_bit=%x]\n",data[0]);
    for (int port = 0; port < PORTS_COUNT; port++){
        if ((1 << port) & data[0])
            sceHSyn_SEAllSoundOff(&synthCtx, port);
    }
}
void SesqSetVolume(uint *data){
    Kprintf("[SESQ] : SesqSetVolume()  : [port_bit=%x vol=%d]\n",data[0],data[1]);
    for (int port = 0; port < PORTS_COUNT; port++){
        if ((1 << port) & data[0]){
            ports[port].vol = data[1];
            sceHSyn_SetVolume(&synthCtx, port, (ushort)data[1]);
        }
    }
}

uint SesqCheckVoicesStat(uint *data){
    uint bits = 0;
    for (int voice = 0; voice < sceHSyn_NumVoice; voice++){
        if (pVstat.voice_state[data[0]][voice] & sceHSynChStat_KeyOn)
            bits |= 1 << voice;
    }
    return bits;
}

void SesqDsidbPrint(void *data){
    Kprintf("=================================================================== [ dsidb print ] =\n");
    Sesq *sesq = sesqList;
    int counter = 0;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play){
            Kprintf(" [%2d] : loop=%d port=%d sesq=%3d cpu_id=%8d iop_id=%8x\n", counter, sesq->port, sesq->sesq,
                    sesq->cpu_id, sesq->iop_id);
            counter++;
        }
        sesq++;
    }
    Kprintf("=====================================================================================\n");
}

void SesqLoopCompare(ushort *data){
    goto FINISH;
    Sesq *sesq = sesqList;
    for (int sesq_i = 0; sesq_i < SESQ_COUNT; sesq_i++){
        if (sesq->play && sesq->loop){
            uint condition = FALSE;
            ushort *list = data + 1;
            while (data[0])
            {
                if (sesq->cpu_id == list[0]){
                    condition = TRUE;
                    break;
                }
                sesq_i++;
            }
            if (!condition){
                Kprintf("[SESQ ASSERT]  : SesqLoopCompare() : loop se conflict. will stop. [port_num=%d, sesq=%d, cpu_id=%d iop_id=%x]\n",
                        sesq->port, sesq->sesq, sesq->cpu_id, sesq->iop_id);
                int ret = sceSESq2Stop(&sesqCtx, sesq->iop_id);
                SESq2StopErrCheck(ret);
            }
        }
        sesq++;
    }
    FINISH:
}
