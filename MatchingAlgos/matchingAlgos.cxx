#include "MatchingAlgos.h"
#include <iostream>

/*
    -> sumDeltaMatch return the sum of the value differences char. by char. But
   if find a Delta mismatch returns -1.
*/
int MatchingAlgos::sumDeltaMatch(std::wstring_view x, std::wstring_view y,
                                 int delta) {
  int sum = 0;
  if (x.length() != y.length() || delta < 0)
    return -1;
  for (int i = 0; i < x.length(); ++i) {
    if (std::abs(alph.getValue(x[i]) - alph.getValue(y[i])) > delta)
      return -1;
    sum += std::abs(alph.getValue(x[i]) - alph.getValue(y[i]));
  }
  return sum;
}

/*
    CONSTRUCTOR of the class MatchingAlgos just initialize the alphabet.
*/
MatchingAlgos::MatchingAlgos(std::string alphPath) {
  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale(""));
  std::cin.imbue(std::locale(""));
  alph = alphPath.empty() ? Alphabet() : Alphabet(alphPath);
  // alph.print();
}
MatchingAlgos::MatchingAlgos(int begin, int end, int step) {
  // Allow receive UTF-8 characters by console.
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale(""));
  std::cin.imbue(std::locale(""));
  alph = Alphabet(begin, end, step);
  // alph.print();
}
/*
     -> isDeltaGammaMatch return whether 'x' Delta match 'y' or not. If Gamma is
   negative just check the Delta match.
*/
bool MatchingAlgos::isDeltaGammaMatch(std::wstring_view x, std::wstring_view y,
                                      int delta, int gamma) {
  int aux = sumDeltaMatch(x, y, delta);
  return 0 <= aux && (aux <= gamma || gamma < 0);
}

///////////////////////////////////////////////////////////

std::wstring getText(std::string filePath, bool isNumber) {
  /* If your native locale doesn't use UTF-8 encoding
   * you need to replace the empty string with a
   * locale like "en_US.utf8"
   */
  std::locale::global(std::locale(""));
  std::cout.imbue(std::locale());
  // Open the file (encoded with UTF-8) and read the values.
  std::wifstream paramFileW(filePath, std::ios::binary);
  paramFileW.imbue(std::locale("en_US.UTF-8"));
  if (paramFileW.fail()) {
    throw std::invalid_argument("Can't open the specified file path: " +
                                filePath);
  }
  std::wstring line, num, answ;
  long n;
  std::wstringstream ss;
  while (paramFileW.good()) {
    std::getline(paramFileW, line);
    ss = std::wstringstream(line);
    if (isNumber) {
      while (ss >> n) {
        answ += static_cast<wchar_t>(n);
        if (ss.peek() == L',')
          ss.ignore();
      }
      /*
       while (std::getline(ss, num, L','))
         answ += (wchar_t)std::stol(num);
       */
    } else {
      answ += line;
    }
  }
  paramFileW.close();
  return answ;
}

////////////////////////////////////////
/* READ MIDI NOTES FROM MIDI FILE. CODE FROM:
  https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_MIDI.cpp
*/
////////////////////////////////////////
struct MidiEvent {
  enum class Type { NoteOff, NoteOn, Other } event;

  uint8_t nKey = 0;
  uint8_t nVelocity = 0;
  uint32_t nDeltaTick = 0;
};

struct MidiNote {
  uint8_t nKey = 0;
  uint8_t nVelocity = 0;
  uint32_t nStartTime = 0;
  uint32_t nDuration = 0;
};

struct MidiTrack {
  std::string sName;
  std::string sInstrument;
  std::vector<MidiEvent> vecEvents;
  std::vector<MidiNote> vecNotes;
  uint8_t nMaxNote = 64;
  uint8_t nMinNote = 64;
};

std::vector<int> getMIDINumbers(std::string filePath) {
  std::vector<int> answ;

  std::vector<MidiTrack> vecTracks;
  uint32_t m_nTempo = 0;
  uint32_t m_nBPM = 0;

  enum EventName : uint8_t {
    VoiceNoteOff = 0x80,
    VoiceNoteOn = 0x90,
    VoiceAftertouch = 0xA0,
    VoiceControlChange = 0xB0,
    VoiceProgramChange = 0xC0,
    VoiceChannelPressure = 0xD0,
    VoicePitchBend = 0xE0,
    SystemExclusive = 0xF0,
  };

  enum MetaEventName : uint8_t {
    MetaSequence = 0x00,
    MetaText = 0x01,
    MetaCopyright = 0x02,
    MetaTrackName = 0x03,
    MetaInstrumentName = 0x04,
    MetaLyrics = 0x05,
    MetaMarker = 0x06,
    MetaCuePoint = 0x07,
    MetaChannelPrefix = 0x20,
    MetaEndOfTrack = 0x2F,
    MetaSetTempo = 0x51,
    MetaSMPTEOffset = 0x54,
    MetaTimeSignature = 0x58,
    MetaKeySignature = 0x59,
    MetaSequencerSpecific = 0x7F,
  };

  // Open the MIDI File as a stream
  std::ifstream ifs;
  ifs.open(filePath, std::fstream::in | std::ios::binary);
  if (!ifs.is_open())
    return answ;

  // Helper Utilities ====================

  // Swaps byte order of 32-bit integer
  auto Swap32 = [](uint32_t n) {
    return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) |
            ((n << 24) & 0xff000000));
  };

  // Swaps byte order of 16-bit integer
  auto Swap16 = [](uint16_t n) { return ((n >> 8) | (n << 8)); };

  // Reads nLength bytes form file stream, and constructs a text string
  auto ReadString = [&ifs](uint32_t nLength) {
    std::string s;
    for (uint32_t i = 0; i < nLength; i++)
      s += ifs.get();
    return s;
  };

  // Reads a compressed MIDI value. This can be up to 32 bits long. Essentially
  // if the first byte, first bit is set to 1, that indicates that the next byte
  // is required to construct the full word. Only the bottom 7 bits of each byte
  // are used to construct the final word value. Each successive byte that has
  // MSB set, indicates a further byte needs to be read.
  auto ReadValue = [&ifs]() {
    uint32_t nValue = 0;
    uint8_t nByte = 0;

    // Read byte
    nValue = ifs.get();

    // Check MSB, if set, more bytes need reading
    if (nValue & 0x80) {
      // Extract bottom 7 bits of read byte
      nValue &= 0x7F;
      do {
        // Read next byte
        nByte = ifs.get();

        // Construct value by setting bottom 7 bits, then shifting 7 bits
        nValue = (nValue << 7) | (nByte & 0x7F);
      } while (nByte & 0x80); // Loop whilst read byte MSB is 1
    }

    // Return final construction (always 32-bit unsigned integer internally)
    return nValue;
  };

  uint32_t n32 = 0;
  uint16_t n16 = 0;

  // Read MIDI Header (Fixed Size)
  ifs.read((char *)&n32, sizeof(uint32_t));
  uint32_t nFileID = Swap32(n32);
  ifs.read((char *)&n32, sizeof(uint32_t));
  uint32_t nHeaderLength = Swap32(n32);
  ifs.read((char *)&n16, sizeof(uint16_t));
  uint16_t nFormat = Swap16(n16);
  ifs.read((char *)&n16, sizeof(uint16_t));
  uint16_t nTrackChunks = Swap16(n16);
  ifs.read((char *)&n16, sizeof(uint16_t));
  uint16_t nDivision = Swap16(n16);

  for (uint16_t nChunk = 0; nChunk < nTrackChunks; nChunk++) {
    std::cout << "===== NEW TRACK" << std::endl;
    // Read Track Header
    ifs.read((char *)&n32, sizeof(uint32_t));
    uint32_t nTrackID = Swap32(n32);
    ifs.read((char *)&n32, sizeof(uint32_t));
    uint32_t nTrackLength = Swap32(n32);

    bool bEndOfTrack = false;

    vecTracks.push_back(MidiTrack());

    uint32_t nWallTime = 0;

    uint8_t nPreviousStatus = 0;

    while (!ifs.eof() && !bEndOfTrack) {
      // Fundamentally all MIDI Events contain a timecode, and a status byte*
      uint32_t nStatusTimeDelta = 0;
      uint8_t nStatus = 0;

      // Read Timecode from MIDI stream. This could be variable in length
      // and is the delta in "ticks" from the previous event. Of course this
      // value could be 0 if two events happen simultaneously.
      nStatusTimeDelta = ReadValue();

      // Read first byte of message, this could be the status byte, or it could
      // not...
      nStatus = ifs.get();

      // All MIDI Status events have the MSB set. The data within a standard
      // MIDI event does not. A crude yet utilised form of compression is to
      // omit sending status bytes if the following sequence of events all refer
      // to the same MIDI Status. This is called MIDI Running Status, and is
      // essential to succesful decoding of MIDI streams and files.
      //
      // If the MSB of the read byte was not set, and on the whole we were
      // expecting a status byte, then Running Status is in effect, so we refer
      // to the previous confirmed status byte.
      if (nStatus < 0x80) {
        // MIDI Running Status is happening, so refer to previous valid MIDI
        // Status byte
        nStatus = nPreviousStatus;

        // We had to read the byte to assess if MIDI Running Status is in
        // effect. But! that read removed the byte form the stream, and that
        // will desync all of the following code because normally we would have
        // read a status byte, but instead we have read the data contained
        // within a MIDI message. The simple solution is to put the byte back :P
        ifs.seekg(-1, std::ios_base::cur);
      }

      if ((nStatus & 0xF0) == EventName::VoiceNoteOff) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nNoteID = ifs.get();
        uint8_t nNoteVelocity = ifs.get();
        vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::NoteOff,
                                               nNoteID, nNoteVelocity,
                                               nStatusTimeDelta});
      }

      else if ((nStatus & 0xF0) == EventName::VoiceNoteOn) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nNoteID = ifs.get();
        uint8_t nNoteVelocity = ifs.get();
        if (nNoteVelocity == 0)
          vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::NoteOff,
                                                 nNoteID, nNoteVelocity,
                                                 nStatusTimeDelta});
        else
          vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::NoteOn,
                                                 nNoteID, nNoteVelocity,
                                                 nStatusTimeDelta});
      }

      else if ((nStatus & 0xF0) == EventName::VoiceAftertouch) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nNoteID = ifs.get();
        uint8_t nNoteVelocity = ifs.get();
        vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::Other});
      }

      else if ((nStatus & 0xF0) == EventName::VoiceControlChange) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nControlID = ifs.get();
        uint8_t nControlValue = ifs.get();
        vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::Other});
      }

      else if ((nStatus & 0xF0) == EventName::VoiceProgramChange) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nProgramID = ifs.get();
        vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::Other});
      }

      else if ((nStatus & 0xF0) == EventName::VoiceChannelPressure) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nChannelPressure = ifs.get();
        vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::Other});
      }

      else if ((nStatus & 0xF0) == EventName::VoicePitchBend) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nLS7B = ifs.get();
        uint8_t nMS7B = ifs.get();
        vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::Other});

      }

      else if ((nStatus & 0xF0) == EventName::SystemExclusive) {
        nPreviousStatus = 0;

        if (nStatus == 0xFF) {
          // Meta Message
          uint8_t nType = ifs.get();
          uint8_t nLength = ReadValue();

          switch (nType) {
          case MetaSequence:
            std::cout << "Sequence Number: " << ifs.get() << ifs.get()
                      << std::endl;
            break;
          case MetaText:
            std::cout << "Text: " << ReadString(nLength) << std::endl;
            break;
          case MetaCopyright:
            std::cout << "Copyright: " << ReadString(nLength) << std::endl;
            break;
          case MetaTrackName:
            vecTracks[nChunk].sName = ReadString(nLength);
            std::cout << "Track Name: " << vecTracks[nChunk].sName << std::endl;
            break;
          case MetaInstrumentName:
            vecTracks[nChunk].sInstrument = ReadString(nLength);
            std::cout << "Instrument Name: " << vecTracks[nChunk].sInstrument
                      << std::endl;
            break;
          case MetaLyrics:
            std::cout << "Lyrics: " << ReadString(nLength) << std::endl;
            break;
          case MetaMarker:
            std::cout << "Marker: " << ReadString(nLength) << std::endl;
            break;
          case MetaCuePoint:
            std::cout << "Cue: " << ReadString(nLength) << std::endl;
            break;
          case MetaChannelPrefix:
            std::cout << "Prefix: " << ifs.get() << std::endl;
            break;
          case MetaEndOfTrack:
            bEndOfTrack = true;
            break;
          case MetaSetTempo:
            // Tempo is in microseconds per quarter note
            if (m_nTempo == 0) {
              (m_nTempo |= (ifs.get() << 16));
              (m_nTempo |= (ifs.get() << 8));
              (m_nTempo |= (ifs.get() << 0));
              m_nBPM = (60000000 / m_nTempo);
              std::cout << "Tempo: " << m_nTempo << " (" << m_nBPM << "bpm)"
                        << std::endl;
            }
            break;
          case MetaSMPTEOffset:
            std::cout << "SMPTE: H:" << ifs.get() << " M:" << ifs.get()
                      << " S:" << ifs.get() << " FR:" << ifs.get()
                      << " FF:" << ifs.get() << std::endl;
            break;
          case MetaTimeSignature:
            std::cout << "Time Signature: " << ifs.get() << "/"
                      << (2 << ifs.get()) << std::endl;
            std::cout << "ClocksPerTick: " << ifs.get() << std::endl;

            // A MIDI "Beat" is 24 ticks, so specify how many 32nd notes
            // constitute a beat
            std::cout << "32per24Clocks: " << ifs.get() << std::endl;
            break;
          case MetaKeySignature:
            std::cout << "Key Signature: " << ifs.get() << std::endl;
            std::cout << "Minor Key: " << ifs.get() << std::endl;
            break;
          case MetaSequencerSpecific:
            std::cout << "Sequencer Specific: " << ReadString(nLength)
                      << std::endl;
            break;
          default:
            std::cout << "Unrecognised MetaEvent: " << nType << std::endl;
          }
        }

        if (nStatus == 0xF0) {
          // System Exclusive Message Begin
          std::cout << "System Exclusive Begin: " << ReadString(ReadValue())
                    << std::endl;
        }

        if (nStatus == 0xF7) {
          // System Exclusive Message Begin
          std::cout << "System Exclusive End: " << ReadString(ReadValue())
                    << std::endl;
        }
      } else {
        std::cout << "Unrecognised Status Byte: " << nStatus << std::endl;
      }
    }
  }

  // Convert Time Events to Notes
  for (auto &track : vecTracks) {
    std::wcout << "\n ===== NEW TRACK ==== \n" ;
    uint32_t nWallTime = 0;

    std::vector<MidiNote> listNotesBeingProcessed;

    for (auto &event : track.vecEvents) {
      nWallTime += event.nDeltaTick;

      if (event.event == MidiEvent::Type::NoteOn) {
        // New Note
        listNotesBeingProcessed.push_back(
            {event.nKey, event.nVelocity, nWallTime, 0});
      }

      if (event.event == MidiEvent::Type::NoteOff) {
        auto note = std::find_if(
            listNotesBeingProcessed.begin(), listNotesBeingProcessed.end(),
            [&](const MidiNote &n) { return n.nKey == event.nKey; });
        if (note != listNotesBeingProcessed.end()) {
          note->nDuration = nWallTime - note->nStartTime;
          track.vecNotes.push_back(*note);
          track.nMinNote = std::min(track.nMinNote, note->nKey);
          track.nMaxNote = std::max(track.nMaxNote, note->nKey);
          listNotesBeingProcessed.erase(note);
          std::wcout << note->nKey << " ,";
          answ.push_back(note->nKey);
        }
      }
    }
  }
  ifs.close();
  return answ;
}
