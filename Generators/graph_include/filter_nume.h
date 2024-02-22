#include <fstream>
#include <string>
#include <sstream>

static bool filter_nume(const int m)
{
  std::string line;

  bool flag1, flag2;
  int count = 0;
  std::istringstream f(NUME);
  while (std::getline(f, line, '@'))
  {
    if (count == 0) {
      flag2 = false;
      flag1 = (m >= stoi(line));
    } else {
      flag2 = (m <= stoi(line));
    }
    if (flag1 && flag2) {
      return true;
    }
    count = (count + 1) % 2;
  }
  return false;
}