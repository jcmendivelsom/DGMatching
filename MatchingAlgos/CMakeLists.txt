# SET LIB
add_library(MatchingAlgos matchingAlgos.cxx)
# ADD SOURCE LIBS
# ALPHABET
add_subdirectory(Alphabet)
list(APPEND EXTRA_LIBS Alphabet)
# BITWISE
add_subdirectory(BitwiseAlgos)
list(APPEND EXTRA_LIBS BitwiseAlgos)
# DELTABM
add_subdirectory(DeltaBMAlgos)
list(APPEND EXTRA_LIBS DeltaBMAlgos)
# DELTABM
add_subdirectory(OccurrenceAlgos)
list(APPEND EXTRA_LIBS OccurrenceAlgos)
# Anybody linking to MatchingAlgos needs to include the current source directory
target_link_libraries(MatchingAlgos PUBLIC ${EXTRA_LIBS})
target_include_directories(MatchingAlgos INTERFACE 
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )