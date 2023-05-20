# ADD LIB
add_library(OccurrenceAlgos bruteForce.cxx deltaSkipSearch.cxx intervalSearch.cxx)
# add_subdirectory(${PROJECT_BINARY_DIR}/Alphabet+)
# Anybody linking to OccurrenceAlgos needs to include the current source directory
target_link_libraries(OccurrenceAlgos PUBLIC MatchingAlgos)
target_include_directories(OccurrenceAlgos INTERFACE 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           # "${PROJECT_SOURCE_DIR}/Alphabet" # ADD ALPHABET
                           )