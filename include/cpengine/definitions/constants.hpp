#pragma once

#define MAT4_IDENTITY   MAT4(1.0)
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