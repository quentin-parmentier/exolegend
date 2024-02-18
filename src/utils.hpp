#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cmath>
#include "MyPosition.hpp"
#include "gladiator.h"

#undef abs

float moduloPi(float a);
bool checkIfIsOutside(int x, int y, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength);
MyPosition getNearestValidPosition(int robotX, int robotY, int originalMazeHeight, int originalMazeLength, int nextMazeHeight, int nextMazeLength);
float distance(MyPosition pos1, MyPosition pos2);
float distanceFromPosition(Position pos1, Position pos2);
float calculerAngle(Position position1, Position position2);
bool isCloseEnoughToEnnemy(Position ennemy1, Position ennemy2, Position myPosition, float threshold);
RobotData getClosestRobotData(RobotData robotData1, RobotData robotData2, Position myPosition);
RobotData *getEnnemiesData(Gladiator *gladiator, RobotData ennemies[2]);

#endif