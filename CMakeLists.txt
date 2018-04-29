cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17) #Use c++17
set(CMAKE_CXX_STANDARD_REQUIRED ON) #require the above (otherwise allows fallback to earlier version)
set(CMAKE_CXX_EXTENSIONS OFF) #Don't use non-standard compiler extensions

project (LSystems)
set(SOURCES
Cmd.h
Cmd.cpp
Color.hpp
Color.cpp
Context.h
Context.cpp
DrawStrategy.hpp
DrawStrategy.cpp
GlutFunctions.hpp
GlutFunctions.cpp
Lexer.cpp
Lexer.h
Lsystem.cpp
Lsystem.h
LSystemsExpander.hpp
LSystemsExpander.cpp
Parsenode.cpp
Parsenode.h
Parser.cpp
Parser.h
PointMotion.h
Rule.cpp
Rule.h
Rulefwd.hpp
Token.cpp
Token.h
Turtle.h
Turtle.cpp
)

#turn on all warnings, minus some that are not useful
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Weverything \
    -Wno-padded -Wno-c++98-compat -Wno-weak-vtables -Wno-missing-prototypes \
    -Wno-global-constructors -Wno-exit-time-destructors -Wno-deprecated-declarations \
    ")

#explicitly set/clear some warnings that Xcode has flags for
#(otherwise these are set to Xcode defaults, despite -Weverything, etc.)
#This list is probably actually longer, these are just the ones I have caught by
#using Unix Makefiles as well as Xcode
set(CMAKE_CXX_FLAGS
"${CMAKE_CXX_FLAGS} -Wconversion -Wcomma \
")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(testsuite ${SOURCES} catchmain.cpp tests.cpp )
target_link_libraries(testsuite ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(lsystems ${SOURCES} main.cpp "config file.txt")
configure_file("config file.txt" "config file.txt" COPYONLY)
target_link_libraries(lsystems ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )


