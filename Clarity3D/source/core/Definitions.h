#pragma once
#include <cinttypes>
#include <memory>
#include <iostream>
#include <filesystem>

namespace std {
	namespace filesystem = experimental::filesystem;
}

using namespace std::string_literals;

// Alias smart pointers
template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
using sptr = std::shared_ptr<T>;

// Alias fixed width int types
using uint32 = uint32_t;
using uint16 = uint16_t;
using uint8 = uint8_t;

using int32 = int32_t;
using int16 = int16_t;
using int8 = int8_t;

