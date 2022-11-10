#pragma once

#include <spdlog/spdlog.h>

namespace rec
{
#ifdef DEBUG

#define logInfo(...)
#define logWarn(...)
#define logError(...)
#define logCritical(...)

#endif

#define logInfo(...) spdlog::info(__VA_ARGS__)
#define logWarn(...) spdlog::warn(__VA_ARGS__)
#define logError(...) spdlog::error(__VA_ARGS__)
#define logCritical(...) spdlog::critical(__VA_ARGS__)
}