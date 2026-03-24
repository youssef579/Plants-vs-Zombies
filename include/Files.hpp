#pragma once

void initFiles();
void loadLevelsFile();
void updateFiles();

const int MAX_LEVELS = 18;
const int LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE = 6;

extern int levelSelectorCurrentPage;
extern int maxLevelUnlocked;
