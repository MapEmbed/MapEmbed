#ifndef __CXXMPH_SEEDED_HASH_H__
#define __CXXMPH_SEEDED_HASH_H__

#include <stdint.h>  // for uint32_t and friends

#include <cstdlib>
#include <cstring>
#include <unordered_map>  // for std::hash

#include "MurmurHash3.h"
#include "stringpiece.h"

namespace cxxmph {

struct h128 {
  const uint32_t& operator[](uint8_t i) const { return uint32[i]; }
  uint32_t& operator[](uint8_t i) { return uint32[i]; }
  const uint64_t get64(bool second) const { return (static_cast<uint64_t>(uint32[second << 1]) << 32) | uint32[1 + (second << 1)]; }
  void set64(uint64_t v, bool second) { uint32[second << 1] = v >> 32; uint32[1+(second<<1)] = ((v << 32) >> 32); }
  bool operator==(const h128 rhs) const { return memcmp(uint32, rhs.uint32, sizeof(uint32)) == 0; }

  uint32_t uint32[4];

  struct hash32 { uint32_t operator()(const cxxmph::h128& h) const { return h[3]; } };
};

template <class HashFcn>
struct seeded_hash_function {
  template <class Key>
  uint32_t operator()(const Key& k, uint32_t seed) const {
    uint32_t h;
    uint32_t h0 = HashFcn()(k);
    MurmurHash3_x86_32(reinterpret_cast<const void*>(&h0), 4, seed, &h);
    return h;
  }
  template <class Key>
  h128 hash128(const Key& k, uint32_t seed) const {
    h128 h;
    uint32_t h0 = HashFcn()(k);
    MurmurHash3_x64_128(reinterpret_cast<const void*>(&h0), 4, seed, &h);
    return h;
  }
};

struct Murmur3 {
  template<class Key>
  uint32_t operator()(const Key& k) const {
    uint32_t out;
    MurmurHash3_x86_32(reinterpret_cast<const void*>(&k), sizeof(Key), 1 /* seed */, &out);
    return out;
  }
  template <class Key>
  h128 hash128(const Key& k) const {
    h128 h;
    MurmurHash3_x64_128(reinterpret_cast<const void*>(&k), sizeof(Key), 1 /* seed */, &h);
    return h;
  }
};

struct Murmur3StringPiece {
  template <class Key>
  uint32_t operator()(const Key& k) const {
    StringPiece s(k);
    uint32_t out;
    MurmurHash3_x86_32(s.data(), s.length(), 1 /* seed */, &out);
    return out;
  }
  template <class Key>
  h128 hash128(const Key& k) const {
    h128 h;
    StringPiece s(k);
    MurmurHash3_x64_128(s.data(), s.length(), 1 /* seed */, &h);
    return h;
  }
};

template <>
struct seeded_hash_function<Murmur3> {
  template <class Key>
  uint32_t operator()(const Key& k, uint32_t seed) const {
    uint32_t out;
    MurmurHash3_x86_32(reinterpret_cast<const void*>(&k), sizeof(Key), seed, &out);
    return out;
  }
  template <class Key>
  h128 hash128(const Key& k, uint32_t seed) const {
    h128 h;
    MurmurHash3_x64_128(reinterpret_cast<const void*>(&k), sizeof(Key), seed, &h);
    return h;
  }
};

template <>
struct seeded_hash_function<Murmur3StringPiece> {
  template <class Key>
  uint32_t operator()(const Key& k, uint32_t seed) const {
    StringPiece s(k);
    uint32_t out;
    MurmurHash3_x86_32(s.data(), s.length(), seed, &out);
    return out;
  }
  template <class Key>
  h128 hash128(const Key& k, uint32_t seed) const {
    h128 h;
    StringPiece s(k);
    MurmurHash3_x64_128(s.data(), s.length(), seed, &h);
    return h;
  }
};

template <class HashFcn> struct seeded_hash
{ typedef seeded_hash_function<HashFcn> hash_function; };
// Use Murmur3 instead for all types defined in std::hash, plus
// std::string which is commonly extended.
template <> struct seeded_hash<std::hash<char*> >
{ typedef seeded_hash_function<Murmur3StringPiece> hash_function; };
template <> struct seeded_hash<std::hash<const char*> >
{ typedef seeded_hash_function<Murmur3StringPiece> hash_function; };
template <> struct seeded_hash<std::hash<std::string> >
{ typedef seeded_hash_function<Murmur3StringPiece> hash_function; };
template <> struct seeded_hash<std::hash<cxxmph::StringPiece> >
{ typedef seeded_hash_function<Murmur3StringPiece> hash_function; };

template <> struct seeded_hash<std::hash<char> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<unsigned char> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<short> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<unsigned short> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<int> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<unsigned int> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<long> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<unsigned long> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<long long> >
{ typedef seeded_hash_function<Murmur3> hash_function; };
template <> struct seeded_hash<std::hash<unsigned long long> >
{ typedef seeded_hash_function<Murmur3> hash_function; };

}  // namespace cxxmph

#endif  // __CXXMPH_SEEDED_HASH_H__
