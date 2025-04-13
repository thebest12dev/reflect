#pragma once
#include "TypeDefinitions.h"
#include <fstream>
#include <streambuf>
namespace CinnamonToast {
// Class to redirect std::cout to a file and console
class LogBuffer : public std::streambuf {
public:
  CTOAST_API LogBuffer();

protected:
  CTOAST_API int overflow(int c) override;

  CTOAST_API int sync() override;

private:
  std::streambuf *sb1;
  std::streambuf *sb2;
  std::ofstream fileStream;
};
} // namespace CinnamonToast