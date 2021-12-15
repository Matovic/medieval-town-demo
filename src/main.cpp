// main.cpp

#include <iostream>
#include <map>
#include <list>
#include <unistd.h>
#include <cstdlib> //system

#include <ppgso/ppgso.h>

#include "scene_window.h"

// set size of a window
//const unsigned int HEIGHT = 1920;
const unsigned int WIDTH = 1400;//1080;

int main() {
  // Initialize our window
  SceneWindow window{WIDTH, WIDTH};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
