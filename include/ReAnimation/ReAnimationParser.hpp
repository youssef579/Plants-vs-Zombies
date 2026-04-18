#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <ReAnimation/ReAnimation.hpp>



struct ReAnimationParser {
  static const float DEFAULT_VALUE_PLACEHOLDER;

  static void parse(ReAnimationDefinition &reAnimDef, std::string reAnimPath);
  static void inheritData(Transform &prev, Transform &data);
  static void fillMissingData(float &prev, float &data);

  static void bindAllParents(std::string reAnimPath, ReAnimationDefinition &def);
  static void reportImageMap(std::string reAnimPath, std::string assetPath);

};
