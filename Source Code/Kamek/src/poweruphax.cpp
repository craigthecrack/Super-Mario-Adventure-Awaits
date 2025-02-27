#include <common.h>
#include <game.h>
#include <g3dhax.h>
#include <profileid.h>
#include <sfx.h>

void ThwompHammer(dEn_c *thwomp, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (thwomp->profileId == ProfileId::EN_DOSUN) {
		thwomp->dEn_c::collisionCat13_Hammer(apThis, apOther);
	}
	return;
}

void BooHammer(dEn_c *boo, ActivePhysics *apThis, ActivePhysics *apOther) {
	if (boo->profileId == ProfileId::EN_TERESA) {
		boo->dEn_c::collisionCat13_Hammer(apThis, apOther);
	}
	return;
}

void UrchinHammer(dEn_c *urchin, ActivePhysics *apThis, ActivePhysics *apOther) {
	return;
}


#include "poweruphax.h"

void SetCullModeForMaterial(m3d::mdl_c *model, int materialID, u32 cullMode);


dHammerSuitRenderer_c *dHammerSuitRenderer_c::build() {
	return new dHammerSuitRenderer_c;
}

dHammerSuitRenderer_c::dHammerSuitRenderer_c() { }
dHammerSuitRenderer_c::~dHammerSuitRenderer_c() { }

void dHammerSuitRenderer_c::setup(dPlayerModelHandler_c *handler) {
	setup(handler, 0);
}

void dHammerSuitRenderer_c::setup(dPlayerModelHandler_c *handler, int sceneID) {
	victim = (dPlayerModel_c*)handler->mdlClass;

	allocator.link(-1, GameHeaps[0], 0, 0x20);

	nw4r::g3d::ResFile hammer(getResource("hammerM", "g3d/suit.brres"));
	nw4r::g3d::ResFile boomer(getResource("boomerangM", "g3d/suit.brres"));

	if (victim->player_id_2 <= 1) {
		/********/
		/*Hammer*/
		/********/
		nw4r::g3d::ResMdl rmhammer = hammer.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		hammerHelmet.setup(rmhammer, &allocator, 0, 1, 0);
		SetupTextures_Player(&hammerHelmet, sceneID);
		/***********/
		/*Boomerang*/
		/***********/
		nw4r::g3d::ResMdl rmboomer = boomer.GetResMdl((victim->player_id_2 == 0) ? "marioHelmet" : "luigiHelmet");
		boomerHelmet.setup(rmboomer, &allocator, 0, 1, 0);
		SetupTextures_Player(&boomerHelmet, sceneID);
	}

	const char *shellNames[] = {
		"shell", "shell", "shell", "shell", "shell"
	};
	/********/
	/*Hammer*/
	/********/
	hammerShell.setup(hammer.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_Player(&hammerShell, sceneID);
	/***********/
	/*Boomerang*/
	/***********/
	boomerShell.setup(boomer.GetResMdl(shellNames[victim->player_id_2]), &allocator, 0, 1, 0);
	SetupTextures_Player(&boomerShell, sceneID);

	allocator.unlink();


	victimModel = &victim->models[0].body;
	nw4r::g3d::ResMdl *playerResMdl =
		(nw4r::g3d::ResMdl*)(((u32)victimModel->scnObj) + 0xE8);

	//headNodeID = playerResMdl->GetResNode("player_head").GetID();
	if (victim->player_id_2 <= 1) {
		nw4r::g3d::ResNode face_1 = playerResMdl->GetResNode("face_1");
		headNodeID = face_1.GetID();
	}

	nw4r::g3d::ResNode skl_root = playerResMdl->GetResNode("skl_root");
	rootNodeID = skl_root.GetID();
}

void dHammerSuitRenderer_c::draw() {
	if (victim->powerup_id < 7)
		return;

	if(victim->powerup_id == 7) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, &headMtx);
	
			hammerHelmet.setDrawMatrix(&headMtx);
			hammerHelmet.setScale(1.0f, 1.0f, 1.0f);
			hammerHelmet.calcWorld(false);
	
			hammerHelmet.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, &rootMtx);
	
		hammerShell.setDrawMatrix(&rootMtx);
		hammerShell.setScale(1.0f, 1.0f, 1.0f);
		hammerShell.calcWorld(false);
	
		hammerShell.scheduleForDrawing();
	}
	if(victim->powerup_id == 9) {
		if (victim->player_id_2 <= 1) {
			// Materials: 2=hair 3=hat; Modes: BACK=visible ALL=invisible
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 3, GX_CULL_ALL);
			SetCullModeForMaterial(&victim->getCurrentModel()->head, 2, GX_CULL_ALL);
	
			Mtx headMtx;
			victimModel->getMatrixForNode(headNodeID, &headMtx);
	
			boomerHelmet.setDrawMatrix(&headMtx);
			boomerHelmet.setScale(1.0f, 1.0f, 1.0f);
			boomerHelmet.calcWorld(false);
	
			boomerHelmet.scheduleForDrawing();
		}
	
		Mtx rootMtx;
		victimModel->getMatrixForNode(rootNodeID, &rootMtx);
	
		boomerShell.setDrawMatrix(&rootMtx);
		boomerShell.setScale(1.0f, 1.0f, 1.0f);
		boomerShell.calcWorld(false);
	
		boomerShell.scheduleForDrawing();
	}
}








// NEW VERSION
void CrapUpPositions(Vec *out, const Vec *in);

void dStockItem_c::setScalesOfSomeThings() {
	// 4:3 mode fix for stockitem
	nw4r::lyt::Pane *N_frameStock_00 = layout.findPaneByName("N_frameStock_00");
	nw4r::lyt::Pane *N_ItemShadow_00 = shadow->layout.findPaneByName("N_ItemShadow_00");
	Vec2 paneScale = {0.85f, 0.85f};
	Vec modelScale = {0.85f, 0.85f, 0.85f};

	if (!IsWideScreen()) {
		N_frameStock_00->scale = paneScale;
		N_ItemShadow_00->scale = paneScale;
	}
	
	nw4r::lyt::Pane *ppos = N_forUse_PPos[playerCount];

	int howManyPlayers = 0;
	for (int i = 0; i < 4; i++) {
		if (isPlayerActive[i]) {
			int picID = getIconPictureIDforPlayer(howManyPlayers);
			int charID = Player_ID[i];

			if (picID != 24) {
				nw4r::lyt::Picture *pic = P_icon[picID];

				Vec in, out;

				in.x = pic->effectiveMtx[0][3];
				in.y = pic->effectiveMtx[1][3];
				in.z = pic->effectiveMtx[2][3];

				CrapUpPositions(&out, &in);

				u8 *wmp = (u8*)player2d[charID];
				*((float*)(wmp+0xAC)) = out.x;
				*((float*)(wmp+0xB0)) = out.y;
				*((float*)(wmp+0xB4)) = out.z;
				*((float*)(wmp+0x220)) = 0.89999998f;
				*((float*)(wmp+0x224)) = 0.89999998f;
				*((float*)(wmp+0x228)) = 0.89999998f;
				*((float*)(wmp+0x25C)) = 26.0f;
			}
			howManyPlayers++;
		}
	}


	for (int i = 0; i < 10; i++) {
		u8 *item = (u8*)newItemPtr[i];

		nw4r::lyt::Pane *icon = newIconPanes[i];

		Vec in, out;
		in.x = icon->effectiveMtx[0][3];
		in.y = icon->effectiveMtx[1][3];
		in.z = icon->effectiveMtx[2][3];

		CrapUpPositions(&out, &in);

		*((float*)(item+0xAC)) = out.x;
		*((float*)(item+0xB0)) = out.y;
		*((float*)(item+0xB4)) = out.z;
		// use different scales depending on the aspect ratio
		if (IsWideScreen()) {
			*((float*)(item+0x1F4)) = P_buttonBase[i]->scale.x;
			*((float*)(item+0x1F8)) = P_buttonBase[i]->scale.y;
			*((float*)(item+0x1FC)) = 1.0f;
		} else {
			*((float*)(item+0x1F4)) = modelScale.x;
			*((float*)(item+0x1F8)) = modelScale.y;
			*((float*)(item+0x1FC)) = modelScale.z;
		}
	}


	nw4r::lyt::Pane *shdRoot = shadow->rootPane;
	shdRoot->trans.x = N_stockItem->effectiveMtx[0][3];
	shdRoot->trans.y = N_stockItem->effectiveMtx[1][3];
	shdRoot->trans.z = N_stockItem->effectiveMtx[2][3];
	shdRoot->scale.x = N_stockItem_01->effectiveMtx[0][0];
	shdRoot->scale.y = N_stockItem_01->effectiveMtx[1][1];

	for (int i = 0; i < 7; i++)
		shadow->buttonBases[i]->scale = newButtonBase[i]->scale;
	shadow->hammerButtonBase->scale = newButtonBase[7]->scale;
	shadow->bubbleButtonBase->scale = newButtonBase[8]->scale;
	shadow->boomerButtonBase->scale = newButtonBase[9]->scale;
}

extern "C" void *SoundRelatedClass;
extern "C" void *MapSoundPlayer(void *SoundRelatedClass, int soundID, int unk);

void NewItemsBeforeStarAndMini(dStockItem_c *self, int *count) {
	int selectedMaybe;

	if (self->actionTaken == 1) { //going left
		while (*count > -1) {
			switch (*count) {
				default:
					selectedMaybe = *count - 1;
					*count = selectedMaybe;
					break;
				case 5: //mini
					selectedMaybe = 7;
					*count = 7; //go to hammer
					break;
				case 7: //hammer
					selectedMaybe = 9;
					*count = 9; //go to boomerang
					break;
				case 8: //Bubble
					selectedMaybe = 4;
					*count = 4; //go to pengi
					break;
			}
			if ((selectedMaybe > -1) && (self->newCounts[selectedMaybe] != 0)) {
				return;
			}
		}
	} else { //going right
		while (*count < 10) {
			switch (*count) {
				default:
					selectedMaybe = *count + 1;
					*count = selectedMaybe;
					break;
				case 4: //pengi
					selectedMaybe = 8;
					*count = 8; //go to bubble
					break;
				case 7: //hammer
					selectedMaybe = 5;
					*count = 5; //go to mini
					break;
				case 9: //boomer
					selectedMaybe = 7;
					*count = 7; //go to hammer
					break;
				case 6:
					*count = self->selectedItem;
					MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
					return;
			}
			if ((selectedMaybe < 10) && (self->newCounts[selectedMaybe] != 0)) {
				return;
			}
		}
	}
	*count = self->selectedItem;
	MapSoundPlayer(SoundRelatedClass, SE_SYS_INVALID, 1);
	return;
}


