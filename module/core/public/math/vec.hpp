#pragma once
#include <cmath>

template <typename T>
struct Vec2
{
    Vec2() = default;
    constexpr Vec2(T inX, T inY) : x(inX),y(inY) {};

    T x {};
    T y {};

    template<typename TOther>
    explicit Vec2(const TOther& other) : x(other.x),y(other.y) {};

    inline Vec2 operator+(const Vec2<T> inOther)
    {
        return Vec2<T>{x + inOther.x,y + inOther.y};
    }

    inline Vec2 operator+=(const Vec2<T> inOther)
    {
        *this = operator+(inOther);
        return *this;
    }

    inline Vec2 operator+(const T inOther)
    {
        return Vec2<T>{x + inOther,y + inOther};
    }

    inline Vec2 operator+=(const T inOther)
    {
        *this = operator+(inOther);
        return *this;
    }

    inline Vec2 operator-(const Vec2<T> inOther)
    {
        return Vec2<T>{x - inOther.x,y - inOther.y};
    }

    inline Vec2 operator-=(const Vec2<T> inOther)
    {
        *this = operator-(inOther);
        return *this;
    }

    inline Vec2 operator-(const T inOther)
    {
        return Vec2<T>{x - inOther,y - inOther};
    }

    inline Vec2 operator-=(const T inOther)
    {
        *this = operator-(inOther);
        return *this;
    }

    inline Vec2 operator*(const T inOther)
    {
        return Vec2<T>{x * inOther,y * inOther};
    }

    inline Vec2 operator*(const Vec2<T> inOther)
    {
        return Vec2<T>{x * inOther.x,y * inOther.y};
    }

    inline Vec2 operator*=(const T inOther)
    {
        *this = operator*(inOther);
        return *this;
    }

    inline Vec2 operator/(const T inOther)
    {
        return Vec2<T>{x / inOther,y / inOther};
    }

    inline Vec2 operator/=(const T inOther)
    {
        *this = operator/(inOther);
        return *this;
    }

    inline Vec2<T> clamp(Vec2<T> min, Vec2<T> max)
    {
        return {
            x < min.x ? min.x : (x > max.x ? max.x : x),
            y < min.y ? min.y : (y > max.y ? max.y : y),
        };
    }

    inline T length()
    {
        return std::sqrt(x*x+y*y);
    }

    inline Vec2<T> normalize()
    {
        float l = length();
        return *this/l;
    }

};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

template <typename T>
struct Vec3
{
    Vec3() = default;
    Vec3(T inX, T inY,T inZ) : x(inX),y(inY),z(inZ) {};

    T x {};
    T y {};
    T z {};

    inline Vec3 operator+(const Vec3<T> inOther) const
    {
        return Vec3<T>{x + inOther.x,y + inOther.y,z+inOther.z};
    }

    inline Vec3 operator+=(const Vec3<T> inOther)
    {
        *this = operator+(inOther);
        return *this;
    }

    inline Vec3 operator+(const T inOther) const
    {
        return Vec3<T>{x + inOther,y + inOther,z + inOther};
    }

    inline Vec3 operator+=(const T inOther)
    {
        *this = operator+(inOther);
        return *this;
    }

    inline Vec3 operator-(const Vec3<T> inOther) const
    {
        return Vec3<T>{x - inOther.x,y - inOther.y,z-inOther.z};
    }

    inline Vec3 operator-=(const Vec3<T> inOther)
    {
        *this = operator-(inOther);
        return *this;
    }

    inline Vec3 operator-(const T inOther) const
    {
        return Vec3<T>{x - inOther,y - inOther,z - inOther};
    }

    inline Vec3 operator-=(const T inOther)
    {
        *this = operator-(inOther);
        return *this;
    }

    inline Vec3 operator*(const T inOther) const
    {
        return Vec3<T>{x * inOther,y * inOther,z * inOther};
    }

    inline Vec3 operator*=(const T inOther)
    {
        *this = operator*(inOther);
        return *this;
    }

    inline Vec3 operator/(const T inOther) const
    {
        return Vec3<T>{x / inOther,y / inOther,z / inOther};
    }


    inline T length()
    {
        return std::sqrt(x*x+y*y+z*z);
    }

    inline Vec3<T> cross(const Vec3<T> inOther)
    {
        return {
            y*inOther.z - z*inOther.y,
            z*inOther.x - x*inOther.z,
            x*inOther.y - y*inOther.x
        };
    }

    inline T dot(const Vec3<T> inOther)
    {
        return (x*inOther.x)+(y*inOther.y)+(z*inOther.z);
    }

    inline Vec3<T> normalize()
    {
        float l = length();
        return *this/l;
    }

};

using Vec3i = Vec3<int>;
using Vec3f = Vec3<float>;

class Vec4f
{
public:
    float x,y,z {0};
    float w {1};

    Vec4f() = default;
    Vec4f(Vec3f in) : x(in.x),y(in.y),z(in.z),w(1) {}

    operator Vec3f() const
    {
        return {x,y,z};
    }

    operator Vec2f() const
    {
        return {x,y};
    }

    operator Vec2i() const
    {
        return {int(x),int(y)};
    }
};
