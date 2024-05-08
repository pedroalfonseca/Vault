#ifndef LINALG_H
#define LINALG_H

#include "types.h"

#include <math.h>
#include <assert.h>

// --------------------------------------------------------------------------------

#define EPSILON             (1e-6)
#define PI                  3.14159265358979323846

// --------------------------------------------------------------------------------

#define min(x, y)           ((x) < (y) ? (x) : (y))
#define max(x, y)           ((x) > (y) ? (x) : (y))

#define clamp(x, low, high) ((x) < (low) ? (low) : ((x) > (high) ? (high) : (x)))

// --------------------------------------------------------------------------------

// Adapted from https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_math.c#L552
inline f32 rsqrtf(f32 num) {
    f32 ret = num;
    i32 i = *(i32 *)&ret;
    i = 0x5f3759df - (i >> 1);
    ret = *(f32 *)&i;

    return ret * (1.5f - 0.5f * num * ret * ret);
}

// Adapted from https://github.com/id-Software/Quake-III-Arena/blob/master/code/game/q_math.c#L574
inline f32 absf(f32 x) {
    f32 ret = x;
    i32 tmp = *(i32 *)&ret;
    tmp &= 0x7fffffff;

    return *(f32 *)&tmp;
}

// --------------------------------------------------------------------------------

inline f32 radians(f32 degs) {
    return (degs * PI) / 180.0f;
}

inline f32 degrees(f32 rads) {
    return (rads * 180.0f) / PI;
}

// --------------------------------------------------------------------------------

inline f32 lerp(f32 a, f32 t, f32 b) {
    return (1.0f - t) * a + t * b;
}

// --------------------------------------------------------------------------------

typedef union _Vec2 {
    struct { f32 x, y; };

    f32 data[2];
} Vec2;

inline Vec2 vec2(f32 x, f32 y) {
    Vec2 ret;
    ret.x = x;
    ret.y = y;

    return ret;
}

inline Vec2 vec2_add(Vec2 a, Vec2 b) {
    Vec2 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;

    return ret;
}

inline Vec2 vec2_sub(Vec2 a, Vec2 b) {
    Vec2 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;

    return ret;
}

inline Vec2 vec2_neg(Vec2 v) {
    Vec2 ret;
    ret.x = -v.x;
    ret.y = -v.y;

    return ret;
}

inline Vec2 vec2_scale(Vec2 a, f32 b) {
    Vec2 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;

    return ret;
}

inline f32 vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

inline f32 vec2_len2(Vec2 v) {
    return vec2_dot(v, v);
}

inline f32 vec2_len(Vec2 v) {
    return sqrtf(vec2_len2(v));
}

inline Vec2 vec2_norm(Vec2 v) {
    return vec2_scale(v, rsqrtf(vec2_len2(v)));
}

inline Vec2 vec2_lerp(Vec2 a, f32 t, Vec2 b) {
    return vec2_add(vec2_scale(a, 1.0f - t), vec2_scale(b, t));
}

inline bool vec2_equal(Vec2 a, Vec2 b) {
    return a.x == b.x && a.y == b.y;
}

// --------------------------------------------------------------------------------

typedef union _iVec2 {
    struct { i32 x, y; };

    i32 data[2];
} iVec2;

inline iVec2 ivec2(i32 x, i32 y) {
    iVec2 ret;
    ret.x = x;
    ret.y = y;

    return ret;
}

inline iVec2 ivec2_add(iVec2 a, iVec2 b) {
    iVec2 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;

    return ret;
}

inline iVec2 ivec2_sub(iVec2 a, iVec2 b) {
    iVec2 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;

    return ret;
}

inline iVec2 ivec2_neg(iVec2 v) {
    iVec2 ret;
    ret.x = -v.x;
    ret.y = -v.y;

    return ret;
}

inline iVec2 ivec2_scale(iVec2 a, i32 b) {
    iVec2 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;

    return ret;
}

inline iVec2 ivec2_mul(iVec2 a, iVec2 b) {
    iVec2 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;

    return ret;
}

inline i32 ivec2_dot(iVec2 a, iVec2 b) {
    return a.x * b.x + a.y * b.y;
}

inline i32 ivec2_len2(iVec2 v) {
    return ivec2_dot(v, v);
}

inline f32 ivec2_len(iVec2 v) {
    return sqrtf((f32)ivec2_len2(v));
}

inline iVec2 ivec2_norm(iVec2 v) {
    return ivec2_scale(v, (i32)rsqrtf(ivec2_len2(v)));
}

inline iVec2 ivec2_lerp(iVec2 a, f32 t, iVec2 b) {
    return ivec2_add(ivec2_scale(a, 1.0f - t), ivec2_scale(b, t));
}

inline bool ivec2_equal(iVec2 a, iVec2 b) {
    return a.x == b.x && a.y == b.y;
}

// --------------------------------------------------------------------------------

typedef union _Vec3 {
    struct { f32 x, y, z; };
    struct { f32 u, v, w; };
    struct { f32 r, g, b; };

    struct { Vec2 xy; f32  _z; };
    struct { f32  _x; Vec2 yz; };

    struct { Vec2 uv; f32  _w; };
    struct { f32  _u; Vec2 vw; };

    f32 data[3];
} Vec3;

inline Vec3 vec3(f32 x, f32 y, f32 z) {
    Vec3 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;

    return ret;
}

inline Vec3 vec3_add(Vec3 a, Vec3 b) {
    Vec3 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;

    return ret;
}

inline Vec3 vec3_sub(Vec3 a, Vec3 b) {
    Vec3 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;

    return ret;
}

inline Vec3 vec3_neg(Vec3 v) {
    Vec3 ret;
    ret.x = -v.x;
    ret.y = -v.y;
    ret.z = -v.z;

    return ret;
}

inline Vec3 vec3_scale(Vec3 a, f32 b) {
    Vec3 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    ret.z = a.z * b;

    return ret;
}

inline Vec3 vec3_mul(Vec3 a, Vec3 b) {
    Vec3 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    ret.z = a.z * b.z;

    return ret;
}

inline Vec3 vec3_cross(Vec3 a, Vec3 b) {
    Vec3 ret;
    ret.x = a.y * b.z - a.z * b.y;
    ret.y = a.z * b.x - a.x * b.z;
    ret.z = a.x * b.y - a.y * b.x;

    return ret;
}

inline f32 vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 vec3_len2(Vec3 v) {
    return vec3_dot(v, v);
}

inline f32 vec3_len(Vec3 v) {
    return sqrtf(vec3_len2(v));
}

inline Vec3 vec3_norm(Vec3 v) {
    return vec3_scale(v, rsqrtf(vec3_len2(v)));
}

inline Vec3 vec3_lerp(Vec3 a, f32 t, Vec3 b) {
    return vec3_add(vec3_scale(a, 1.0f - t), vec3_scale(b, t));
}

inline bool vec3_equal(Vec3 a, Vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// --------------------------------------------------------------------------------

typedef union _Vec4 {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };

    struct { Vec2 xy; f32  _z; f32   _w; };
    struct { f32  _x; Vec2 yz; f32  __w; };
    struct { f32 __x; f32  _y; Vec2  zw; };

    f32 data[4];
} Vec4;

inline Vec4 vec4(f32 x, f32 y, f32 z, f32 w) {
    Vec4 ret;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    ret.w = w;

    return ret;
}

inline Vec4 vec4_add(Vec4 a, Vec4 b) {
    Vec4 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;
    ret.w = a.w + b.w;

    return ret;
}

inline Vec4 vec4_sub(Vec4 a, Vec4 b) {
    Vec4 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;
    ret.w = a.w - b.w;

    return ret;
}

inline Vec4 vec4_neg(Vec4 v) {
    Vec4 ret;
    ret.x = -v.x;
    ret.y = -v.y;
    ret.z = -v.z;
    ret.w = -v.w;

    return ret;
}

inline Vec4 vec4_scale(Vec4 a, f32 b) {
    Vec4 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    ret.z = a.z * b;
    ret.w = a.w * b;

    return ret;
}

inline Vec4 vec4_mul(Vec4 a, Vec4 b) {
    Vec4 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    ret.z = a.z * b.z;
    ret.w = a.w * b.w;

    return ret;
}

inline f32 vec4_dot(Vec4 a, Vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32 vec4_len2(Vec4 v) {
    return vec4_dot(v, v);
}

inline f32 vec4_len(Vec4 v) {
    return sqrtf(vec4_len2(v));
}

inline Vec4 vec4_norm(Vec4 v) {
    return vec4_scale(v, rsqrtf(vec4_len2(v)));
}

inline Vec4 vec4_lerp(Vec4 a, f32 t, Vec4 b) {
    return vec4_add(vec4_scale(a, 1.0f - t), vec4_scale(b, t));
}

inline bool vec4_equal(Vec4 a, Vec4 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

#define COLOR_WHITE     ((Vec4){{1.0f, 1.0f, 1.0f, 1.0f}})
#define COLOR_BLACK     ((Vec4){{0.0f, 0.0f, 0.0f, 1.0f}})
#define COLOR_RED       ((Vec4){{1.0f, 0.0f, 0.0f, 1.0f}})
#define COLOR_GREEN     ((Vec4){{0.0f, 1.0f, 0.0f, 1.0f}})
#define COLOR_BLUE      ((Vec4){{0.0f, 0.0f, 1.0f, 1.0f}})
#define COLOR_YELLOW    ((Vec4){{1.0f, 1.0f, 0.0f, 1.0f}})
#define COLOR_CYAN      ((Vec4){{0.0f, 1.0f, 1.0f, 1.0f}})
#define COLOR_MAGENTA   ((Vec4){{1.0f, 0.0f, 1.0f, 1.0f}})
#define COLOR_ORANGE    ((Vec4){{1.0f, 0.5f, 0.0f, 1.0f}})
#define COLOR_PURPLE    ((Vec4){{0.5f, 0.0f, 1.0f, 1.0f}})
#define COLOR_TURQUOISE ((Vec4){{0.0f, 1.0f, 0.5f, 1.0f}})

// --------------------------------------------------------------------------------

typedef union _Mat4 {
    Vec4 columns[4];
    f32  data[16];
} Mat4;

inline Mat4 mat4_mul(Mat4 a, Mat4 b) {
    Vec4 a0 = a.columns[0];
    Vec4 a1 = a.columns[1];
    Vec4 a2 = a.columns[2];
    Vec4 a3 = a.columns[3];

    Vec4 b0 = b.columns[0];
    Vec4 b1 = b.columns[1];
    Vec4 b2 = b.columns[2];
    Vec4 b3 = b.columns[3];

    Mat4 ret;
    ret.columns[0] = vec4_add(
                        vec4_add(
                            vec4_scale(a0, b0.x),
                            vec4_scale(a1, b0.y)
                        ),
                        vec4_add(
                            vec4_scale(a2, b0.z),
                            vec4_scale(a3, b0.w)
                        )
                     );

    ret.columns[1] = vec4_add(
                        vec4_add(
                            vec4_scale(a0, b1.x),
                            vec4_scale(a1, b1.y)
                        ),
                        vec4_add(
                            vec4_scale(a2, b1.z),
                            vec4_scale(a3, b1.w)
                        )
                     );

    ret.columns[2] = vec4_add(
                        vec4_add(
                            vec4_scale(a0, b2.x),
                            vec4_scale(a1, b2.y)
                        ),
                        vec4_add(
                            vec4_scale(a2, b2.z),
                            vec4_scale(a3, b2.w)
                        )
                     );

    ret.columns[3] = vec4_add(
                        vec4_add(
                            vec4_scale(a0, b3.x),
                            vec4_scale(a1, b3.y)
                        ),
                        vec4_add(
                            vec4_scale(a2, b3.z),
                            vec4_scale(a3, b3.w)
                        )
                     );

    return ret;
}

inline Mat4 diagonal(f32 x) {
    Mat4 ret = DEFAULT_VAL;
    ret.data[4 * 0 + 0] = x;
    ret.data[4 * 1 + 1] = x;
    ret.data[4 * 2 + 2] = x;
    ret.data[4 * 3 + 3] = x;

    return ret;
}

inline Mat4 perspective(f32 vfov, f32 aspect_ratio, f32 near, f32 far) {
    assert(absf(aspect_ratio - EPSILON) > 0.0f);

    f32 tan_half_vfov = tanf(vfov / 2.0f);

    Mat4 ret = DEFAULT_VAL;
    ret.data[4 * 0 + 0] =  1.0f / (aspect_ratio * tan_half_vfov);
    ret.data[4 * 1 + 1] =  1.0f / tan_half_vfov;
    ret.data[4 * 2 + 2] = -(far + near) / (far - near);
    ret.data[4 * 2 + 3] = -1.0f;
    ret.data[4 * 3 + 2] = -(2.0f * far * near) / (far - near);

    return ret;
}

inline Mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    Mat4 ret = DEFAULT_VAL;

    ret.data[4 * 0 + 0] =  2.0f / (right - left);
    ret.data[4 * 1 + 1] =  2.0f / (top - bottom);
    ret.data[4 * 2 + 2] = -2.0f / (far - near);
    ret.data[4 * 3 + 3] =  1.0f;

    ret.data[4 * 3 + 0] = -(right + left) / (right - left);
    ret.data[4 * 3 + 1] = -(top + bottom) / (top - bottom);
    ret.data[4 * 3 + 2] = -(far + near) / (far - near);

    return ret;
}

inline Mat4 scale(Mat4 m, Vec3 v) {
    Mat4 ret;
    ret.columns[0] = vec4_scale(m.columns[0], v.x);
    ret.columns[1] = vec4_scale(m.columns[1], v.y);
    ret.columns[2] = vec4_scale(m.columns[2], v.z);
    ret.columns[3] = m.columns[3];

    return ret;
}

inline Mat4 translate(Mat4 m, Vec3 v) {
    Mat4 ret = m;
    ret.columns[3] = vec4_add(
                        vec4_add(
                            vec4_scale(
                                m.columns[0],
                                v.x
                            ),
                            vec4_scale(
                                m.columns[1],
                                v.y
                            )
                        ),
                        vec4_add(
                            vec4_scale(
                                m.columns[2],
                                v.z
                            ),
                            m.columns[3]
                        )
                     );

    return ret;
}

inline Mat4 rotate(Mat4 m, f32 angle, Vec3 v) {
    f32 cos_angle = cosf(angle);
    f32 sin_angle = sinf(angle);

    Vec3 axis = vec3_norm(v);
    Vec3 tmp = vec3_scale(axis, 1.0f - cos_angle);

    Mat4 rot;
    rot.data[4 * 0 + 0] = cos_angle + tmp.x * axis.x;
    rot.data[4 * 0 + 1] = tmp.x * axis.y + sin_angle * axis.z;
    rot.data[4 * 0 + 2] = tmp.x * axis.z - sin_angle * axis.y;

    rot.data[4 * 1 + 0] = tmp.y * axis.x - sin_angle * axis.z;
    rot.data[4 * 1 + 1] = cos_angle + tmp.y * axis.y;
    rot.data[4 * 1 + 2] = tmp.y * axis.z + sin_angle * axis.x;

    rot.data[4 * 2 + 0] = tmp.z * axis.x + sin_angle * axis.y;
    rot.data[4 * 2 + 1] = tmp.z * axis.y - sin_angle * axis.x;
    rot.data[4 * 2 + 2] = cos_angle + tmp.z * axis.z;

    Mat4 ret;
    ret.columns[0] = vec4_add(
                        vec4_add(
                            vec4_scale(
                                m.columns[0],
                                rot.columns[0].x
                            ),
                            vec4_scale(
                                m.columns[1],
                                rot.columns[0].y
                            )
                        ),
                        vec4_scale(
                            m.columns[2],
                            rot.columns[0].z
                        )
                     );

    ret.columns[1] = vec4_add(
                        vec4_add(
                            vec4_scale(
                                m.columns[0],
                                rot.columns[1].x
                            ),
                            vec4_scale(
                                m.columns[1],
                                rot.columns[1].y
                            )
                        ),
                        vec4_scale(
                            m.columns[2],
                            rot.columns[2].z
                        )
                     );

    ret.columns[2] = vec4_add(
                        vec4_add(
                            vec4_scale(
                                m.columns[0],
                                rot.columns[2].x
                            ),
                            vec4_scale(
                                m.columns[1],
                                rot.columns[2].y
                            )
                        ),
                        vec4_scale(
                            m.columns[2],
                            rot.columns[2].z
                        )
                     );

    ret.columns[3] = m.columns[3];

    return ret;
}

inline Mat4 look_at(Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 forward = vec3_norm(vec3_sub(center, eye));
    Vec3 sideways = vec3_norm(vec3_cross(forward, up));
    Vec3 upward = vec3_norm(vec3_cross(sideways, forward));

    Mat4 ret = diagonal(1.0f);
    ret.data[4 * 0 + 0] =  sideways.x;
    ret.data[4 * 1 + 0] =  sideways.y;
    ret.data[4 * 2 + 0] =  sideways.z;

    ret.data[4 * 0 + 1] =  upward.x;
    ret.data[4 * 1 + 1] =  upward.y;
    ret.data[4 * 2 + 1] =  upward.z;

    ret.data[4 * 0 + 2] = -forward.x;
    ret.data[4 * 1 + 2] = -forward.y;
    ret.data[4 * 2 + 2] = -forward.z;

    ret.data[4 * 3 + 0] = -vec3_dot(sideways, eye);
    ret.data[4 * 3 + 1] = -vec3_dot(upward, eye);
    ret.data[4 * 3 + 2] =  vec3_dot(forward, eye);

    return ret;
}

// --------------------------------------------------------------------------------

typedef union _Quat {
    struct { union { Vec3 xyz; struct { f32 x, y, z; }; }; f32 w; };

    f32 data[4];
} Quat;

inline f32 quat_dot(Quat a, Quat b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline Quat quat_norm(Quat q) {
    f32 factor = rsqrtf(quat_dot(q, q));

    Quat ret;
    ret.x = q.x * factor;
    ret.y = q.y * factor;
    ret.z = q.z * factor;
    ret.w = q.w * factor;

    return ret;
}

inline Quat quat_nlerp(Quat a, f32 t, Quat b) {
    Quat actual_b = b;

    if (quat_dot(a, b) < 0.0f) {
        actual_b.xyz = vec3_neg(b.xyz);
        actual_b.w = -b.w;
    }

    Quat ret;
    ret.xyz = vec3_lerp(a.xyz, t, actual_b.xyz);
    ret.w = lerp(a.w, t, actual_b.w);

    return quat_norm(ret);
}

// --------------------------------------------------------------------------------

typedef union _Rect {
    struct { f32 x, y, width, height; };

    struct { Vec2 pos, size; };
} Rect;

inline Rect rect(f32 x, f32 y, f32 w, f32 h) {
    Rect ret;
    ret.x = x;
    ret.y = y;
    ret.width = w;
    ret.height = h;

    return ret;
}

inline bool point_in_rect(Vec2 p, Rect r) {
    return p.x >= r.x            &&
           p.x <= r.x + r.width  &&
           p.y >= r.y            &&
           p.y <= r.y + r.height;
}

bool rects_collide(Rect a, Rect b) {
    return a.x < b.x + b.width   &&
           a.x + a.width > b.x   &&
           a.y < b.y + b.height  &&
           a.y + a.height > b.y;
}

// --------------------------------------------------------------------------------

typedef union _iRect {
    struct { i32 x, y, width, height; };

    struct { iVec2 pos, size; };
} iRect;

inline iRect irect(i32 x, i32 y, i32 w, i32 h) {
    iRect ret;
    ret.x = x;
    ret.y = y;
    ret.width = w;
    ret.height = h;

    return ret;
}

inline bool point_in_irect(iVec2 p, iRect r) {
    return p.x >= r.x            &&
           p.x <= r.x + r.width  &&
           p.y >= r.y            &&
           p.y <= r.y + r.height;
}

inline bool irects_collide(iRect a, iRect b) {
    return a.x < b.x + b.width   &&
           a.x + a.width > b.x   &&
           a.y < b.y + b.height  &&
           a.y + a.height > b.y;
}

// --------------------------------------------------------------------------------

#ifdef __cplusplus
inline bool   operator==(Vec2   a, Vec2  b) { return a.x == b.x && a.y == b.y; }
inline bool   operator==(iVec2  a, iVec2 b) { return a.x == b.x && a.y == b.y; }
inline bool   operator==(Vec3   a, Vec3  b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
inline bool   operator==(Vec4   a, Vec4  b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

inline bool   operator!=(Vec2   a, Vec2  b) { return !(a == b); }
inline bool   operator!=(iVec2  a, iVec2 b) { return !(a == b); }
inline bool   operator!=(Vec3   a, Vec3  b) { return !(a == b); }
inline bool   operator!=(Vec4   a, Vec4  b) { return !(a == b); }

inline Vec2   operator- (Vec2   v)          { return Vec2 (-v.x, -v.y); }
inline iVec2  operator- (iVec2  v)          { return iVec2(-v.x, -v.y); }
inline Vec3   operator- (Vec3   v)          { return Vec3 (-v.x, -v.y, -v.z); }
inline Vec4   operator- (Vec4   v)          { return Vec4 (-v.x, -v.y, -v.z, -v.w); }

inline Vec2   operator+ (Vec2   a, Vec2  b) { return Vec2 (a.x + b.x, a.y + b.y); }
inline iVec2  operator+ (iVec2  a, iVec2 b) { return iVec2(a.x + b.x, a.y + b.y); }
inline Vec3   operator+ (Vec3   a, Vec3  b) { return Vec3 (a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vec4   operator+ (Vec4   a, Vec4  b) { return Vec4 (a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

inline Vec2   operator- (Vec2   a, Vec2  b) { return Vec2 (a.x - b.x, a.y - b.y); }
inline iVec2  operator- (iVec2  a, iVec2 b) { return iVec2(a.x - b.x, a.y - b.y); }
inline Vec3   operator- (Vec3   a, Vec3  b) { return Vec3 (a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vec4   operator- (Vec4   a, Vec4  b) { return Vec4 (a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

inline Vec2   operator* (Vec2   a, Vec2  b) { return Vec2 (a.x * b.x, a.y * b.y); }
inline iVec2  operator* (iVec2  a, iVec2 b) { return iVec2(a.x * b.x, a.y * b.y); }
inline Vec3   operator* (Vec3   a, Vec3  b) { return Vec3 (a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vec4   operator* (Vec4   a, Vec4  b) { return Vec4 (a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }

inline Vec2   operator* (Vec2   a, f32   b) { return Vec2 (a.x * b, a.y * b); }
inline iVec2  operator* (iVec2  a, f32   b) { return iVec2(a.x * b, a.y * b); }
inline Vec3   operator* (Vec3   a, f32   b) { return Vec3 (a.x * b, a.y * b, a.z * b); }
inline Vec4   operator* (Vec4   a, f32   b) { return Vec4 (a.x * b, a.y * b, a.z * b, a.w * b); }

inline Vec2   operator* (f32    a, Vec2  b) { return b * a; }
inline iVec2  operator* (f32    a, iVec2 b) { return b * a; }
inline Vec3   operator* (f32    a, Vec3  b) { return b * a; }
inline Vec4   operator* (f32    a, Vec4  b) { return b * a; }

inline Vec2   operator/ (Vec2   a, f32   b) { return a * (1.0f / b); }
inline iVec2  operator/ (iVec2  a, f32   b) { return a * (1.0f / b); }
inline Vec3   operator/ (Vec3   a, f32   b) { return a * (1.0f / b); }
inline Vec4   operator/ (Vec4   a, f32   b) { return a * (1.0f / b); }

inline Vec2  &operator+=(Vec2  &a, Vec2  b) { a = a + b; return a; }
inline iVec2 &operator+=(iVec2 &a, iVec2 b) { a = a + b; return a; }
inline Vec3  &operator+=(Vec3  &a, Vec3  b) { a = a + b; return a; }
inline Vec4  &operator+=(Vec4  &a, Vec4  b) { a = a + b; return a; }

inline Vec2  &operator-=(Vec2  &a, Vec2  b) { a = a - b; return a; }
inline iVec2 &operator-=(iVec2 &a, iVec2 b) { a = a - b; return a; }
inline Vec3  &operator-=(Vec3  &a, Vec3  b) { a = a - b; return a; }
inline Vec4  &operator-=(Vec4  &a, Vec4  b) { a = a - b; return a; }

inline Vec2  &operator*=(Vec2  &a, Vec2  b) { a = a * b; return a; }
inline iVec2 &operator*=(iVec2 &a, iVec2 b) { a = a * b; return a; }
inline Vec3  &operator*=(Vec3  &a, Vec3  b) { a = a * b; return a; }
inline Vec4  &operator*=(Vec4  &a, Vec4  b) { a = a * b; return a; }

inline Vec2  &operator*=(Vec2  &a, f32   b) { a = a * b; return a; }
inline iVec2 &operator*=(iVec2 &a, f32   b) { a = a * b; return a; }
inline Vec3  &operator*=(Vec3  &a, f32   b) { a = a * b; return a; }
inline Vec4  &operator*=(Vec4  &a, f32   b) { a = a * b; return a; }

inline Vec2  &operator/=(Vec2  &a, f32   b) { a = a / b; return a; }
inline iVec2 &operator/=(iVec2 &a, f32   b) { a = a / b; return a; }
inline Vec3  &operator/=(Vec3  &a, f32   b) { a = a / b; return a; }
inline Vec4  &operator/=(Vec4  &a, f32   b) { a = a / b; return a; }

// --------------------------------------------------------------------------------

inline Mat4 operator*(Mat4 a, Mat4 b) {
    Vec4 a0 = a.columns[0];
    Vec4 a1 = a.columns[1];
    Vec4 a2 = a.columns[2];
    Vec4 a3 = a.columns[3];

    Vec4 b0 = b.columns[0];
    Vec4 b1 = b.columns[1];
    Vec4 b2 = b.columns[2];
    Vec4 b3 = b.columns[3];

    Mat4 ret;
    ret.columns[0] = a0 * b0.x + a1 * b0.y + a2 * b0.z + a3 * b0.w;
    ret.columns[1] = a0 * b1.x + a1 * b1.y + a2 * b1.z + a3 * b1.w;
    ret.columns[2] = a0 * b2.x + a1 * b2.y + a2 * b2.z + a3 * b2.w;
    ret.columns[3] = a0 * b3.x + a1 * b3.y + a2 * b3.z + a3 * b3.w;

    return ret;
}
#endif // __cplusplus

// --------------------------------------------------------------------------------

#endif // LINALG_H