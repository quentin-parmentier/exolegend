#include <RobotsData.hpp>
#include "unity.h"
#include "IRobotData.hpp"

class MockRobotData : public IRobotData
{
public:
  RobotData getData() override
  {
    Position pos = {0, 0, 0};
    return {pos, 0, 1, 1};
  };

  RobotData getData(double x, double y, byte id, byte teamID) override
  {
    Position pos = {x, y, 0};
    return {pos, 0, id, teamID};
  };
};

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_isAllyClose(void)
{
  MockRobotData mock_current_robot;
  MockRobotData mock_ally_robot;

  RobotData currentRobotData = mock_current_robot.getData(2, 1, 41, 1);
  RobotData allyRobotData = mock_ally_robot.getData(4, 1, 85, 1);
  RobotsData data;

  TEST_ASSERT_TRUE_MESSAGE(data.isAllyClose(2, currentRobotData, allyRobotData), "Ally not detected in the proximity zone");
}

void test_isEnnemyClose(void)
{
  MockRobotData mock_current_robot;
  MockRobotData mock_enemy_robot;

  RobotData currentRobotData = mock_current_robot.getData(2, 1, 41, 1);
  RobotData enemy1RobotData = mock_enemy_robot.getData(4, 1, 85, 2);
  RobotData enemy2RobotData = mock_enemy_robot.getData(4, 2, 82, 2);
  RobotsData data;

  TEST_ASSERT_TRUE_MESSAGE(data.isEnemyClose(1, currentRobotData, enemy1RobotData, enemy2RobotData), "Ally not detected in the proximity zone");
}

int runUnityTests(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_isAllyClose);
  return UNITY_END();
}

/**
 * For native dev-platform or for some embedded frameworks
 */
int main(void)
{
  return runUnityTests();
}

/**
 * For ESP-IDF framework
 */
void app_main()
{
  runUnityTests();
}