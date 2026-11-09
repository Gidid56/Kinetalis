#pragma once
#include "color.h"
#include "enums.h"
#include "guards.h"
#include "permit.h"
#include "ranges.h"
#include "sequence.h"
#include "storage.h"
#include "timer.h"

namespace utility
{
#ifdef _DEBUG
    constexpr bool is_debug{ true };
#else
    constexpr bool is_debug{ false };
#endif
}
