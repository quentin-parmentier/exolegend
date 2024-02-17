#include "GameData.hpp"

GameState::GameState(Gladiator* gladiator) : gladiator(gladiator){
    currentRobotData = gladiator->robot->getData();
    isFirstTurn = true;
    // coopRobotData
    actual_maze_height = MAZE_HEIGHT;
    actual_maze_length = MAZE_LENGTH;
}

void GameState::GetMazeData(Gladiator* gladiator){
     // On récupère tous les points
        for (int x = 0; x < MAZE_LENGTH; x++)
        {
            for (int y = 0; y < MAZE_HEIGHT; y++)
            {
                const MazeSquare *msq = gladiator->maze->getSquare(x, y);
                maze[x][y] = msq;
            }
        }
}

void GameState::reduceMaze(){
    actual_maze_height = actual_maze_height - 2;
    actual_maze_length = actual_maze_length - 2;
}

void GameState::reset(){
    //currentRobotData = NULL;
    isFirstTurn = true;
    // coopRobotData
    actual_maze_height = MAZE_HEIGHT;
    actual_maze_length = MAZE_LENGTH;
}