#include "gladiator.h"
// #include <random>
#include <algorithm> // Pour std::copy

Gladiator *gladiator;
RobotData currentRobotData;

#include <chrono>

#undef abs

const float CELL_SIZE = 3.0 / 12;
const int MAZE_HEIGHT = 12;
const int MAZE_LENGTH = 12;
int ACTUAL_MAZE_HEIGHT = MAZE_HEIGHT;
int ACTUAL_MAZE_LENGTH = MAZE_LENGTH;
const MazeSquare *maze[MAZE_HEIGHT][MAZE_LENGTH];
const int TIME_FOR_NEW_WALL = 20;
const int DEPTH_WALKING = 5;

class Vector2
{
public:
    Vector2() : _x(0.), _y(0.) {}
    Vector2(float x, float y) : _x(x), _y(y) {}

    float norm1() const { return std::abs(_x) + std::abs(_y); }
    float norm2() const { return std::sqrt(_x * _x + _y * _y); }
    void normalize()
    {
        _x /= norm2();
        _y /= norm2();
    }
    Vector2 normalized() const
    {
        Vector2 out = *this;
        out.normalize();
        return out;
    }

    Vector2 operator-(const Vector2 &other) const { return {_x - other._x, _y - other._y}; }
    Vector2 operator+(const Vector2 &other) const { return {_x + other._x, _y + other._y}; }
    Vector2 operator*(float f) const { return {_x * f, _y * f}; }

    bool operator==(const Vector2 &other) const { return std::abs(_x - other._x) < 1e-5 && std::abs(_y - other._y) < 1e-5; }
    bool operator!=(const Vector2 &other) const { return !(*this == other); }

    float x() const { return _x; }
    float y() const { return _y; }

    float dot(const Vector2 &other) const { return _x * other._x + _y * other._y; }
    float cross(const Vector2 &other) const { return _x * other._y - _y * other._x; }
    float angle(const Vector2 &m) const { return std::atan2(cross(m), dot(m)); }
    float angle() const { return std::atan2(_y, _x); }

private:
    float _x, _y;
};

class MyPosition
{
public:
    MyPosition(int x, int y) : _x(x), _y(y) {}
    Vector2 toVector() const { return Vector2(_x * CELL_SIZE + 0.5 * CELL_SIZE, _y * CELL_SIZE + 0.5 * CELL_SIZE); }
    int getX() const { return _x; }
    int getY() const { return _y; }

private:
    int _x, _y;
};

const MyPosition ROBOT_POSITION = MyPosition(0, 0); /// On peut peut être récupérer ça avec le Gladiator @todo

void reset()
{
}

inline float moduloPi(float a) // return angle in [-pi; pi]
{
    return (a < 0.0) ? (std::fmod(a - M_PI, 2 * M_PI) + M_PI) : (std::fmod(a + M_PI, 2 * M_PI) - M_PI);
}

inline bool aim(Gladiator *gladiator, const Vector2 &target, bool showLogs)
{
    constexpr float ANGLE_REACHED_THRESHOLD = 0.1;
    constexpr float POS_REACHED_THRESHOLD = 0.05;

    auto posRaw = gladiator->robot->getData().position;
    Vector2 pos{posRaw.x, posRaw.y};

    Vector2 posError = target - pos;

    float targetAngle = posError.angle();
    float angleError = moduloPi(targetAngle - posRaw.a);

    bool targetReached = false;
    float leftCommand = 0.f;
    float rightCommand = 0.f;

    if (posError.norm2() < POS_REACHED_THRESHOLD) //
    {
        targetReached = true;
    }
    else if (std::abs(angleError) > ANGLE_REACHED_THRESHOLD)
    {
        float factor = 0.2;
        if (angleError < 0)
            factor = -factor;
        rightCommand = factor;
        leftCommand = -factor;
    }
    else
    {
        float factor = 0.5;
        rightCommand = factor; //+angleError*0.1  => terme optionel, "pseudo correction angulaire";
        leftCommand = factor;  //-angleError*0.1   => terme optionel, "pseudo correction angulaire";
    }

    gladiator->control->setWheelSpeed(WheelAxis::LEFT, leftCommand);
    gladiator->control->setWheelSpeed(WheelAxis::RIGHT, rightCommand);

    if (showLogs || targetReached)
    {
        // gladiator->log("ta %f, ca %f, ea %f, tx %f cx %f ex %f ty %f cy %f ey %f", targetAngle, posRaw.a, angleError, target.x(), pos.x(), posError.x(), target.y(), pos.y(), posError.y());
    }

    return targetReached;
}

void setup()
{
    // instanciation de l'objet gladiator
    gladiator = new Gladiator();
    // enregistrement de la fonction de reset qui s'éxecute à chaque fois avant qu'une partie commence
    gladiator->game->onReset(&reset);
    // gladiator->game->enableFreeMode(RemoteMode::OFF);

    // recuperer les informations du robot
    currentRobotData = gladiator->robot->getData();
}

class Timer
{
private:
    std::chrono::steady_clock::time_point lastTime;

public:
    Timer() : lastTime(std::chrono::steady_clock::now()) {}

    bool hasElapsed()
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
        return elapsedTime >= TIME_FOR_NEW_WALL;
    }

    void reset()
    {
        lastTime = std::chrono::steady_clock::now();
    }
};

bool isOnMazeBorder(int i, int j)
{
    if (i == (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 || i == ACTUAL_MAZE_LENGTH + (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 - 1)
    {
        return true;
    }
    else if (j == (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 || j == ACTUAL_MAZE_HEIGHT + (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isOutside(int i, int j)
{
    if (i < (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 || i >= ACTUAL_MAZE_LENGTH + (MAZE_LENGTH - ACTUAL_MAZE_LENGTH) / 2 - 1)
    {
        return true;
    }
    else if (j < (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 || j >= ACTUAL_MAZE_HEIGHT + (MAZE_HEIGHT - ACTUAL_MAZE_HEIGHT) / 2 - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int valueOfMS(const MazeSquare *ms, const bool throughWall)
{
    // return 1;
    int score = 0;

    const int caseAdverse = 15;
    const int caseEquipe = 0;
    const int caseRoquette = 20;
    const int caseDanger = -20;
    const int caseNeutre = 10;
    const int caseBorder = 20;
    const int caseOustide = -500;
    const int caseGoingThrougWall = -40;

    /// Si la case a une roquette ++
    if ((ms->coin).value)
    {
        score += caseRoquette;
    }
    /// si case en danger --
    if (ms->danger)
        score += caseDanger;
    /// si case est vide ++
    if (ms->possession == 0)
        score += caseNeutre;
    /// Si la case est à nous --
    else if (ms->possession == currentRobotData.teamId)
        score += caseEquipe;
    /// Si la case est aux autre ++
    else
        score += caseAdverse;
    /// Si la case est sur le bord du maze
    if (isOnMazeBorder(ms->i, ms->j))
        score += caseBorder;

    if (isOutside(ms->i, ms->j))
        score += caseOustide;

    if (throughWall)
        score += caseGoingThrougWall

            return score;
    /// Si la case est près d'un ennemie et qu'on a pas de roquette --
    /// Si la case est près d'un ennemie (en ligne droite) et qu'on a une roquette ++

    return 1;
}

int costOfMS(MazeSquare ms)
{
    /// Pour l'instant on ignore les couts
    return 1;

    /// On peut se dire qu'aller dans le même sens que ce qu'on a déjà coute moins cher que de tourner et encore moins cher qu'un demi tour
}

enum Direction
{
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
};

Direction getRandomDirection()
{
    int8_t rand_index = random() % 4;

    gladiator->log("RANDOM: %d", rand_index);

    // Sélection aléatoire d'une valeur de l'énuméré
    return static_cast<Direction>(rand_index);
}

MyPosition getNextCase(Direction direction, MyPosition position)
{
    /// On récupère la position d'après
    return position;
}

bool goingThroughWall(Direction direction, const MazeSquare *mazeSquare)
{
    bool isGoingThroughWall = false;

    if (mazeSquare == nullptr)
    {
        gladiator->log("On fait de la merde");
    }

    if (direction == Direction::TOP)
    {
        gladiator->log("TOP");
        if (mazeSquare != nullptr && mazeSquare->northSquare == nullptr)
        {
            gladiator->log("TOP");
            return true;
        }

        return false;
    }

    if (direction == Direction::LEFT)
    {
        gladiator->log("LEFT");
        if (mazeSquare != nullptr && mazeSquare->westSquare == nullptr)
        {
            gladiator->log("LEFT");
            return true;
        }

        return false;
    }

    if (direction == Direction::BOTTOM)
    {
        gladiator->log("BOTTOM");
        if (mazeSquare != nullptr && mazeSquare->southSquare == nullptr)
        {
            gladiator->log("BOTTOM");
            return true;
        }

        return false;
    }

    if (direction == Direction::RIGHT)
    {
        gladiator->log("RIGHT");
        if (mazeSquare != nullptr && mazeSquare->eastSquare == nullptr)
        {
            gladiator->log("RIGHT");
            return true;
        }

        return false;
    }

    gladiator->log("Traverse pas de mur");
    return isGoingThroughWall;
}

Direction actualDirection = Direction::LEFT;

void getRandomPathing(Direction *directionTab)
{
    const int numberOfTry = 10;
    int maxScore = 0;
    Direction directions[DEPTH_WALKING] = {};

    for (int i = 0; i < numberOfTry; i++)
    {
        int scoreOfTry = 0;
        Direction directionsOfTry[DEPTH_WALKING] = {};
        MyPosition actualPosition = ROBOT_POSITION;
        /// On tente d'avoir un parcours cool
        for (size_t j = 0; j < DEPTH_WALKING; j++)
        {
            /// On récupère une position random pour calculer sa valeur
            const Direction nextDirection = getRandomDirection();

            /// On ne peut pas forcément annuler le fait d'aller dans un mur parce que parfois ça sera la meilleure chose à faire
            /// Regarder si on est bloqué pour traverser un mur parce que la meilleure solution peut être de ne rien faire !
            const MyPosition nextPosition = getNextCase(nextDirection, actualPosition);
            const int x = nextPosition.getX();
            const int y = nextPosition.getY();

            gladiator->log("Next x : %d, Next y : %d", x, y);

            const int actualX = actualPosition.getX();
            const int actualY = actualPosition.getY();

            gladiator->log("Actual x : %d, Actual y : %d", actualX, actualY);

            const MazeSquare *nextMazeSquare = maze[x][y];
            const MazeSquare *actualMazeSquare = maze[actualX][actualY];

            const bool isGoingThroughWall = goingThroughWall(nextDirection, actualMazeSquare);

            gladiator->log("Though wall : %d", isGoingThroughWall);

            /// On récupère le prochain MazeSquare
            const int valueOfNextMazeSquare = valueOfMS(nextMazeSquare, isGoingThroughWall);

            gladiator->log("VALEUR: %d", valueOfNextMazeSquare);

            /// On update
            actualPosition = nextPosition;
            scoreOfTry += valueOfNextMazeSquare;
            directionsOfTry[j] = nextDirection;

            gladiator->log("NEXT %d", nextDirection);
        }

        if (maxScore < scoreOfTry)
        {
            maxScore = scoreOfTry;
            for (size_t i = 0; i < DEPTH_WALKING; i++)
            {
                directions[i] = directionsOfTry[i];
            }
            // std::copy(std::begin(directionsOfTry), std::end(directionsOfTry), std::begin(directions));
        }
    }

    for (size_t i = 0; i < DEPTH_WALKING; i++)
    {
        directionTab[i] = directions[i];
    }
}

bool isFirst = true;

void loop()
{
    if (isFirst)
    {
        if (gladiator->maze == nullptr)
        {
            gladiator->log("MAZE EST NULL");
        }

        // On récupère tous les points
        for (int x = 0; x < MAZE_LENGTH; x++)
        {
            for (int y = 0; y < MAZE_HEIGHT; y++)
            {
                const MazeSquare *msq = gladiator->maze->getSquare(x, y);
                maze[x][y] = msq;
            }
        }

        gladiator->log("STRAT TEST");
        isFirst = false;
        Direction direction[DEPTH_WALKING] = {};

        getRandomPathing(direction);

        for (size_t i = 0; i < DEPTH_WALKING; i++)
        {
            std::cout << "Valeur de Direction : " << static_cast<int>(direction[i]) << std::endl;
            gladiator->log("PATH : %d", direction[i]);
        }
    }

    if (gladiator->game->isStarted())
    {
        /// On set la vrai direction du robot
        /// actualDirection = Direction::?? @todo
        /// ROBOT_POSITION aussi : @todo

        static Timer timer = Timer();

        if (timer.hasElapsed())
        {
            ACTUAL_MAZE_HEIGHT = ACTUAL_MAZE_HEIGHT - 2;
            ACTUAL_MAZE_LENGTH = ACTUAL_MAZE_LENGTH - 2;
            timer.reset();
            // gladiator->log("ça fait 20 secondes");
        }
        else
        {
            // gladiator->log("Pas encore 20 secondes");
        }

        MyPosition pos = MyPosition(5, 5);

        static unsigned i = 0;
        bool showLogs = (i % 50 == 0);
        if (aim(gladiator, pos.toVector(), showLogs))
        {
            // gladiator->log("target atteinte !");
        }
        i++;
    }
    delay(10); // boucle à 100Hz
}