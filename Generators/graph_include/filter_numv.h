#include <fstream>
#include <string>
#include <sstream>

static bool filter_numv(const int n)
{
  std::string line;
  bool flag1, flag2;
  int count = 0;
  std::istringstream f(NUMV);
  while (std::getline(f, line, '@'))
  {
    if (count == 0) {
      flag2 = false;
      flag1 = (n >= stoi(line));
    } else {
      flag2 = (n <= stoi(line));
    }
    if (flag1 && flag2) {
      return true;
    }
    count = (count + 1) % 2; // a bool flag to indicate odd/even
  }
  return false;
}