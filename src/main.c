#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"
#include "recomputils.h"
#include "dummystruct.h"

extern void setFlag(s16 flagIndex, u8 newValue, u8 flagType);
extern Struct80755340 D_global_asm_80755340;
extern u16 D_global_asm_80755358[];
extern s32 func_global_asm_80712548(void);
extern void func_global_asm_80712574(void);
extern int gameIsInAdventureMode(void);
extern void func_global_asm_8071261C(void);
extern void func_global_asm_80713C8C(void);
extern u16 D_global_asm_8075531C;
extern OSTime D_global_asm_807445B0;
#define initHelmTimer func_global_asm_80712574
extern u8 isFlagSet(s16 flagIndex, u8 flagType);
extern void playSong(MUSIC_E arg0, f32 arg1);
extern u32 global_properties_bitfield;
extern s16 playSound(s16 arg0, s32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5);
extern void func_global_asm_807127B4(void);
extern u64 __ull_div(u64, u64);
extern u8 func_global_asm_80712628(void);
extern u8 D_global_asm_80755350;

RECOMP_PATCH void func_global_asm_8071261C(void) {
}


RECOMP_PATCH void func_global_asm_80713C8C(void) {
    OSTime temp_ret;
    s32 new_var;

    D_global_asm_8075531C = 1;
    if (!D_global_asm_80755340.unk10) {
        return;
    }
    if (isFlagSet(PERMFLAG_PROGRESS_HELM_SHUTDOWN, 0U)) {
        func_global_asm_8071261C();
        playSong(MUSIC_116_HIDEOUT_HELM_BLAST_O_MATIC_OFF, 1.0f);
    }
    D_global_asm_80755340.unk0 += D_global_asm_807445B0;
    if (global_properties_bitfield & 2) {
        return;
    }
    temp_ret = (osGetTime() - D_global_asm_80755340.unk0) * 64;
    new_var = __ull_div(__ull_div(temp_ret, 3000), 1000000);
    D_global_asm_80755340.unk8 = D_global_asm_80755340.unkC - new_var;
    if ((D_global_asm_80755340.unk11 < 0xAU) && (D_global_asm_80755340.unk8 == D_global_asm_80755358[D_global_asm_80755340.unk11])) {
        playSound(0x2D4, 0x7FFFU, 63.0f, 1.0f, 0, 0);
        D_global_asm_80755340.unk11++;
    }
    if (D_global_asm_80755340.unk8 <= 0) {
        func_global_asm_807127B4();
    }
}



static u8 helm_timer_started = 0;

RECOMP_PATCH void initHelmTimer(void) {
    u32 i;
    s32 found;

    found = FALSE;
    D_global_asm_80755340.unk10 = 1;
    D_global_asm_80755340.unk11 = 0;
    D_global_asm_80755340.unk0 = osGetTime();
    D_global_asm_80755340.unkC = 60 * 0x3C;
    for (i = 0; i < 10 && !found; i++) {
        if (D_global_asm_80755358[i] < D_global_asm_80755340.unkC) {
            D_global_asm_80755340.unk11 = i;
            found = TRUE;
        }
    }
}


RECOMP_PATCH u8 func_global_asm_80712628(void){
if (D_global_asm_80755350) { // Helm Timer enabled
  if (global_properties_bitfield & 3) { // Game is paused
    return FALSE;
  }
  return TRUE;
}
return FALSE;
}


RECOMP_CALLBACK("*", dk64recomp_every_frame) void OpenIsles(void) {
        if (gameIsInAdventureMode()) {
            if (!helm_timer_started) {
            setFlag(0x304, TRUE, FLAG_TYPE_PERMANENT);
            initHelmTimer();
            helm_timer_started = 1;
        }
    }
}