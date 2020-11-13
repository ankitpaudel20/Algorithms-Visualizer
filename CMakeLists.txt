cmake_minimum_required(VERSION 3.5)

project(Application LANGUAGES C CXX)

add_executable(Application main.cpp )
target_include_directories(Application PRIVATE include vendor/imgui vendor/include)

      file(GLOB_RECURSE SDL2_GFX_SOURCES vendor/include/SDL2/*.c)

      target_sources(Application PRIVATE 
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_impl_sdl.cpp
        vendor/imgui/imgui_impl_opengl3.cpp
        vendor/glad/glad.c
        sorts.cpp
        ${SDL2_GFX_SOURCES})



if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_directories(Application PRIVATE vendor/libs)
     target_link_libraries(Application PRIVATE sdl2-static libfreetype-6 SDL2main opengl32 Imm32 version winmm SetupAPI)
    message("Linked to windows using MSVC")
else()
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        target_link_libraries(Application PRIVATE pthread GL X11 GLU SDL2 ${CMAKE_DL_LIBS} freetype) 
        message("Linked to Linux")
    elseif (CMAKE_SYSTEM_NAME STREQUAL Windows)
        #target_link_libraries(Application PRIVATE libglew32.dll.a libSDl2.dll.a libSDl2main.a libSDL2_image.dll.a libglew32.a opengl32)
        target_link_directories(Application PRIVATE vendor/libs )
        target_link_libraries(Application PRIVATE freetype libSDL2-static.a libSDL2main.a opengl32 Imm32 version winmm SetupAPI pthread)
        message("Linked to windows")
    endif ()
endif()



