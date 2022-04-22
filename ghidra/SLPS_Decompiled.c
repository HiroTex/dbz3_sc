#include <stdbool.h>

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;

unsigned int DAT_002ff10c;
unsigned int DAT_002ff11c;
unsigned int PTR_MemoryCard;
int PTR_Resident_Chara_Param = *(int *)(DAT_002ff11c + 0xc);
int PTR_Progress_Chara_Param = (*(uint *)(PTR_Resident_Chara_Param + 4) & 0xfffffffc);
int PTR_Progress_ZItem_Param = (*(uint *)(PTR_Resident_Chara_Param + 8) & 0xfffffffc);
int Language_Setting = (*(uint *)(PTR_MemoryCard + 0x1608) & 1);

// FUN_001c3cf0
// Forces Rest Position on Certain Bones for Certain Characters
// param_1 = Animation File Start Offset (Pointer)
// param_2 = Character ID
void Force_Robot_Anim(long param_1, int param_2)

{
	int iVar1;
	// If Animation Start Pointer is not zero
	if (param_1 != 0) {
		iVar1 = (int)param_1;
		// If Character is Pilaf Machine [0x98]
		if (param_2 == 0x98) {
			Force_Anm_Rest_Position(iVar1, 0x10); // Force BELLY Bone
			Force_Anm_Rest_Position(iVar1, 0x11); // Force CHEST Bone
			Force_Anm_Rest_Position(iVar1, 0x2e); // Force NECK Bone
			Force_Anm_Rest_Position(iVar1, 0x2f); // Force HEAD Bone
		}
		else {
			if (param_2 < 0x99) {
				// If Character is Dr. Wheelo [0x78]
				if (param_2 == 0x78) {
					Force_Anm_Rest_Position(iVar1, 0x11); // Force CHEST Bone
					Force_Anm_Rest_Position(iVar1, 0x2e); // Force NECK Bone
					Force_Anm_Rest_Position(iVar1, 0x23); // Force WRIST_L Bone
					Force_Anm_Rest_Position(iVar1, 0x15); // Force WRIST_R Bone
				}
			}
			else {
				// If Character is Fused Pilaf Machine [0x99]
				if (param_2 == 0x99) {
					Force_Anm_Rest_Position(iVar1, 0x11); // Force CHEST Bone
				}
			}
		}
	}
	return;
}

// FUN_001c3dc8
// param_1 = Animation Start Offset (Pointer)
// param_2 = Character ID
void Force_Wheelo_Anim(long param_1, int param_2)
{
	// If Animation Pointer is not equal zero
	// And Character is Dr Wheelo [0x78]
	if ((param_1 != 0) && (param_2 == 0x78)) {
		Force_Anm_Rest_Position((int)param_1, 0x10); // Force BELLY Bone
	}
	return;
}

// FUN_0024c6e0
// param_1 = Animation File Start Offset?
// param_2 = Bone ID
void Force_Anm_Rest_Position(int param_1, int param_2)

{
	ushort uVar1;
	ushort *puVar2;
	undefined4 *puVar3;
	int iVar4;
	uint uVar5;

	// Stores Pointer to Bone Animation Start Index
	// 6 (anm file Header) + (Bone ID * 2)
	uVar1 = *(ushort *)(param_1 + 6 + param_2 * 2);

	if (uVar1 == 0) {
		return; // failsafe for Bone Animation Index equal zero
	}

	// Stores Real Index to Bone Animation (Index * 4)
	puVar2 = (ushort *)(param_1 + (uint)uVar1 * 4);

	// Stores Frame Number Data of Bone Animation on uVar5
	uVar5 = (uint)puVar2[1];

	// If Animation is Translation Type (0x00)
	if ((*puVar2 & 1) == 0) {

		puVar2 = puVar2 + 2;
		if (uVar5 != 0) {
			do {
				uVar5 = uVar5 - 1;
				*(int *)(puVar2 + 8) = 0xfff7ffff;
				*(int *)(puVar2 + 10) = 0x37ffff7f;
				puVar2 = puVar2 + 0xc;
			} while (uVar5 != 0); // Do for each frame data
			return;
		}
	}
	else {
		// If Animation is Rotation Type (0x01)

		puVar3 = (int *)(puVar2 + 2);
		iVar4 = 0;
		if (uVar5 != 0) {
			do {
				iVar4 = iVar4 + 1;
				*puVar3 = 0xfff7ffff;
				puVar3[1] = 0x37ffff7f;
				puVar3 = puVar3 + 2;
			} while (iVar4 < (int)uVar5); // Do for each frame data
		}
	}
	return;
}

// FUN_00259228
// Returns Current GSC ID
int Get_Current_GSC_ID(void)
{
	return DAT_00333bc0;
}

// FUN_002594c8
// Get Story Mode ADX File
// param_1 = Audio Number to Load
int Get_FileID_ADX_GSC(int audioID)
{
	int gscID; // iVar1;
	int fileID; // iVar2;

	fileID = 0xcf6; // Start of Japanese Story Mode ADX Files

	// If Language Selected is not Japanese
	if (Language_Setting != 0) {
		fileID = 0x478e; // Start of English Story Mode ADX Files
	}

	gscID = Get_Current_GSC_ID(); // Loads Current GSC ID selected

	return fileID + gscID * 300 + audioID;
}

// FUN_0025d2c0
// Get Character Data Offset of Progress Chara Param
// param_1 = Character ID
int Get_Progress_Character_Data_PTR(ulong charaID)
{
	int PTR_Character_Data = 0; // iVar1;
	// If Character Selected is less than Random ID [0xa1]
	if (charaID < 0xa1) { 
		// Stores Character Data Pointer of Progress Chara Param
		PTR_Character_Data = PTR_Resident_Chara_Param + PTR_Progress_Chara_Param + (int)charaID * 0x3c;
	}
	return PTR_Character_Data;
}


// FUN_0025d468
// Gets Character Index for File List Table
// param_1 = Character ID
// param_2 = Character Costume
// param_3 = Flag for Damaged
int Get_FileID_Character_Costume(int charaID, int charaColor, long charaDMG)
{
	// If param_3 is not equal to zero returns the Damaged Costume
	// 0x53e = First Costume File / 0x542 First Damaged Costume File
	if (charaDMG != 0) {
		return charaID * 10 + charaColor + 0x542;
	}
	return charaID * 10 + charaColor + 0x53e;
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
				if ((*(ulong *)(PTR_MemoryCard + 0xc28) >> lVar2 & 1) == 0) {
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
				if ((*(ulong *)(PTR_MemoryCard + 0xc28) >> lVar2 & 1) == 0) {
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

// FUN_00260b48
// Used to Get Costume Amount of Character on progress_chara_param
// param_1 Character Selected
// param_2 Pointer to Current Costume Number?
short Get_Character_Costume_Count(int param_1, long param_2)

{
	int iVar1;

	// iVar1 Stores Pointer to Start of progress_chara_param.dat
	iVar1 = PTR_Resident_Chara_Param + PTR_Progress_Chara_Param;

	// if param_2 not equal zero
	// And Character Costume Count Less or Equal to param_2
	if ((param_2 != 0) && ((int)(uint)*(ushort *)(param_1 * 0x3c + iVar1 + 10) <= *(int *)param_2)) { 
		*(int *)param_2 = 0; // Resets Costume on Address param_2?
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
	return *(short *) (param_1 * 0x3c + PTR_Resident_Chara_Param + PTR_Progress_Chara_Param + 0xc);
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
		uVar1 = *(ushort *)(param_2 * 0x38 + PTR_MemoryCard + 0x183c);
	}
	else { // Character is a Password Character
		   // Gets Unlocked Z Items Slots of Char Selected from Custom Characters Zone [0x183c] on Memory Card
		uVar1 = *(ushort *)(param_2 * 0x1c + PTR_MemoryCard + 0x2d54);
	}

	// Gets Character's Z Item Slots Unlocked by Default from Progress Chara Param
	// Adds Data from uVar1 to get full Z Item Slots Unlocked Currently
	return (int)((uint)*(ushort *)(param_1 * 0x3c + PTR_Resident_Chara_Param + PTR_Progress_Chara_Param + 0xe) + (uint)uVar1);
}

// FUN_00260c50
// Gets current Zeni Limit
// param_1 = Character ID
// param_2 = Character Grid Position
int Get_ZItem_Zeni_Limit(int param_1, int param_2)

{
	// Gets Unlocked Z Item Slots of Char Selected from EvoZ Data [0x183c] on Memory Card and multiplies it by 4
	// Gets Current Zeni Limit of Character from Progress_Chara_Param 
	return *(int *)((uint)*(ushort *)(param_2 * 0x38 + PTR_MemoryCard + 0x183c) * 4 + param_1 * 0x3c + PTR_Resident_Chara_Param + PTR_Progress_Chara_Param + 0x10);
}

// FUN_00260ca0
// Get Z Item Slot Zeni Limit
// param_1 = Character ID
int Get_Character_ItemSlot_Price(int param_1)

{
	int iVar1;
	int iVar2;
	int iVar3;
	int *piVar4;
	int iVar5;

	// Stores Character Data Start Pointer of Progress Chara Param
	iVar3 = param_1 * 0x3c + PTR_Resident_Chara_Param + PTR_Progress_Chara_Param;

	iVar1 = *(int *)(iVar3 + 0x10); // Stores First Z Item Slot Price
	iVar2 = 0;
	iVar5 = 0;

	// If Z Item Slot Price not equal Zero
	if (iVar1 != 0) {
		piVar4 = (int *)(iVar3 + 0x10); // Store Z Item Slot Prices Pointer
		do {
			iVar5 = iVar1;
			iVar2 = iVar2 + 1;
			piVar4 = piVar4 + 1;
			if (6 < iVar2) { // Failsafe for 7 Z Item Slot Prices
				return iVar5; // Return last Z Item Slot Price loaded
			}
			iVar1 = *piVar4; // Loads next Z Item Slot Price
		} while (*piVar4 != 0); // Do While Z Item Slot Price Value is not Zero
	}
	return iVar5; // Return Z Item Slot Price
}


// FUN_00261228
// Returns if Z Item can be equipped?
// param_1 = Z Item ID
// param_2 = Character ID
// param_3 = PTR_Progress_ZItem_Param
bool ZItem_Can_BeEquipped(int param_1, int param_2, int param_3)

{
	uint uVar1;
	bool bVar2;
	// Get Equip Flags from Z Item Data [Z Item Parameter 0x14]
	uVar1 = *(uint *)(param_1 * 0x28 + param_3 + 0x14);
	bVar2 = false;

	// If Item has Parameter [0x01] disabled
	// And Parameter [0x04] disabled
	if ((uVar1 & 5) == 0) {
		// If Character Is Good (& 1 != 0)
		if ((*(ushort *)(param_2 * 0x3c + PTR_Resident_Chara_Param + PTR_Progress_Chara_Param + 8) & 1) != 0) {
			return (uVar1 & 8) != 0; // Check if Z Item can be Equipped to Good Hearted Characters
		}
		bVar2 = (uVar1 & 0x10) != 0; // Check if Z Item can be Equipped to Evil Characters
	}
	return bVar2;
}


// FUN_00261650
// Loads the Character Restrictions from progress_chara_param
int Get_Character_Select_Restrictions(uint param_1, uint param_2)

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
		iVar3 = PTR_Resident_Chara_Param + PTR_Progress_Chara_Param;
		if (param_1 != param_2) {
			iVar1 = 0x20;
			iVar4 = 0;
			while (true) {
				uVar2 = (uint)*(byte *)(param_1 * 0x3c + iVar3 + 0xc + iVar1);
				if (((uVar2 != 0xff) && (uVar2 == param_2))) break;

				uVar2 = (uint)*(byte *)(param_2 * 0x3c + iVar3 + 0xc + iVar1);
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
long Set_Battle_CPU_Value(ulong param_1)

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

// FUN_002664b0
// Writes Amount of Zeni on Savedata
// param_1 = Zeni Amount to Sum
void Set_MemoryCard_Zeni_Data(int param_1)

{
	// Writes the value of Zeni that it is already on the Savedata Address plus param_1
	*(int *)(PTR_MemoryCard + 0x3028) = *(int *)(PTR_MemoryCard + 0x3028) + param_1;

	// If Amount of Zeni is bigger than 9.999.999
	if (9999999 < *(int *)(PTR_MemoryCard + 0x3028)) {
		*(uint *)(PTR_MemoryCard + 0x3028) = 9999999; // Caps it at 9.999.999
		return;
	}

	// If Amount of Zeni less than Zero
	if (*(int *)(PTR_MemoryCard + 0x3028) < 0) {
		*(uint *)(PTR_MemoryCard + 0x3028) = 0; // Resets it
	}
	return;
}

// FUN_002664f8
// Writes Duel Mode Default Settings on Savedata
void Set_MemoryCard_Default_Duel_Data(void)

{
	*(uint *)(PTR_MemoryCard + 0xc34) = 3; // Time Setting on Duel Mode
	*(uint *)(PTR_MemoryCard + 0xc38) = 2; // Difficulty Setting on Duel Mode
	*(uint *)(PTR_MemoryCard + 0xc3c) = 2; // Announcer Setting on Duel Mode
	*(uint *)(PTR_MemoryCard + 0xc40) = 0; // Player 1 Transformations Enabled 
	*(uint *)(PTR_MemoryCard + 0xc44) = 0; // Player 2 Transformations Enabled 
	*(uint *)(PTR_MemoryCard + 0xc48) = 0; // Map Destruction Enabled
	return;
}

// FUN_0026c028
// Returns a Int16 (short) of the Max Length of a File in the AFS 
// Takes param_1 as the Max Length value
int Get_Short_File_Length(int param_1)

{
	int iVar1;

	iVar1 = param_1;
	if (param_1 < 0) {
		iVar1 = param_1 + 0x7ff;
	}

	// Returns Max Value 0xXXXXXXXX into 0xXXXX by doing a Right Shift by 0x0B
	// Does a Left Shift Again by 0x0B then compares if the original is the same
	// If not, return 0xXXXX + 0x0001;
	return (int)((iVar1 >> 0xb) + (uint)(0 < param_1 + (iVar1 >> 0xb) * -0x800));
}

// FUN_0026be00
// Maybe a Deleted Debug Function?
void Void_0026be00(void)

{
	// Just Calls an empty function and returns
	Void_002890a0();
	return;
}

// FUN_0026be18
// Maybe a Deleted Debug Function?
void Void_0026be18(void)

{
	// Just Calls an empty function and returns
	Void_002890a8();
	return;
}

// FUN_002890a0
// Deleted Debug Function?
void Void_002890a0(void)
{
	return;
}

// FUN_002890a8
// Deleted Debug Function?
void Void_002890a8(void)

{
	return;
}

// FUN_00398958
// param_2 = Character ID
// param_3 = Character Position on Grid
int Get_Next_ZItem_Slot_Price(long param_1, int param_2, int param_3)

{
	ushort uVar1;

	// Gets Current Unlocked Z Item Slots
	uVar1 = *(ushort *)(PTR_MemoryCard + param_3 * 0x38 + 0x183c);

	// If Current Unlocked Z Item Slots not equal zero
	if (uVar1 != 0) {
		// Returns Next Not Unlocked Z Item Slot Price
		return *(int *)((uint)uVar1 * 4 + param_2 * 0x3c + PTR_Resident_Chara_Param + PTR_Progress_Chara_Param + 0xc);
	}
	return 0;
}