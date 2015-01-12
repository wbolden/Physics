#ifndef MATH_CUH
#define MATH_CUH

#include "device_launch_parameters.h"
#include "cuda_runtime.h"

inline __device__ uchar4 operator*(const float &b, const uchar4 &a)
{
	return make_uchar4(a.x*b, a.y*b, a.z*b, 0xFF);
}

inline __device__ uchar4 operator*(const uchar4 &a,const float &b)
{
	return make_uchar4(a.x*b, a.y*b, a.z*b, 0xFF);
}

inline __device__ uchar4 operator+(const uchar4 &a,const uchar4 &b)
{
	return make_uchar4(a.x+b.x, a.y+b.y, a.z+b.z, 0xFF);
}

inline __device__ uchar4 operator-(const uchar4 &a,const uchar4 &b)
{
	return make_uchar4(a.x-b.x, a.y-b.y, a.z-b.z, 0xFF);
}

inline __device__ void operator+=(uchar4 a,const uchar4 &b)
{
	a = a+b;
}

inline __device__ void operator-=(uchar4 a,const uchar4 &b)
{
	a = a-b;
}

inline __device__ float3 operator-(const float3 &a, const float3 &b)
{
	return make_float3(a.x-b.x, a.y-b.y, a.z-b.z);
}

inline  __device__ float3 operator+(const float3 &a, const float3 &b)
{
	return make_float3(a.x+b.x, a.y+b.y, a.z+b.z);
}

inline __device__ void operator-=(float3 &a, const float3 &b)
{
	a = a-b;
}

inline __device__ void operator+=(float3 &a, const float3 &b)
{
	a = a+b;
}

inline __device__ float3 operator/(const float3 &a, const float &b)
{
	return make_float3(a.x/b, a.y/b, a.z/b);
}

inline __device__ float3 operator*(const float3 &a, const float &b)
{
	return make_float3(a.x*b, a.y*b, a.z*b);
}

inline __device__ float3 operator*(const float &b, const float3 &a)
{
	return make_float3(a.x*b, a.y*b, a.z*b);
}

inline __device__ void operator/=(float3 &a, const float &b)
{
	a = a/b;
}

inline __device__ void operator*=(float3 &a, const float &b)
{
	a = a*b;
}


//Returns the dot product of vectors a and b
inline __device__ float dot(const float3 &a, const float3 &b)
{
	return a.x*b.x+ a.y*b.y+ a.z*b.z;
}

//Returns the dot product of vectors a and a
inline __device__ float dot(const float3 &a)
{
	return a.x*a.x+ a.y*a.y+ a.z*a.z;
}

//Returns the inverse magnitude of a vector
inline __device__ float rnorm(const float3 &a)
{
	return rsqrtf(dot(a));
}

//Retruns the magnitude of a vector
inline __device__ float norm(const float3 &a)
{
	return sqrtf(dot(a));
}

//Returns a normalized vector
inline __device__ float3 normalize(const float3 &a)
{
	return a * rnorm(a);
}

//Returns the direction of a ray shot from it's origin to a point
inline __device__ float3 direction(float3 origin, float3 point)
{
	return normalize(point - origin);
}

inline __device__ float3 reflect(float3 incidentDirection, float3 normal)
{
	return -2*(dot(incidentDirection, normal)*normal) + incidentDirection;
}

//Creates a hexadecimal RGBA color from 
inline __device__ __host__ uchar4 rgb(int r, int g, int b)
{
	return make_uchar4(r, g, b, 0xFF);
}   

inline __device__ __host__ uchar4 rgb(int c)
{
	return make_uchar4(c, c, c, 0xFF);
}   

//Returns the lambert coefficient of two direction vectors
inline __device__ float lambert(float3 direction1, float3 direction2)
{
	return dot(direction1, direction2)/(norm(direction1) * norm(direction2));
}

inline __device__ float frensel(float f0, float3 normal, float3 incidentDirection)
{
	float nv = 1 - dot(normal, -1*incidentDirection);

	return f0 + (1 - f0)*(nv * nv * nv * nv * nv); //F(0)  + (1 - F(0))(1 - (n . v))^5
}
/*
class Quaternion
{
public:
	float w, x, y, z;
};

*/

#endif