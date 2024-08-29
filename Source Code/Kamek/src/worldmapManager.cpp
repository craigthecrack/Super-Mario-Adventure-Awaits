#include <game.h>
#include <common.h>
#include <profile.h>
#include <sfx.h>
#include "levelinfo.h"

//map manager actor, do whatever you want with it

int spawnMapManager() {
    dActor_c* worldmapManager = (dActor_c*)fBase_c::searchByProfileId(ProfileId::WM_MANAGER);
	if(!worldmapManager) {
		dActor_c::create(WM_MANAGER, 0, 0, 0);
	}
	return 0;
}

class dWMManager_c : public dActor_c {
public:
	static dActor_c* build();
	static dWMManager_c *instance;

	void CSGLoadLevelNumberFromLevelInfo(int *param_1,int param_2); 
	void setWorldNumber(nw4r::lyt::TextBox *textbox, u8 world);

	void setupFooterInfo();
	void setupHeaderInfo();

    m2d::EmbedLayout_c *layout;
    Remocon* rem;

    nw4r::lyt::TextBox
        *T_worldNum_00, *T_cSelect_00, *T_cSelect_pic,
        *T_guideViewLS_00, *T_guideViewL_01, *T_levelName_00,
		*T_worldCoin_00, *T_worldName_00;
    
    u8 World;
    u8 Level;

	int onCreate();
	int onDelete();
	int onExecute();
	int onDraw();

};

dWMManager_c *dWMManager_c::instance = 0;

const char* WMManagerNameList[] = {0};
Profile WMManagerProfile(&dWMManager_c::build, ProfileId::WM_MANAGER, NULL, ProfileId::WM_SINKSHIP, ProfileId::WM_MANAGER, "WM_MANAGER", WMManagerNameList);

dActor_c* dWMManager_c::build() {
	void *buffer = AllocFromGameHeap1(sizeof(dWMManager_c));
	dWMManager_c *c = new(buffer) dWMManager_c;

	instance = c;
	return c;
}

int dWMManager_c::onCreate() {
	SaveFile *file = GetSaveFile();
	SaveBlock *block = file->GetBlock(file->header.current_file);
    if (block->hasWrittenNewPow == false) {
		dStockItem_c *stockItem = (dStockItem_c*)searchByProfileId(ProfileId::STOCK_ITEM);
        for (int i = 0; i < 7; i++) {
            block->new_powerups_available[i] = block->powerups_available[i];
			stockItem->newCounts[i] = block->new_powerups_available[i];
        }
        block->hasWrittenNewPow = true;
    }
	return true;
}


int dWMManager_c::onDelete() {
	return true;
}


int dWMManager_c::onExecute() {
	return true;
}

void dWMManager_c::CSGLoadLevelNumberFromLevelInfo(int *param_1, int param_2) {
	layout = (m2d::EmbedLayout_c*)((int)(dCourseSelectManager_c::instance) + 208);

    T_worldNum_00 = layout->findTextBoxByName("T_worldNum_00");
    T_cSelect_00 = layout->findTextBoxByName("T_cSelect_00");
    T_cSelect_pic = layout->findTextBoxByName("T_cSelect_pic");
    T_levelName_00 = layout->findTextBoxByName("T_levelName_00");

    T_cSelect_pic->SetVisible(false);
    T_cSelect_00->SetVisible(true);

	World = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D3);
	Level = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);

	this->setWorldNumber(T_worldNum_00, World);

	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(World, Level);
	if (level) {
        T_levelName_00->SetVisible(true);
        
		const wchar_t *convWorldName;
		const wchar_t *convLevelName;

		convWorldName = getWorldNumber(level->displayWorld);
		convLevelName = getLevelNumber(World, level->displayLevel);

		T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_cSelect_pic->SetVisible(true);
			T_cSelect_00->SetVisible(false);
			T_cSelect_pic->SetString(convLevelName);
		} else {
			T_cSelect_pic->SetVisible(false);
			T_cSelect_00->SetVisible(true);
			T_cSelect_00->SetString(convLevelName);
		}

        const char *levelname = dLevelInfo_c::s_info.getNameForLevel(level);
		wchar_t lbuffer[0x40];
		for (int i = 0; i < 0x40; i++) {
			lbuffer[i] = (unsigned short)levelname[i];
		}
		T_levelName_00->SetString(lbuffer);

	} else {
		if(Level > 254) { //get a dot
			T_cSelect_pic->SetVisible(true);
    		T_cSelect_00->SetVisible(false);
            T_levelName_00->SetVisible(false);
			T_cSelect_pic->SetString(L"6");
		} else {
			T_cSelect_pic->SetVisible(false);
    		T_cSelect_00->SetVisible(true);
            T_levelName_00->SetVisible(true);
			T_cSelect_00->SetString(L"?");
            T_levelName_00->SetString(L"Not Found In LevelInfo!");
		}
	}
	return;
}