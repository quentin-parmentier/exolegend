#include "utils.hpp"
#include <iostream>

float moduloPi(float a) // return angle in [-pi; pi]
{
    return (a < 0.0) ? (std::fmod(a - M_PI, 2 * M_PI) + M_PI) : (std::fmod(a + M_PI, 2 * M_PI) - M_PI);
}

bool checkIfIsOutside(int x, int y, int originalMazeHeight, int originalMazeLength, int *mazeHeight, int *mazeLength)
{
    int xoffset = (originalMazeLength - *mazeLength) / 2;
    int yoffset = (originalMazeHeight - *mazeHeight) / 2;
    return x < xoffset || y < yoffset || x >= (xoffset + *mazeLength) || y >= (yoffset + *mazeHeight);
}

MyPosition getNearestValidPosition(int robotX, int robotY, int originalMazeHeight, int originalMazeLength, int nextMazeHeight, int nextMazeLength)
{
    MyPosition nearestPosition = MyPosition();
    double distanceMin = std::numeric_limits<double>::max();

    /// On parcours tous les bords du terrain
    int minX = (originalMazeLength - nextMazeLength) / 2;
    int minY = (originalMazeHeight - nextMazeHeight) / 2;
    int maxX = minX + nextMazeLength;
    int maxY = minY + nextMazeHeight;

    MyPosition robotPos = MyPosition(robotX, robotY);

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            MyPosition testedPos = MyPosition(x, y);
            double distanceForPos = distance(robotPos, testedPos);
            if (distanceForPos < distanceMin)
            {
                distanceMin = distanceForPos;
                nearestPosition = testedPos;
            }
        }
    }

    std::cout << "La position la plus proche safe - " << nearestPosition.getX() << ":" << nearestPosition.getY();
    return nearestPosition;
}

float calculerAngle(Position position1, Position position2)
{
    // Calcul de la différence entre les coordonnées x et y des deux robots
    float deltaX = position2.x - position1.x;
    float deltaY = position2.y - position1.y;

    // Utilisation de la fonction atan2 pour calculer l'angle
    float angle = std::atan2(deltaY, deltaX);

    // Conversion de l'angle de radians à degrés si nécessaire
    // angle = angle * 180 / M_PI; // Décommentez cette ligne si vous souhaitez l'angle en degrés

    return angle;
}

float distance(MyPosition pos1, MyPosition pos2)
{
    return std::sqrt(std::pow(pos1.getX() - pos2.getX(), 2) + std::pow(pos1.getY() - pos2.getY(), 2));
}

float distanceFromPosition(Position pos1, Position pos2)
{
    return std::sqrt(std::pow(pos1.x - pos2.x, 2) + std::pow(pos1.y - pos2.y, 2));
}

bool isCloseEnoughToEnnemy(Position ennemy1, Position ennemy2, Position myPosition, float threshold)
{
    if (distanceFromPosition(ennemy1, myPosition) < threshold || distanceFromPosition(ennemy2, myPosition))
    {
        return true;
    }

    return false;
}

RobotData getClosestRobotData(RobotData robotData1, RobotData robotData2, Position myPosition)
{
    Position rp1 = robotData1.position;
    Position rp2 = robotData2.position;

    float drp1 = distanceFromPosition(rp1, myPosition);
    float drp2 = distanceFromPosition(rp2, myPosition);

    if (drp1 < drp2 && robotData1.lifes > 0)
    {
        return robotData1;
    }

    return robotData2;
}

RobotData *getEnnemiesData(Gladiator *gladiator, RobotData ennemies[2])
{
    RobotData myRobotData = gladiator->robot->getData();
    uint8_t myTeam = myRobotData.teamId;

    RobotList robotList = gladiator->game->getPlayingRobotsId();

    int id = 0;
    for (size_t i = 0; i < 3; i++)
    {
        RobotData robotData = gladiator->game->getOtherRobotData(robotList.ids[i]);
        if (robotData.teamId != myTeam)
        {
            ennemies[id] = robotData;
            id++;
        }
    }

    return ennemies;
}