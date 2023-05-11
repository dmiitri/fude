/***************************************************
 * NOTE:
 * (1) Implementation
 * "llamath.h" works like stb headers library. 
 * You need to create an implementation in a c/c++ file.
 * After an implementation made in one file, 
 * you include "llamath.h" as an ordinary header file
 *
 * Do this to create an implementation:
 * ```c
 * #define LLAMA_IMPLEMENTATION
 * #include "llamath.h"
 * ```
 * For example your implementation file will look like this:
 * ```c
 * #include <stdio.h>
 * #include <stdlib.h>
 * #define LLAMA_IMPLEMENTATION
 * #include "llamath.h"
 * ```
 * 
 * (2) Platform
 * If you are working on linux or other posix platform
 * You will need to link with the math library
 * 
 ***************************************************/

#ifndef LLAMA_H
#define LLAMA_H

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define LLAMA_PI 3.14159265358979323846f
#define LLAMA_PI_2 2.0f * LLAMA_PI
#define LLAMA_HALF_PI 0.5f * LLAMA_PI
#define LLAMA_QUARTER_PI 0.25f * LLAMA_PI
#define LLAMA_ONE_OVER_PI 1.0f / LLAMA_PI
#define LLAMA_ONE_OVER_TWO_PI 1.0f / LLAMA_PI_2
#define LLAMA_SQRT_TWO 1.41421356237309504880f
#define LLAMA_SQRT_THREE 1.73205080756887729352f
#define LLAMA_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define LLAMA_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define LLAMA_DEG2RAD_MULTIPLIER LLAMA_PI / 180.0f
#define LLAMA_RAD2DEG_MULTIPLIER 180.0f / LLAMA_PI
#define LLAMA_SEC_TO_US_MULTIPLIER (1000.0f * 1000.0f)
#define LLAMA_SEC_TO_MS_MULTIPLIER 1000.0f
#define LLAMA_MS_TO_SEC_MULTIPLIER 0.001f
#define LLAMA_INFINITY 1e30f
#define LLAMA_FLOAT_EPSILON 1.192092896e-07f

#define LLADEF static inline

typedef union vec2 {
	float elements[2];
	struct {
		union {
			float x, r, s, u;
		};
		union {
			float y, g, t, v;
		};
	};
} vec2_t;

typedef union vec3 {
	float elements[3];
	struct {
		union {
			float x, r, s, u;
		};
		union {
			float y, g, t, v;
		};
		union {
			float z, b, p, w;
		};
	};
} vec3_t;

typedef union vec4 {
	float elements[4];
	struct {
		union {
			float x, r, s;
		};
		union {
			float y, g, t;
		};
		union {
			float z, b, p;
		};
		union {
			float w, a, q;
		};
	};
} vec4_t;

typedef union mat4 {
	vec4_t rows[4];
	float elements[16];
} mat4_t;

/** OLD CODES **/
// LLADEF vec2_t 	vec2(float x, float y);
// LLADEF vec2_t 	vec2_zeros();
// LLADEF vec2_t 	vec2_ones();
// LLADEF vec2_t 	vec2_add(vec2_t a, vec2_t b);
// LLADEF vec2_t 	vec2_sub(vec2_t a, vec2_t b);
// LLADEF vec2_t 	vec2_mul(vec2_t a, vec2_t b);
// LLADEF vec2_t 	vec2_div(vec2_t a, vec2_t b);
// LLADEF float  	vec2_length(vec2_t a);
// LLADEF void	vec2_normalize(vec2_t* a);
// LLADEF bool 	vec2_compare(vec2_t a, vec2_t b);
// LLADEF float 	vec2_distance(vec2_t a, vec2_t b);

// LLADEF vec3_t 	vec3(float x, float y, float z);
// LLADEF vec3_t 	vec3_zeros();
// LLADEF vec3_t 	vec3_ones();
// LLADEF vec3_t 	vec3_add(vec3_t a, vec3_t b);
// LLADEF vec3_t 	vec3_sub(vec3_t a, vec3_t b);
// LLADEF vec3_t 	vec3_mul(vec3_t a, vec3_t b);
// LLADEF vec3_t 	vec3_div(vec3_t a, vec3_t b);
// LLADEF float	vec3_dot(vec3_t a, vec3_t b);
// LLADEF float  	vec3_length(vec3_t a);
// LLADEF void	vec3_normalize(vec3_t* a);
// LLADEF vec3_t 	vec3_normalized(vec3_t a);
// LLADEF bool 	vec3_compare(vec3_t a, vec3_t b);
// LLADEF float 	vec3_distance(vec3_t a, vec3_t b);
// LLADEF vec3_t 	vec3_cross(vec3_t a, vec3_t b);

// LLADEF vec4_t 	vec4(float x, float y, float z, float w);
// LLADEF vec4_t 	vec4_zeros();
// LLADEF vec4_t 	vec4_ones();
// LLADEF vec4_t 	vec4_add(vec4_t a, vec4_t b);
// LLADEF vec4_t 	vec4_sub(vec4_t a, vec4_t b);
// LLADEF vec4_t 	vec4_mul(vec4_t a, vec4_t b);
// LLADEF vec4_t 	vec4_div(vec4_t a, vec4_t b);
// LLADEF float	vec4_dot(vec4_t a, vec4_t b);
// LLADEF float  	vec4_length(vec4_t a);
// LLADEF void	vec4_normalize(vec4_t* a);

// LLADEF mat4_t mat4(float a);
// LLADEF mat4_t mat4_identity();
// LLADEF mat4_t mat4_add(mat4_t a, mat4_t b);
// LLADEF mat4_t mat4_sub(mat4_t a, mat4_t b);
// LLADEF mat4_t mat4_mul(mat4_t a, mat4_t b);
// LLADEF mat4_t mat4_div(mat4_t a, mat4_t b);
// LLADEF mat4_t mat4_dot(mat4_t a, mat4_t b);
// LLADEF mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far);
// LLADEF mat4_t mat4_perspective(float fov_radians, float aspect_ratio, float near, float far);
// LLADEF mat4_t Mat4_look_at(vec3_t pos, vec3_t target, vec3_t up);
// LLADEF mat4_t mat4_inverse(mat4_t matrix);
// LLADEF mat4_t mat4_translation(vec3_t position);
// LLADEF mat4_t mat4_scale(vec3_t scale);
// LLADEF mat4_t mat4_transpose(mat4_t matrix);
// LLADEF mat4_t mat4_euler_x(float angle_radians);
// LLADEF mat4_t mat4_euler_y(float angle_radians);
// LLADEF mat4_t mat4_euler_z(float angle_radians);
// LLADEF mat4_t mat4_euler_xyz(float x_radians, float y_radians, float z_radians);

// vec2
LLADEF vec2_t vec2(float x, float y) {
	vec2_t res = {0};
	res.x = x;
	res.y = y;
	return res;
}

LLADEF vec2_t vec2_zeros() {
	return vec2(0.0f, 0.0f);
}

LLADEF vec2_t vec2_ones() {
	return vec2(1.0f, 1.0f);
}

LLADEF vec2_t vec2_add(vec2_t a, vec2_t b) {
	return vec2(
		a.x + b.x,
		a.y + b.y
	);
}

LLADEF vec2_t vec2_sub(vec2_t a, vec2_t b) {
	return vec2(
		a.x - b.x,
		a.y - b.y
	);
}
LLADEF vec2_t vec2_mul(vec2_t a, vec2_t b) {
	return vec2(
		a.x * b.x,
		a.y * b.y
	);
}

LLADEF vec2_t vec2_div(vec2_t a, vec2_t b) {
	return vec2(
		a.x / b.x,
		a.y / b.y
	);
}

LLADEF float vec2_length(vec2_t a) {
	return sqrtf( a.x * a.x + a.y * a.y );
}

LLADEF void vec2_normalize(vec2_t* a) {
	const float length = vec2_length(*a);
	a->x /= length;
	a->y /= length;
}

LLADEF bool vec2_compare(vec2_t a, vec2_t b) {
	if(fabsf(a.x - b.x) > LLAMA_FLOAT_EPSILON) return false;
	if(fabsf(a.y - b.y) > LLAMA_FLOAT_EPSILON) return false;
	return true;
}

LLADEF float vec2_distance(vec2_t a, vec2_t b) {
	vec2_t d = vec2(a.x - b.x, a.y - b.y);
	return vec2_length(d);
}

// vec3
LLADEF vec3_t vec3(float x, float y, float z) {
	vec3_t res = {0};
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

LLADEF vec3_t vec3_zeros() {
	return vec3(0.0f, 0.0f, 0.0f);
}

LLADEF vec3_t vec3_ones() {
	return vec3(1.0f, 1.0f, 1.0f);
}

LLADEF vec3_t vec3_add(vec3_t a, vec3_t b) {
	return vec3(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	);
}

LLADEF vec3_t vec3_sub(vec3_t a, vec3_t b) {
	return vec3(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	);
}

LLADEF vec3_t vec3_mul(vec3_t a, vec3_t b) {
	return vec3(
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	);
}

LLADEF vec3_t vec3_div(vec3_t a, vec3_t b) {
	return vec3(
		a.x / b.x,
		a.y / b.y,
		a.z / b.z
	);
}

LLADEF float vec3_dot(vec3_t a, vec3_t b) {
	float p = 0;
	p += a.x * b.x;
	p += a.y * b.y;
	p += a.z * b.z;
	return p;
}

LLADEF float vec3_length(vec3_t a) {
	return sqrtf( a.x * a.x + a.y * a.y + a.z * a.z );
}

LLADEF void vec3_normalize(vec3_t* a) {
	const float length = vec3_length(*a);
    a->x /= length;
    a->y /= length;
    a->z /= length;
}

LLADEF vec3_t vec3_normalized(vec3_t a) {
	vec3_normalize(&a);
	return a;
}

LLADEF bool vec3_compare(vec3_t a, vec3_t b) {
    if(fabsf(a.x - b.x) > LLAMA_FLOAT_EPSILON) return false;
    if(fabsf(a.y - b.y) > LLAMA_FLOAT_EPSILON) return false;
    if(fabsf(a.z - b.z) > LLAMA_FLOAT_EPSILON) return false;
    return true;
}

LLADEF float vec3_distance(vec3_t a, vec3_t b) {
	vec3_t d = vec3(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	);
	return vec3_length(d);
}

LLADEF vec3_t vec3_cross(vec3_t a, vec3_t b) {
	return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// vec4
LLADEF vec4_t vec4(float x, float y, float z, float w) {
	vec4_t res = {0};
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
	return res;
}

LLADEF vec4_t vec4_zeros() {
	return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

LLADEF vec4_t vec4_ones() {
	return vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

LLADEF vec4_t vec4_add(vec4_t a, vec4_t b) {
	return vec4(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	);
}

LLADEF vec4_t vec4_sub(vec4_t a, vec4_t b) {
	return vec4(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	);
}

LLADEF vec4_t vec4_mul(vec4_t a, vec4_t b) {
	return vec4(
		a.x * b.x,
		a.y * b.y,
		a.z * b.z,
		a.w * b.w
	);
}

LLADEF vec4_t vec4_div(vec4_t a, vec4_t b) {
	return vec4(
		a.x / b.x,
		a.y / b.y,
		a.z / b.z,
		a.w / b.w
	);
}

LLADEF float vec4_dot(vec4_t a, vec4_t b) {
    float p = 0;
    p += a.x * b.x;
    p += a.y * b.y;
    p += a.z * b.z;
    p += a.w * b.w;
    return p;
}

LLADEF float vec4_length(vec4_t a) {
	return sqrtf( a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w );	
}

LLADEF void vec4_normalize(vec4_t* a) {
	const float length = vec4_length(*a);
    a->x /= length;
    a->y /= length;
    a->z /= length;
	a->w /= length;
}

// mat4
LLADEF mat4_t mat4(float a) {
	mat4_t res = {0};
	for(int i = 0; i < 16; ++i) {
		res.elements[i] = a;
	}
	return res;
}

LLADEF mat4_t mat4_identity() {
	mat4_t res = {0};
	res.elements[0] = 1.0f;
	res.elements[5] = 1.0f;
	res.elements[10] = 1.0f;
	res.elements[15] = 1.0f;
	return res;
}

LLADEF mat4_t mat4_add(mat4_t a, mat4_t b) {
	mat4_t res = {0};
	for(int i = 0; i < 16; i++) {
		res.elements[i] = a.elements[i] + b.elements[i];
	}
	return res;
}

LLADEF mat4_t mat4_sub(mat4_t a, mat4_t b) {
	mat4_t res = {0};
	for(int i = 0; i < 16; i++) {
		res.elements[i] = a.elements[i] - b.elements[i];
	}
	return res;
}

LLADEF mat4_t mat4_mul(mat4_t a, mat4_t b) {
	mat4_t res = {0};
	for(int i = 0; i < 16; i++) {
		res.elements[i] = a.elements[i] * b.elements[i];
	}
	return res;
}

LLADEF mat4_t mat4_div(mat4_t a, mat4_t b) {
	mat4_t res = {0};
	for(int i = 0; i < 16; i++) {
		res.elements[i] = a.elements[i] / b.elements[i];
	}
	return res;
}

LLADEF mat4_t mat4_dot(mat4_t a, mat4_t b) {
	mat4_t res = mat4_identity();
    const float* a_ptr = a.elements;
    const float* b_ptr = b.elements;
    float* dst_ptr = res.elements;

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            *dst_ptr = 
                a_ptr[0] * b_ptr[0 + j] +
                a_ptr[1] * b_ptr[4 + j] +
                a_ptr[2] * b_ptr[8 + j] +
                a_ptr[3] * b_ptr[12 + j];
            dst_ptr++;
        }
        b_ptr += 4;
    }
    return res;
}

LLADEF mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
	mat4_t res = mat4_identity();
    float lr = 1.0f / (left - right);
    float bt = 1.0f / (bottom - top);
    float nf = 1.0f / (near - far);

    res.elements[0] = -2.0f * lr;
    res.elements[5] = -2.0f * bt;
    res.elements[10] = 2.0f * nf;

    res.elements[12] = (left + right) * lr;
    res.elements[13] = (top + bottom) * bt;
    res.elements[14] = (near + far) * nf;	

    return res;
}

LLADEF mat4_t mat4_perspective(float fov_radians, float aspect_ratio, float near, float far) {
	float half_tan_fov = tan(fov_radians * 0.5f);
	mat4_t res = mat4(0);
	res.elements[0] = 1.0f / (aspect_ratio * half_tan_fov);
	res.elements[5] = 1.0f / half_tan_fov;
	res.elements[10] = -((far + near) / (far - near));
	res.elements[11] = -1.0f;
	res.elements[14] = -((2.0f * far * near) / (far - near));	
	return res;
}

LLADEF mat4_t Mat4_look_at(vec3_t pos, vec3_t target, vec3_t up) {
	mat4_t result;
	vec3_t z_axis;
	z_axis.x = target.x - pos.x;
	z_axis.y = target.y - pos.y;
	z_axis.z = target.z - pos.z;

	z_axis = vec3_normalized(z_axis);
	vec3_t x_axis = vec3_normalized(vec3_cross(z_axis, up));
	vec3_t y_axis = vec3_cross(x_axis, z_axis);

	result.elements[0] = x_axis.x;
	result.elements[1] = y_axis.x;
	result.elements[2] = -z_axis.x;
	result.elements[3] = 0;
	result.elements[4] = x_axis.y;
	result.elements[5] = y_axis.y;
	result.elements[6] = -z_axis.y;
	result.elements[7] = 0;
	result.elements[8] = x_axis.z;
	result.elements[9] = y_axis.z;
	result.elements[10] = -z_axis.z;
	result.elements[11] = 0;
	result.elements[12] = -vec3_dot(x_axis, pos);
	result.elements[13] = -vec3_dot(y_axis, pos);
	result.elements[14] = vec3_dot(z_axis, pos);
	result.elements[15] = 1.0f;

	return result;	
}

LLADEF mat4_t mat4_inverse(mat4_t matrix) {
	const float* m = matrix.elements;

    float t0 = m[10] * m[15];
    float t1 = m[14] * m[11];
    float t2 = m[6] * m[15];
    float t3 = m[14] * m[7];
    float t4 = m[6] * m[11];
    float t5 = m[10] * m[7];
    float t6 = m[2] * m[15];
    float t7 = m[14] * m[3];
    float t8 = m[2] * m[11];
    float t9 = m[10] * m[3];
    float t10 = m[2] * m[7];
    float t11 = m[6] * m[3];
    float t12 = m[8] * m[13];
    float t13 = m[12] * m[9];
    float t14 = m[4] * m[13];
    float t15 = m[12] * m[5];
    float t16 = m[4] * m[9];
    float t17 = m[8] * m[5];
    float t18 = m[0] * m[13];
    float t19 = m[12] * m[1];
    float t20 = m[0] * m[9];
    float t21 = m[8] * m[1];
    float t22 = m[0] * m[5];
    float t23 = m[4] * m[1];

    mat4_t result;
    float* o = result.elements;

    o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
    o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
    o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
    o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

    float d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

    o[0] = d * o[0];
    o[1] = d * o[1];
    o[2] = d * o[2];
    o[3] = d * o[3];
    o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
    o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
    o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
    o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
    o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
    o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
    o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
    o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
    o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
    o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
    o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
    o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

    return result;
}

LLADEF mat4_t mat4_translation(vec3_t position) {
	mat4_t result = mat4_identity();
    result.elements[12] = position.x;
    result.elements[13] = position.y;
    result.elements[14] = position.z;
    return result;
}

LLADEF mat4_t mat4_scale(vec3_t scale) {
	mat4_t result = mat4_identity();
    result.elements[0] = scale.x;
    result.elements[5] = scale.y;
    result.elements[10] = scale.z;
    return result;
}

LLADEF mat4_t mat4_transpose(mat4_t matrix) {
	mat4_t out_matrix = mat4_identity();
    out_matrix.elements[0] = matrix.elements[0];
    out_matrix.elements[1] = matrix.elements[4];
    out_matrix.elements[2] = matrix.elements[8];
    out_matrix.elements[3] = matrix.elements[12];
    out_matrix.elements[4] = matrix.elements[1];
    out_matrix.elements[5] = matrix.elements[5];
    out_matrix.elements[6] = matrix.elements[9];
    out_matrix.elements[7] = matrix.elements[13];
    out_matrix.elements[8] = matrix.elements[2];
    out_matrix.elements[9] = matrix.elements[6];
    out_matrix.elements[10] = matrix.elements[10];
    out_matrix.elements[11] = matrix.elements[14];
    out_matrix.elements[12] = matrix.elements[3];
    out_matrix.elements[13] = matrix.elements[7];
    out_matrix.elements[14] = matrix.elements[11];
    out_matrix.elements[15] = matrix.elements[15];
    return out_matrix;
}

LLADEF mat4_t mat4_euler_x(float angle_radians) {
    mat4_t out_matrix = mat4_identity();
    float c = cos(angle_radians);
    float s = sin(angle_radians);
    out_matrix.elements[5] = c;
    out_matrix.elements[6] = s;
    out_matrix.elements[9] = -s;
    out_matrix.elements[10] = c;
    return out_matrix;
}

LLADEF mat4_t mat4_euler_y(float angle_radians) {
    mat4_t out_matrix = mat4_identity();
    float c = cos(angle_radians);
    float s = sin(angle_radians);

    out_matrix.elements[0] = c;
    out_matrix.elements[2] = -s;
    out_matrix.elements[8] = s;
    out_matrix.elements[10] = c;
    return out_matrix;
}

LLADEF mat4_t mat4_euler_z(float angle_radians) {
    mat4_t out_matrix = mat4_identity();

    float c = cos(angle_radians);
    float s = sin(angle_radians);

    out_matrix.elements[0] = c;
    out_matrix.elements[1] = s;
    out_matrix.elements[4] = -s;
    out_matrix.elements[5] = c;
    return out_matrix;
}

LLADEF mat4_t mat4_euler_xyz(float x_radians, float y_radians, float z_radians) {
    mat4_t rx = mat4_euler_x(x_radians);
    mat4_t ry = mat4_euler_y(y_radians);
    mat4_t rz = mat4_euler_z(z_radians);
    mat4_t out_matrix = mat4_dot(rx, ry);
    out_matrix = mat4_dot(out_matrix, rz);
    return out_matrix;

}

#endif // LLAMA_H