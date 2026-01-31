
macro(defineModule MODULE)
    #LIBRARY
    file(
            GLOB_RECURSE sources
            LIST_DIRECTORIES true
            CONFIGURE_DEPENDS true
            "${CMAKE_CURRENT_LIST_DIR}/private/*.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/public/*.cpp"
    )
    #list(FILTER sources EXCLUDE REGEX "^.*Test\.cpp$")

    add_library(
            ${CMAKE_PROJECT_NAME}_${MODULE} STATIC
            ${sources}
    )
    target_include_directories(
            ${CMAKE_PROJECT_NAME}_${MODULE} PUBLIC
            "${CMAKE_CURRENT_LIST_DIR}/public"
    )

    set_target_properties(${CMAKE_PROJECT_NAME}_${MODULE} PROPERTIES LINKER_LANGUAGE CXX)
endmacro()


macro(addTestToModule MODULE DEPENDENCY)

    #TESTS
    enable_testing()

    file(
            GLOB_RECURSE sources
            LIST_DIRECTORIES true
            CONFIGURE_DEPENDS true
            "${CMAKE_CURRENT_LIST_DIR}/test/*.cpp"
    )

    #Test target
    add_executable(
            ${CMAKE_PROJECT_NAME}_${MODULE}_test
            ${sources}
    )
    target_link_libraries(
            ${CMAKE_PROJECT_NAME}_${MODULE}_test PUBLIC
            ${CMAKE_PROJECT_NAME}_${MODULE}
            GTest::gtest_main
            GTest::gmock_main
    )

    #Discover Test
    include(GoogleTest)
    gtest_discover_tests(${CMAKE_PROJECT_NAME}_${MODULE}_test)
endmacro()

macro(addDependency MODULE DEPENDENCY)
    target_link_libraries(
            ${CMAKE_PROJECT_NAME}_${MODULE} PUBLIC
            ${DEPENDENCY}
    )
endmacro()

macro(addModuleDependency MODULE DEPENDENCY)
    target_link_libraries(
            ${CMAKE_PROJECT_NAME}_${MODULE} PUBLIC
            ${CMAKE_PROJECT_NAME}_${DEPENDENCY}
    )
endmacro()

macro(addExecutableDependency MODULE DEPENDENCY)
    target_link_libraries(
            ${CMAKE_PROJECT_NAME}_${MODULE} PUBLIC
            ${CMAKE_PROJECT_NAME}_${DEPENDENCY}
    )
endmacro()

macro(defineExecutable MODULE)
    #EXECUTABLE
    add_executable(
            ${CMAKE_PROJECT_NAME}_${MODULE}
            "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
    )

    target_link_options(${CMAKE_PROJECT_NAME}_${MODULE} PRIVATE
            -static-libgcc
            -static-libstdc++
    )


    #DLL
    if (WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_${MODULE} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${CMAKE_PROJECT_NAME}_${MODULE}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}_${MODULE}> COMMAND_EXPAND_LISTS)
    endif()

endmacro()
