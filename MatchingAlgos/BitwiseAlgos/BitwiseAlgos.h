#pragma once
#include "MatchingAlgos.h"
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

/* /////////////////// */
// Change this to 1 and MAX_BITS use your desired word length.
#define USE_MORE_MACHINE_WORD 0
/* /////////////////// */

constexpr size_t WORD_LEN = 64;

#if USE_MORE_MACHINE_WORD
constexpr size_t MAX_BITS = 190; // Change this to use your desired word length.
#else
const size_t MAX_BITS = WORD_LEN;
#endif

class BitwiseAlgos : public MatchingAlgos {

  template <size_t BIT_SIZE> class BitArray;

#if USE_MORE_MACHINE_WORD
  typedef std::bitset<MAX_BITS> BitSet;
  // typedef BitArray<MAX_BITS> BitSet;
#else
  typedef unsigned long long BitSet;
#endif

  BitSet sum(BitSet a, BitSet b);
  unsigned long long crop(BitSet x);

public:
  using MatchingAlgos::MatchingAlgos;
  std::vector<int> backwardScan(std::wstring_view t, std::wstring_view p,
                                int delta, int gamma = -1);
  std::vector<int> auxBackwardScan(std::wstring_view t, std::wstring_view p,
                                int delta, int gamma = -1);                                
  std::vector<int> forwardScan(std::wstring_view t, std::wstring_view p,
                               int delta, int gamma = -1);
  std::vector<int> auxForwardScan(std::wstring_view t, std::wstring_view p,
                               int delta, int gamma = -1);
  std::vector<int> shiftPlus(std::wstring_view t, std::wstring_view p,
                             int delta, int gamma = -1);
  std::vector<int> auxShiftPlus(std::wstring_view t, std::wstring_view p,
                             int delta, int gamma = -1);
  std::vector<int> shiftAnd(std::wstring_view t, std::wstring_view p,
                            int delta);
  std::vector<int> auxShiftAnd(std::wstring_view t, std::wstring_view p,
                            int delta);
  std::vector<int> shiftOr(std::wstring_view t, std::wstring_view p);
  std::vector<int> auxShiftOr(std::wstring_view t, std::wstring_view p);
};

template <size_t BIT_SIZE> class BitwiseAlgos::BitArray {
private:
  std::array<unsigned long long, (BIT_SIZE + 63) / 64> bits;

public:
  BitArray(unsigned long long u = 0) {
    bits.fill(0ULL);
    bits[bits.size() - 1] = u;
  }

  BitArray &operator&=(const BitArray &other) {
    std::transform(bits.begin(), bits.end(), other.bits.begin(), bits.begin(),
                   std::bit_and<unsigned long long>());
    return *this;
  }

  BitArray &operator|=(const BitArray &other) {
    std::transform(bits.begin(), bits.end(), other.bits.begin(), bits.begin(),
                   std::bit_or<unsigned long long>());
    return *this;
  }

  BitArray &operator^=(const BitArray &other) {
    std::transform(bits.begin(), bits.end(), other.bits.begin(), bits.begin(),
                   std::bit_xor<unsigned long long>());
    return *this;
  }

  BitArray operator~() const {
    BitArray result(0);
    std::transform(bits.begin(), bits.end(), result.bits.begin(),
                   std::bit_not<unsigned long long>());
    return result;
  }

  bool operator==(const BitArray &other) const { return bits == other.bits; }
  bool operator!=(const BitArray &other) const { return bits != other.bits; }

  friend BitArray operator&(BitArray a, const BitArray &b) {
    a &= b;
    return a;
  }

  friend BitArray operator|(BitArray a, const BitArray &b) {
    a |= b;
    return a;
  }

  BitArray &operator<<=(size_t shift) {

    if (shift >= BIT_SIZE * (BIT_SIZE + 63) / 64) {
      bits.fill(0ULL);
      return *this;
    }

    const size_t block_shift = shift / 64;
    const size_t bit_shift = shift % 64;
    size_t i = (BIT_SIZE + 63) / 64 - 1;

    if (block_shift > 0) {
      for (; i >= (BIT_SIZE + 63) / 64 - block_shift; --i) {
        bits[i - 1] = bits[i];
        bits[i] = 0ULL;
      }
      // std::fill(bits.begin(), bits.begin() + block_shift, 0);
    }

    if (bit_shift > 0) {
      for (size_t j = 0; j < i; ++j) {
        bits[j] <<= bit_shift;
        bits[j] |= bits[j + 1] >> (64 - bit_shift);
      }
      bits[i] <<= bit_shift;
    }

    return *this;
  }

  BitArray &operator>>=(size_t shift) {
    if (shift >= BIT_SIZE * (BIT_SIZE + 63) / 64) {
      bits.fill(0ULL);
      return *this;
    }

    const size_t block_shift = shift / 64;
    const size_t bit_shift = shift % 64;
    size_t i = 0;

    if (block_shift > 0) {
      for (; i < (BIT_SIZE + 63) / 64 - block_shift; ++i) {
        bits[i + 1] = bits[i];
        bits[i] = 0ULL;
      }
      // std::fill(bits.begin(), bits.begin() + block_shift, 0);
    }

    if (bit_shift > 0) {
      for (size_t j = (BIT_SIZE + 63) / 64 - 1; j > i; --j) {
        bits[j] >>= bit_shift;
        bits[j] |= bits[j - 1] << (64 - bit_shift);
      }
      bits[i] >>= bit_shift;
    }

    return *this;
  }

  friend BitArray operator<<(BitArray a, const size_t &n) {
    a <<= n;
    return a;
  }

  friend BitArray operator>>(BitArray a, const size_t &n) {
    a >>= n;
    return a;
  }

  BitArray &operator+=(const BitArray &other) {
    for (size_t i = (BIT_SIZE + 63) / 64 - 1; i > 0; --i) {
      bits[i] += other.bits[i];
      bits[i - 1] += other.bits[i - 1] + (bits[i] < other.bits[i]);
    }
    bits[0] += other.bits[0];
    return *this;
  }

  friend BitArray operator+(BitArray a, const BitArray &b) {
    a += b;
    return a;
  }

  unsigned long long get(size_t i = (BIT_SIZE + 63) / 64 - 1) {
    return bits[i];
  }

  void print() {
    for (const auto b : bits)
      std::wcout << std::bitset<64>(b) << " | ";
    std::wcout << std::endl;
  }
};