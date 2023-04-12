# ADD LIB
add_library(BitwiseAlgos shiftPlus.cxx shiftAnd.cxx shiftOr.cxx forwardScan.cxx backwardScan.cxx)
# add_subdirectory(${PROJECT_BINARY_DIR}/Alphabet+)
# Anybody linking to BitwiseAlgos needs to include the current source directory
target_link_libraries(BitwiseAlgos PUBLIC MatchingAlgos)
target_include_directories(BitwiseAlgos INTERFACE 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           # ADD ALPHABET "${PROJECT_SOURCE_DIR}/Alphabet" 
                           )