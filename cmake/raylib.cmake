include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
        ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG fdc084f532189fda8474079f79e74fa5e3541c9f
        GIT_PROGRESS TRUE) # v1.90.5
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

add_library(
        imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})



FetchContent_Declare(
        rlImGui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui
        GIT_TAG 3c6986358c001851d9bc20e55acf9de803c73b5d
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(rlImGui)
FetchContent_GetProperties(rlImGui SOURCE_DIR RLIMGUI_DIR)
add_library(rlimgui STATIC ${rlimgui_SOURCE_DIR}/rlImGui.cpp)
target_link_libraries(rlimgui PRIVATE imgui raylib)
target_include_directories(rlimgui INTERFACE ${rlimgui_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        GIT_PROGRESS TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)

enable_testing()