#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <memory>
#include <thread>

#include <filesystem>
#include <fstream>

#include "Core/Logger.h"

namespace MintEditor
{
	using namespace std::chrono_literals;
}