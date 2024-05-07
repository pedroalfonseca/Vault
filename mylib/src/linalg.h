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

typedef union _vec2 {
    struct { f32 x, y; };

    f32 data[2];
} vec2;

#define vec2(x, y) ((vec2){{(x), (y)}})

inline vec2 vec2_add(vec2 a, vec2 b) {
    vec2 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;

    return ret;
}

inline vec2 vec2_sub(vec2 a, vec2 b) {
    vec2 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;

    return ret;
}

inline vec2 vec2_neg(vec2 v) {
    vec2 ret;
    ret.x = -v.x;
    ret.y = -v.y;

    return ret;
}

inline vec2 vec2_scale(vec2 a, f32 b) {
    vec2 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;

    return ret;
}

inline f32 vec2_dot(vec2 a, vec2 b) {
    return a.x * b.x + a.y * b.y;
}

inline f32 vec2_len2(vec2 v) {
    return vec2_dot(v, v);
}

inline f32 vec2_len(vec2 v) {
    return sqrtf(vec2_len2(v));
}

inline vec2 vec2_norm(vec2 v) {
    return vec2_scale(v, rsqrtf(vec2_len2(v)));
}

inline vec2 vec2_lerp(vec2 a, f32 t, vec2 b) {
    return vec2_add(vec2_scale(a, 1.0f - t), vec2_scale(b, t));
}

inline bool vec2_equal(vec2 a, vec2 b) {
    return a.x == b.x && a.y == b.y;
}

// --------------------------------------------------------------------------------

typedef union _ivec2 {
    struct { i32 x, y; };

    i32 data[2];
} ivec2;

#define ivec2(x, y) ((ivec2){{(x), (y)}})

inline ivec2 ivec2_add(ivec2 a, ivec2 b) {
    ivec2 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;

    return ret;
}

inline ivec2 ivec2_sub(ivec2 a, ivec2 b) {
    ivec2 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;

    return ret;
}

inline ivec2 ivec2_neg(ivec2 v) {
    ivec2 ret;
    ret.x = -v.x;
    ret.y = -v.y;

    return ret;
}

inline ivec2 ivec2_scale(ivec2 a, i32 b) {
    ivec2 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;

    return ret;
}

inline ivec2 ivec2_mul(ivec2 a, ivec2 b) {
    ivec2 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;

    return ret;
}

inline i32 ivec2_dot(ivec2 a, ivec2 b) {
    return a.x * b.x + a.y * b.y;
}

inline i32 ivec2_len2(ivec2 v) {
    return ivec2_dot(v, v);
}

inline f32 ivec2_len(ivec2 v) {
    return sqrtf((f32)ivec2_len2(v));
}

inline ivec2 ivec2_norm(ivec2 v) {
    return ivec2_scale(v, (i32)rsqrtf(ivec2_len2(v)));
}

inline ivec2 ivec2_lerp(ivec2 a, f32 t, ivec2 b) {
    return ivec2_add(ivec2_scale(a, 1.0f - t), ivec2_scale(b, t));
}

inline bool ivec2_equal(ivec2 a, ivec2 b) {
    return a.x == b.x && a.y == b.y;
}

// --------------------------------------------------------------------------------

typedef union _vec3 {
    struct { f32 x, y, z; };
    struct { f32 u, v, w; };
    struct { f32 r, g, b; };

    struct { vec2  xy; f32 _z; };
    struct { f32 _x; vec2  yz; };

    struct { vec2  uv; f32 _w; };
    struct { f32 _u; vec2  vw; };

    f32 data[3];
} vec3;

#define vec3(x, y, z) ((vec3){{(x), (y), (z)}})

inline vec3 vec3_add(vec3 a, vec3 b) {
    vec3 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;

    return ret;
}

inline vec3 vec3_sub(vec3 a, vec3 b) {
    vec3 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;

    return ret;
}

inline vec3 vec3_neg(vec3 v) {
    vec3 ret;
    ret.x = -v.x;
    ret.y = -v.y;
    ret.z = -v.z;

    return ret;
}

inline vec3 vec3_scale(vec3 a, f32 b) {
    vec3 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    ret.z = a.z * b;

    return ret;
}

inline vec3 vec3_mul(vec3 a, vec3 b) {
    vec3 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    ret.z = a.z * b.z;

    return ret;
}

inline vec3 vec3_cross(vec3 a, vec3 b) {
    vec3 ret;
    ret.x = a.y * b.z - a.z * b.y;
    ret.y = a.z * b.x - a.x * b.z;
    ret.z = a.x * b.y - a.y * b.x;

    return ret;
}

inline f32 vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 vec3_len2(vec3 v) {
    return vec3_dot(v, v);
}

inline f32 vec3_len(vec3 v) {
    return sqrtf(vec3_len2(v));
}

inline vec3 vec3_norm(vec3 v) {
    return vec3_scale(v, rsqrtf(vec3_len2(v)));
}

inline vec3 vec3_lerp(vec3 a, f32 t, vec3 b) {
    return vec3_add(vec3_scale(a, 1.0f - t), vec3_scale(b, t));
}

inline bool vec3_equal(vec3 a, vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// --------------------------------------------------------------------------------

typedef union _vec4 {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };

    struct { vec2   xy; f32 _z; f32  _w; };
    struct { f32  _x; vec2  yz; f32 __w; };
    struct { f32 __x; f32 _y; vec2   zw; };

    f32 data[4];
} vec4;

#define vec4(x, y, z, w) ((vec4){{(x), (y), (z), (w)}})

inline vec4 vec4_add(vec4 a, vec4 b) {
    vec4 ret;
    ret.x = a.x + b.x;
    ret.y = a.y + b.y;
    ret.z = a.z + b.z;
    ret.w = a.w + b.w;

    return ret;
}

inline vec4 vec4_sub(vec4 a, vec4 b) {
    vec4 ret;
    ret.x = a.x - b.x;
    ret.y = a.y - b.y;
    ret.z = a.z - b.z;
    ret.w = a.w - b.w;

    return ret;
}

inline vec4 vec4_neg(vec4 v) {
    vec4 ret;
    ret.x = -v.x;
    ret.y = -v.y;
    ret.z = -v.z;
    ret.w = -v.w;

    return ret;
}

inline vec4 vec4_scale(vec4 a, f32 b) {
    vec4 ret;
    ret.x = a.x * b;
    ret.y = a.y * b;
    ret.z = a.z * b;
    ret.w = a.w * b;

    return ret;
}

inline vec4 vec4_mul(vec4 a, vec4 b) {
    vec4 ret;
    ret.x = a.x * b.x;
    ret.y = a.y * b.y;
    ret.z = a.z * b.z;
    ret.w = a.w * b.w;

    return ret;
}

inline f32 vec4_dot(vec4 a, vec4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline f32 vec4_len2(vec4 v) {
    return vec4_dot(v, v);
}

inline f32 vec4_len(vec4 v) {
    return sqrtf(vec4_len2(v));
}

inline vec4 vec4_norm(vec4 v) {
    return vec4_scale(v, rsqrtf(vec4_len2(v)));
}

inline vec4 vec4_lerp(vec4 a, f32 t, vec4 b) {
    return vec4_add(vec4_scale(a, 1.0f - t), vec4_scale(b, t));
}

inline bool vec4_equal(vec4 a, vec4 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

#define COLOR_WHITE     ((vec4){{1.0f, 1.0f, 1.0f, 1.0f}})
#define COLOR_BLACK     ((vec4){{0.0f, 0.0f, 0.0f, 1.0f}})
#define COLOR_RED       ((vec4){{1.0f, 0.0f, 0.0f, 1.0f}})
#define COLOR_GREEN     ((vec4){{0.0f, 1.0f, 0.0f, 1.0f}})
#define COLOR_BLUE      ((vec4){{0.0f, 0.0f, 1.0f, 1.0f}})
#define COLOR_YELLOW    ((vec4){{1.0f, 1.0f, 0.0f, 1.0f}})
#define COLOR_CYAN      ((vec4){{0.0f, 1.0f, 1.0f, 1.0f}})
#define COLOR_MAGENTA   ((vec4){{1.0f, 0.0f, 1.0f, 1.0f}})
#define COLOR_ORANGE    ((vec4){{1.0f, 0.5f, 0.0f, 1.0f}})
#define COLOR_PURPLE    ((vec4){{0.5f, 0.0f, 1.0f, 1.0f}})
#define COLOR_TURQUOISE ((vec4){{0.0f, 1.0f, 0.5f, 1.0f}})

// --------------------------------------------------------------------------------

typedef union _mat4 {
    vec4  columns[4];
    f32 data[16];
} mat4;

inline mat4 mat4_mul(mat4 a, mat4 b) {
    vec4 a0 = a.columns[0];
    vec4 a1 = a.columns[1];
    vec4 a2 = a.columns[2];
    vec4 a3 = a.columns[3];

    vec4 b0 = b.columns[0];
    vec4 b1 = b.columns[1];
    vec4 b2 = b.columns[2];
    vec4 b3 = b.columns[3];

    mat4 ret;
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

inline mat4 diagonal(f32 x) {
    mat4 ret = DEFAULT_VAL;
    ret.data[4 * 0 + 0] = x;
    ret.data[4 * 1 + 1] = x;
    ret.data[4 * 2 + 2] = x;
    ret.data[4 * 3 + 3] = x;

    return ret;
}

inline mat4 perspective(f32 vfov, f32 aspect_ratio, f32 near, f32 far) {
    assert(absf(aspect_ratio - EPSILON) > 0.0f);

    f32 tan_half_vfov = tanf(vfov / 2.0f);

    mat4 ret = DEFAULT_VAL;
    ret.data[4 * 0 + 0] =  1.0f / (aspect_ratio * tan_half_vfov);
    ret.data[4 * 1 + 1] =  1.0f / tan_half_vfov;
    ret.data[4 * 2 + 2] = -(far + near) / (far - near);
    ret.data[4 * 2 + 3] = -1.0f;
    ret.data[4 * 3 + 2] = -(2.0f * far * near) / (far - near);

    return ret;
}

inline mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    mat4 ret = DEFAULT_VAL;

    ret.data[4 * 0 + 0] =  2.0f / (right - left);
    ret.data[4 * 1 + 1] =  2.0f / (top - bottom);
    ret.data[4 * 2 + 2] = -2.0f / (far - near);
    ret.data[4 * 3 + 3] =  1.0f;

    ret.data[4 * 3 + 0] = -(right + left) / (right - left);
    ret.data[4 * 3 + 1] = -(top + bottom) / (top - bottom);
    ret.data[4 * 3 + 2] = -(far + near) / (far - near);

    return ret;
}

inline mat4 scale(mat4 m, vec3 v) {
    mat4 ret;
    ret.columns[0] = vec4_scale(m.columns[0], v.x);
    ret.columns[1] = vec4_scale(m.columns[1], v.y);
    ret.columns[2] = vec4_scale(m.columns[2], v.z);
    ret.columns[3] = m.columns[3];

    return ret;
}

inline mat4 translate(mat4 m, vec3 v) {
    mat4 ret = m;
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

inline mat4 rotate(mat4 m, f32 angle, vec3 v) {
    f32 cos_angle = cosf(angle);
    f32 sin_angle = sinf(angle);

    vec3 axis = vec3_norm(v);
    vec3 tmp = vec3_scale(axis, 1.0f - cos_angle);

    mat4 rot;
    rot.data[4 * 0 + 0] = cos_angle + tmp.x * axis.x;
    rot.data[4 * 0 + 1] = tmp.x * axis.y + sin_angle * axis.z;
    rot.data[4 * 0 + 2] = tmp.x * axis.z - sin_angle * axis.y;

    rot.data[4 * 1 + 0] = tmp.y * axis.x - sin_angle * axis.z;
    rot.data[4 * 1 + 1] = cos_angle + tmp.y * axis.y;
    rot.data[4 * 1 + 2] = tmp.y * axis.z + sin_angle * axis.x;

    rot.data[4 * 2 + 0] = tmp.z * axis.x + sin_angle * axis.y;
    rot.data[4 * 2 + 1] = tmp.z * axis.y - sin_angle * axis.x;
    rot.data[4 * 2 + 2] = cos_angle + tmp.z * axis.z;

    mat4 ret;
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

inline mat4 look_at(vec3 eye, vec3 center, vec3 up) {
    vec3 forward = vec3_norm(vec3_sub(center, eye));
    vec3 sideways = vec3_norm(vec3_cross(forward, up));
    vec3 upward = vec3_norm(vec3_cross(sideways, forward));

    mat4 ret = diagonal(1.0f);
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

typedef union _quat {
    struct { union { vec3 xyz; struct { f32 x, y, z; }; }; f32 w; };

    f32 data[4];
} quat;

inline f32 quat_dot(quat a, quat b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline quat quat_norm(quat q) {
    f32 factor = rsqrtf(quat_dot(q, q));

    quat ret;
    ret.x = q.x * factor;
    ret.y = q.y * factor;
    ret.z = q.z * factor;
    ret.w = q.w * factor;

    return ret;
}

inline quat quat_nlerp(quat a, f32 t, quat b) {
    quat actual_b = b;

    if (quat_dot(a, b) < 0.0f) {
        actual_b.xyz = vec3_neg(b.xyz);
        actual_b.w = -b.w;
    }

    quat ret;
    ret.xyz = vec3_lerp(a.xyz, t, actual_b.xyz);
    ret.w = lerp(a.w, t, actual_b.w);

    return quat_norm(ret);
}

// --------------------------------------------------------------------------------

typedef union _rect {
    struct { f32 x, y, width, height; };

    struct { vec2 pos, size; };
} rect;

#define rect(x, y, w, h) ((rect){{(x), (y), (w), (h)}})

inline bool poi32_in_rect(vec2 p, rect r) {
    return p.x >= r.x            &&
           p.x <= r.x + r.width  &&
           p.y >= r.y            &&
           p.y <= r.y + r.height;
}

bool rects_collide(rect a, rect b) {
    return a.x < b.x + b.width   &&
           a.x + a.width > b.x   &&
           a.y < b.y + b.height  &&
           a.y + a.height > b.y;
}

// --------------------------------------------------------------------------------

typedef union _irect {
    struct { i32 x, y, width, height; };

    struct { ivec2 pos, size; };
} irect;

#define irect(x, y, w, h) ((irect){{(x), (y), (w), (h)}})

inline bool poi32_in_irect(ivec2 p, irect r) {
    return p.x >= r.x            &&
           p.x <= r.x + r.width  &&
           p.y >= r.y            &&
           p.y <= r.y + r.height;
}

inline bool irects_collide(irect a, irect b) {
    return a.x < b.x + b.width   &&
           a.x + a.width > b.x   &&
           a.y < b.y + b.height  &&
           a.y + a.height > b.y;
}

// --------------------------------------------------------------------------------

#endif // LINALG_H