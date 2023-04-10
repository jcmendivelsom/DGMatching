# ADD LIB
add_library(DeltaBMAlgos deltaBM1.cxx deltaBM2.cxx deltaBM3.cxx deltaTunedBM.cxx)
# add_subdirectory(${PROJECT_BINARY_DIR}/Alphabet+)
# Anybody linking to DeltaBMAlgos needs to include the current source directory
target_link_libraries(DeltaBMAlgos PUBLIC Alphabet)
target_include_directories(DeltaBMAlgos INTERFACE 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           "${PROJECT_SOURCE_DIR}/Alphabet" # ADD ALPHABET
                           )