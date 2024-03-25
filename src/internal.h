typedef struct Vec3f {
    float x, y, z;
} Vec3f;

typedef struct Vec4f {
    float x, y, z, w;
} Vec4f;

typedef struct Mat4f {
    Vec4f vectors[4];
} Mat4f;

void mat4f_identity(Mat4f *matrix);
void mat4f_scale(Mat4f *matrix, Vec3f vector);
void mat4f_translate(Mat4f *matrix, Vec3f vector);
void mat4f_rotate(Mat4f *matrix, float angle, Vec3f vector);
