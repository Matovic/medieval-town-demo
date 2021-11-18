// main.cpp

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "scene_window.h"

// set size of a window
const unsigned int SIZE = 1024;

int main() {
  // Initialize our window
  SceneWindow window{SIZE};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
