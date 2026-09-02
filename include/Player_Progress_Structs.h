#include "ultra64.h"

typedef struct character_progress {
    u8 moves; // at 0x00
    u8 simian_slam; // at 0x01
    u8 weapon; // at 0x02, bitfield, xxxxxshw
    u8 ammo_belt; // at 0x03, see ScriptHawk's Game.getMaxStandardAmmo() for formula
    u8 instrument; // at 0x04, bitfield, xxxx321i
    u8 unk5;
    u16 coins; // at 0x06
    u16 instrument_ammo; // at 0x08, also used as lives in multiplayer
    u16 coloured_bananas[14]; // TODO: Better datatype?
    u16 coloured_bananas_fed_to_tns[14]; // TODO: Better datatype?
    u16 golden_bananas[14]; // TODO: Better datatype?
} CharacterProgress;

typedef struct PlayerProgress {
    union {
        CharacterProgress character_progress[6]; // 0x5E * 6 (5 Kongs + Krusha)
        u8 character_progress_as_bytes[6][0x5E]; // Note: Can't use sizeof(CharacterProgress) because mips_to_c can't do struct maths yet
        u16 character_progress_as_shorts[6][0x2F]; // Note: Can't use sizeof(CharacterProgress) because mips_to_c can't do struct maths yet
    };
    u8 unk234[0x2F0 - 0x234];
    u16 standardAmmo; // 0x2F0
    u16 homingAmmo; // 0x2F2
    u16 oranges; // 0x2F4
    u16 crystals; // 0x2F6 // Note: Multiplied by 150 compared to on screen counter
    u16 film; // 0x2F8
    s8 unk2FA;
    s8 health; // 0x2FB
    u8 melons; // 0x2FC
    s8 unk2FD; // Something to do with health... hmm
    u16 unk2FE[(0x306 - 0x2FE) / 2];
} PlayerProgress;