#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"
#include "recomputils.h"
#include "dummystruct.h"

extern Struct80755340 D_global_asm_80755340;
extern u16 D_global_asm_80755358[];
extern s32 func_global_asm_80712548(void);
extern void func_global_asm_80712574(void);
extern int gameIsInAdventureMode(void);
#define initHelmTimer func_global_asm_80712574
void setFlag(s16 flagIndex, u8 newValue, u8 flagType);

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



RECOMP_CALLBACK("*", dk64recomp_every_frame) void OpenIsles(void) {
        if (gameIsInAdventureMode()) {
            if (!helm_timer_started) {
            initHelmTimer();
            helm_timer_started = 1;
        }
    }
}