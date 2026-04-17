#include <ReAnimation/ReAnimation.hpp>
#include <ReAnimation/ReAnimationParser.hpp>

const float TO_RAD = 3.141592653589793f / 180.0f;

Array<ReAnimationDefinition*> definitions;

ReAnimator::ReAnimator(ReAnimationDefinition *def, float X, float Y, sf::RenderWindow *w) {
  reAnimDef = def;
  window = w;
  x = X + def->offset.x, y = Y + def->offset.y;
  trackInstances.resize(reAnimDef->totalTracks);
}

void ReAnimationDefinition::loadFiles(std::string reAnimPath, int tracksNum, std::string tracks[],
  std::unordered_map<std::string, std::string> images) {

  for (auto image : images) {
    //std::cout << "IMG: " << image.first << " -> " << image.second << "\n";
    textureMap[image.first] = new sf::Texture;
    textureMap[image.first]->loadFromFile(image.second);
  }

  ReAnimationParser::parse(*this, reAnimPath);

  createTrackMap();
  calculateBasePoses();
  ReAnimationParser::bindAllParents(reAnimPath, *this);


}


float ReAnimator::lerp(float a, float b, float t) { return (a + (b - a) * t); }

Transform ReAnimator::lerpTransform(Transform a, Transform b, float t) {
  return {
    lerp(a.x, b.x, t),
    lerp(a.y, b.y, t),
    lerp(a.sx, b.sx, t),
    lerp(a.sy, b.sy, t),
    lerp(a.kx, b.kx, t),
    lerp(a.ky, b.ky, t),
    (t < 0.5f) ? a.f : b.f,
    lerp(a.a, b.a, t),
    (t < 0.5f) ? a.i : b.i
  };
}

void ReAnimator::update(float dt) {

  float lastGroundX;
  if (curTransforms.count("_ground")) lastGroundX = curTransforms["_ground"].x;
  else lastGroundX = 0;

  timer += (dt * reAnimDef->fps * animSpeedMulti);
  //std::cout << "dt: " << dt << " / fps: " << reAnimDef->fps << " / animSpeedMulti: " << animSpeedMulti << "\n";
    //std::cout << "timer += " << (dt * reAnimDef->fps * animSpeedMulti) << "\n";
  //if(x == 400)
    //std::cout << "Time: " << timer << "\n";
    
  //std::cout << "Active Labels---------------------------------\n";

  /*for (auto label : reAnimDef->labels) {
    float labelTime = timer - label.start;

  }*/

  //float labelTime = (((int)label.start + (int)timer)%((int)label.end - (int)label.start));
  
  curTransforms.clear();

  std::vector<std::string> queuedLabels;
  for (auto lab = activeLabels.begin(); lab != activeLabels.end(); ) {
    if (!updateLabel(*lab)) {
      if (lab->label->next != "") {
        queuedLabels.push_back(lab->label->next);
      }
      lab = activeLabels.erase(lab);
    }
    else
      lab++;
  }

  for (auto ql : queuedLabels)
    playAnimation(ql, true);


  float deltaGround;
  if (curTransforms.count("_ground"))
    deltaGround = curTransforms["_ground"].x - lastGroundX;
  else deltaGround = 0;

  //std::cout << "DG: " << deltaGround << "\n";

  // TODO: set dg threshold
  if(deltaGround > 0.0f) x -= deltaGround;


  //std::cout << "END\n";
  //std::cout << "\n----------------------------\n";

  //Label label = reAnimDef->labels[0];
  //float labelTime = label.start + std::fmod(timer, (float)(label.end - label.start));

  //std::cout << "Lerping " << timer << " between " << (int)labelTime << " and " << (int)labelTime + 1 << " / t=" << timer - (int)timer << "\n";
  //for (auto track : reAnimDef->tracks) {
  //  if (track.transforms[(int)labelTime].f == -1) continue;
  //  /*Transform lerped = lerpTransform(track.transforms[(int)labelTime], track.transforms[(int)labelTime + 1], timer - (int)timer);
  //  dqi.transform = transformToSFML(lerped);
  //  dqi.a = lerped.a, dqi.f = lerped.f;
  //  dqi.i = lerped.i;*/
  //  curTransforms[track.name] = lerpTransform(track.transforms[(int)labelTime], track.transforms[(int)labelTime + 1], timer - (int)timer);

  //}

  //label = reAnimDef->labels[1];
  //labelTime = label.start + std::fmod(timer, (float)(label.end - label.start));
  //std::cout << "Lerping " << timer << " between " << (int)labelTime << " and " << (int)labelTime + 1 << " / t=" << timer - (int)timer << "\n";
  //for (auto track : reAnimDef->tracks) {
  //  if (track.transforms[(int)labelTime].f == -1) continue;
  //  curTransforms[track.name] = lerpTransform(track.transforms[(int)labelTime], track.transforms[(int)labelTime + 1], timer - (int)timer);
  //}
  
  
  //drawTransforms();

  /*std::cout << "Update: " << timer << " / labelTime: " << labelTime << "\n";

  sf::Transform finalMatrix;
  finalMatrix.translate({ x, y });
  finalMatrix = finalMatrix* sf::Transform(
    std::cos(0) * 1, std::sin(0) * 1, 0,
    -std::sin(0) * 1, std::cos(0) * 1, 0,
    0.0f, 0.0f, 1.0f
  );

  std::cout << "Track " << reAnimDef->tracks[7].name << "\n";
  std::cout << "IMG: " << reAnimDef->tracks[7].transforms[4].i << "\n";
  sf::Texture text = *reAnimDef->tracks[7].transforms[4].i;
  sf::Sprite sp(text);

  sf::RenderStates states;
  states.transform = finalMatrix;

  window->draw(sp, states);*/



  /*sf::Transform finalMatrix;
  finalMatrix.translate({ x, y });
  finalMatrix = finalMatrix * sf::Transform(
    std::cos(skewX) * t.sx, std::sin(skewY) * t.sy, t.x,
    -std::sin(skewX) * t.sx, std::cos(skewY) * t.sy, t.y,
    0.0f, 0.0f, 1.0f
  );*/


}


bool ReAnimator::updateLabel(ActiveLabel lab) {
  Label label = *lab.label;
  float labelOffseted = timer - lab.offset;
  float labelTime = label.start + std::fmod(labelOffseted, (float)(label.end - label.start));

  if (labelTime >= label.end - 1) {
    if(!lab.loop)
      return false; // Label will be Destroyed
    //labelTime = label.start -1 + 0.0001f;
    //labelTime = label.start;
    //labelOffseted = 0;
    //labelOffseted = 1;
    //return true;
  }
  //std::cout << "Update Label " << label.name << ": " << timer << " / labelTime: " << labelTime << "\n";
  for (auto track : reAnimDef->tracks) {
    if (track.transforms[(int)labelTime].f == -1) continue;
    //std::cout << "Frame " << labelTime << "\n";
    curTransforms[track.name] = lerpTransform(track.transforms[(int)labelTime], track.transforms[(int)labelTime + 1], labelOffseted - (int)labelOffseted);

  }
  return true;
}


//void ReAnimator::drawTransforms(const std::vector<Transform> &transforms) {
//
//  for (const auto &t : transforms) {
//    //std::cout << "Drawing " << "f(" << t.f << ") , " << t.x << ", " << t.y << " with scale " << t.sx << ", " << t.sy << " and skew " << t.kx << ", " << t.ky << " and alpha " << (int)(uint8_t)(t.a * 255.0f) << "\n";
//    //std::cout << "Image: " << (t.i ? "Texture Loaded" : "No Texture") << "\n";
//
//    /*sf::Texture *texture = t.i;
//    sf::Sprite sp(*texture);
//    window->draw(sp);
//    continue;*/
//
//    if (!t.i || t.a <= 0.0f) {
//      //std::cout << "Skip\n";
//      continue;
//    }
//
//    sf::Sprite sprite(*t.i);
//
//
//    uint8_t alpha255 = (uint8_t)(t.a * 255.0f);
//    sprite.setColor(sf::Color(255, 255, 255, alpha255));
//
//    float skewX = -t.kx * TO_RAD;
//    float skewY = -t.ky * TO_RAD;
//
//    sf::Transform localMatrix(
//      std::cos(skewX) * t.sx, std::sin(skewY) * t.sy, t.x,
//      -std::sin(skewX) * t.sx, std::cos(skewY) * t.sy, t.y,
//      0.0f, 0.0f, 1.0f
//    );
//
//    sf::Transform rootMatrix;
//    rootMatrix.translate({ x, y }).scale({sx, sy});
//
//    sf::Transform finalMatrix = rootMatrix * localMatrix;
//
//    sf::RenderStates states;
//    states.transform = finalMatrix;
//
//    window->draw(sprite, states);
//  }
//}

//void ReAnimator::drawTransforms() {
//  //std::cout << "\n---------------------------\nStarted Drawing\n";
//  sf::Transform rootMatrix;
//  rootMatrix.translate({ x, y });
//  rootMatrix.scale({ sx, sy });
//  for (int i = 0; i < reAnimDef->tracks.size(); i++) {
//    //if (!curTransforms.count(reAnimDef->tracks[i].name)) continue;
//    std::pair<std::string, Transform> t = { reAnimDef->tracks[i].name, curTransforms[reAnimDef->tracks[i].name]};
//    
//    //for (auto t : curTransforms) { // t.first = trackName, t.second = trackTransform
//      if (!t.second.i || t.second.a <= 0.0f) continue;
//
//      sf::Sprite sprite(*t.second.i);
//      uint8_t alpha255 = (uint8_t)(t.second.a * 255.0f);
//      sprite.setColor(sf::Color(255, 255, 255, alpha255));
//
//      sf::Transform localMatrix = transformToSFML(t.second);
//      sf::Transform finalMatrix = localMatrix; // default (no parent)
//
//      if (trackMap[t.first]->parent) { // has parent
//        Track *parentTrack = trackMap[t.first]->parent;
//
//        sf::Transform parentBasePose = basePoses[parentTrack->name];
//        sf::Transform parentLocal = transformToSFML(curTransforms[parentTrack->name]);
//        if (trackMap[t.first]->fullInherit) {
//          std::cout << t.first << " is Inheriting\n";
//          finalMatrix = (parentLocal * parentBasePose.getInverse() * localMatrix);
//        }
//        else {
//          std::cout << t.first << " is NOT Inheriting\n";
//          sf::Vector2f parentBaseWorld = (parentBasePose).transformPoint({ 0, 0 });
//          sf::Vector2f parentLocalWorld = (parentLocal).transformPoint({ 0, 0 });
//
//          sf::Transform attach;
//          attach.translate(parentLocalWorld - parentBaseWorld);
//          finalMatrix = (attach * localMatrix);
//        }
//
//
//        /*sf::Transform parentTranslation;
//        parentTranslation.translate(parentLocalV - parentBasePoseV + sf::Vector2f(0, 0));*/
//        //finalMatrix = parentTranslation * localMatrix;
//
//
//      }
//
//
//      sf::RenderStates states;
//      states.transform = rootMatrix * finalMatrix;
//      //if(t.first == "stalk_top")
//      //std::cout << "Draw " << t.first << "\n";
//      window->draw(sprite, states);
//
//    //}
//  }
//
//  //std::cout << "End Draw ------------------------------------\n";
//}



void ReAnimator::draw() {
  sf::Transform rootMatrix;
  rootMatrix.translate({ x, y });
  rootMatrix.scale({ sx, sy });


  effectiveBasePoses.clear();
  effectiveTransforms.clear();

  for (auto &trackDef : reAnimDef->tracks) {
    const std::string &name = trackDef.name;

    auto curIt = curTransforms.find(name);
    if (curIt == curTransforms.end()) continue;

    Transform &t = curIt->second;
    if (!t.i || t.a <= 0.0f) continue; // Skip no image / invisible

    //change loop later:
    int idx = 0;
    for (auto tr : reAnimDef->tracks)
      if (tr.name == name) {
        break;
      }
      else idx++;

    if (!trackInstances[idx].isVisible) continue;

    //std::cout << "idx: " << idx << "\n";
    //std::cout << "size: " << trackInstances.size() << "\n";

    sf::Sprite *sprite;
    if(!trackInstances[idx].imageOverride)
      sprite = new sf::Sprite(*t.i);
    else
      sprite = new sf::Sprite(*trackInstances[idx].imageOverride);

    /*if(trackInstances[idx].colorOverlay.a > 0.0f){
      uint8_t alpha255 = (uint8_t)(t.a * 255.0f);
      sprite->setColor(multiplyColor(trackInstances[idx].colorOverlay, trackInstances[idx].colorOverlayIntensity));
    }*/
    //else {
      uint8_t alpha255 = (uint8_t)(t.a * 255.0f * opacityMultiplier);
      sprite->setColor(sf::Color(255, 255, 255, alpha255));
    //}

    

    sf::Transform localMatrix = transformToSFML(t);
    sf::Transform finalLocal = localMatrix;

    if (trackDef.parent) {
      Track *parent = trackDef.parent;
      const std::string &pname = parent->name;

      if (trackDef.fullInherit) {
        //if (name == "anim_sprout")
          //std::cout << "Sprout is inheriting\n";
        finalLocal = getEffectiveTransform(name);
      }
      else {
        //if (name == "anim_sprout")
          //std::cout << "Sprout is NOT inheriting\n";
        sf::Transform parentCurEff = getEffectiveTransform(pname);
        sf::Transform parentBaseEff = getEffectiveBasePose(pname);

        //if (name == "anim_face") {
        //  //std::cout << "Effective Cur for " << pname << ": " << parentCurEff.getMatrix()[12] << ", " << parentCurEff.getMatrix()[13] << "\n";
        //  //std::cout << "Effective Base for " << pname << ": " << parentBaseEff.getMatrix()[12] << ", " << parentBaseEff.getMatrix()[13] << "\n";
        //}

        sf::Vector2f parentCurPoint = parentCurEff.transformPoint({ 0.0f, 0.0f });
        sf::Vector2f parentBasePoint = parentBaseEff.transformPoint({ 0.0f, 0.0f });

        sf::Transform attach;
        attach.translate(parentCurPoint - parentBasePoint);

        finalLocal = attach * localMatrix;
      }
    }

    sf::RenderStates states;

    //if (name == "anim_sprout")
      //debugTransform(finalLocal);

    states.transform = rootMatrix * finalLocal;
    //if(name == "anim_face")
    if (window) {
      window->draw(*sprite, states);
      if (trackInstances[idx].colorOverlay.a > 0.0f) {
        sprite->setColor(trackInstances[idx].colorOverlay);
        states.blendMode = sf::BlendAdd;
        window->draw(*sprite, states);
      }
    }

    delete sprite;
  }
}



sf::Transform ReAnimator::getEffectiveBasePose(std::string trackName) {

  if (effectiveBasePoses.count(trackName))
    return effectiveBasePoses[trackName];


  Track *track = reAnimDef->trackMap[trackName];
  sf::Transform base = reAnimDef->basePoses[trackName];


  if (track->parent && reAnimDef->basePoses.find(track->parent->name) != reAnimDef->basePoses.end()) {
    sf::Transform parentEff = getEffectiveBasePose(track->parent->name);
    sf::Transform parentBaseInv = reAnimDef->basePoses[track->parent->name].getInverse();
    sf::Transform res = parentEff * parentBaseInv * base;
    effectiveBasePoses[trackName] = res;
  }
  else {
    effectiveBasePoses[trackName] = base;
  }
  return effectiveBasePoses[trackName];
}

sf::Transform ReAnimator::getEffectiveTransform(std::string trackName) {

  if (effectiveTransforms.count(trackName)) // if calculated before
    return effectiveTransforms[trackName];

  Track *track = reAnimDef->trackMap[trackName];

  sf::Transform local;
  auto curIt = curTransforms.find(trackName);
  if (curIt != curTransforms.end()) {
    local = transformToSFML(curIt->second);
  }
  else {
    auto baseIt = reAnimDef->basePoses.find(trackName);
    local = (baseIt != reAnimDef->basePoses.end()) ? baseIt->second : sf::Transform();
  }

  if (track->parent && reAnimDef->basePoses.find(track->parent->name) != reAnimDef->basePoses.end()) {
    sf::Transform parentEff = getEffectiveTransform(track->parent->name);
    sf::Transform parentBaseInv = reAnimDef->basePoses[track->parent->name].getInverse();
    sf::Transform res = parentEff * parentBaseInv * local;
    effectiveTransforms[trackName] = res;
  }
  else {
    effectiveTransforms[trackName] = local;
  }
  return effectiveTransforms[trackName];
}



int ReAnimator::getFirstValidIdx(Track &track) {
  for (int i = 0; i < track.transforms.size(); i++) {
    //std::cout << "Checking frame " << i << " with f=" << track.transforms[i].f << "\n";
    if (track.transforms[i].f != -1) {
      return i;
    }
  }
  return -1; // No valid frame found
}

sf::Vector2f ReAnimationDefinition::getBasePose(Track &track) {
  int baseIdx = ReAnimator::getFirstValidIdx(track); // 29

  if (baseIdx != -1) {
    //std::cout << "Track[" << track.name << "] base pose found at index " << baseIdx << " = {" << track.transforms[baseIdx].x << ", " << track.transforms[baseIdx].y << "}\n";
    return sf::Vector2f(track.transforms[baseIdx].x, track.transforms[baseIdx].y);
  }

  return sf::Vector2f(0, 0);
}

void ReAnimationDefinition::calculateBasePoses() {
  for (auto &track : tracks) {
    //basePoses[track.name] = transformToSFML((Track&)getBasePose(track));
    basePoses[track.name] = ReAnimator::transformToSFML(getBaseTransform(track));
  }
}

Transform ReAnimationDefinition::getBaseTransform(Track &track) {
  int baseIdx = ReAnimator::getFirstValidIdx(track);
  Transform t;
  t.x = 0; t.y = 0;
  t.sx = 1; t.sy = 1;
  t.kx = 0; t.ky = 0;
  t.f = 0; t.a = 1;
  t.i = nullptr;

  if (baseIdx == -1) return t;
  //std::cout << "CALCULATE BASE TRANSFORM for " << track.name << " at index " << baseIdx << "\n";

  /*t.x = track.transforms[baseIdx].x;
  t.y = track.transforms[baseIdx].y;*/

  return track.transforms[baseIdx];
  //return track.transforms[baseIdx];
}

sf::Transform ReAnimator::transformToSFML(Transform &t) {
  return sf::Transform(
    std::cos(-t.kx * TO_RAD) * t.sx, std::sin(-t.ky * TO_RAD) * t.sy, t.x,
    -std::sin(-t.kx * TO_RAD) * t.sx, std::cos(-t.ky * TO_RAD) * t.sy, t.y,
    0.0f, 0.0f, 1.0f
  );
}

void ReAnimationDefinition::createTrackMap() {
  for (auto &track : tracks) {
    trackMap[track.name] = &track;
  }
}

int ReAnimationDefinition::getLabelIndex(std::string labelName) {
  for (int i = 0; i < labels.size(); i++) {
    if (labels[i].name == labelName)
      return i;
  }
  return -1; // label not found
}

void ReAnimator::playLabel(std::string labelName, bool loop) {
  activeLabels.push_back({&reAnimDef->labels[reAnimDef->getLabelIndex(labelName)],
    loop, timer});
}

void ReAnimator::stopLabel(int labelIdx) {
  //if (activeLabels.find({ labelIdx , true }) != activeLabels.end())
    //activeLabels.erase(activeLabels.find({ labelIdx , true }));
  //if (activeLabels.find({ labelIdx , false }) != activeLabels.end())
    //activeLabels.erase(activeLabels.find({ labelIdx , false }));
}


//void ReAnimationDefinition::defineAnimation(std::string name, int labelCount, int labels[], bool loop) {
//  animations[name] = Animation();
//  animations[name].loop = loop;
//  for (int i = 0; i < labelCount; i++) {
//    animations[name].labels.push_back(labels[i]);
//  }
//}


void ReAnimator::playAnimation(std::string labelName, bool loop) {
  //for(auto lab : reAnimDef->labels[getLabelIndex(labelName)].labels)
  playLabel(reAnimDef->labels[reAnimDef->getLabelIndex(labelName)].name,
    loop);
}

void ReAnimator::stopAnimation(std::string labelName) {
  for (auto lab = activeLabels.begin(); lab != activeLabels.end(); ) {
    if (lab->label->name == labelName) {
      lab = activeLabels.erase(lab);
    }
    else
      lab++;
  }
}

void ReAnimator::setTrackVisibility(std::string trackName, bool newVisibility) {
  for(int i=0; i<reAnimDef->tracks.size(); i++)
    if (reAnimDef->tracks[i].name == trackName) {
      trackInstances[i].isVisible = newVisibility;
      return;
    }
}

void ReAnimator::setTrackVisibility(std::vector<std::string> trackNames, bool newVisibility) {
  for (int i = 0; i < reAnimDef->tracks.size(); i++)
    for(auto trackName : trackNames)
      if (reAnimDef->tracks[i].name == trackName) {
        trackInstances[i].isVisible = newVisibility;
        return;
      }
}


//void ReAnimator::replaceWithQueueAnimation(std::string labelNameA, std::string labelNameB) {
//  stopAnimation(labelNameA);
//  playAnimation(labelNameB);
//}

//void ReAnimator::replaceAnimation(std::string targetName, std::string newAnimName) {
//  for (auto &lab : reAnimDef->animations[targetName].labels) {
//    //std::cout << "? " << lab.label->name << " vs " << targetName << "\n";
//    //if (lab.label->name == targetName) {
//      //std::cout << "Erased " << lab.label->name << " for " << newAnimName << "\n";
//      for(auto it : activeLabels)
//        if(it.label->name)
//      //ActiveLabel *newLab = new ActiveLabel;
//      //for (auto &defLab : reAnimDef->labels) {
//        //if (defLab.name == newAnimName) {
//          //newLab->label = &defLab;
//          //break;
//      break;
//
//        //}
//      }
//
//      //newLab->loop = false;
//      //newLab->offset = lab.offset;
//    }
//
//
//  for (auto lab : reAnimDef->animations[newAnimName].labels) {
//    std::cout << "playing new label " << lab << " for " << newAnimName << "\n";
//    playLabel(lab, reAnimDef->animations[newAnimName].loop);
//  }
//}


void ReAnimator::forceSyncAll() {
  for (auto &lab : activeLabels) {
    lab.offset = timer;
  }
}

sf::Color ReAnimator::multiplyColor(sf::Color color, float multiplier) {
  return sf::Color(
    color.r * multiplier,
    color.g * multiplier,
    color.b * multiplier,
    color.a
  );
}

void ReAnimator::setScale(float SX, float SY) {
  sx = SX, sy = SY;
}

void ReAnimator::setOpacity(uint8_t newOpacity) {
  opacityMultiplier = newOpacity;
}


void ReAnimator::report() {
  std::cout << "FPS: " << reAnimDef->fps << "\n";

  std::cout << "Labels:\n";
  for (auto label : reAnimDef->labels) {
    std::cout << "Label(" << label.name << "): " << label.start << " -> " << label.end << "\n";
  }

  std::cout << "\n----------------------------------\nTracks:\n";
  for (auto track : reAnimDef->tracks) {
    std::cout << "Track(" << track.name << "):\n";
    if (track.name != "stalk_bottom") continue;
    for (auto &frame : track.transforms) {
      std::cout << "  Transform: x=" << frame.x << ", y=" << frame.y << ", sx=" << frame.sx
        << ", sy=" << frame.sy << ", kx=" << frame.kx << ", ky=" << frame.ky
        << ", f=" << frame.f << ", a=" << frame.a
        << ", i=" << (frame.i ? "Texture Loaded" : "No Texture") << "\n";
    }
    std::cout << "\n----------------------------------\n";
  }

}




void debugTransform(const sf::Transform &matrix) {
  const float TO_DEG = 180.0f / 3.141592653589793f;

  const float *m = matrix.getMatrix();

  float m00 = m[0];  // scaleX / skewX
  float m10 = m[1];  // scaleX / skewX
  float m01 = m[4];  // scaleY / skewY
  float m11 = m[5];  // scaleY / skewY
  float m02 = m[12]; // translateX
  float m12 = m[13]; // translateY

  float x = m02;
  float y = m12;

  float sx = std::sqrt(m00 * m00 + m10 * m10);
  float sy = std::sqrt(m01 * m01 + m11 * m11);


  float kx = std::atan2(m10, m00) * TO_DEG;
  float ky = -std::atan2(m01, m11) * TO_DEG;

  std::cout << "--- Debug  ---\n"
    << "Pos:   x=" << x << ", y=" << y << "\n"
    << "Scale: sx=" << sx << ", sy=" << sy << "\n"
    << "Skew:  kx=" << kx << ", ky=" << ky << "\n"
    << "---------------------------\n";
}



void initReAnimDefs() {
  ReAnimationDefinition *def = new ReAnimationDefinition;
  std::string trackNames_sun[] = { "Sun1", "Sun2", "Sun3" };
  def->loadFiles("assets/Sun/Sun.json", 3, trackNames_sun, {
    {"IMAGE_REANIM_SUN3", "assets/Sun/Sun3.png"},
    {"IMAGE_REANIM_SUN2", "assets/Sun/Sun2.png"},
    {"IMAGE_REANIM_SUN1", "assets/Sun/Sun1.png"}
    });
  definitions.push(def);

  def = new ReAnimationDefinition;
  std::string trackNames_sunflower[] = {"backleaf", "backleaf_left_tip", "backleaf_right_tip",
  "stalk_bottom", "stalk_top", "frontleaf", "frontleaf_right_tip", "frontleaf_left_tip",
  "SunFlower_leftpetal8", "SunFlower_leftpetal7", "SunFlower_leftpetal6",
  "SunFlower_leftpetal5", "SunFlower_leftpetal4", "SunFlower_leftpetal3",
  "SunFlower_leftpetal2", "SunFlower_leftpetal1", "SunFlower_bottompetals",
  "SunFlower_rightpetal9", "SunFlower_rightpetal8", "SunFlower_rightpetal7",
  "SunFlower_rightpetal6", "SunFlower_rightpetal5", "SunFlower_rightpetal4",
  "SunFlower_rightpetal3", "SunFlower_rightpetal2", "SunFlower_rightpetal1",
  "SunFlower_toppetals", "anim_idle", "anim_blink"};
  def->loadFiles("assets/Plants/sunflower/sunflower.json", 29, trackNames_sunflower, {
    {"IMAGE_REANIM_PEASHOOTER_BACKLEAF",           "assets/Plants/sunflower/PeaShooter_backleaf.png"},
    {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_LEFTTIP",   "assets/Plants/sunflower/PeaShooter_backleaf_lefttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_RIGHTTIP",  "assets/Plants/sunflower/PeaShooter_backleaf_righttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_STALK_BOTTOM",       "assets/Plants/sunflower/PeaShooter_stalk_bottom.png"},
    {"IMAGE_REANIM_PEASHOOTER_STALK_TOP",          "assets/Plants/sunflower/PeaShooter_stalk_top.png"},
    {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF",          "assets/Plants/sunflower/PeaShooter_frontleaf.png"},
    {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_RIGHTTIP", "assets/Plants/sunflower/PeaShooter_frontleaf_righttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_LEFTTIP",  "assets/Plants/sunflower/PeaShooter_frontleaf_lefttip.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL8",          "assets/Plants/sunflower/SunFlower_leftpetal8.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL7",          "assets/Plants/sunflower/SunFlower_leftpetal7.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL6",          "assets/Plants/sunflower/SunFlower_leftpetal6.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL5",          "assets/Plants/sunflower/SunFlower_leftpetal5.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL4",          "assets/Plants/sunflower/SunFlower_leftpetal4.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL3",          "assets/Plants/sunflower/SunFlower_leftpetal3.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL2",          "assets/Plants/sunflower/SunFlower_leftpetal2.png"},
    {"IMAGE_REANIM_SUNFLOWER_LEFTPETAL1",          "assets/Plants/sunflower/SunFlower_leftpetal1.png"},
    {"IMAGE_REANIM_SUNFLOWER_BOTTOMPETALS",        "assets/Plants/sunflower/SunFlower_bottompetals.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL9",         "assets/Plants/sunflower/SunFlower_rightpetal9.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL8",         "assets/Plants/sunflower/SunFlower_rightpetal8.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL7",         "assets/Plants/sunflower/SunFlower_rightpetal7.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL6",         "assets/Plants/sunflower/SunFlower_rightpetal6.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL5",         "assets/Plants/sunflower/SunFlower_rightpetal5.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL4",         "assets/Plants/sunflower/SunFlower_rightpetal4.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL3",         "assets/Plants/sunflower/SunFlower_rightpetal3.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL2",         "assets/Plants/sunflower/SunFlower_rightpetal2.png"},
    {"IMAGE_REANIM_SUNFLOWER_RIGHTPETAL1",         "assets/Plants/sunflower/SunFlower_rightpetal1.png"},
    {"IMAGE_REANIM_SUNFLOWER_TOPPETALS",           "assets/Plants/sunflower/SunFlower_toppetals.png"},
    {"IMAGE_REANIM_SUNFLOWER_HEAD",                "assets/Plants/sunflower/SunFlower_head.png"},
    {"IMAGE_REANIM_SUNFLOWER_BLINK2",              "assets/Plants/sunflower/SunFlower_blink2.png"},
    {"IMAGE_REANIM_SUNFLOWER_BLINK1",              "assets/Plants/sunflower/SunFlower_blink1.png"}
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_peashooter[] = { "anim_idle", "anim_head_idle", "anim_shooting",
    "anim_full_idle", "backleaf", "backleaf_left_tip", "backleaf_right_tip", "stalk_bottom",
    "stalk_top", "frontleaf", "frontleaf_right_tip", "frontleaf_tip_left", "anim_stem",
    "anim_sprout", "anim_face", "idle_mouth", "idle_shoot_blink", "anim_blink" };

  def->loadFiles("assets/Plants/peashooter/peashooter.json", 18, trackNames_peashooter, {
      {"IMAGE_REANIM_PEASHOOTER_HEAD",               "assets/Plants/peashooter/PeaShooter_Head.png"},
      {"IMAGE_REANIM_PEASHOOTER_MOUTH",              "assets/Plants/peashooter/PeaShooter_mouth.png"},
      {"IMAGE_REANIM_PEASHOOTER_BACKLEAF",           "assets/Plants/peashooter/PeaShooter_backleaf.png"},
      {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_RIGHTTIP",  "assets/Plants/peashooter/PeaShooter_backleaf_righttip.png"},
      {"IMAGE_REANIM_PEASHOOTER_STALK_BOTTOM",       "assets/Plants/peashooter/PeaShooter_stalk_bottom.png"},
      {"IMAGE_REANIM_PEASHOOTER_STALK_TOP",          "assets/Plants/peashooter/PeaShooter_stalk_top.png"},
      {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF",          "assets/Plants/peashooter/PeaShooter_frontleaf.png"},
      {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_RIGHTTIP", "assets/Plants/peashooter/PeaShooter_frontleaf_righttip.png"},
      {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_LEFTTIP",  "assets/Plants/peashooter/PeaShooter_frontleaf_lefttip.png"},
      {"IMAGE_REANIM_PEASHOOTER_BLINK1",             "assets/Plants/peashooter/PeaShooter_blink1.png"},
      {"IMAGE_REANIM_PEASHOOTER_BLINK2",             "assets/Plants/peashooter/PeaShooter_blink2.png"},
      {"IMAGE_REANIM_ANIM_SPROUT",                   "assets/Plants/peashooter/PeaShooter_sprout.png"}
    });
  definitions.push(def);

  



  def = new ReAnimationDefinition;
  std::string trackNames_wallnut[] = { "_ground", "anim_idle", "anim_face", "anim_blink_twitch",
      "anim_blink_twice", "anim_blink_thrice" };

  def->loadFiles("assets/Plants/wallnut/wallnut.json", 6, trackNames_wallnut, {
    {"IMAGE_REANIM_WALLNUT_BODY",     "assets/Plants/wallnut/Wallnut_body.png"},
    {"IMAGE_REANIM_WALLNUT_TWITCH",   "assets/Plants/wallnut/Wallnut_twitch.png"},
    {"IMAGE_REANIM_WALLNUT_BLINK1",   "assets/Plants/wallnut/Wallnut_blink1.png"},
    {"IMAGE_REANIM_WALLNUT_BLINK2",   "assets/Plants/wallnut/Wallnut_blink2.png"},

    {"IMAGE_REANIM_WALLNUT_CRACKED1", "assets/Plants/wallnut/Wallnut_cracked1.png"},
    {"IMAGE_REANIM_WALLNUT_CRACKED2", "assets/Plants/wallnut/Wallnut_cracked2.png"}
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_snowpea[] = { "anim_full_idle", "anim_shooting", "anim_head_idle",
    "anim_idle", "backleaf", "backleaf_left_tip", "backleaf_right_tip", "stalk_bottom",
    "stalk_top", "frontleaf", "frontleaf_right_tip", "frontleaf_tip_left", "anim_stem",
    "SnowPea_crystals1", "SnowPea_crystals2", "SnowPea_crystals3", "anim_face", "SnowPea_mouth",
    "idle_shoot_blink", "anim_blink" };
  def->loadFiles("assets/Plants/snowpea/snowpea.json", 20, trackNames_snowpea, {
      {"IMAGE_REANIM_PEASHOOTER_BACKLEAF",           "assets/Plants/peashooter/PeaShooter_backleaf.png"},
      {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_LEFTTIP",   "assets/Plants/peashooter/PeaShooter_frontleaf_lefttip.png"},
      {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_RIGHTTIP",  "assets/Plants/peashooter/PeaShooter_backleaf_righttip.png"},
      {"IMAGE_REANIM_PEASHOOTER_STALK_BOTTOM",       "assets/Plants/peashooter/PeaShooter_stalk_bottom.png"},
      {"IMAGE_REANIM_PEASHOOTER_STALK_TOP",          "assets/Plants/peashooter/PeaShooter_stalk_top.png"},
      {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF",          "assets/Plants/peashooter/PeaShooter_frontleaf.png"},
      {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_RIGHTTIP", "assets/Plants/peashooter/PeaShooter_frontleaf_righttip.png"},
      {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_LEFTTIP",  "assets/Plants/peashooter/PeaShooter_frontleaf_lefttip.png"},
      {"IMAGE_REANIM_SNOWPEA_CRYSTALS1",             "assets/Plants/snowpea/SnowPea_crystals1.png"},
      {"IMAGE_REANIM_SNOWPEA_CRYSTALS2",             "assets/Plants/snowpea/SnowPea_crystals2.png"},
      {"IMAGE_REANIM_SNOWPEA_CRYSTALS3",             "assets/Plants/snowpea/SnowPea_crystals3.png"},
      {"IMAGE_REANIM_SNOWPEA_HEAD",                  "assets/Plants/snowpea/SnowPea_head.png"},
      {"IMAGE_REANIM_SNOWPEA_MOUTH",                 "assets/Plants/snowpea/SnowPea_mouth.png"},
      {"IMAGE_REANIM_SNOWPEA_BLINK1",                "assets/Plants/snowpea/SnowPea_blink1.png"},
      {"IMAGE_REANIM_SNOWPEA_BLINK2",                "assets/Plants/snowpea/SnowPea_blink2.png"}
    });
  definitions.push(def);



  def = new ReAnimationDefinition;
  std::string trackNames_repeater[] = { "anim_full_idle", "anim_shooting", "anim_head_idle",
    "anim_idle", "backleaf", "backleaf_left_tip", "backleaf_right_tip", "stalk_bottom",
    "stalk_top", "frontleaf", "frontleaf_right_tip", "frontleaf_tip_left", "anim_stem",
    "idle_headleaf_farthest", "idle_headleaf_3rdfarthest", "idle_headleaf_2ndfarthest",
    "idle_headleaf_tip_bottom", "idle_headleaf_nearest", "idle_headleaf_tip_top", "anim_face",
    "idle_mouth", "idle_shoot_blink", "anim_blink", "PeaShooter_eyebrow" };
  def->loadFiles("assets/Plants/repeater/repeater.json", 24, trackNames_repeater, {
    {"IMAGE_REANIM_PEASHOOTER_BACKLEAF",             "assets/Plants/repeater/PeaShooter_backleaf.png"},
    {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_LEFTTIP",     "assets/Plants/repeater/PeaShooter_backleaf_lefttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_BACKLEAF_RIGHTTIP",    "assets/Plants/repeater/PeaShooter_backleaf_righttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_STALK_BOTTOM",         "assets/Plants/repeater/PeaShooter_stalk_bottom.png"},
    {"IMAGE_REANIM_PEASHOOTER_STALK_TOP",            "assets/Plants/repeater/PeaShooter_stalk_top.png"},
    {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF",            "assets/Plants/repeater/PeaShooter_frontleaf.png"},
    {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_RIGHTTIP",   "assets/Plants/repeater/PeaShooter_frontleaf_righttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_FRONTLEAF_LEFTTIP",    "assets/Plants/repeater/PeaShooter_frontleaf_lefttip.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEADLEAF_FARTHEST",    "assets/Plants/repeater/PeaShooter_headleaf_farthest.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEADLEAF_3RDFARTHEST", "assets/Plants/repeater/PeaShooter_headleaf_3rdfarthest.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEADLEAF_NEAREST",     "assets/Plants/repeater/PeaShooter_headleaf_nearest.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEADLEAF_TIP_BOTTOM",  "assets/Plants/repeater/PeaShooter_headleaf_tip_bottom.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEADLEAF_2RDFARTHEST", "assets/Plants/repeater/PeaShooter_headleaf_2rdfarthest.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEADLEAF_TIP_TOP",     "assets/Plants/repeater/PeaShooter_headleaf_tip_top.png"},
    {"IMAGE_REANIM_PEASHOOTER_HEAD",                 "assets/Plants/repeater/PeaShooter_Head.png"},
    {"IMAGE_REANIM_PEASHOOTER_MOUTH",                "assets/Plants/repeater/PeaShooter_mouth.png"},
    {"IMAGE_REANIM_PEASHOOTER_BLINK1",               "assets/Plants/repeater/PeaShooter_blink1.png"},
    {"IMAGE_REANIM_PEASHOOTER_BLINK2",               "assets/Plants/repeater/PeaShooter_blink2.png"},
    {"IMAGE_REANIM_PEASHOOTER_EYEBROW",              "assets/Plants/repeater/PeaShooter_eyebrow.png"}
    });
  definitions.push(def);


}

ReAnimationDefinition* ReAnimator::getDefinition(ReAnimationDef defId) {
  return definitions[defId];
}



sf::FloatRect ReAnimator::getGlobalBounds() {
  /*return sf::FloatRect(
    { x + (reAnimDef->hitbox.position * sx),
    y + (localHitbox.top * sy) },
    { localHitbox.width * sx,
    localHitbox.height * sy }
  );*/
  return sf::FloatRect(
    {
      x + (reAnimDef->hitbox.position.x * sx) /*+ reAnimDef->offset.x*/,     // Position x
      y + (reAnimDef->hitbox.position.x * sy) /*+ reAnimDef->offset.y*/ },   // Position y
    {
      reAnimDef->hitbox.size.x * sx,               // size x
      reAnimDef->hitbox.size.y * sy                // size y
    }
  );
}

void ReAnimator::setOverlayAlpha(float newAlpha) {
  uint8_t castedAlpha = (uint8_t)(newAlpha * 255.0f);
  for (auto &trackI : trackInstances)
    trackI.colorOverlay.a = castedAlpha;
}

void ReAnimator::move(sf::Vector2f OFFSET) {
  x += OFFSET.x;
  y += OFFSET.y;
}

sf::Vector2f ReAnimator::getPosition() {
  return sf::Vector2f(
    x - reAnimDef->offset.x,
    y - reAnimDef->offset.y);
}

void ReAnimator::setPosition(sf::Vector2f newPos) {
  x = newPos.x + reAnimDef->offset.x;
  y = newPos.y + reAnimDef->offset.y;
}

bool ReAnimator::isPlayingAnimation(std::string animName) {
  for (auto lab : activeLabels)
    if (lab.label->name == animName)
      return true;
  return false;
}


void ReAnimator::drawHitbox() {
  sf::RectangleShape rec(getGlobalBounds().size);
  rec.setPosition({ getGlobalBounds().position.x, getGlobalBounds().position.y });
  rec.setFillColor(sf::Color(255, 0, 0, 100));
  window->draw(rec);
}
