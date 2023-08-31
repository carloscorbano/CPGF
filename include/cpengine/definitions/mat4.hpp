#pragma once

#include "stringify.hpp"

struct DecomposedMAT4
{
    VEC3 position;
    QUAT rotation;
    VEC3 scale;

    DecomposedMAT4(const MAT4& matrix)
    {
        position = matrix[3];
        for(int i = 0; i < 3; i++)
            scale[i] = glm::length(VEC3(matrix[i]));
        const glm::mat3 rotMtx(
            VEC3(matrix[0]) / scale[0],
            VEC3(matrix[1]) / scale[1],
            VEC3(matrix[2]) / scale[2]);
        rotation = glm::quat_cast(rotMtx);
    }
};

template<>
static STRING ToString<MAT4>(const MAT4& matrix)
{
    DecomposedMAT4 decomposed(matrix);
    VEC3 eulerRot = glm::eulerAngles(decomposed.rotation);
    std::stringstream ss;
    ss << "Translation X: " << decomposed.position.x << " ";
    ss << "Y: " << decomposed.position.y << " ";
    ss << "Z: " << decomposed.position.z << " | ";
    ss << "Euler Rotation X:" << eulerRot.x << " ";
    ss << "Y:" << eulerRot.y << " ";
    ss << "Z:" << eulerRot.z << " | ";
    ss << "Scale X: " << decomposed.scale.x << " ";
    ss << "Y: " << decomposed.scale.y << " ";
    ss << "Z: " << decomposed.scale.z;

    return ss.str();
}