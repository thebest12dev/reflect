#pragma once
#include "TypeDefinitions.h"
#include <string>
namespace CinnamonToast {
class Object {
  CTOAST_API virtual inline std::string toString() noexcept;
  CTOAST_API virtual inline unsigned int getUID() const noexcept;
  CTOAST_API virtual ~Object() = 0;
};
} // namespace CinnamonToast