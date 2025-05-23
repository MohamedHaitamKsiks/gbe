#pragma once

#include <doctest.h>

#define GBE_TEST_SUITE(suiteName) \
    DOCTEST_TEST_SUITE_IMPL(#suiteName, suiteName##__TestSuite)
