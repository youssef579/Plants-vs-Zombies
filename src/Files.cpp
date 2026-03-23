#include <Files.hpp>
#include <globals.hpp>



//Level Selector
int maxLevelUnlocked = 1;
std::string levelSelectorNames[LEVEL_SELECTOR_MAX_PAGES][LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE];
sf::Color levelSelectorColors[LEVEL_SELECTOR_MAX_PAGES][LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE];

int levelSelectorCurrentPage = 1;
void loadLevelsFile() {
	std::string data;
	std::ifstream levelsFile("storage/levels.txt");
	if (levelsFile.is_open()) {
		//std::getline(levelsFile, data);
		//maxLevelUnlocked = std::stoi(data);
		levelsFile >> maxLevelUnlocked;
	}
	else { //levels.txt not found
		levelsFile.close();
		std::ofstream levelsFile("storage/levels.txt");
		levelsFile << 1;
	}
	levelsFile.close();
}

void updateFiles() {
	std::ofstream levelsFile("storage/levels.txt");
	levelsFile << maxLevelUnlocked;
	levelsFile.close();
}

void createLevelSelectorVars() {
	//page number = ceil(i/5) = (i+5-1)/5 = (i+4)/5 [integer division]
	//level index in page (1 based) = i%5 (or 5 if i%5==0)
	for (int i = 1; i <= MAX_LEVELS; i++) {
		levelSelectorNames[(i + LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1) / LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE][i % LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE ? i % LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1 : LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1] = "Level " + std::to_string(i);
		if (i <= maxLevelUnlocked) levelSelectorColors[(i + LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1) / LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE][i % LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE ? i % LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1 : LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1] = UNLOCKED_LEVEL_COLOR;
		else levelSelectorColors[(i + LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1) / LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE][i % LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE ? i % LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1 : LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE - 1] = LOCKED_LEVEL_COLOR;
	}
}

void initFiles() {
	loadLevelsFile();
	createLevelSelectorVars();
	//load settings file

}