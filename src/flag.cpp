// flag.cpp
//
// Copyright (c) 2019-2021 Kristofer Berggren
// All rights reserved.
//
// falanet is distributed under the MIT license, see LICENSE for details.

#include "flag.h"

bool Flag::GetSeen(uint32_t p_Flag)
{
  return (p_Flag & Seen);
}

void Flag::SetSeen(uint32_t& p_Flags, bool p_Seen)
{
  p_Flags = p_Seen ? (p_Flags | Seen) : (p_Flags & ~Seen);
}
