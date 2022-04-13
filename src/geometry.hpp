#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <functional>
/*
struct Point2D
{
    float values[2] {};

    Point2D() {}
    Point2D(float x, float y) : values { x, y } {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    Point2D& operator+=(const Point2D& other)
    {
        x() += other.x();
        y() += other.y();
        return *this;
    }

    Point2D& operator*=(const Point2D& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    Point2D& operator*=(const float scalar)
    {
        x() *= scalar;
        y() *= scalar;
        return *this;
    }

    Point2D operator+(const Point2D& other) const
    {
        Point2D result = *this;
        result += other;
        return result;
    }

    Point2D operator*(const Point2D& other) const
    {
        Point2D result = *this;
        result *= other;
        return result;
    }

    Point2D operator*(const float scalar) const
    {
        Point2D result = *this;
        result *= scalar;
        return result;
    }
};

struct Point3D
{
    
    std::array<float,3> values;
    Point3D() {}
    Point3D(float x, float y, float z) : values { x, y, z } {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() {return values[1]; }
    float y() const { return values[1]; }

    float& z() { return values[2]; }
    float z() const { return values[2]; }

    Point3D& operator+=(const Point3D& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus<float>());

        return *this;
    }

    Point3D& operator-=(const Point3D& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::minus<float>());
        return *this;
    }

    Point3D& operator*=(const float scalar)
    {
      
        std::transform(values.begin(), values.end(), values.begin(),[scalar](float f) {return f * scalar;});
        return *this;
    }

    Point3D operator+(const Point3D& other) const
    {
        Point3D result = *this;
        result += other;
        return result;
    }

    Point3D operator-(const Point3D& other) const
    {
        Point3D result = *this;
        result -= other;
        return result;
    }

    Point3D operator*(const float scalar) const
    {
        Point3D result = *this;
        result *= scalar;
        return result;
    }

    Point3D operator-() const { return Point3D { -x(), -y(), -z() }; }

    float length() const { return std::sqrt(std::accumulate(values.begin(), values.end(), 0.0, [](float f1, float f2){ return f1 + f2*f2; })); }
    float distance_to(const Point3D& other) const { return (*this - other).length(); }

    Point3D& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point3D& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
};
*/
// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}


 template <size_t dimension, typename type>
 
 struct Point
 {    
    Point() = default;
    std::array<type, dimension> values;
    
    template<typename... Args>/*variadic-template*/
    Point(type t, Args&&... args) : values { t, static_cast<type>( std::forward<Args>(args))... } {/*This function is used to implement "perfect forwarding".*/
        static_assert(sizeof...(args)+1 == dimension && "You must enter the exact number of coordinates.");
    }

    Point(type x, type y) : values { x, y } {}
    Point(type x, type y, type z) : values { x, y, z } {static_assert(dimension > 2, "Dimension size is too small!");}

    type& x() { return values[0]; }
    type x() const { return values[0]; }

    type& y() { static_assert(dimension > 1, "Dimension size is too small!"); return values[1]; }
    type y() const { static_assert(dimension > 1, "Dimension size is too small!"); return values[1]; }

    type& z() { static_assert(dimension > 2, "Dimension size is too small!"); return values[2]; }
    type z() const { static_assert(dimension > 2, "Dimension size is too small!"); return values[2]; }

    Point& operator+=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), [](type value1, type value2) { return value1+value2; });
        return *this;
    }

    Point& operator-=(const Point& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), [](type value1, type value2) { return value1-value2; });
        return *this;
    }

    Point& operator*=(const type scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(), [scalar](type value) { return value*scalar; });
        return *this;
    }

    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const type scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator*(const Point& other) const
    {
        Point result = *this;
        result *= other;
        return result;
    }

    Point& operator*=(const Point& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    Point operator-() const { return Point { -x(), -y(), -z() }; }
   
    type length() const 
    { 

        return std::sqrt( std::accumulate(values.begin(), values.end(), 0.0, [](type v1, type v2) {return v1+v2*v2; }) ) ;
    }

    type distance_to(const Point& other) const { return (*this - other).length(); }

    Point& normalize(const type target_len = 1.0f)
    {
        const type current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point& cap_length(const type max_len)
    {
        assert(max_len > 0);

        const type current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }
 };
   
using Point2D = Point<2, float>;
using Point3D = Point<3, float>;

inline void test_generic_points()
{
    auto p1 = Point3D(2.0f, 5,6);
    auto p2 = Point3D(2.0f, 15.0f,3.0f);
    auto p3 = p1 + p2;
    p1 += p2;
    p1 *= 3;
    p3 -= p1;
}



inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}