#include "Application.h"

#include "ui/scene/Scene.h"

Application *Application::_current = nullptr;

void Application::onUpdate() {
  Component::onUpdate();

  if (_activeScene != nullptr) {
    _activeScene->update();
  }

  while (_toBeDestroyedScenes.size() > 0) {
    auto scene = _toBeDestroyedScenes.top();
    _toBeDestroyedScenes.pop();
    delete scene;
  }
}
