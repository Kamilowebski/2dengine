cmake_minimum_required(VERSION 3.28)

project(Engine2D VERSION 0.1.0 LANGUAGES CXX)

#STANDARD C++17, wymagany
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Plik compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Statycznie, bez kopiowania dll
set(BUILD_SHARED_LIBS OFF)

#Pobieranie SFML
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

#Bez sieci
set(SFML_BUILD_NETWORK OFF)
FetchContent_MakeAvailable(SFML)

#Podprojekty
add_subdirectory(engine)
add_subdirectory(sandbox)

#doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
        set(DOXYGEN_PROJECT_NAME "Engine2D")
        set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
        set(DOXYGEN_OUTPUT_LANGUAGE Polish)
        set(DOXYGEN_EXTRACT_ALL YES)
        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_GENERATE_LATEX NO)
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
        doxygen_add_docs(docs
            ${CMAKE_SOURCE_DIR}/README.md
            ${CMAKE_SOURCE_DIR}/engine
            COMMENT "Generowanie dokumentacji Doxygen"
        )
else ()
        message(STATUS "Doxygen nie zostal znaleziony, target docs niedostepny")
endif ()
