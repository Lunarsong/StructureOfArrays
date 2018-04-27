#include <iostream>
#include <string>

#include <soa.h>

void print_soa_iis(const SoA<int, std::string, int>& soa_iis) {
  for (int i = 0; i < soa_iis.size(); ++i) {
    std::cout << "(" << i << "): " << soa_iis.get<0>(i) << ", \""
              << soa_iis.get<1>(i) << "\", " << soa_iis.get<2>(i) << std::endl;
  }
}

int main() {
  SoA<int, std::string, int> soa_iis;
  std::cout << "size: " << soa_iis.size() << std::endl;
  std::cout << "empty: " << soa_iis.empty() << std::endl;

  soa_iis.push_back(22, "Kitty", 4);
  soa_iis.push_back(0, "Hi", 2);

  std::cout << "size: " << soa_iis.size() << std::endl;
  std::cout << "empty: " << soa_iis.empty() << std::endl;
  std::cout << std::endl;

  // Iterate the arrays and print out the elements.
  std::cout << "Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << std::endl;

  // Swap the elements.
  soa_iis.swap(0, 1);
  std::cout << "Swapped first two elements. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << std::endl;

  // Modify an element
  soa_iis.get<1>(0) = "I am changed!";
  std::cout << "Modified the 0th element. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << std::endl;

  // Erase an element.
  soa_iis.erase(0);
  std::cout << "Erased 0th element. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << std::endl;

  // Add two new elements...
  soa_iis.push_back(1, "Hi... for now...", 5);
  soa_iis.push_back(80, "I'll survive!'", 20);
  std::cout << "Added 2 new elements. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << std::endl;

  // And erase the first two.
  soa_iis.erase(0, 2);
  std::cout << "Erased first 2 elements. Arrays now contain:\n";
  print_soa_iis(soa_iis);

  // Kill the arrays via resize.
  soa_iis.resize(0);
  std::cout << "Resized to 0. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << "size: " << soa_iis.size() << std::endl;
  std::cout << "empty: " << soa_iis.empty() << std::endl;
  std::cout << std::endl;

  // Resize to have some new elements.
  soa_iis.resize(5);
  std::cout << "Resized to 5. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << "size: " << soa_iis.size() << std::endl;
  std::cout << "empty: " << soa_iis.empty() << std::endl;
  std::cout << std::endl;

  // Modify an element and insert an element.
  soa_iis.get<1>(3) = "Oh hai!";
  soa_iis.get<2>(3) = 11;
  soa_iis.get<0>(3) = 22;
  soa_iis.push_back(100, "I'm new!.", 20);
  std::cout
      << "Modified the 4th element and added a new one. Arrays now contain:\n";
  print_soa_iis(soa_iis);
  std::cout << "size: " << soa_iis.size() << std::endl;
  std::cout << "empty: " << soa_iis.empty() << std::endl;
  std::cout << std::endl;

  // Get arrays and print them.
  std::cout << "Printing arrays individually:\n";
  int* ints0 = soa_iis.array<0>();
  std::string* strings = soa_iis.array<1>();
  int* ints1 = soa_iis.array<2>();
  for (int i = 0; i < soa_iis.size(); ++i) {
    std::cout << "Index: " << i << ": ";
    std::cout << ints0[i] << ", \"";
    std::cout << strings[i] << "\", ";
    std::cout << ints1[i] << std::endl;
  }
}
