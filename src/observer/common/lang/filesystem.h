#pragma once
#include <filesystem>

namespace common {
namespace filesystem {
using namespace std::filesystem;
}
}

using common::filesystem::path;
using common::filesystem::directory_iterator;
using common::filesystem::exists;
using common::filesystem::create_directory;
using common::filesystem::remove;
using common::filesystem::file_size;
