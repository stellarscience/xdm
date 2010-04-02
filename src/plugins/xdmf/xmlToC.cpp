/**
 * Utility program to transform an XML document into an escaped C string for
 * inclusion into C or C++ source code. This is useful for compiling a schema
 * directly into a source file for XML document validation.
 */

#include <fstream>
#include <iostream>

int main(int argc, char * argv[]) {
  /* Make sure a file name was passed in. */
  if ( argc < 3 ) {
    std::cerr << "Usage: " << argv[0]; 
    std::cerr << " <symbol> <inputxmlfile>" << std::endl;
    return -1;
  }

  /* Open the input file for reading. */
  std::ifstream input(argv[2]);
  if ( !(input.good() && input.is_open()) ) {    
    std::cerr << argv[2] << ": File does not exist." << std::endl;
    return -1;
  }

  // File exists and is open.
  std::cout << "#ifndef " << argv[1] << "_SYMBOL_HEADER_" << std::endl;
  std::cout << "#define " << argv[1] << "_SYMBOL_HEADER_" << std::endl;
  std::cout << "#define " << argv[1] << " \"";
  std::size_t count = 0;
  for (char c = input.get(); !input.eof(); c = input.get()) {
    if (c == '\"') {
      // escape the quote
      std::cout << "\\\"";
      count += 2;
    } else if (c == '\n') {
      // print a quote, continuation character, newline, and quote
      std::cout << "\"\\\n\"";
      count++;
    } else {
      std::cout << c;
      count++;
    }
  }
  std::cout << "\"\n";
  // print a definition for the size of the xml string
  std::cout << "#define " << argv[1] << "_LENGTH " << count << std::endl;
  std::cout << "#endif" << std::endl;

  return 0;
}
