#biblioteka statyczna silnika
add_library(engine STATIC
        src/Application.cpp
        src/FpsCounter.cpp
        src/Input.cpp
        src/Logger.cpp
        src/PrimitiveRenderer.cpp
        src/Point2d.cpp
        src/LineSegment.cpp
)

#Naglowki publiczne widoczne dla kazdego kto linkuje silnik
target_include_directories(engine PUBLIC include)

#SFML jako zaleznisc publiczna
target_link_libraries(engine PUBLIC SFML::Graphics)

#Ostrzezenia kompilatora
if (MSVC)
    target_compile_options(engine PRIVATE /W4)
else ()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif ()
