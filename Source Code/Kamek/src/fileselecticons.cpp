#include <common.h>
#include <game.h>

class dDateFile_c : public dBase_c {
    public:
        u32 unk;
        m2d::EmbedLayout_c layout;

        mEf::es2 efClass0_0, efClass0_1, efClass0_2, efClass0_3, efClass0_4;
        mEf::es2 efClass1_0, efClass1_1, efClass1_2, efClass1_3, efClass1_4;

        dStateWrapper_c<dDateFile_c> stateThing;
        u32 nextActionToExecute;

        nw4r::lyt::Pane 
            *rootPane, *N_dateFile_00, *N_dateFile_01,
            *N_NewFile_00, *N_save, *W_select_00, *W_shadow_00;
        
        nw4r::lyt::TextBox
            *T_worldNumber_00, *T_worldNumber_01, *T_x_00, *T_zanki_00,
            *T_fileNumber_00, *T_fileNumber_01, *T_NewdateFile_00, *T_NewdateFileS;
        
        nw4r::lyt::Picture 
            *P_Star_00, *P_Star_01, *P_Star_02, *P_Star_03, *P_Star_04, 
            *P_Star_05, *P_Star_06, *P_Star_07, *P_Star_08, *P_Star_09;
        
        f32 inexplicably_float;
        u32 fileNum_maybe;
        bool hasLoaded;
        u8 doActuallyDrawMe, isNew, doesHaveBalloon_maybe, mustLoadMyInfo;
        u8 isWaitingForAnim, probablyActivatesHit, doesDrawStarEffects;
        u8 inited_to_1_E50, unk2[3];
        u32 _E54;

        //new func
        void newLoadInfoForSaveBlock(SaveBlock *block);
};

void dDateFile_c::newLoadInfoForSaveBlock(SaveBlock *block) {
    //init variables
    dScript::Res_c *msgRes = GetBMG();
    int world = 8;
    int livesCount;
    int worldNum = 9;

    //do worldnumber
    do {
        if (1 & block->worlds_available[world]) break;
        world--;
        worldNum--;
    } while (world > -1);
    WriteNumberToTextBox(&worldNum, T_worldNumber_01, 1);
    WriteBMGToTextBox(T_worldNumber_00, msgRes, 0x32, 0xF, 0);

    //lives number
    livesCount = block->player_lives[block->lastP1Character];
    const int length = 2;
    WriteNumberToTextBox(&livesCount, &length, T_zanki_00, 1);
    WriteBMGToTextBox(T_x_00, msgRes, 0x32, 0x12, 0);

    //handle stars
    P_Star_00->SetVisible(false);
    P_Star_01->SetVisible(false);
    P_Star_02->SetVisible(false);
    P_Star_03->SetVisible(false);
    P_Star_04->SetVisible(false);
    if (block->bitfield & SAVE_BIT_GAME_COMPLETE) P_Star_00->SetVisible(true);
    if (block->bitfield & SAVE_BIT_ALL_EXITS) P_Star_01->SetVisible(true);
    if (block->bitfield & SAVE_BIT_ALL_STAR_COINS) P_Star_02->SetVisible(true);
    if (block->bitfield & SAVE_BIT_ALL_STAR_COINS_W9) P_Star_03->SetVisible(true);
    if (block->bitfield & SAVE_BIT_EVERYTHING_TRULY_DONE) P_Star_04->SetVisible(true);

    // handle new player icons
    nw4r::lyt::Picture *P_iconM_00 = layout.findPictureByName("P_iconM_00");
    nw4r::lyt::Picture *P_iconL_00 = layout.findPictureByName("P_iconL_00");
    nw4r::lyt::Picture *P_iconY_00 = layout.findPictureByName("P_iconY_00");
    nw4r::lyt::Picture *P_iconB_00 = layout.findPictureByName("P_iconB_00");
    P_iconM_00->SetVisible(false);
    P_iconL_00->SetVisible(false);
    P_iconY_00->SetVisible(false);
    P_iconB_00->SetVisible(false);

    switch (block->lastP1Character) {
        default: //mario
            P_iconM_00->SetVisible(true);
            T_x_00->colour2 = marioColor;
            T_zanki_00->colour2 = marioColor;
            break;
        case 1: //luigi
            P_iconL_00->SetVisible(true);
            T_x_00->colour2 = luigiColor;
            T_zanki_00->colour2 = luigiColor;
            break;
        case 2: //boad
            P_iconB_00->SetVisible(true);
            T_x_00->colour2 = kinoBColor;
            T_zanki_00->colour2 = kinoBColor;
            break;
        case 3: //yoad
            P_iconY_00->SetVisible(true);
            T_x_00->colour2 = kinoYColor;
            T_zanki_00->colour2 = kinoYColor;
            break;
    }
    
    return;
}

// save P1
extern "C" int CSMonExecute(dBase_c *self);

int CSMStorePlayer(dBase_c *self) {
    SaveBlock *saveBlock = GetSaveFile()->GetBlock(-1);

    if (saveBlock->lastP1Character != Player_ID[0]) {
		OSReport("Player 1 id:%d\n", Player_ID[0]);
		saveBlock->lastP1Character = Player_ID[0];
	}

    return CSMonExecute(self);
}