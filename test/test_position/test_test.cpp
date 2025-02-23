// Copié/collé et modifié de https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityGettingStartedGuide.md
#include <MyPosition.hpp>
#include "unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_my_posititon_constructor(void) {
 MyPosition position(0, 0);
 TEST_ASSERT_EQUAL(0, position.getX());
 TEST_ASSERT_EQUAL(0, position.getY());
}


int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_my_posititon_constructor);
  return UNITY_END();
}

/**
  * For native dev-platform or for some embedded frameworks
  */
int main(void) {
  return runUnityTests();
}

/**
  * For ESP-IDF framework
  */
void app_main() {
  runUnityTests();
}