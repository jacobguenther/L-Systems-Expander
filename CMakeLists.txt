cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17) #Use c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) #require the above (otherwise allows fallback to earlier version)
set(CMAKE_CXX_EXTENSIONS OFF) #Don't use non-standard compiler extensions

set(CMAKE_CXX_FLAGS #turn on all warnings, minus some that are not useful
    "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Weverything \
    -Wno-padded -Wno-c++98-compat -Wno-weak-vtables -Wno-missing-prototypes \
    -Wno-global-constructors -Wno-deprecated-declarations\
    ")

project (LSystems)
set(SOURCES 
    Cmd.h
    Context.h
    Graphic.cpp
    Graphic.h
    Lexer.cpp
    Lexer.h
    Lsystem.cpp
    Lsystem.h
    Parsenode.h
    Parser.cpp
    Parser.h
    PointMotion.h
    Rule.cpp
    Rule.h
    Rulerunner.cpp
    Rulerunner.h
    Token.h
    Turtle.h
    main.cpp
    )

add_executable(lsystems ${SOURCES} "config file.txt")
message ("${EXECUTABLE_OUTPUT_PATH}/config\ file.txt")
configure_file("config file.txt" "config file.txt" COPYONLY)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(lsystems ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
