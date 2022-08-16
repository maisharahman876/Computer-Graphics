#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
#define pi 22/7
using namespace std;
struct point
{
    double x,y,z;
};
//Ray class
class Ray
{
public:
    struct point origin;
    struct point direction;
    Ray(struct point origin, struct point direction)
    {
        this->origin = origin;
        this->direction = direction;
    }

};
void drawSphere(double radius,int slices,int stacks,double *color)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f(color[0],color[1],color[2]);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

class Object
{
public:
    struct point reference_point;
    double height, width, length;
    double *color;
    double *coEfficients ;
    int shine ;

    virtual void draw() {}
    virtual double intersect(Ray r, double *color,int l) { return 0.0;}
    void setColor(double *color)
    {
        this->color = color;
    }
    void setCoEfficients(double *coEfficients)
    {
        this->coEfficients = coEfficients;
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
};
//class Sphere inherits from Object
class Sphere : public Object
{
public:

    Sphere(struct point center, double radius, double *color, double *coEfficients, int shine)
    {
        this->reference_point = center;
        this->length = radius;
        this->setColor(color);
        this->setCoEfficients(coEfficients);
        this->setShine(shine);
    }
    void draw()
    {
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        drawSphere(this->length,60,60,color);
        glPopMatrix();
    }
    double intersect(Ray r, double *color,int l)
    {
        // double a,b,c,d,t1,t2;
        // a = 1;
        // b = 2*(r.origin.x*(r.direction.x) + r.origin.y*(r.direction.y) + r.origin.z*(r.direction.z));
        // c = r.origin.x*r.origin.x + r.origin.y*r.origin.y + r.origin.z*r.origin.z - this->length*this->length;
        // d = b*b - 4*a*c;
        // if(d<0)
        // {
        //     return -1;
        // }
        // else
        // {
        //     t1 = (-b + sqrt(d))/(2*a);
        //     t2 = (-b - sqrt(d))/(2*a);
        //     if(t1<t2)
        //     {
        //         return t1;
        //     }
        //     else
        //     {
        //         return t2;
        //     }
        // }
         return 0.0;
    }


};
//class triangle inherits from Object
class Triangle : public Object
{
public:
    struct point p1, p2, p3;
    Triangle(struct point p1, struct point p2, struct point p3, double *color, double *coEfficients, int shine)
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
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        {
            glColor3f(this->color[0], this->color[1], this->color[2]);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        }
        glEnd();
        glPopMatrix();
    }
     double intersect(Ray r, double *color,int l)
    {
        // double a,b,c,d,t1,t2;
        // a = (p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y);
        // b = (p2.z - p1.z)*(r.direction.x - r.origin.x) - (p2.x - p1.x)*(p3.z - p1.z);
        // c = (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(r.direction.x - r.origin.x);
        // d = -(a*r.origin.x + b*r.origin.y + c*r.origin.z);
        // t1 = -(a*r.origin.x + b*r.origin.y + c*r.origin.z + d)/(a*r.direction.x + b*r.direction.y + c*r.direction.z);
        // t2 = -(a*r.origin.x + b*r.origin.y + c*r.origin.z + d + a*r.direction.x + b*r.direction.y + c*r.direction.z)/(a*r.direction.x + b*r.direction.y + c*r.direction.z);
        // if(t1<0)
        // {
        //     if(t2<0)
        //     {
        //         return -1;
        //     }
        //     else
        //     {
        //         return t2;
        //     }
        // }
        // else
        // {
        //     if(t2<0)
        //     {
        //         return t1;
        //     }
        //     else
        //     {
        //         if(t1<t2)
        //         {
        //             return t1;
        //         }
        //         else
        //         {
        //             return t2;
        //         }
        //     }
        // }
         return 0.0;
    }
};
//class with general quadratic equation inherits from Object
class Quadratic : public Object
{
public:
    double *parameters;
    Quadratic(double *params,struct point reference_point, double length, double width, double height, double *color, double *coEfficients, int shine)
    {
        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        this->setColor(color);
        this->setCoEfficients(coEfficients);
        this->setShine(shine);
        this->parameters = params;

    }
    void draw()
    {
        //draw quadratic
    }
     double intersect(Ray r, double *color,int l)
    {
         return 0.0;
    }
};
// point light class
class PointLight
{
public:
    struct point position;
    double *color;
    PointLight(struct point p, double *color)
    {
        this->color=color;
        this->position = p;
    }
    void draw()
    {

    }
      double intersect(Ray r, double *color,int l)
    {
        // double a,b,c,d,t1,t2;
        // a = 1;
        // b = 2*(r.origin.x*(r.direction.x) + r.origin.y*(r.direction.y) + r.origin.z*(r.direction.z));
        // c = r.origin.x*r.origin.x + r.origin.y*r.origin.y + r.origin.z*r.origin.z - this->length*this->length;
        // d = b*b - 4*a*c;
        // if(d<0)
        // {
        //     return -1;
        // }
        // else
        // {
        //     t1 = (-b + sqrt(d))/(2*a);
        //     t2 = (-b - sqrt(d))/(2*a);
        //     if(t1<t2)
        //     {
        //         return t1;
        //     }
        //     else
        //     {
        //         return t2;
        //     }
        // }
         return 0.0;
    }
};
//spot light class
class SpotLight
{
public:
    struct point position;
    double *color;
    struct point direction;
    double angle;
    SpotLight(struct point p, double *color, struct point dir, double angle)
    {

        this->color=color;
        this->position = p;
        this->angle = angle;
        this->direction = dir;
    }
    void draw()
    {

    }
     double intersect(Ray r, double *color,int l)
    {
         return 0.0;
    }
};
//floor class inherits from Object class like chess board black and white squares
class Floor : public Object
{
public:
    double tile_size;
    Floor(double floor_width,double tile_width)
    {
        this->width = floor_width/2;
        this->length = floor_width/2;
        // this->setCoEfficients(coEfficients);
        // this->setShine(shine);
        this->tile_size = tile_width;
    }
    void draw()
    {
        int tiles=int(this->width/(this->tile_size));
        //cout<<tiles<<endl;
        double col=1.0;
        for(int i = 0; i < 2*tiles; i++)
        {
            for(int j = 0; j < 2*tiles; j++)
            {

                glPushMatrix();

                // draw rectangle
                glColor3f((i+j)%2,(i+j)%2,(i+j)%2);

                glBegin(GL_QUADS);
                {

                    glVertex3f(-this->width+i*this->tile_size,-this->width+j*this->tile_size,0);
                    glVertex3f(-this->width+i*this->tile_size,-this->width+(j+1)*this->tile_size,0);
                    glVertex3f(-this->width+(i+1)*this->tile_size,-this->width+(j+1)*this->tile_size,0);
                    glVertex3f(-this->width+(i+1)*this->tile_size,-this->width+j*this->tile_size,0);
                }
                glEnd();
                glPopMatrix();

            }
        }

    }
     double intersect(Ray r, double *color,int l)
    {
        return 0.0;
    }



};




