#pragma once
#include "actor.hpp"

class QModelInstance;

class EditorContext {
public:
    std::weak_ptr<ActorBase> selectedModel {};
    int selectedActorFactoryEntry {0};
};
