#pragma once

#include <memory>

// it's just too much typing
template<typename T>
using box = std::unique_ptr<T>;
