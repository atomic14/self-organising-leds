#pragma once

#pragma once

#include <stdint.h>
#include <algorithm>

#undef min

class Palette
{
protected:
  uint16_t colors[256];

public:
  Palette();
  inline uint16_t get_color(int index)
  {
    return colors[std::max(0, std::min(255, index))];
  }
};
