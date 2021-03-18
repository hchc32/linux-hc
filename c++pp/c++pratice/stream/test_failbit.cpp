#include <iostream>     // std::cerr
#include <fstream>      // std::ifstream

int main () {
  std::ifstream is;
  is.open ("test.txt",std::fstream::in);
  std::cout << is.rdstate() << std::endl;
  if ( (is.rdstate() & std::ifstream::failbit ) != 0 )
    std::cerr << "Error opening 'test.txt'\n";
  return 0;
}
