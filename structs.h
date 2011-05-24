#ifndef STRUCTS_H
#define STRUCTS_H

#include <cstdlib>
#include <cmath>

void int_switch(int* x, int* y);
double frac(double x);
double invfrac(double x);

struct t_color {
    int R, G, B, A;
};

struct t_matrix4 {
    double m[16];

    double operator[] (size_t index)
    {
        return m[index];
    }

    t_matrix4 operator*(t_matrix4& b) const
    {
        t_matrix4 dst;
        for(int i = 0; i < 16; i++) dst.m[i] = 0;

        int col;
        for(int y=0; y<4; ++y){
            col = y*4;
            for(int x=0; x<4; ++x){
                for(int i=0; i<4; ++i){
                    dst.m[x+col] += m[i+col]*b[x+i*4];
                }
            }
        }
        return dst;
    }

    t_matrix4 translate(double x, double y, double z)
    {
        t_matrix4 b = {1, 0, 0, x,
                       0, 1, 0, y,
                       0, 0, 1, z,
                       0, 0, 0, 1};

        return *this*b;
    }
};


struct t_point {
    double x;
    double y;
    double z;
    double w;

    t_point operator*(t_matrix4& m) const
    {
        t_point dst;
        dst.x = x*m[0 ] + y*m[1 ] + z*m[2 ] + w*m[3 ];
        dst.y = x*m[4 ] + y*m[5 ] + z*m[6 ] + w*m[7 ];
        dst.z = x*m[8 ] + y*m[9 ] + z*m[10] + w*m[11];
        dst.w = x*m[12] + y*m[13] + z*m[14] + w*m[15];
        return dst;
    }

    t_point operator*(t_point& b) const
    {
        t_point out;
        out.x = y*b.z-z*b.y;
        out.y = z*b.y-x*b.z;
        out.z = x*b.y-y*b.x;

        return out;
    }

    t_point& normalize() {
        double invLength = 1 / sqrtf(x*x + y*y + z*z);
        x *= invLength;
        y *= invLength;
        z *= invLength;
        return *this;
    }

    t_point cross(t_point a, t_point b)
    {
        t_point out;
        out.x = a.y*b.z-a.z*b.y;
        out.y = a.z*b.y-a.x*b.z;
        out.z = a.x*b.y-a.y*b.x;

        return out;
    }

    void rotateX(double angle)
    {
        y = y*cos(angle) + z*sin(angle);
        z = y*-sin(angle) + z*cos(angle);
    }

    void rotateY(double angle)
    {
        x = x*cos(angle) + z*-sin(angle);
        z = x*sin(angle) + z*cos(angle);
    }

    void rotateZ(double angle)
    {
        x = x*cos(angle) + y*-sin(angle);
        y = x*sin(angle) + y*cos(angle);
    }

    void translateX(double val)
    {
        x+=val;
    }
};

struct t_line
{
    t_point point1;
    t_point point2;
    t_color color;
};

#endif // STRUCTS_H
