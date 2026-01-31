defineModule("Editor")
addModuleDependency("Editor" "CoreEditor")

defineExecutable("EditorTarget")
addExecutableDependency("EditorTarget" "Editor")