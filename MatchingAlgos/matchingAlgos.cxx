#include "MatchingAlgos.h"

/*
    CONSTRUCTOR of the class MatchingAlgos just initialize the alphabet.
*/
MatchingAlgos::MatchingAlgos(std::string alphPath) {
  alph = alphPath.empty() ? Alphabet() : Alphabet(alphPath);
}
