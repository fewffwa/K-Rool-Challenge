#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"
#include "recomputils.h"
#include "dummystruct.h"
#include "Player_Progress_Structs.h"
#include "recompui.h"

extern void setFlag(s16 flagIndex, u8 newValue, u8 flagType);
extern Struct80755340 D_global_asm_80755340;
extern u16 D_global_asm_80755358[];
extern s32 func_global_asm_80712548(void);
extern void func_global_asm_80712574(void);
extern int gameIsInAdventureMode(void);
extern int gameIsInQuitGameMode(void);
extern void func_global_asm_8071261C(void);
extern void func_global_asm_80713C8C(void);
extern u16 D_global_asm_8075531C;
extern OSTime D_global_asm_807445B0;
extern u8 isFlagSet(s16 flagIndex, u8 flagType);
extern void playSong(MUSIC_E arg0, f32 arg1);
extern u32 global_properties_bitfield;
extern s16 playSound(s16 arg0, s32 arg1, f32 arg2, f32 arg3, u8 arg4, u8 arg5);
extern void func_global_asm_807127B4(void);
extern u64 __ull_div(u64, u64);
extern u8 func_global_asm_80712628(void);
extern u8 D_global_asm_80755350;
extern s32 func_global_asm_806C9D7C(void);
extern PlayerProgress D_global_asm_807FC950[4];
extern u8 current_character_index[];
extern Gfx *printStyledText(Gfx *dl, s16 style, s16 x, s16 y, u8 *string, u32 extraBitfield);
extern Actor *gCurrentActorPointer;
extern Gfx **D_1000118;;
extern void addActorToTextOverlayRenderArray(void *arg0, Actor *arg1, u8 arg2);
extern s32 countSetFlags(s32 startIndex, s32 length, u8 flagType);
extern Maps current_map;
extern int func_global_asm_80714464(void);

#define initHelmTimer func_global_asm_80712574
#define gameIsInEndSequenceMode func_global_asm_80714464

RecompuiContext ui_context;
RecompuiResource score_label;

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
    if (isFlagSet(PERMFLAG_PROGRESS_K_ROOL_DEFEATED, 0U)) {
       return;
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
static u8 isnewfile = 0;
static u8 filehasstarted = 0;

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


RECOMP_CALLBACK("*", recomp_on_new_file_start) void Setnewfile(void) {
    isnewfile = 1;
    helm_timer_started = 0;
    filehasstarted = 1;
     ui_context = recompui_create_context();
    recompui_open_context(ui_context);

    RecompuiResource root = recompui_context_root(ui_context);

    score_label = recompui_create_label(
        ui_context,
        root,
        "Score: 0",
        LABELSTYLE_NORMAL
    );

    recompui_show_context(ui_context);
    recompui_close_context(ui_context);
}

RECOMP_CALLBACK("*", recomp_on_dirty_file_start) void Setdirtyfile(void) {
    isnewfile = 0;
    helm_timer_started = 0;
    filehasstarted = 1;
     ui_context = recompui_create_context();
    recompui_open_context(ui_context);

    RecompuiResource root = recompui_context_root(ui_context);

    score_label = recompui_create_label(
        ui_context,
        root,
        "File in-progress detected! Please start a new file to start the challenge!",
        LABELSTYLE_NORMAL
    );
    recompui_set_context_captures_input(ui_context, 0);
    recompui_set_context_captures_mouse(ui_context, 0);
    recompui_show_context(ui_context);
    recompui_close_context(ui_context);
}
s32 getgbpoints(void){
    s32 new_var;

    PlayerProgress *new_var2;
    s32 levelIndex;
    s32 kong;
    s32 levelindex;
    new_var2 = &D_global_asm_807FC950[0];
    new_var = 14;
    s32 totalGBs = 0;
        for (levelIndex = 0; levelIndex < new_var; levelIndex++) {
        for (kong = 0; kong < 5; kong++) {
            totalGBs += new_var2->character_progress[kong].golden_bananas[levelIndex];
        }
    } 
    return totalGBs * 2;
}
s32 getblueprintpoints(void){
    return countSetFlags(PERMFLAG_ITEM_BLUEPRINT_JAPES_DK, 40, FLAG_TYPE_PERMANENT);
}

s32 getfairypoints(void){
    return countSetFlags(PERMFLAG_ITEM_FAIRY_JAPES_POOL, 20, FLAG_TYPE_PERMANENT)*5;
}

s32 getmedalpoints(void){
    return countSetFlags(PERMFLAG_ITEM_MEDAL_JAPES_DK, 40, FLAG_TYPE_PERMANENT)*3;
}

s32 getcrownpoints(void){
    return countSetFlags(PERMFLAG_ITEM_CROWN_JAPES, 10, FLAG_TYPE_PERMANENT)*5;
}
s32 getkeypoints(void){
    s32 keyflags = (isFlagSet(PERMFLAG_ITEM_KEY_1,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_KEY_2,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_KEY_3,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_KEY_4,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_KEY_5,FLAG_TYPE_PERMANENT)) +(isFlagSet(PERMFLAG_ITEM_KEY_6,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_KEY_7,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_KEY_8,FLAG_TYPE_PERMANENT)); 
    return keyflags*10;
}

s32 getcompanycoinpoints(void){
    s32 coinflags= (isFlagSet(PERMFLAG_ITEM_NINTENDO_COIN,FLAG_TYPE_PERMANENT)) + (isFlagSet(PERMFLAG_ITEM_ISLES_RAREWARE_GB,FLAG_TYPE_PERMANENT));
    return coinflags*10;
}

s32 getkroolbonus(void){
    return isFlagSet(PERMFLAG_PROGRESS_K_ROOL_DEFEATED, FLAG_TYPE_PERMANENT)*30;
}

s32 tallyScore(void){
    s32 Score = getgbpoints()+getblueprintpoints()+getfairypoints()+getmedalpoints()+getcrownpoints()+getcompanycoinpoints()+getkeypoints()+getkroolbonus();
return Score;
}

Gfx *TestGFX(Gfx *dl, Actor *ac){
 char buf[8];
 s32 displaynumber = tallyScore();
    _sprintf(buf, "%d", displaynumber);
    dl = printStyledText(dl, 3, 260, 80, (u8*)buf, 1);
    return dl;
}



void updatescore(void){
 char buffer[32];

    s32 score = tallyScore();

    _sprintf(buffer, "Score: %d", score);

    recompui_open_context(ui_context);
    recompui_set_context_captures_input(ui_context, 0);
    recompui_set_context_captures_mouse(ui_context, 0);
    recompui_set_text(score_label, buffer);    
    recompui_close_context(ui_context);
}

void deletescoreui(void) {
    recompui_open_context(ui_context);

    RecompuiResource root = recompui_context_root(ui_context);
    recompui_destroy_element(root, score_label);

    recompui_close_context(ui_context);

    score_label = RECOMPUI_NULL_RESOURCE;
}

RECOMP_CALLBACK("*", dk64recomp_every_frame) void KRoolChallenge(void) {
    if (isnewfile){
            if (gameIsInAdventureMode()|| gameIsInQuitGameMode()||gameIsInEndSequenceMode()) {
                if (!helm_timer_started) {
                initHelmTimer();
                helm_timer_started = 1;
            }
            updatescore();
        }
}


if (filehasstarted){
        if (!gameIsInAdventureMode()&& !gameIsInQuitGameMode()&& !gameIsInEndSequenceMode()) {
             if (score_label != RECOMPUI_NULL_RESOURCE) {
                deletescoreui();  
            }  
    }
}
}