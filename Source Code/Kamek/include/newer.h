#ifndef NEWER_H
#define NEWER_H 

extern int lastLevelIDs[];

void WriteAsciiToTextBox(nw4r::lyt::TextBox *tb, const char *source);

int getUnspentStarCoinCount();
int getStarCoinCount();
void getNewerLevelNumberString(int world, int level, wchar_t *dest);

const wchar_t *getWorldNumber(int world);
const wchar_t *getLevelNumber(int world, int level);

#endif /* NEWER_H */
