#pragma once

class QModelInstance;

class EditorContext {
public:
    std::weak_ptr<QModelInstance> selectedModel {};
};
