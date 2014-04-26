#ifndef WHITGL_MATH_H_
#define WHITGL_MATH_H_

#include <stdbool.h>

typedef long int whitgl_int;
typedef double whitgl_float;

typedef struct
{
 	whitgl_int x, y;
} whitgl_ivec;
static const whitgl_ivec whitgl_ivec_zero = {0,0};

typedef struct
{
	whitgl_float x, y;
} whitgl_fvec;
static const whitgl_fvec whitgl_fvec_zero = {0,0};

typedef struct
{
	whitgl_ivec a, b;
} whitgl_iaabb;
static const whitgl_iaabb whitgl_iaabb_zero = { {0,0}, {0,0} };

typedef struct
{
	whitgl_fvec a, b;
} whitgl_faabb;
static const whitgl_faabb whitgl_faabb_zero = { {0,0}, {0,0} };

typedef struct
{
	whitgl_fvec pos;
	whitgl_float size;
} whitgl_fcircle;
static const whitgl_fcircle whitgl_fcircle_zero = { {0,0}, 0};

whitgl_int whitgl_imin(whitgl_int a, whitgl_int b);
whitgl_int whitgl_imax(whitgl_int a, whitgl_int b);
whitgl_int whitgl_iminmax(whitgl_int lower, whitgl_int upper, whitgl_int n);

whitgl_float whitgl_fmin(whitgl_float a, whitgl_float b);
whitgl_float whitgl_fmax(whitgl_float a, whitgl_float b);
whitgl_float whitgl_fminmax(whitgl_float lower, whitgl_float upper, whitgl_float n);

whitgl_ivec whitgl_ivec_add(whitgl_ivec a, whitgl_ivec b);
whitgl_ivec whitgl_ivec_sub(whitgl_ivec a, whitgl_ivec b);
whitgl_ivec whitgl_ivec_inverse(whitgl_ivec a);
whitgl_ivec whitgl_ivec_scale(whitgl_ivec a, whitgl_ivec s);
whitgl_ivec whitgl_ivec_divide(whitgl_ivec a, whitgl_ivec s);
whitgl_int whitgl_ivec_sqmagnitude(whitgl_ivec a);

whitgl_fvec whitgl_fvec_add(whitgl_fvec a, whitgl_fvec b);
whitgl_fvec whitgl_fvec_sub(whitgl_fvec a, whitgl_fvec b);
whitgl_fvec whitgl_fvec_inverse(whitgl_fvec a);
whitgl_fvec whitgl_fvec_scale(whitgl_fvec a, whitgl_fvec s);
whitgl_fvec whitgl_fvec_divide(whitgl_fvec a, whitgl_fvec s);
whitgl_float whitgl_fvec_sqmagnitude(whitgl_ivec a);

whitgl_iaabb whitgl_iaabb_add(whitgl_iaabb a, whitgl_ivec b);
whitgl_iaabb whitgl_iaabb_sub(whitgl_iaabb a, whitgl_ivec b);
whitgl_iaabb whitgl_iaabb_scale(whitgl_iaabb a, whitgl_ivec s);
whitgl_iaabb whitgl_iaabb_divide(whitgl_iaabb a, whitgl_ivec s);
whitgl_iaabb whitgl_iaabb_intersection(whitgl_iaabb a, whitgl_iaabb b);
bool whitgl_iaabb_intersects(whitgl_iaabb a, whitgl_iaabb b);

whitgl_faabb whitgl_faabb_add(whitgl_faabb a, whitgl_fvec b);
whitgl_faabb whitgl_faabb_sub(whitgl_faabb a, whitgl_fvec b);
whitgl_faabb whitgl_faabb_scale(whitgl_faabb a, whitgl_fvec s);
whitgl_faabb whitgl_faabb_divide(whitgl_faabb a, whitgl_fvec s);
whitgl_faabb whitgl_faabb_intersection(whitgl_faabb a, whitgl_faabb b);
bool whitgl_faabb_intersects(whitgl_faabb a, whitgl_faabb b);

bool whitgl_ivec_point_in_rect(whitgl_ivec p, whitgl_iaabb rect);
bool whitgl_fvec_point_in_rect(whitgl_fvec p, whitgl_faabb rect);

whitgl_fvec whitgl_ivec_to_fvec(whitgl_ivec in);
whitgl_ivec whitgl_fvec_to_ivec(whitgl_fvec in);

whitgl_faabb whitgl_iaabb_to_faabb(whitgl_iaabb in);
whitgl_iaabb whitgl_faabb_to_iaabb(whitgl_faabb in);

whitgl_fvec whitgl_angle_to_fvec(whitgl_float in);
whitgl_float whitgl_fvec_to_angle(whitgl_fvec in);

void whitgl_randseed(whitgl_int seed);
whitgl_int whitgl_randint(whitgl_int size);
whitgl_float whitgl_randfloat();

static const whitgl_float whitgl_pi = 3.14159;

#endif // WHITGL_MATH_H_