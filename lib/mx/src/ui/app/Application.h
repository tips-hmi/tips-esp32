#pragma once

#include <stack>

#include "Component.h"

class Scene;

class Application : public Component {
 public:
  Application() { _current = this; }
  ~Application() {}

  static Application *current() { return _current; }

  Scene *getActiveScene() const { return _activeScene; }

  void start() {
    begin();
    onLaunch();
  }

 protected:
  void setActiveScene(Scene *scene) { _activeScene = scene; }

  void destroyScene(Scene *scene) { _toBeDestroyedScenes.push(scene); }

  virtual void onLaunch() {};

  virtual void onUpdate();

 private:
  static Application *_current;
  Scene *_activeScene = nullptr;
  std::stack<Scene *> _toBeDestroyedScenes;

  friend class Scene;
};
