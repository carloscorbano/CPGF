#pragma once

#include <string>

typedef std::string STRING;
#define STRINGIFY(x) #x

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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

typedef glm::vec1 VEC1;
typedef glm::vec2 VEC2;
typedef glm::vec3 VEC3;
typedef glm::vec4 VEC4;
typedef glm::vec4 COLOR;
typedef glm::mat4 MAT4;
typedef glm::quat QUAT;

#define MAT4_IDENTITY MAT4(1.0)

#define FLOAT_PRECISION 0.001f
#define DOUBLE_PRECISION 0.001

#define DEFAULT_FIXED_UPDATE_TIME (1.0 / 60.0)
#define DEFAULT_DRAW_TIMER (1.0 / 60.0)

#include <thread>
typedef std::thread::id THREAD_ID;
#define NULL_THREAD_ID THREAD_ID()

#define VEC2_ZERO       VEC2(0.0f, 0.0f)
#define VEC2_ONE        VEC2(1.0f, 1.0f)
#define VEC2_UP         VEC2(0.0f, 1.0f)
#define VEC2_RIGHT      VEC2(1.0f, 0.0f)

#define VEC3_ZERO       VEC3(0.0f, 0.0f, 0.0f)
#define VEC3_ONE        VEC3(1.0f, 1.0f, 1.0f)
#define VEC3_FORWARD    VEC3(0.0f, 0.0f, 1.0f)
#define VEC3_RIGHT      VEC3(1.0f, 0.0f, 0.0f)
#define VEC3_UP         VEC3(0.0f, 1.0f, 0.0f)

#define WORLD_UP        VEC3_UP
#define WORLD_RIGHT     VEC3_RIGHT
#define WORLD_FORWARD   VEC3_FORWARD
enum class Space { LOCAL, WORLD };

#define PI (3.14159f)
#define DegToRad(x) (x * (PI / 180.0f))
#define RadToDeg(x) (x * (180.0f / PI))

static FLOAT CLAMP_ROT(const FLOAT& value)
{
    if(value > 180.0f || value < -180.0f ||
        (value > -FLOAT_PRECISION && value < FLOAT_PRECISION))
    {
        return 0.0f;
    }

    return value;
}