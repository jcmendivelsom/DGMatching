# INIT MAIN PROJECT
cmake_minimum_required(VERSION 3.10)
# PROJECT NAME AND VERSION
project(StrMatching VERSION 0.5)
# SPECIFY C++ STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# CONFIGURE HEADER FILE TO PASS SOME CMAKE vars to source code.
configure_file(StrMatchConfig.h.in StrMatchConfig.h)
# ADD EXECUTABLE
add_executable(StrMatching main.cxx)
# ADD LIBRARIES !!!
add_subdirectory(MatchingAlgos)
list(APPEND EXTRA_LIBS MatchingAlgos)
# INCLUDE LIBS
target_link_libraries(StrMatching PUBLIC ${EXTRA_LIBS})
target_include_directories(StrMatching PUBLIC
                           "${PROJECT_BINARY_DIR}" # CONFIG FILE
                           )