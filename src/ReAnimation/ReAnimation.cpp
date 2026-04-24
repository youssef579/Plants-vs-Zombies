#include <ReAnimation/ReAnimation.hpp>
#include <ReAnimation/ReAnimationParser.hpp>

const float TO_RAD = 3.141592653589793f / 180.0f;

Array<ReAnimationDefinition*> definitions;
Array<ReAnimator> ReAnimator::orphanAnimators;

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
    playAnimation(ql, LoopType::PlayOnce);


  float deltaGround;
  if (curTransforms.count("_ground"))
    deltaGround = curTransforms["_ground"].x - lastGroundX;
  else deltaGround = 0;

  //std::cout << "DG: " << deltaGround << "\n";

  // TODO: set dg threshold
  if(deltaGround > 0.0f && allowMotion) x -= deltaGround;


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
  if (child)
    child->update(dt);

}


bool ReAnimator::updateLabel(ActiveLabel &lab) {
  Label label = *lab.label;
  float labelOffseted = timer - lab.offset;
  if (labelOffseted > label.end - label.start)
    lab.loopCount++;
  //if(lab.label->name == "anim_flame" || lab.label->name == "anim_done") std::cout << "loopCount: " << lab.loopCount << " / targetLoops: " << lab.targetLoops << "\n";
  if(lab.loop == LoopType::LoopTimes && lab.loopCount >= lab.targetLoops)
    return false; // Label will be Destroyed
    
  float labelTime = label.start + std::fmod(labelOffseted, (float)(label.end - label.start));

  //std::cout << "offs: " << labelOffseted << " / labEnd: " << label.end << "\n";
  //std::cout << "conditions: " << (labelOffseted >= label.end - 1) << "&" << (lab.loop == LoopType::HoldLastFrame) << "\n";
  if (labelOffseted >= label.end - label.start - 1 && lab.loop == LoopType::HoldLastFrame) {
    labelTime = label.end - 2;
    labelOffseted = label.end - label.start + 0.999f;
    if (timer - lab.offset - (label.end - label.start) >= lab.holdTimer * reAnimDef->fps)
      return false; // Label will be Destroyed
  }
  else if (labelTime >= label.end - 1) {
    if (lab.loop == LoopType::PlayOnce)
      return false; // Label will be Destroyed
  }
  //std::cout << "labelTime: " << labelTime << "\n";
  //std::cout << "Update Label " << label.name << ": " << timer << " / labelTime: " << labelTime << "\n";

  for (auto track : reAnimDef->tracks) {
    if (track.transforms[(int)labelTime].f == -1) continue;
    //std::cout << "Frame " << labelTime << "\n";
    //std::cout << "lerping (" << (int)labelTime << " -> " << (int)labelTime + 1 << ")\n";
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
  if (!hasParent) {
    rootMatrix = sf::Transform::Identity; // reset root
    rootMatrix.translate({ x, y });
    rootMatrix.scale({ sx, sy });
  }

  if (child) {
    //sf::Transform parCur;
    //sf::Transform parBase;
    if (childsParentTrack == "") {
      child->rootMatrix = rootMatrix;
    }
    else { // child inherits transformation of difference of parent
      sf::Transform parCur = getEffectiveTransform(childsParentTrack);
      sf::Transform parBase = getEffectiveBasePose(childsParentTrack);
      child->rootMatrix = rootMatrix * (parCur * parBase.getInverse());
    }
    child->draw();
  }

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

    //std::cout << "Track[" << name << "]: " << idx << "\n";
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
      sprite->setColor(sf::Color(globalColor.r, globalColor.g, globalColor.b, alpha255));
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
        sprite->setColor({
            trackInstances[idx].colorOverlay.r,
            trackInstances[idx].colorOverlay.g,
            trackInstances[idx].colorOverlay.b,
            (uint8_t)(trackInstances[idx].colorOverlay.a * opacityMultiplier)
          });
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

sf::Transform ReAnimator::transformToSFML(Transform t) {
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

void ReAnimator::playLabel(std::string labelName, LoopType loop, float holdTimer) {
  activeLabels.push_back({ &reAnimDef->labels[reAnimDef->getLabelIndex(labelName)],
    loop, timer, holdTimer });
}

void ReAnimator::playLabel(std::string labelName, LoopType loop, int loopCnt) {
  ActiveLabel a;
  activeLabels.push_back({ &reAnimDef->labels[reAnimDef->getLabelIndex(labelName)],
    loop, timer, 0, 0, loopCnt });
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


void ReAnimator::playAnimation(std::string labelName, LoopType loop, float holdTimer) {
  //for(auto lab : reAnimDef->labels[getLabelIndex(labelName)].labels)
  playLabel(reAnimDef->labels[reAnimDef->getLabelIndex(labelName)].name,
    loop, holdTimer);
}
void ReAnimator::playAnimation(std::string labelName, LoopType loop, int loopCnt) {
  //for(auto lab : reAnimDef->labels[getLabelIndex(labelName)].labels)
  playLabel(reAnimDef->labels[reAnimDef->getLabelIndex(labelName)].name,
    loop, loopCnt);
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



  def = new ReAnimationDefinition;
  std::string trackNames_tallnut[] = { "anim_idle", "anim_blink_twice", "anim_blink_thrice" };
  def->loadFiles("assets/Plants/tallnut/tallnut.json", 24, trackNames_tallnut, {
      {"IMAGE_REANIM_TALLNUT_BODY", "assets/Plants/tallnut/Tallnut_body.png"},
      {"IMAGE_REANIM_TALLNUT_BLINK1", "assets/Plants/tallnut/Tallnut_blink1.png"},
      {"IMAGE_REANIM_TALLNUT_BLINK2", "assets/Plants/tallnut/Tallnut_blink2.png"},

    {"IMAGE_REANIM_TALLNUT_CRACKED1", "assets/Plants/tallnut/Tallnut_cracked1.png"},
    {"IMAGE_REANIM_TALLNUT_CRACKED2", "assets/Plants/tallnut/Tallnut_cracked2.png"}
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_zombieBasic[42] = { "anim_superlongdeath", "anim_swim", "anim_death2",
    "anim_death", "anim_walk2", "anim_walk", "anim_idle2", "anim_idle", "anim_eat",
    "anim_waterdeath", "anim_dance", "_ground", "anim_innerarm3", "anim_innerarm2",
    "anim_innerarm1", "Zombie_flaghand", "Zombie_innerarm_screendoor", "Zombie_neck",
    "anim_head1", "Zombie_innerleg_upper", "Zombie_innerleg_lower", "Zombie_innerleg_foot",
    "Zombie_outerleg_upper", "Zombie_outerleg_foot", "Zombie_outerleg_lower", "Zombie_body",
    "Zombie_duckytube", "Zombie_whitewater", "Zombie_tie", "anim_head2", "anim_tongue",
    "Zombie_mustache", "anim_screendoor", "Zombie_innerarm_screendoor_hand",
    "Zombie_outerarm_screendoor", "Zombie_outerarm_hand", "Zombie_outerarm_upper",
    "Zombie_whitewater2", "Zombie_outerarm_lower", "anim_hair", "anim_cone", "anim_bucket" };
  def->loadFiles("assets/Zombies/Basic/zombieBasic.json", 42, trackNames_zombieBasic, {
      {"IMAGE_REANIM_ZOMBIE_INNERARM_HAND",             "assets/Zombies/Basic/Zombie_innerarm_hand.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERARM_LOWER",           "assets/Zombies/Basic/Zombie_innerarm_lower.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERARM_UPPER",           "assets/Zombies/Basic/Zombie_innerarm_upper.png" },
      { "IMAGE_REANIM_ZOMBIE_FLAGHAND",                 "assets/Zombies/Basic/Zombie_flaghand.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERARM_SCREENDOOR",      "assets/Zombies/Basic/Zombie_innerarm_screendoor.png" },
      { "IMAGE_REANIM_ZOMBIE_NECK",                     "assets/Zombies/Basic/Zombie_neck.png" },
      { "IMAGE_REANIM_ZOMBIE_HEAD",                     "assets/Zombies/Basic/Zombie_head.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERLEG_UPPER",           "assets/Zombies/Basic/Zombie_innerleg_upper.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERLEG_LOWER",           "assets/Zombies/Basic/Zombie_innerleg_lower.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERLEG_FOOT",            "assets/Zombies/Basic/Zombie_innerleg_foot.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERLEG_UPPER",           "assets/Zombies/Basic/Zombie_outerleg_upper.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERLEG_FOOT",            "assets/Zombies/Basic/Zombie_outerleg_foot.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERLEG_LOWER",           "assets/Zombies/Basic/Zombie_outerleg_lower.png" },
      { "IMAGE_REANIM_ZOMBIE_BODY",                     "assets/Zombies/Basic/Zombie_body.png" },
      { "IMAGE_REANIM_ZOMBIE_DUCKYTUBE",                "assets/Zombies/Basic/Zombie_duckytube.png" },
      { "IMAGE_REANIM_ZOMBIE_DUCKYTUBE_INWATER",        "assets/Zombies/Basic/Zombie_duckytube_inwater.png" },
      { "IMAGE_REANIM_ZOMBIE_WHITEWATER1",              "assets/Zombies/Basic/Zombie_whitewater1.png" },
      { "IMAGE_REANIM_ZOMBIE_WHITEWATER2",              "assets/Zombies/Basic/Zombie_whitewater2.png" },
      { "IMAGE_REANIM_ZOMBIE_WHITEWATER3",              "assets/Zombies/Basic/Zombie_whitewater3.png" },
      { "IMAGE_REANIM_ZOMBIE_TIE",                      "assets/Zombies/Basic/Zombie_tie.png" },
      { "IMAGE_REANIM_ZOMBIE_JAW",                      "assets/Zombies/Basic/Zombie_jaw.png" },
      { "IMAGE_REANIM_ZOMBIE_TONGUE",                   "assets/Zombies/Basic/Zombie_tongue.png" },
      { "IMAGE_REANIM_ZOMBIE_MUSTACHE1",                "assets/Zombies/Basic/Zombie_mustache1.png" },
      { "IMAGE_REANIM_ZOMBIE_SCREENDOOR1",              "assets/Zombies/Basic/Zombie_screendoor1.png" },
      { "IMAGE_REANIM_ZOMBIE_INNERARM_SCREENDOOR_HAND", "assets/Zombies/Basic/Zombie_innerarm_screendoor_hand.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERARM_SCREENDOOR",      "assets/Zombies/Basic/Zombie_outerarm_screendoor.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERARM_HAND",            "assets/Zombies/Basic/Zombie_outerarm_hand.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERARM_HAND2",           "assets/Zombies/Basic/Zombie_outerarm_hand2.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER",           "assets/Zombies/Basic/Zombie_outerarm_upper.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERARM_UPPER2",          "assets/Zombies/Basic/Zombie_outerarm_upper2.png" },
      { "IMAGE_REANIM_ZOMBIE_SNORKLE_WHITEWATER3",      "assets/Zombies/Basic/Zombie_snorkle_whitewater1.png" },
      { "IMAGE_REANIM_ZOMBIE_SNORKLE_WHITEWATER1",      "assets/Zombies/Basic/Zombie_snorkle_whitewater2.png" },
      { "IMAGE_REANIM_ZOMBIE_SNORKLE_WHITEWATER2",      "assets/Zombies/Basic/Zombie_snorkle_whitewater3.png" },
      { "IMAGE_REANIM_ZOMBIE_OUTERARM_LOWER",           "assets/Zombies/Basic/Zombie_outerarm_lower.png" },
      { "IMAGE_REANIM_ZOMBIE_HAIR",                     "assets/Zombies/Basic/Zombie_hair.png" },
      { "IMAGE_REANIM_ZOMBIE_CONE1",                    "assets/Zombies/Basic/Zombie_cone1.png" },
      { "IMAGE_REANIM_ZOMBIE_CONE2",                    "assets/Zombies/Basic/Zombie_cone2.png" },
      { "IMAGE_REANIM_ZOMBIE_CONE3",                    "assets/Zombies/Basic/Zombie_cone3.png" },
      { "IMAGE_REANIM_ZOMBIE_BUCKET1",                  "assets/Zombies/Basic/Zombie_bucket1.png" },
      { "IMAGE_REANIM_ZOMBIE_BUCKET2",                  "assets/Zombies/Basic/Zombie_bucket2.png" },
      { "IMAGE_REANIM_ZOMBIE_BUCKET3",                  "assets/Zombies/Basic/Zombie_bucket3.png" }
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_flagpole[] = { "Zombie_flagpole", "Zombie_flag" };

  def->loadFiles("assets/Zombies/Basic/flagpole.json", 2, trackNames_flagpole, {
      {"IMAGE_REANIM_ZOMBIE_FLAGPOLE",     "assets/Zombies/Basic/Zombie_flagpole.png"},
      {"IMAGE_REANIM_ZOMBIE_FLAG1",        "assets/Zombies/Basic/Zombie_flag1.png"},
      {"IMAGE_REANIM_ZOMBIE_FLAG3",        "assets/Zombies/Basic/Zombie_flag3.png"}
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_cherrybomb[] = { "anim_explode", "anim_idle", "CherryBomb_leftstem", "CherryBomb_left1",
    "CherryBomb_left3", "CherryBomb_lefteye11", "CherryBomb_lefteye21", "CherryBomb_leftmouth1",
    "CherryBomb_rightstem", "CherryBomb_right1", "CherryBomb_right3", "CherryBomb_righteye11",
    "CherryBomb_righteye21", "CherryBomb_rightmouth1", "CherryBomb_leaf3", "CherryBomb_leaf2tip",
    "CherryBomb_leaf2", "CherryBomb_leaf1", "CherryBomb_leaf1tip" };
  def->loadFiles("assets/Plants/cherrybomb/cherryBomb.json", 19, trackNames_cherrybomb, {
      {"IMAGE_REANIM_CHERRYBOMB_LEFTSTEM",        "assets/Plants/cherrybomb/CherryBomb_leftstem.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEFT1",          "assets/Plants/cherrybomb/CherryBomb_left1.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEFT3",          "assets/Plants/cherrybomb/CherryBomb_left3.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEFTEYE11",      "assets/Plants/cherrybomb/CherryBomb_lefteye11.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEFTEYE21",      "assets/Plants/cherrybomb/CherryBomb_lefteye21.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEFTMOUTH1",     "assets/Plants/cherrybomb/CherryBomb_leftmouth1.png" },
      { "IMAGE_REANIM_CHERRYBOMB_RIGHTSTEM",      "assets/Plants/cherrybomb/CherryBomb_rightstem.png" },
      { "IMAGE_REANIM_CHERRYBOMB_RIGHT1",         "assets/Plants/cherrybomb/CherryBomb_right1.png" },
      { "IMAGE_REANIM_CHERRYBOMB_RIGHT3",         "assets/Plants/cherrybomb/CherryBomb_right3.png" },
      { "IMAGE_REANIM_CHERRYBOMB_RIGHTEYE11",     "assets/Plants/cherrybomb/CherryBomb_righteye11.png" },
      { "IMAGE_REANIM_CHERRYBOMB_RIGHTEYE21",     "assets/Plants/cherrybomb/CherryBomb_righteye21.png" },
      { "IMAGE_REANIM_CHERRYBOMB_RIGHTMOUTH1",    "assets/Plants/cherrybomb/CherryBomb_rightmouth1.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEAF3",          "assets/Plants/cherrybomb/CherryBomb_leaf3.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEAF2TIP",       "assets/Plants/cherrybomb/CherryBomb_leaf2tip.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEAF2",          "assets/Plants/cherrybomb/CherryBomb_leaf2.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEAF1",          "assets/Plants/cherrybomb/CherryBomb_leaf1.png" },
      { "IMAGE_REANIM_CHERRYBOMB_LEAF1TIP",       "assets/Plants/cherrybomb/CherryBomb_leaf1tip.png" }
    });
  definitions.push(def);
  

  def = new ReAnimationDefinition;
  std::string trackNames_zombieCharred[] = { "anim_blink", "anim_crumble", "Zombie_charred_body",
    "Zombie_charred_tail", "Layer 10", "Zombie_charred_pile2", "Zombie_charred_pile2",
    "Zombie_charred_pile1", "Zombie_charred_pile1.2", "Zombie_charred_head", "Zombie_hair",
    "Zombie_charred_blink" };

  def->loadFiles("assets/Zombies/Charred/zombie/zombie.json", 12, trackNames_zombieCharred, {
    {"IMAGE_REANIM_ZOMBIE_CHARRED1",          "assets/Zombies/Charred/zombie/Zombie_charred1.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED2",          "assets/Zombies/Charred/zombie/Zombie_charred2.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED3",          "assets/Zombies/Charred/zombie/Zombie_charred3.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED4",          "assets/Zombies/Charred/zombie/Zombie_charred4.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED5",          "assets/Zombies/Charred/zombie/Zombie_charred5.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED6",          "assets/Zombies/Charred/zombie/Zombie_charred6.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED7",          "assets/Zombies/Charred/zombie/Zombie_charred7.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED8",          "assets/Zombies/Charred/zombie/Zombie_charred8.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED9",          "assets/Zombies/Charred/zombie/Zombie_charred9.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED10",         "assets/Zombies/Charred/zombie/Zombie_charred10.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED_TAIL",      "assets/Zombies/Charred/zombie/Zombie_charred_tail.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED_PILE2",     "assets/Zombies/Charred/zombie/Zombie_charred_pile2.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED_PILE1",     "assets/Zombies/Charred/zombie/Zombie_charred_pile1.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED_HEAD",      "assets/Zombies/Charred/zombie/Zombie_charred_head.png"},
    {"IMAGE_REANIM_ZOMBIE_HAIR",              "assets/Zombies/Charred/zombie/Zombie_hair.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED_EYES1",     "assets/Zombies/Charred/zombie/Zombie_charred_eyes1.png"},
    {"IMAGE_REANIM_ZOMBIE_CHARRED_EYES2",     "assets/Zombies/Charred/zombie/Zombie_charred_eyes2.png"}
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_jalapeno[] = { "anim_blink", "anim_idle", "anim_explode", "Jalapeno_stem",
    "Jalapeno_body", "Jalapeno_mouth", "Jalapeno_eye1", "Jalapeno_eye2", "Jalapeno_cheek",
    "Jalapeno_eyebrow1", "Jalapeno_eyebrow2" };

  def->loadFiles("assets/Plants/jalapeno/jalapeno.json", 11, trackNames_jalapeno, {
      {"IMAGE_REANIM_JALAPENO_STEM",     "assets/Plants/jalapeno/Jalapeno_stem.png"},
      {"IMAGE_REANIM_JALAPENO_BODY",     "assets/Plants/jalapeno/Jalapeno_body.png"},
      {"IMAGE_REANIM_JALAPENO_MOUTH",    "assets/Plants/jalapeno/Jalapeno_mouth.png"},
      {"IMAGE_REANIM_JALAPENO_EYE1",     "assets/Plants/jalapeno/Jalapeno_eye1.png"},
      {"IMAGE_REANIM_JALAPENO_EYE2",     "assets/Plants/jalapeno/Jalapeno_eye2.png"},
      {"IMAGE_REANIM_JALAPENO_CHEEK",    "assets/Plants/jalapeno/Jalapeno_cheek.png"},
      {"IMAGE_REANIM_JALAPENO_EYEBROW1", "assets/Plants/jalapeno/Jalapeno_eyebrow1.png"},
      {"IMAGE_REANIM_JALAPENO_EYEBROW2", "assets/Plants/jalapeno/Jalapeno_eyebrow2.png"}
    });
  definitions.push(def);

  //ReAnimationParser::reportImageMap("assets/Particles/fire/fire.json", "assets/Particles/fire/aaaa.png");


  def = new ReAnimationDefinition;
  std::string trackNames_fire[] = { "anim_flame", "anim_done", "Layer 1" };
  def->loadFiles("assets/Particles/fire/fire.json", 3, trackNames_fire, {
      {"IMAGE_REANIM_FIRE1",       "assets/Particles/fire/fire1.png"},
      {"IMAGE_REANIM_FIRE2",       "assets/Particles/fire/fire2.png"},
      {"IMAGE_REANIM_FIRE3",       "assets/Particles/fire/fire3.png"},
      {"IMAGE_REANIM_FIRE4",       "assets/Particles/fire/fire4.png"},
      {"IMAGE_REANIM_FIRE4B",      "assets/Particles/fire/fire4b.png"},
      {"IMAGE_REANIM_FIRE5",       "assets/Particles/fire/fire5.png"},
      {"IMAGE_REANIM_FIRE5B",      "assets/Particles/fire/fire5b.png"},
      {"IMAGE_REANIM_FIRE6",       "assets/Particles/fire/fire6.png"},
      {"IMAGE_REANIM_FIRE6B",      "assets/Particles/fire/fire6b.png"},
      {"IMAGE_REANIM_FIRE7",       "assets/Particles/fire/fire7.png"},
      {"IMAGE_REANIM_FIRE7B",      "assets/Particles/fire/fire7b.png"},
      {"IMAGE_REANIM_FIRE8",       "assets/Particles/fire/fire8.png"}
    });

  definitions.push(def);


  def = new ReAnimationDefinition;
  //ReAnimationParser::reportImageMap("assets/Background/LawnMower/lawnmower.json", "assets/Background/LawnMower/aaaaaaa.png");
  std::string trackNames_lawnmower[] = { "anim_normal", "anim_tricked", "lawnmower_backwheelpiece1",
    "Lawnmower_backwheel1", "Lawnmower_backwheelshine1", "lawnmower_backwheelpiece2",
    "Lawnmower_backwheel2", "Lawnmower_backwheelshine2", "LawnMower_dice", "LawnMower_body",
    "LawnMower_pull", "LawnMower_engine", "lawnmower_frontwheelpiece1", "Lawnmower_frontwheel1",
    "Lawnmower_wheelshine1", "lawnmower_frontwheelpiece2", "Lawnmower_frontwheel2",
    "Lawnmower_wheelshine2", "Lawnmower_exhaust" };
  def->loadFiles("assets/Background/LawnMower/lawnmower.json", 19, trackNames_lawnmower, {
      {"IMAGE_REANIM_LAWNMOWER_WHEELPIECE",      "assets/Background/LawnMower/LawnMower_wheelpiece.png"},
      {"IMAGE_REANIM_LAWNMOWER_WHEEL2",          "assets/Background/LawnMower/LawnMower_wheel2.png"},
      {"IMAGE_REANIM_LAWNMOWER_WHEELSHINE",      "assets/Background/LawnMower/LawnMower_wheelshine.png"},
      {"IMAGE_REANIM_LAWNMOWER_DICE_TRICKED",    "assets/Background/LawnMower/LawnMower_dice_tricked.png"},
      {"IMAGE_REANIM_LAWNMOWER_BODY",            "assets/Background/LawnMower/LawnMower_body.png"},
      {"IMAGE_REANIM_LAWNMOWER_BODY_TRICKED",    "assets/Background/LawnMower/LawnMower_body_tricked.png"},
      {"IMAGE_REANIM_LAWNMOWER_PULL",            "assets/Background/LawnMower/LawnMower_pull.png"},
      {"IMAGE_REANIM_LAWNMOWER_ENGINE",          "assets/Background/LawnMower/LawnMower_engine.png"},
      {"IMAGE_REANIM_LAWNMOWER_ENGINE_TRICKED",  "assets/Background/LawnMower/LawnMower_engine_tricked.png"},
      {"IMAGE_REANIM_LAWNMOWER_WHEEL1",          "assets/Background/LawnMower/LawnMower_wheel1.png"},
      {"IMAGE_REANIM_LAWNMOWER_EXHAUST",         "assets/Background/LawnMower/LawnMower_exhaust.png"},
      {"IMAGE_REANIM_LAWNMOWER_EXHAUST_TRICKED", "assets/Background/LawnMower/LawnMower_exhaust_tricked.png"}
    });
  definitions.push(def);


  def = new ReAnimationDefinition;
  std::string trackNames_lawnmoweredZombie[] = { "locator" };

  def->loadFiles("assets/Background/LawnMower/lawnmoweredZombie.json", 1, trackNames_lawnmoweredZombie, {
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
  if (newAlpha > 1.0f) newAlpha = 1.0f;
  else if (newAlpha < 0.0f) newAlpha = 0.0f;
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
  if (animName == "")
    return (activeLabels.size() > 0); // if any animation is playing

  for (auto &lab : activeLabels)
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


void ReAnimator::switchDefinition(ReAnimationDef newDefID) {
  reAnimDef = definitions[newDefID];
  activeLabels.clear();
  trackInstances.clear();
  trackInstances.resize(reAnimDef->totalTracks);
  if (child)
    child = nullptr;
}


void ReAnimator::updateOrphans(float dt) {
  for (int i = 0; i < orphanAnimators.size; i++)
    orphanAnimators[i].update(dt);
  orphanAnimators.erase([](ReAnimator &re) { return !re.isPlayingAnimation(); });
}


void ReAnimator::drawOrphans() {
  for (int i = 0; i < orphanAnimators.size; i++)
    orphanAnimators[i].draw();
}
