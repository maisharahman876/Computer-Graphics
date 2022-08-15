#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
using namespace std;
struct point
{
    double x,y,z;
};
class Object
{
    public:
struct point reference_point;
double height, width, length;
double color[3];
double coEfficients[4] ;
int shine ;

virtual void draw();
void setColor(double color[3]){
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
}
void setCoEfficients(double coEfficients[4]){
    this->coEfficients[0] = coEfficients[0];
    this->coEfficients[1] = coEfficients[1];
    this->coEfficients[2] = coEfficients[2];
    this->coEfficients[3] = coEfficients[3];
}
void setShine(int shine){
    this->shine = shine;
}
};
//class Sphere inherits from Object
class Sphere : public Object
{
public:

    Sphere(struct point center, double radius, double color[3], double coEfficients[4], int shine)
    {
        this->reference_point = center;
        this->length = radius;
        this->setColor(color);
        this->setCoEfficients(coEfficients);
        this->setShine(shine);
    }
    void draw()
    {
       //draw sphere
    }

};
//class triangle inherits from Object
class Triangle : public Object
{
public:
struct point p1, p2, p3;
    Triangle(struct point p1, struct point p2, struct point p3, double color[3], double coEfficients[4], int shine)
    {
        this->p1 = p1;
        this->p2 = p2;
        this->p3 = p3;
        this->setColor(color);
        this->setCoEfficients(coEfficients);
        this->setShine(shine);
    }
    void draw()
    {
        //draw triangle
    }
};
//class with general quadratic equation inherits from Object
class Quadratic : public Object
{
public:
double parameters[10];
    Quadratic(double params[10],struct point reference_point, double height, double width, double length, double color[3], double coEfficients[4], int shine)
    {
        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        this->setColor(color);
        this->setCoEfficients(coEfficients);
        this->setShine(shine);
        for(int i = 0; i < 10; i++)
        {
            this->parameters[i] = params[i];
        }
    }
    void draw()
    {
        //draw quadratic
    }
};

