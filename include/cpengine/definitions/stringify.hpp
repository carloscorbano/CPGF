#pragma once

#include "typedefs.hpp"
#include <sstream>

#define STRINGIFY(x) #x

template<typename T>
static STRING ToString(const T& value)
{}

template<>
static STRING ToString<VEC1>(const VEC1& vec1)
{
    std::stringstream ss;
    ss << "VEC1(X:" << vec1.x << ")";
    return ss.str();
}

template<>
static STRING ToString<VEC2>(const VEC2& vec2)
{
    std::stringstream ss;
    ss << "VEC2(X:" << vec2.x << " Y:" << vec2.y << ")";
    return ss.str();
}

template<>
static STRING ToString<VEC3>(const VEC3& vec3)
{
    std::stringstream ss;
    ss << "VEC3(X:" << vec3.x << " Y:" << vec3.y << " Z:" << vec3.z << ")";
    return ss.str();
}

template<>
static STRING ToString<VEC4>(const VEC4& vec4)
{
    std::stringstream ss;
    ss << "VEC4(X(R):" << vec4.x << " Y(G):" << vec4.y << " Z(B):" << vec4.z << " W(A):" << vec4.w << ")";
    return ss.str();
}