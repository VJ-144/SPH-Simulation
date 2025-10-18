#include <iostream>

#include "particleMethods.cpp"

int main() {
  
  // simulation args
  int sweeps = 10;
 
  vector<double> vel = {0.0, 0.0};
  vector<double> pos = {0.0, 1.0};

  particle p;
  p.Init(vel, pos);

  for (int i = 0; i < sweeps; i++) {

    p.update();

  }


  return 0;
}
