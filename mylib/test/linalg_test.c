#include "../src/linalg.h"

#include <stdio.h>

int main(void) {
    Vec2 a = vec2(5.0f, 2.0f);
    printf("{%f, %f}\n", a.x, a.y);

    iVec2 b = ivec2(-5, 2);
    printf("{%d, %d}\n", b.x, b.y);

    Vec4 c = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    printf("{%f, %f, %f, %f}\n", c.r, c.g, c.b, c.a);

    Rect d = rect(0.0f, 0.0f, 5.0f, 3.0f);
    printf("{%f, %f, %f, %f}\n", d.x, d.y, d.width, d.height);

    iRect e = irect(0, 0, 5, 3);
    printf("{%d, %d, %d, %d}\n", e.x, e.y, e.width, e.height);

    Vec4 color = COLOR_PURPLE;
    printf("{%f, %f, %f, %f}\n", color.x, color.y, color.z, color.w);

    return 0;
}