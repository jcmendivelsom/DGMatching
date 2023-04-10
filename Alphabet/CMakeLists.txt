# ADD LIB
add_library(Alphabet alphabet.cxx)
# Anybody linking to Alphabet needs to include the current source directory.
target_include_directories(Alphabet INTERFACE 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )