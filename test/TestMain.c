#include "unity.h"
#include "Main.h"

void setUp(void) {}
void tearDown(void) {}

void test_SomeTest(void) {
    some_fn();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_SomeTest);

    return UNITY_END();
}


