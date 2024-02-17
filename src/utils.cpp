#include "utils.hpp"

float moduloPi(float a) // return angle in [-pi; pi]
{
  return (a < 0.0) ? (std::fmod(a - M_PI, 2 * M_PI) + M_PI) : (std::fmod(a + M_PI, 2 * M_PI) - M_PI);
}

MazeSquare *msTransfo(const MazeSquare *currentCase)
{
  MazeSquare *ms;
  if (currentCase->northSquare != nullptr)
  {
    ms = currentCase->northSquare->southSquare;
  }
  else if (currentCase->southSquare != nullptr)
  {
    ms = currentCase->southSquare->northSquare;
  }
  else if (currentCase->eastSquare != nullptr)
  {
    ms = currentCase->eastSquare->westSquare;
  }
  else
  {
    ms = currentCase->westSquare->eastSquare;
  }

  if (ms != nullptr)
  {
    std::cout << "Case courante : x " << ((int)ms->i) << " y " << ((int)ms->j) << "\n";
    msList->printDebug();
  }
  return ms;
}
