#include <ReAnimation/ReAnimationParser.hpp>

using json = nlohmann::json;

const float ReAnimationParser::DEFAULT_VALUE_PLACEHOLDER = -1000;

void ReAnimationParser::parse(ReAnimationDefinition &reAnimDef, std::string reAnimPath) {
  

  std::ifstream file(reAnimPath);

  json data = json::parse(file);

  reAnimDef.fps = data["fps"];

  //std::cout << "x: " << data["hitbox"]["x"] << "\n";

  if(data.contains("hitbox"))
    reAnimDef.hitbox = sf::FloatRect(
      { data["hitbox"]["x"], data["hitbox"]["y"] },
      { data["hitbox"]["w"], data["hitbox"]["h"] }
      );
  if (data.contains("offset"))
    reAnimDef.offset = sf::Vector2f(
      data["offset"]["x"], data["offset"]["y"]);


  for (auto label : data["labels"]) {
    reAnimDef.labels.push({ label["name"], label["start"], label["end"] });
  }

  //Transform defaultTrans = { -10000, -10000 , -10000 , -10000 , -10000 , -10000 , -10000 , -10000 , ""};

  reAnimDef.totalTracks = data["tracks"].size();
  for (auto track : data["tracks"]) {
    Track t;
    t.name = track["name"];
    Transform newTrans;
    // For each track
    int frameCounter = 0;
    for (auto trans : track["transforms"]) {
      frameCounter++;
      reAnimDef.frameCount = std::max(reAnimDef.frameCount, frameCounter);
      // Set known values and default values for missing ones
      if (trans.count("x")) newTrans.x = trans["x"];
      else newTrans.x = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("y")) newTrans.y = trans["y"];
      else newTrans.y = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("sx")) newTrans.sx = trans["sx"];
      else newTrans.sx = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("sy")) newTrans.sy = trans["sy"];
      else newTrans.sy = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("kx")) newTrans.kx = trans["kx"];
      else newTrans.kx = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("ky")) newTrans.ky = trans["ky"];
      else newTrans.ky = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("f")) newTrans.f = trans["f"];
      else newTrans.f = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("a")) newTrans.a = trans["a"];
      else newTrans.a = DEFAULT_VALUE_PLACEHOLDER;

      if (trans.count("i")) newTrans.i = reAnimDef.textureMap[trans["i"]];
      else newTrans.i = nullptr;

      t.transforms.push_back(newTrans);

    }
    //std::cout << "PUSHED TRACK: " << t.name << " with " << frameCounter << " Frames\n";
    reAnimDef.tracks.push(t);
  }

  //re.report();
  // inherit missing transform data
  Transform prev;
  for (int i = 0; i < reAnimDef.tracks.size; i++) {
    Track &track = reAnimDef.tracks[i];
    prev = { 0, 0, 1, 1, 0, 0, 0, 1, nullptr };
    for (auto &trans : track.transforms) {
      inheritData(prev, trans);
    }
  }

  file.close();
}

void ReAnimationParser::inheritData(Transform &prev, Transform &data) {
  fillMissingData(prev.x, data.x);
  fillMissingData(prev.y, data.y);
  fillMissingData(prev.sx, data.sx);
  fillMissingData(prev.sy, data.sy);
  fillMissingData(prev.kx, data.kx);
  fillMissingData(prev.ky, data.ky);
  fillMissingData(prev.f, data.f);
  fillMissingData(prev.a, data.a);

  if (data.i == nullptr)
    data.i = prev.i;
  else
    prev.i = data.i;

}

void ReAnimationParser::fillMissingData(float &prev, float &data) {
  if (data == DEFAULT_VALUE_PLACEHOLDER)
    data = prev; // inherit prev data
  else
    prev = data; // update prev data
}


void ReAnimationParser::bindAllParents(std::string reAnimPath, ReAnimationDefinition &def) {
  std::ifstream file(reAnimPath);

  json data = json::parse(file);


  if (data.contains("families"))
    for (auto family : data["families"]) {
      //std::cout << "child: " << family["child"] << "\n";
      //std::cout << "parent: " << family["parent"] << "\n";

      //def.trackMap[family["child"]]->parent = def.trackMap[family["parent"]];
      //def.trackMap[family["child"]]->fullInherit = family["FullInherit"];

      def.tracks[def.getTrackIndex(family["child"])].parent = &def.tracks[def.getTrackIndex(family["parent"])];
      def.tracks[def.getTrackIndex(family["child"])].fullInherit = family["FullInherit"];
      //std::cout << "Done\n";
    }

  // Bind "next" pointer

  for (auto lab : data["labels"]) {
    //std::cout << "X: " << lab["next"] << "\n";
    if (lab.contains("next")) {
      //std::cout << "Set up next for " << lab["name"] << " = " << 
      def.labels[def.getLabelIndex(lab["name"])].next = lab["next"];
    }
  }

}

void ReAnimationParser::reportImageMap(std::string reAnimPath, std::string assetPath) {
  std::ifstream file(reAnimPath);

  json data = json::parse(file);

  std::vector<int> activeFrames;

  std::cout << "Tracks:\n";
  std::cout << "{";
  for (auto track : data["tracks"])
    std::cout << track["name"] << ", ";
  std::cout << "}\n\n";


  std::cout << "Image Map:\n";
  std::cout << "\n{\n";
  std::set<std::string> seenIMGs;
  for (auto track : data["tracks"])
    for (auto keyF : track["transforms"])
      if (keyF.contains("i") && !seenIMGs.count(keyF["i"])) {
        std::cout << "{" << keyF["i"] << ", \"" << assetPath << "\"},\n";
        seenIMGs.insert(keyF["i"]);
      }
  std::cout << "}\n";

  bool currState;
  int i;
  for (auto track : data["tracks"]) {
    currState = true;
    i = 0;
    for (auto kf : track["transforms"]) {
      if (kf.contains("f") && kf["f"] == 0) currState = true;
      else if (kf.contains("f") && kf["f"] == -1) currState = false;
      if (currState) activeFrames.push_back(i);
      i++;
    }

    std::cout << "{\"name\" : " << track["name"] << ", \"start\" : " << activeFrames.front() << ", \"end\" : " << activeFrames.back() << "}\n";
    activeFrames.clear();
  }

}
