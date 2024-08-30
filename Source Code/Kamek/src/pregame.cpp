#include <game.h>
#include "levelinfo.h"
#include <newer.h>

class PregameLytHandler {
	public:
		m2d::EmbedLayout_c layout;

		nw4r::lyt::Pane *rootPane;

		nw4r::lyt::TextBox
			*T_minus_00, *T_world_00, *T_worldNum_00,
			*T_pictureFont_00, *T_corseNum_00,
			*T_remainder_00, *T_remainder_01, *T_remainder_02, *T_remainder_03,
			*T_remainder_10, *T_remainder_11, *T_remainder_12, *T_remainder_13,
			*T_x_00, *T_x_01, *T_x_02, *T_x_03, *T_x_10, *T_x_11, *T_x_12, *T_x_13,
			*T_x_00_o, *T_x_10_o,
			*T_otasukePlay_00, *T_otasukePlay_01,
			*T_recommend_00, *T_remainder_00_o, *T_remainder_10_o;

		nw4r::lyt::Picture
			*P_Wx_00[9], *P_coin_00, *P_free_00,
			*P_batB_0x[4], *P_bat_00,
			*P_batB_1x[4], *P_bat_01,
			*P_batB_2x[4], *P_bat_02,
			*P_batB_3x[4], *P_bat_03,
			*P_luijiIcon_00_o, *P_luijiIcon_10_o, *P_coinStage_00;

		nw4r::lyt::Pane
			*N_mario_00, *N_luiji_00, *N_kinoB_01, *N_kinoY_00,
			*N_zankiPos_x[4], *N_zanki_00,
			*Null_battPosxP[4], *N_batt_x[4],
			*N_batt, *N_otasukePlay_00;

		u8 layoutLoaded, somethingHasBeenDone, isVisible, hasShownLuigiThing_orSomething;

		u32 currentStateID;

		u32 _2E8;

		u32 countdownToEndabilityCopy, activePlayerCountMultBy4_maybe;
		u32 batteryLevels[4];
		u32 pgCountdown;

		void hijack_loadLevelNumber(); // replaces 80B6BDD0
};

// Notes:
// Deleted; P_coinStage_00, T_recommend_00,
// P_Wx_00, P_coin_00, P_free_00

extern char CurrentLevel;
extern char CurrentWorld;

void LoadPregameStyleNameAndNumber(m2d::EmbedLayout_c *layout) {
	const wchar_t *convWorldName;
	const wchar_t *convLevelName;
	const char *levelname;

	int wnum = (int)CurrentWorld;
	int lnum = (int)CurrentLevel;

	nw4r::lyt::TextBox
		*LevelName, *T_worldNum_00,
		*T_corseNum_00, *T_pictureFont_00;

	// Setup text panes
	LevelName = layout->findTextBoxByName("LevelName");
	T_worldNum_00 = layout->findTextBoxByName("T_worldNum_00");
	T_corseNum_00 = layout->findTextBoxByName("T_corseNum_00");
	T_pictureFont_00 = layout->findTextBoxByName("T_pictureFont_00");

	// work out the thing now
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchBySlot(CurrentWorld, CurrentLevel);
	if (level) {
		convWorldName = getWorldNumber(level->displayWorld);
		convLevelName = getLevelNumber((int)CurrentWorld, level->displayLevel);

		// Level ID
		T_worldNum_00->SetString(convWorldName);

		if (level->displayLevel > 19) {
			T_pictureFont_00->SetVisible(true);
			T_corseNum_00->SetVisible(false);
			T_pictureFont_00->SetString(convLevelName);
		} else {
			T_pictureFont_00->SetVisible(false);
			T_corseNum_00->SetVisible(true);
			T_corseNum_00->SetString(convLevelName);
		}

		// Level Name
		if (CurrentLevel == 38) {
			SaveFile *file = GetSaveFile();
			SaveBlock *block = file->GetBlock(file->header.current_file);
			switch (block->toad_level_idx[CurrentWorld]) {
				case 4: //yellow
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 27));
					break;
				case 5: //red
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 26));
					break;
				default: //green
					levelname = dLevelInfo_c::s_info.getNameForLevel(dLevelInfo_c::s_info.searchBySlot(CurrentWorld, 25));
					break;
			}
		} else {
			levelname = dLevelInfo_c::s_info.getNameForLevel(level);
		}
		
	} else {
		T_pictureFont_00->SetVisible(false);
		T_corseNum_00->SetVisible(true);
		T_worldNum_00->SetString(L"?");
		T_corseNum_00->SetString(L"?");
		char levelNumber[15];
		sprintf(levelNumber, "%d-%d (UNNAMED)", wnum+1, lnum+1);
		levelname = levelNumber;
	}

	wchar_t lbuffer[0x40];
	for (int i = 0; i < 0x40; i++) {
		lbuffer[i] = (unsigned short)levelname[i];
	}

	LevelName->SetString(lbuffer);
}

// TODO in the future: implement the pregame stuff ryguy sent me (will need work to be compatible with newer's layout)

#include "fileload.h"
void PregameLytHandler::hijack_loadLevelNumber() {
	LoadPregameStyleNameAndNumber(&layout);

	nw4r::lyt::Picture *LevelSample;
	LevelSample = layout.findPictureByName("LevelSample");

	// this is not the greatest way to read a file but I suppose it works in a pinch
	char tplName[64];
	sprintf(tplName, "/LevelSamples/%02d-%02d.tpl", CurrentWorld+1, CurrentLevel+1);
	static File tpl;
	if (tpl.open(tplName)) {
		LevelSample->material->texMaps[0].ReplaceImage((TPLPalette*)tpl.ptr(), 0);
	}
}




