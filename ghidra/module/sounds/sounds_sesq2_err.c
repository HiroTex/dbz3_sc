#include "sounds_data.h"
#include <sys/types.h>
#include <kernel.h>

uint SESq2InitErrCheck(int ret){
    if (ret != 0){
        Kprintf("[SESQ ASSERT]  : sceSESq2Init() faild. [ret=%x]\n",ret);
        return FALSE;
    }
    return TRUE;
}

uint SESq2LoadErrCheck(int ret){
    if (ret != 0){
        Kprintf("[SESQ ASSERT]  : sceSESq2Load() faild. [ret=%x]\n",ret);
        return FALSE;
    }
    return TRUE;
}

uint SESq2PlayErrCheck(int ret){
    if ((ret >> 0x18 & 0xf8U) != 0xf8){
        Kprintf("[SESQ ASSERT]  : sceSESq2Play() faild. [ret=%x]\n",ret);
        return FALSE;
    }
    return TRUE;
}

uint SESq2StopErrCheck(int ret){
    if (ret != 0 && (ret & 0xffff) != STOP_RET){
        Kprintf("[SESQ ASSERT]  : sceSESq2Stop() faild. [ret=%x]\n",ret);
        return FALSE;
    }
    return TRUE;
}
