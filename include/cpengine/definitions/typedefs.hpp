#pragma once

#include <string>

typedef std::string STRING;
#define STRINGIFY(x) #x

#include <glm/glm.hpp>

typedef glm::uint8 ui8;
typedef glm::int8 i8;
typedef glm::uint16 ui16;
typedef glm::int16 i16;
typedef glm::uint32 ui32;
typedef glm::int32 i32;
typedef glm::uint64 ui64;
typedef glm::int64 i64;

typedef glm::uint8 BOOL;
typedef glm::f32 FLOAT;
typedef glm::f64 DOUBLE;

typedef glm::vec2 VEC2;
typedef glm::vec3 VEC3;
typedef glm::vec4 VEC4;
typedef glm::vec4 COLOR;
typedef glm::mat4 MAT4;
typedef glm::quat QUAT;