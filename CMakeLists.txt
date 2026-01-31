cmake_minimum_required(VERSION 3.16)


project(sandbox
    VERSION 1.0
    LANGUAGES C
)


set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


add_subdirectory(external/SDL)


add_executable(sandbox
    src/main.c
)

target_link_libraries(sandbox PRIVATE SDL3::SDL3)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:sandbox>
        $<TARGET_FILE_DIR:sandbox>
    COMMAND_EXPAND_LISTS
)