// Az array_eraser osztály template hagyományos C-s tömbök kezelését könnyíti
// meg. A hagyományos tömbök mérete nem változhat meg, így a törlés ebben az
// esetben azt jelenti, hogy a tömb elejére kerülnek - ugyanabban a relatív
// sorrendben, mint eredetileg - azok az elemek, amelyek nincsenek törölve, a
// tömb végére pedig a töröltek (tetszőleges sorrendben). Lehet törölni
// értéket (erase), index szerint (erase_index). Az array_eraser létrehozásához
// elvár egy tömböt. A size megadja, hogy hány aktív (nem törölt) eleme van a
// tömbnek, így 0-tól a size-1-ig elérhetőek a tömbben az aktív elemek. A count
// megadja, hogy egy érték hányszor szerepel az aktív elemek között. Végül egy
// iterátort biztosít, ami csak az aktív elemeken iterál.

#include "arerase.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

struct is_even : std::unary_function<int, bool> {

  bool operator()(int x) const { return 0 == x % 2; }
};

struct too_long : std::unary_function<std::string, bool> {

  bool operator()(const std::string &s) const { return s.length() > 4; }
};

const int max = 1000;

bool check() {
  std::string langs[] = {"C++", "Haskell", "Ada", "Python", "Ada"};
  array_eraser<std::string> le(langs, sizeof(langs) / sizeof(langs[0]));
  le.erase("Ada");

  int d[] = {7, 4};
  const array_eraser<int> cd(d, sizeof(d) / sizeof(d[0]));

  int a[max];
  for (int i = 0; i < max; ++i) {
    a[i] = 0 == i % 2 ? max / 2 : i;
  }
  array_eraser<int> ae(a, max);
  ae.erase(max / 2);

  int x[] = {3, 2, 3, 3, 2};
  array_eraser<int> xe(x, sizeof(x) / sizeof(x[0]));
  xe.erase(2);
  xe.erase(3);
  if (3 != le.size() || langs[3] != langs[4] || "Python" != langs[2] ||
      max / 2 != ae.size() || 0 != xe.size() || max / 2 != a[max - 1] ||
      3 != a[1] || 3 != std::count(x, x + sizeof(x) / sizeof(x[0]), 3)) {
    return false;
  }

  le.erase_index(1);
  ae.erase_index(0);

  if ("Python" != langs[1] || 2 != le.size() || 1 != cd.count(7) ||
      1 != std::count(langs, langs + sizeof(langs) / sizeof(langs[0]),
                      "Haskell") ||
      0 != le.count("Haskell") || 0 != ae.count(1) || 1 != ae.count(5) ||
      0 != le.count("Ada") || 0 != ae.count(max / 2)) {
    return false;
  }

  int s[] = {3, 1, 2};
  array_eraser<int> se(s, sizeof(s) / sizeof(s[0]));
  se.erase_index(2);

  array_eraser<std::string>::iterator i =
      std::find(le.begin(), le.end(), "C++");

  return 0 == std::count(se.begin(), se.end(), 2) && i != le.end();
}

int main() {
  std::cout << "Your solution is " << (check() ? "" : "not ")
            << "ready for submission." << std::endl;
}
