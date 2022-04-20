#include <stdbool.h>

typedef unsigned char byte;

unsigned int DAT_002ff10c;
unsigned int DAT_002ff11c;
unsigned int PTR_MemoryCard;

// FUN_0025d468
// Gets Character Index for File List Table
// param_1 = Character ID
// param_2 = Character Costume
// param_3 = Flag for Damaged
int Get_FileID_Character_Costume(int param_1, int param_2, long param_3)

{
	// If param_3 is not equal to zero returns the Damaged Costume
	// 0x53e = First Costume File / 0x542 First Damaged Costume File
	if (param_3 != 0) {
		return param_1 * 10 + param_2 + 0x542;
	}
	return param_1 * 10 + param_2 + 0x53e;
}



// FUN_0025f1c0
// Executed when doing a Right Movement on Map Selection
void MapGrid_Move_Right(int param_1, int *param_2, int param_3)

{
	int iVar1;

	iVar1 = *param_2;
	do {
		iVar1 = iVar1 + 1;
		*param_2 = iVar1;
		if (iVar1 < 7) {
			if (iVar1 == 6) {
				return;
			}
		}
		else {
			*param_2 = 0;
		}
		iVar1 = *param_2;
	} while (0x23 < *(int *)((param_3 * 6 + iVar1) * 4 + param_1));
	return;
}

// FUN_0025f400
// Executed when doing a Left Movement on Map Selection
void MapGrid_Move_Left(int param_1, int *param_2, int param_3)

{
	int iVar1;

	iVar1 = *param_2;
	while (true) {
		iVar1 = iVar1 + -1;
		*param_2 = iVar1;
		if (iVar1 < 0) {
			*param_2 = 6;
			return;
		}
		if (*(int *)((param_3 * 6 + iVar1) * 4 + param_1) < 0x24) break;
		iVar1 = *param_2;
	}
	return;
}

// FUN_0025f2f0
// Map Selection Down Movement?
void MapGrid_Move_Down(int param_1, int *param_2, int *param_3, int param_4)

{
	int iVar1;

	do {
		iVar1 = *param_3;
		*param_3 = iVar1 + 1;
		if (param_4 + -1 < iVar1 + 1) {
			*param_3 = 0;
		}
	} while ((0x23 < *(int *)((*param_3 * 6 + *param_2) * 4 + param_1)) && (iVar1 = FUN_0025f280(param_1, param_2, *param_3), iVar1 == 0));
	return;
}

// FUN_0025f398
// Map Selection Up Movement?
void MapGrid_Move_Up(int param_1, int *param_2, int *param_3, int param_4)

{
	int iVar1;

	do {
		iVar1 = param_4 + -1;
		if (-1 < *param_3 + -1) {
			iVar1 = *param_3 + -1;
		}
		*param_3 = iVar1;
	} while ((0x23 < *(int *)((iVar1 * 6 + *param_2) * 4 + param_1)) && (iVar1 = FUN_0025f280(param_1, param_2, iVar1), iVar1 == 0));
	return;
}

// FUN_00260b48
// Used to Get Costume Amount of Character on progress_chara_param
// param_1 Character Selected
short Get_Character_Costume_Count(int param_1, long param_2)

{
	int iVar1;

	iVar1 = *(int *)(DAT_002ff11c + 0xc) + (*(unsigned int *)(*(int *)(DAT_002ff11c + 0xc) + 4) & 0xfffffffc);
	if ((param_2 != 0) && ((int)(unsigned int)*(unsigned short *)(param_1 * 0x3c + iVar1 + 10) <= *(int *)param_2)) { 
		*(int *)param_2 = 0;
	}

	// 0x3C Amount of Bytes for each character on progress_chara_param
	// 10 [0x0A] Place where Costume Amount is located for each character set
	return *(short *)(param_1 * 0x3c + iVar1 + 10);
}

// FUN_00260bb0
// Gets DP Points of Character on progress_chara_param as Short Int
// param_1 Character Selected
short Get_Character_DP_Value(int param_1)

{
	return *(short *) (param_1 * 0x3c + *(int *)(DAT_002ff11c + 0xc) + (*(unsigned int *)(*(int *)(DAT_002ff11c + 0xc) + 4) & 0xfffffffc) + 0xc);
}

// FUN_0025fa78
// Reads Savedata to store Locked Map Values
void MapGrid_Set_Locked(int *param_1, int *param_2)

{
	int iVar1;
	long lVar2;
	int iVar3;
	int iVar4;

	iVar3 = *param_1;
	iVar1 = *(int *)(DAT_002ff10c + 0x18);
	iVar4 = 0;
	if (0 < iVar3) {
		do {
			lVar2 = (long)*param_2; // Gets Map ID from maps_order.dat
			if (lVar2 == 0x1b) {
			code_r0x0025fb10:
				if ((*(unsigned long *)(PTR_MemoryCard + 0xc28) >> lVar2 & 1) == 0) {
					*param_2 = 0x24;
				}
				else {
					if (iVar1 != 0x28) goto LAB_0025fb74;
					*param_2 = 0x25;
				}
			LAB_0025fb6c:
				iVar3 = *param_1;
			}
			else {
				if (lVar2 < 0x1c) {
					if (lVar2 == 4) goto code_r0x0025fb10;
				}
				else {
					if (lVar2 == 0x23) goto LAB_0025fb74;
				}
				// Gets Bool Value of Savedata Map Address [PTR_MemoryCard + 0x28]
				// lVar2 is Current Map Loaded on Memory
				// If Bool value is 0 (false) writes the map as Locked [0x24] in the Maps_order
				if ((*(unsigned long *)(PTR_MemoryCard + 0xc28) >> lVar2 & 1) == 0) {
					*param_2 = 0x24;
					goto LAB_0025fb6c;
				}
			}
		LAB_0025fb74:
			iVar4 = iVar4 + 1;
			param_2 = param_2 + 1; // Updates Address to Read Next Map on Memory
		} while (iVar4 < iVar3);
	}
	return;
}

// FUN_00260be0
// Gets Currently Unlocked Z Item Slots for Character Selected on Grid
// param_1 = Character Selected ID
// param_2 = Character Grid Position
// param_3 = Password Character boolean
int Get_ZItem_Slots_Count(int param_1, int param_2, long param_3)

{
	unsigned short uVar1;

	// If not a Password Character
	if (param_3 == 0) {
		// Gets Unlocked Z Items Slots of Char Selected from EvoZ Data [0x183c] on Memory Card
		uVar1 = *(unsigned short *)(param_2 * 0x38 + PTR_MemoryCard + 0x183c);
	}
	else { // Character is a Password Character
		   // Gets Unlocked Z Items Slots of Char Selected from Custom Characters Zone [0x183c] on Memory Card
		uVar1 = *(unsigned short *)(param_2 * 0x1c + PTR_MemoryCard + 0x2d54);
	}

	// Gets Character's Z Item Slots Unlocked by Default from Progress Chara Param
	// Adds Data from uVar1 to get full Z Item Slots Unlocked Currently
	return (int)((unsigned int)*(unsigned short *)(param_1 * 0x3c + *(int *)(DAT_002ff11c + 0xc) + (*(unsigned int *)(*(int *)(DAT_002ff11c + 0xc) + 4) & 0xfffffffc) + 0xe) + (unsigned int)uVar1);
}

// FUN_00260c50
// Gets current Zeni Limit
// param_1 = Character ID
// param_2 = Character Grid Position
int Get_ZItem_Zeni_Limit(int param_1, int param_2)

{
	// Gets Unlocked Z Item Slots of Char Selected from EvoZ Data [0x183c] on Memory Card and multiplies it by 4
	// Gets Current Zeni Limit of Character from Progress_Chara_Param 
	return *(int *)((unsigned int)*(unsigned short *)(param_2 * 0x38 + PTR_MemoryCard + 0x183c) * 4 + param_1 * 0x3c + *(int *)(DAT_002ff11c + 0xc) + (*(unsigned int *)(*(int *)(DAT_002ff11c + 0xc) + 4) & 0xfffffffc) + 0x10);
}


// FUN_00261650
// Loads the Character Restrictions from progress_chara_param
int Get_Character_Select_Restrictions(unsigned int param_1, unsigned int param_2)

{
	int iVar1;
	unsigned int uVar2;
	int iVar3;
	int iVar4;

	// If param_1 and param_2 is not equal to Saibamen [0x4F]
	// And param_1 and param_2 is not equal to Cell Jr [0x6D]
	// And param_1 and param_2 is not equal to Metal Cooler [0x80]
	if (((((param_1 != 0x4f) && (param_1 != 0x6d)) && (param_1 != 0x80)) && ((param_2 != 0x4f && (param_2 != 0x6d)))) && (param_2 != 0x80)) {
		// Loads Pointer to chara_progress_param file on iVar3
		iVar3 = *(int *)(DAT_002ff11c + 0xc) + (*(unsigned int *)(*(int *)(DAT_002ff11c + 0xc) + 4) & 0xfffffffc);
		if (param_1 != param_2) {
			iVar1 = 0x20;
			iVar4 = 0;
			while (true) {
				uVar2 = (unsigned int)*(byte *)(param_1 * 0x3c + iVar3 + 0xc + iVar1);
				if (((uVar2 != 0xff) && (uVar2 == param_2))) break;

				uVar2 = (unsigned int)*(byte *)(param_2 * 0x3c + iVar3 + 0xc + iVar1);
				if (uVar2 != 0xff && (uVar2 == param_1)) break;

				if (3 < iVar4 + 1) {
					return 0;
				}
				iVar1 = iVar4 + 0x21;
				iVar4 = iVar4 + 1;
			}
		}
		return 1;
	}
	// The Character is either Saibamen [0x4F] or Cell Jr [0x6D] or Metal Cooler [0x80]
	return 0;
}

// FUN_00261510
// Sets CPU Difficulty from param on Savedata Setting
long Set_Battle_CPU_Value(unsigned long param_1)

{
	long uVar1;
	uVar1 = 0;
	if (param_1 < 5) {
		switch (param_1) {
		case 0: // Very Easy
			uVar1 = 0;
			break;
		case 1: // Easy
			uVar1 = 0x6;
			break;
		case 2: // Normal
			uVar1 = 0xd;
			break;
		case 3: // Hard
			uVar1 = 0x15;
			break;
		case 4: // Very Hard
			uVar1 = 0x1d;
			break;
		}
	}
	return uVar1;
}

// FUN_00261568
// Set CPU IA on Training Mode
int Set_Training_CPU_Value(int param_1)

{
	int uVar1;

	uVar1 = 0;
	switch (param_1) {
	case 0: // Very Easy
		uVar1 = 0;
		break;
	case 6: // Easy
		uVar1 = 1;
		break;
	case 0xd: // Normal
		uVar1 = 2;
		break;
	case 0x15: // Hard
		uVar1 = 3;
		break;
	case 0x1d: // Very Hard
		uVar1 = 4;
		break;
	case 0xffffffff: // Stand
		uVar1 = 0xffffffff;
	}
	return uVar1;
}

// FUN_00265e98
// Debug Function used to Write All Unlocked Savedata values by default
void Set_MemoryCard_Default_Data_DEBUG(int param_1)

{
	unsigned int uVar1;
	unsigned long *puVar2;
	int iVar3;
	int iVar4;
	unsigned int *puVar5;
	unsigned long *puVar6;
	unsigned long uVar7;
	byte *pbVar8;
	int iVar9;
	unsigned int uVar10;
	long lVar11;
	unsigned int *puVar12;

	puVar2 = (unsigned long *)(param_1 + 0xc20);
	iVar9 = 2;
	puVar12 = (unsigned int *)(param_1 + 0x10);
	do {
		iVar9 = iVar9 + -1;
		*puVar2 = 0;
		puVar2 = puVar2 + -1;
	} while (-1 < iVar9);

	iVar9 = 0;
	do {
		iVar3 = iVar9;
		if (iVar9 < 0) {
			iVar3 = iVar9 + 0x3f;
		}
		iVar4 = iVar9 + (iVar3 >> 6) * -0x40;
		puVar6 = (unsigned long *)((iVar3 >> 6) * 8 + PTR_MemoryCard + 0xc10);
		iVar9 = iVar9 + 1;
		*puVar6 = *puVar6 | 1 << (long)iVar4;
	} while (iVar9 < 0xa1); // Do While Character Counter is less than 161 [0xA1]

	lVar11 = 0;
	*(unsigned long *)(PTR_MemoryCard + 0xc28) = 0;
	do {
		uVar7 = 1 << lVar11;
		lVar11 = (long)((int)lVar11 + 1);
		*(unsigned long *)(PTR_MemoryCard + 0xc28) = *(unsigned long *)(PTR_MemoryCard + 0xc28) | uVar7;
	} while (lVar11 < 0x23); // Do While Map Counter is less than 35 [0x23]

	uVar10 = 0;
	do {
		uVar1 = uVar10 & 0x1f;
		uVar10 = uVar10 + 1;
		*(unsigned int *)(PTR_MemoryCard + 0xc30) = *(unsigned int *)(PTR_MemoryCard + 0xc30) | 1 << uVar1;
	} while ((int)uVar10 < 0x19); // Do While Song Counter is less than BGM 20 [0x14]

	uVar10 = 0;
	do {
		uVar1 = uVar10 & 0x1f;
		uVar10 = uVar10 + 1;
		*(unsigned int *)(PTR_MemoryCard + 8) = *(unsigned int *)(PTR_MemoryCard + 8) | 1 << uVar1;
	} while ((int)uVar10 < 7); // Do While Dragon Ball Counter less than 7

	pbVar8 = (byte *)(param_1 + 0x2ec8);
	iVar9 = 0x15d;
	do {
		*pbVar8 = *pbVar8 | 1;
		uVar10 = FUN_00254c30(100);
		iVar9 = iVar9 + -1;
		if ((uVar10 & 1) != 0) {
			*pbVar8 = *pbVar8 | 2;
		}
		pbVar8 = pbVar8 + 1;
	} while (-1 < iVar9);

	puVar5 = (unsigned int *)(param_1 + 0x2ec4);
	*(unsigned int *)(param_1 + 0x3028) = 0x4a0150; // Sets Zeni amount to 4.850.000
	iVar9 = 0xd;
	do {
		iVar9 = iVar9 + -1;
		*puVar5 = 0xffffffff;
		puVar5 = puVar5 + -7;
	} while (-1 < iVar9);
	*(unsigned int *)(param_1 + 0x20c) = 0x14; // Sets All Mission 100 Sets Unlocked (20)
	puVar5 = (unsigned int *)(param_1 + 0x14);
	*(unsigned int *)(param_1 + 0x208) = *(unsigned int *)(param_1 + 0x208) | 1;
	iVar9 = 0;
	do {
		iVar9 = iVar9 + 1;
		*puVar12 = *puVar12 | 3;
		puVar12 = puVar12 + 4;
		*puVar5 = 0xffff;
		puVar5[1] = 0xffff;
		puVar5[2] = 0xffff;
		puVar5 = puVar5 + 4;
	} while (iVar9 < 9);
	return;
}

// FUN_002664b0
// Writes Amount of Zeni on Savedata
// param_1 = Zeni Amount to Sum
void Set_MemoryCard_Zeni_Data(int param_1)

{
	// Writes the value of Zeni that it is already on the Savedata Address plus param_1
	*(int *)(PTR_MemoryCard + 0x3028) = *(int *)(PTR_MemoryCard + 0x3028) + param_1;

	// If Amount of Zeni is bigger than 9.999.999
	if (9999999 < *(int *)(PTR_MemoryCard + 0x3028)) {
		*(unsigned int *)(PTR_MemoryCard + 0x3028) = 9999999; // Caps it at 9.999.999
		return;
	}

	// If Amount of Zeni less than Zero
	if (*(int *)(PTR_MemoryCard + 0x3028) < 0) {
		*(unsigned int *)(PTR_MemoryCard + 0x3028) = 0; // Resets it
	}
	return;
}

// FUN_002664f8
// Writes Duel Mode Default Settings on Savedata
void Set_MemoryCard_Default_Duel_Data(void)

{
	*(unsigned int *)(PTR_MemoryCard + 0xc34) = 3; // Time Setting on Duel Mode
	*(unsigned int *)(PTR_MemoryCard + 0xc38) = 2; // Difficulty Setting on Duel Mode
	*(unsigned int *)(PTR_MemoryCard + 0xc3c) = 2; // Announcer Setting on Duel Mode
	*(unsigned int *)(PTR_MemoryCard + 0xc40) = 0; // Player 1 Transformations Enabled 
	*(unsigned int *)(PTR_MemoryCard + 0xc44) = 0; // Player 2 Transformations Enabled 
	*(unsigned int *)(PTR_MemoryCard + 0xc48) = 0; // Map Destruction Enabled
	return;
}