#include <game.h>

static const int fieldLength = 2;

extern "C" void colorizeMarioLives(dCharacterChangeSelectContents_c *self, int funcRun) {
	if (funcRun == 0) {
		self->T_kakeru_00->colour2 = marioColor;
		self->T_zanki_01->colour2 = marioColor;
	} else {
		self->T_kakeru_01->colour2 = marioColor;
		self->T_zanki_00->colour2 = marioColor;
	}
}

void dCharacterChangeSelectContents_c::colorizeLuigiLives(int funcRun) {
	if (funcRun == 0) {
		WriteNumberToTextBox(&luigiLives, &fieldLength, T_zanki_01, false);
		T_kakeru_00->colour2 = luigiColor;
		T_zanki_01->colour2 = luigiColor;
		P_lui_00->SetVisible(true);
	} else {
		WriteNumberToTextBox(&luigiLives, &fieldLength, T_zanki_00, false);
		T_kakeru_01->colour2 = luigiColor;
		T_zanki_00->colour2 = luigiColor;
		P_lui_01->SetVisible(true);
	}
}

void dCharacterChangeSelectContents_c::colorizeYellowToadLives(int funcRun) {
	if (funcRun == 0) {
		WriteNumberToTextBox(&yellowToadLives, &fieldLength, T_zanki_01, false);
		T_kakeru_00->colour2 = kinoYColor;
		T_zanki_01->colour2 = kinoYColor;
		P_kinoY_00->SetVisible(true);
	} else {
		WriteNumberToTextBox(&yellowToadLives, &fieldLength, T_zanki_00, false);
		T_kakeru_01->colour2 = kinoYColor;
		T_zanki_00->colour2 = kinoYColor;
		P_kinoY_01->SetVisible(true);
	}
}

void dCharacterChangeSelectContents_c::colorizeBlueToadLives(int funcRun) {
	if (funcRun == 0) {
		WriteNumberToTextBox(&blueToadLives, &fieldLength, T_zanki_01, false);
		T_kakeru_00->colour2 = kinoBColor;
		T_zanki_01->colour2 = kinoBColor;
		P_kinoB_00->SetVisible(true);
	} else {
		WriteNumberToTextBox(&blueToadLives, &fieldLength, T_zanki_00, false);
		T_kakeru_01->colour2 = kinoBColor;
		T_zanki_00->colour2 = kinoBColor;
		P_kinoB_01->SetVisible(true);
	}
}