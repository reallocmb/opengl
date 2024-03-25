#include"internal.h"

#include<math.h>

void mat4f_identity(Mat4f *matrix)
{
    matrix->vectors[0].x = 1; matrix->vectors[0].y = 0; matrix->vectors[0].z = 0; matrix->vectors[0].w = 0;
    matrix->vectors[1].x = 0; matrix->vectors[1].y = 1; matrix->vectors[1].z = 0; matrix->vectors[1].w = 0;
    matrix->vectors[2].x = 0; matrix->vectors[2].y = 0; matrix->vectors[2].z = 1; matrix->vectors[2].w = 0;
    matrix->vectors[3].x = 0; matrix->vectors[3].y = 0; matrix->vectors[3].z = 0; matrix->vectors[3].w = 1;
}

void mat4f_scale(Mat4f *matrix, Vec3f vector)
{
    matrix->vectors[0].x = vector.x;
    matrix->vectors[1].y = vector.y;
    matrix->vectors[2].z = vector.z;
}

void mat4f_translate(Mat4f *matrix, Vec3f vector)
{
    matrix->vectors[0].w = vector.x;
    matrix->vectors[1].w = vector.y;
    matrix->vectors[2].w = vector.z;
}

void mat4f_rotate(Mat4f *matrix, float angle, Vec3f vector)
{
    float radians = angle * (3.14159265359 / 180);

    float cos = cosf(angle);
    float sin = cosf(angle);

    matrix->vectors[0].x = cos + vector.x * vector.x * (1 - cos);
    matrix->vectors[0].y = vector.x * vector.y * (1 - cos) - vector.z * sin;
    matrix->vectors[0].z = vector.x * vector.z * (1 - cos) + vector.y * sin;
    matrix->vectors[1].x = vector.y * vector.x * ( 1 - cos) + vector.z * sin;
    matrix->vectors[1].y = cos + vector.y * vector.y * (1 - cos);
    matrix->vectors[1].z = vector.y * vector.z * (1 - cos) - vector.x * sin;
    matrix->vectors[2].x = vector.z * vector.x * (1 - cos) - vector.y * sin;
    matrix->vectors[2].y = vector.z * vector.y * (1 - cos) + vector.x * sin;
    matrix->vectors[2].z = cos + vector.z * vector.z * (1 - cos);
}
