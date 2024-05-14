#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <memory>
#include <algorithm>
#include <utility>
#include <functional>

#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Hazel/Core/Log.h"

#ifdef HZ_PLATFORM_WINDOW
#include <Windows.h>
#endif // HZ_PLATFORM_WINDOW
#include "Hazel/Debug/Instrumentor.h"