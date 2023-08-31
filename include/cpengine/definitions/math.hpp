#pragma once

#include "typedefs.hpp"

#define FLOAT_PRECISION 0.001f
#define DOUBLE_PRECISION 0.001

#define PI (3.14159f)
#define DegToRad(x) (x * (PI / 180.0f))
#define RadToDeg(x) (x * (180.0f / PI))

template<typename T>
static bool IS_EQUAL(const T& left, const T& right)
{
    assert(false);
}

template<>
static bool IS_EQUAL<FLOAT>(const FLOAT& left, const FLOAT& right)
{
    FLOAT result = left - right;
    return result < FLOAT_PRECISION && result > -FLOAT_PRECISION;
}

template<>
static bool IS_EQUAL<DOUBLE>(const DOUBLE& left, const DOUBLE& right)
{
    DOUBLE result = left - right;
    return result < DOUBLE_PRECISION && result > -DOUBLE_PRECISION;
}

template<typename T>
static void APPLY_PRECISION(T& value)
{
    assert(false);
}

template<>
static void APPLY_PRECISION<FLOAT>(FLOAT& value)
{
    if(value > -FLOAT_PRECISION && value < FLOAT_PRECISION)
    {
        value = 0.0f;
    }
}

template<>
static void APPLY_PRECISION<DOUBLE>(DOUBLE& value)
{
    if(value > -DOUBLE_PRECISION && value < DOUBLE_PRECISION)
    {
        value = 0.0;
    }
}

static void CLAMP_ROT(FLOAT& value)
{
    APPLY_PRECISION<FLOAT>(value);
    if(value > 180.0f || value < -180.0f)
    {
        value = 0.0f;
    }
}