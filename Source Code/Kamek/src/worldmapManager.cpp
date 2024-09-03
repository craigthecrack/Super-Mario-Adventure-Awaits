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

int dWMManager_c::onDraw() {
	return true;
}

void dWMManager_c::setWorldNumber(nw4r::lyt::TextBox *textbox, u8 world) {
	dLevelInfo_c::entry_s *liWorld = dLevelInfo_c::s_info.searchBySlot(world, 38);
	if (liWorld) {
		const wchar_t *worldNum;
		worldNum = getWorldNumber(liWorld->displayWorld);
		textbox->SetString(worldNum);
	} else {
		textbox->SetString(L"?");
	}
}

void dWMManager_c::CSGLoadLevelNumberFromLevelInfo(int *param_1, int param_2) {
	if (param_2 == 1) {
		*(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7) = 0xfffffffe;
	} else {
		layout = (m2d::EmbedLayout_c*)((int)(dCourseSelectManager_c::instance) + 208);

    	T_worldNum_00 = layout->findTextBoxByName("T_worldNum_00");
	    T_cSelect_00 = layout->findTextBoxByName("T_cSelect_00");
	    T_cSelect_pic = layout->findTextBoxByName("T_cSelect_pic");

		T_levelName_00 = layout->findTextBoxByName("T_levelName_00");
		nw4r::lyt::Pane *N_zanki_00 = layout->findPaneByName("N_zanki_00");
		N_zanki_00->trans.y = 14.99f; // normal position

		T_cSelect_pic->SetVisible(false);
		T_cSelect_00->SetVisible(true);
		T_levelName_00->SetVisible(true);

		World = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D3);
		Level = *(u8*)((int)(dCourseSelectManager_c::instance) + 0x4D7);

		const char *levelname;
		dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(World, Level);
		if (level) {
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
			if (Level == 38) {
				SaveFile *file = GetSaveFile();
				SaveBlock *block = file->GetBlock(file->header.current_file);
				switch (block->toad_level_idx[World]) {
					case 0: //arrow
						T_levelName_00->SetVisible(false);
						levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(World, 38));
						N_zanki_00->trans.y = 41.99f; // retail position
						break;
					case 4: //yellow
						levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(World, 27));
						break;
					case 5: //red
						levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(World, 26));
						break;
					default: //green
						levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(World, 25));
						break;
				}
			} else {
				// fix for completed levels with the airship icon
				if (level->displayLevel == 38) {
					SaveBlock *save = GetSaveFile()->GetBlock(-1);
					u32 conds = save->GetLevelCondition(World, Level);

					if (conds & COND_NORMAL)
						T_cSelect_pic->SetString(L"?"); // anchor icon
				}

				levelname = dLevelInfo_c::s_info.getNameForLevel(level);
			}
			WriteAsciiToTextBox(T_levelName_00, levelname);
		} else {
			if(Level > 254) { //get a dot
				T_levelName_00->SetVisible(false);
				N_zanki_00->trans.y = 41.99f; // retail position
				T_cSelect_pic->SetVisible(true);
				T_cSelect_00->SetVisible(false);
				T_cSelect_pic->SetString(L"6");
				this->setWorldNumber(T_worldNum_00, World);
			} else {
				T_levelName_00->SetVisible(true);
				T_cSelect_pic->SetVisible(false);
				T_cSelect_00->SetVisible(true);
				char levelNumber[15];
				sprintf(levelNumber, "%d-%d (UNNAMED)", World+1, Level+1);
				levelname = levelNumber;
				WriteAsciiToTextBox(T_levelName_00, levelname);
				T_cSelect_00->SetString(L"?");
				T_worldNum_00->SetString(L"?");
			}
		}
	}

	return;
}