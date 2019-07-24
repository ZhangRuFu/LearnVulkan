#pragma once

#include "Core\Misc\EnumFlags.h"

enum TransformType
{
	kNoScaleTransform = 0,
	kUniformScaleTransform = 1 << 0,
	kNonUniformScaleTransform = 1 << 1,
	kOddNegativeScaleTransform = 1 << 2
};
ENUM_FLAGS(TransformType);

inline bool IsNoScaleTransform(TransformType type) { return type == kNoScaleTransform; }
inline bool IsUniformScaleTransform(TransformType type) { return HasFlag(type, kUniformScaleTransform); }
inline bool IsNonUniformScaleTransform(TransformType type) { return HasFlag(type, kNonUniformScaleTransform); }
inline bool IsOddNegativeScaleTransform(TransformType type) { return HasFlag(type, kOddNegativeScaleTransform); }