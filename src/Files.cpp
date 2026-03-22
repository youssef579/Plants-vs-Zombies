#include <files.hpp>
#include <globals.hpp>
int maxLevelUnlocked = 1;
std::string levelSelectorNames[LEVEL_SELECTOR_MAX_PAGES][LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE];
sf::Color levelSelectorColors[LEVEL_SELECTOR_MAX_PAGES][LEVEL_SELECTOR_MAX_LEVELS_PER_PAGE];

int levelSelectorCurrentPage = 1;
void loadLevelsFile() {
	std::string data;
	std::ifstream levelsFile("storage/levels.txt");
	if (levelsFile.is_open()) {
		std::getline(levelsFile, data);
		maxLevelUnlocked = std::stoi(data);
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
}

void createLevelSelectorVars() {
	for (int i = 1; i <= MAX_LEVELS; i++) {
		levelSelectorNames[(i + 4) / 5][i % 5 ? i % 5 - 1 : 4] = "Level " + std::to_string(i);
		if (i <= maxLevelUnlocked) levelSelectorColors[(i + 4) / 5][i % 5 ? i % 5 - 1 : 4] = UNLOCKED_LEVEL_COLOR;
		else levelSelectorColors[(i + 4) / 5][i % 5 ? i % 5 - 1 : 4] = LOCKED_LEVEL_COLOR;
	}
}

void initFiles() {
	loadLevelsFile();
	createLevelSelectorVars();
	//load settings file

}