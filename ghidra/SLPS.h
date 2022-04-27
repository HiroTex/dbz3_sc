#include <stdbool.h>
#include <stdio.h>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;

extern const unsigned int DAT_002feb14;
extern const unsigned int DAT_002ff10c;
extern const unsigned int DAT_002ff11c;
extern const unsigned int DAT_002ff28c;
extern const unsigned int DAT_00333bc0;

extern const unsigned int PTR_MemoryCard = DAT_002ff28c;
extern const unsigned int PTR_Resident_Battle_Param = *(int *)(DAT_002ff11c + 4);
extern const unsigned int PTR_Common_Character_Info = *(int *)(DAT_002feb14 + 0x30);
extern const unsigned int PTR_Resident_Chara_Param = *(int *)(DAT_002ff11c + 0xc);
extern const unsigned int PTR_Progress_Chara_Param = PTR_Resident_Chara_Param + (*(uint *)(PTR_Resident_Chara_Param + 4) & 0xfffffffc);
extern const unsigned int PTR_Progress_ZItem_Param = PTR_Resident_Chara_Param + (*(uint *)(PTR_Resident_Chara_Param + 8) & 0xfffffffc);
extern const unsigned int Language_Setting = (*(uint *)(PTR_MemoryCard + 0x1608) & 1);
