// Copyright 2024 Vladislav Aleinik
//=======================================
#ifndef HEADER_GUARD_TEST_SYSTEM_HPP_INCLUDED
#define HEADER_GUARD_TEST_SYSTEM_HPP_INCLUDED

#include <cstddef>

namespace TestSystem
{
    typedef bool (*TestScenario)();

    enum TestResult
    {
        FAIL      = 0,
        OK        = 1,
        EXCEPTION = 2,
        ERROR     = 3,
        TIMEOUT   = 4
    };

    TestResult run_test(
        const char* name,
        TestScenario test,
        size_t timeout_ms = 1000U,
        bool inspect = false);
};

#endif // HEADER_GUARD_TEST_SYSTEM_HPP_INCLUDED
