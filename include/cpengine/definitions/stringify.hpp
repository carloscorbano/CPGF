#pragma once

#include <sstream>
#include "typedefs.hpp"
#include <glm/gtx/matrix_decompose.hpp>

struct DecomposedMAT4
{
    VEC3 translation;
    QUAT orientation;
    VEC3 scale;
    VEC3 skew;
    VEC4 perspective;
    bool success;

    DecomposedMAT4(const MAT4& matrix)
    {
        success = glm::decompose(matrix, scale, orientation, translation, skew, perspective);
    }
};


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

template<>
static STRING ToString<MAT4>(const MAT4& matrix)
{
    DecomposedMAT4 decomposed(matrix);
    VEC3 eulerRot = glm::eulerAngles(decomposed.orientation);
    std::stringstream ss;
    ss << "Translation X: " << decomposed.translation.x << " ";
    ss << "Y: " << decomposed.translation.y << " ";
    ss << "Z: " << decomposed.translation.z << " | ";
    ss << "Euler Rotation X:" << eulerRot.x << " ";
    ss << "Y:" << eulerRot.y << " ";
    ss << "Z:" << eulerRot.z << " | ";
    ss << "Scale X: " << decomposed.scale.x << " ";
    ss << "Y: " << decomposed.scale.y << " ";
    ss << "Z: " << decomposed.scale.z;

    return ss.str();
}