#include "Some.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_SomeTest(void) {
    someFn();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_SomeTest);

    return UNITY_END();
}


