#include "string.h"

/// <summary>
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns>
/// 0	They compare equal
/// <0	Either the value of the first character that does not match is lower in the compared string, or all compared characters match but the compared string is shorter.
/// >0	Either the value of the first character that does not match is greater in the compared string, or all compared characters match but the compared string is longer.
/// </returns>

//constexpr int32 string_compare(const char* left, const char* right)
//{
//	if (left == nullptr || right == nullptr)
//	{
//		if (left != nullptr)
//		{
//			return -1;
//		}
//		else if (right != nullptr)
//		{
//			return 1;
//		}
//		else
//		{
//			return 0;
//		}
//	}
//
//	while (*left != k_null_char && *right != k_null_char)
//	{
//		int32 diff = *right - *left;
//
//		if (diff != 0)
//		{
//			return diff;
//		}
//
//		left++;
//		right++;
//	}
//
//	if (*left != k_null_char)
//	{
//		return -1;
//	}
//
//	if (*right != k_null_char)
//	{
//		return 1;
//	}
//
//	return 0;
//}