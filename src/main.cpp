// main.cpp

#include <iostream>
#include <map>
#include <list>
#include <unistd.h>
#include <cstdlib> //system

#include <ppgso/ppgso.h>

#include "scene_window.h"

// set size of a window
const unsigned int SIZE = 1024;

int main() {
  //std::cout << get_current_dir_name() << '\n';
  //auto path = std::filesystem::current_path(); //getting path
  //chdir("cd ./cmake-build/"); //setting path
  //std::string path = "/home/erik/FIIT/Bc/4ZS/PPGSO/cvicenia/medieval_town_demo/cmake-build";
  //std::system("echo hellou");
  //if (path.compare(get_current_dir_name()) != 0)
  //  while (std::system("cd ./cmake-build/") < 0);
  //std::cout << get_current_dir_name() << '\n';
  // Initialize our window
  SceneWindow window{SIZE};

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
