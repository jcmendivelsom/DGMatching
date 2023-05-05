#include "MatchingAlgos.h"

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

// Leer un entero de 32 bits en formato "big-endian"
uint32_t read_uint32_be(std::istream &input) {
  uint32_t value;
  input.read(reinterpret_cast<char *>(&value), sizeof(value));
  return __builtin_bswap32(value);
}

// Leer un entero de 16 bits en formato "big-endian"
uint16_t read_uint16_be(std::istream &input) {
  uint16_t value;
  input.read(reinterpret_cast<char *>(&value), sizeof(value));
  return __builtin_bswap16(value);
}

// Leer un entero variable-length
uint32_t read_var_length(std::istream &input) {
  uint32_t value = 0;
  uint8_t byte;

  do {
    byte = input.get();
    value = (value << 7) | (byte & 0x7F);
  } while (byte & 0x80);

  return value;
}

std::vector<int> getMIDINumbers(std::string filePath) {
  std::vector<int> answ;
  std::ifstream input;
  input.open(filePath, std::fstream::in | std::ios::binary);
  if (!input.is_open())
    return {};

  // Helper Utilities ====================

  // Swaps byte order of 32-bit integer
  auto Swap32 = [](uint32_t n) {
    return (((n >> 24) & 0xff) | ((n << 8) & 0xff0000) | ((n >> 8) & 0xff00) |
            ((n << 24) & 0xff000000));
  };

  // Swaps byte order of 16-bit integer
  auto Swap16 = [](uint16_t n) { return ((n >> 8) | (n << 8)); };

  // Reads nLength bytes form file stream, and constructs a text string
  auto ReadString = [&input](uint32_t nLength) {
    std::string s;
    for (uint32_t i = 0; i < nLength; i++)
      s += input.get();
    return s;
  };

  // Reads a compressed MIDI value. This can be up to 32 bits long. Essentially
  // if the first byte, first bit is set to 1, that indicates that the next byte
  // is required to construct the full word. Only the bottom 7 bits of each byte
  // are used to construct the final word value. Each successive byte that has
  // MSB set, indicates a further byte needs to be read.
  auto ReadValue = [&input]() {
    uint32_t nValue = 0;
    uint8_t nByte = 0;

    // Read byte
    nValue = input.get();

    // Check MSB, if set, more bytes need reading
    if (nValue & 0x80) {
      // Extract bottom 7 bits of read byte
      nValue &= 0x7F;
      do {
        // Read next byte
        nByte = input.get();

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
  input.read((char *)&n32, sizeof(uint32_t));
  uint32_t nFileID = Swap32(n32);
  input.read((char *)&n32, sizeof(uint32_t));
  uint32_t nHeaderLength = Swap32(n32);
  input.read((char *)&n16, sizeof(uint16_t));
  uint16_t nFormat = Swap16(n16);
  input.read((char *)&n16, sizeof(uint16_t));
  uint16_t nTrackChunks = Swap16(n16);
  input.read((char *)&n16, sizeof(uint16_t));
  uint16_t nDivision = Swap16(n16);

  std::wcout << nTrackChunks << std::endl;

  for (uint16_t nChunk = 0; nChunk < nTrackChunks; nChunk++) {
    // std::cout << "===== NEW TRACK" << std::endl;
    // Read Track Header
    input.read((char *)&n32, sizeof(uint32_t));
    uint32_t nTrackID = Swap32(n32);
    input.read((char *)&n32, sizeof(uint32_t));
    uint32_t nTrackLength = Swap32(n32);

    bool bEndOfTrack = false;

    // vecTracks.push_back(MidiTrack());

    uint32_t nWallTime = 0;

    uint8_t nPreviousStatus = 0;

    while (!input.eof() && !bEndOfTrack) {
      // Fundamentally all MIDI Events contain a timecode, and a status byte*
      uint32_t nStatusTimeDelta = 0;
      uint8_t nStatus = 0;

      // Read Timecode from MIDI stream. This could be variable in length
      // and is the delta in "ticks" from the previous event. Of course this
      // value could be 0 if two events happen simultaneously.
      nStatusTimeDelta = ReadValue();

      // Read first byte of message, this could be the status byte, or it could
      // not...
      nStatus = input.get();

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
        input.seekg(-1, std::ios_base::cur);
      }

      if ((nStatus & 0xF0) == 0x80) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nNoteID = input.get();
        uint8_t nNoteVelocity = input.get();
        // std::wcout << nNoteID << ", ";
        // vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::NoteOff,
        // nNoteID, nNoteVelocity, nStatusTimeDelta});
      } else if ((nStatus & 0xF0) == 0x90) {
        nPreviousStatus = nStatus;
        uint8_t nChannel = nStatus & 0x0F;
        uint8_t nNoteID = input.get();
        uint8_t nNoteVelocity = input.get();
        /*
        if (nNoteVelocity == 0)
          vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::NoteOff,
                                                 nNoteID, nNoteVelocity,
                                                 nStatusTimeDelta});
        else
          vecTracks[nChunk].vecEvents.push_back({MidiEvent::Type::NoteOn,
                                                 nNoteID, nNoteVelocity,
                                                 nStatusTimeDelta});
        */
        if (nNoteVelocity != 0)
          std::wcout << nNoteID << ", ";
      }
    }
  }

  input.close();
  return answ;
}
