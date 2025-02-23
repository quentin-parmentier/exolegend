// struct Position
// {
//     double x;
//     double y;
//     double a;
// };



// struct RobotData
// {
//     Position position;
//     // double speedLimit;
//     // double vl;
//     // double vr;
//     // short score;
//     byte lifes;
//     byte id;
//     byte teamId;
// };


class IRobotData
{
public:
    // virtual float getLatitude() const = 0;
    // virtual float getLongitude() const = 0;
    virtual RobotData getData() = 0;

    virtual RobotData getData(double x, double y, byte id, byte teamdID) = 0;
    // virtual byte id;
    // virtual byte lifes;
};