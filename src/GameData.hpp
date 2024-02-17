#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#define MAZE_HEIGHT 12
#define MAZE_LENGTH 12

#include "gladiator.h"
enum class STRATEGY_STATE {
    IDLE, // Claculate direction
    DEFEND,
    ATTACK,
    GETPOINT, //explore the maze to get point 
    BACKONMAZE,
    LAUCNHROCKET // test de lancher une requette 
};

class GameState
{
    public:
        GameState(Gladiator *gladiator);
        void GetMazeData(Gladiator *gladiator);
        void reduceMaze();
        void reset();
        RobotData currentRobotData;
        RobotData coopRobotData;
        bool isFirstTurn; 
        STRATEGY_STATE currentRobotState;
        int actual_maze_height;
        int actual_maze_length;
        const MazeSquare *maze[MAZE_HEIGHT][MAZE_LENGTH];
        const MazeSquare *previousSquare;

    private:
        Gladiator *gladiator;
};

#endif