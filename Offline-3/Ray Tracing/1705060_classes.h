#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
#define pi 22/7
using namespace std;
struct point
{
    double x,y,z;
};
void drawSphere(double radius,int slices,int stacks,double color[3])
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
    double color[3];
    double coEfficients[4] ;
    int shine ;

    virtual void draw(){}
    void setColor(double color[3])
    {
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
    }
    void setCoEfficients(double coEfficients[4])
    {
        this->coEfficients[0] = coEfficients[0];
        this->coEfficients[1] = coEfficients[1];
        this->coEfficients[2] = coEfficients[2];
        this->coEfficients[3] = coEfficients[3];
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
        glPushMatrix();
        glTranslatef(reference_point.x, reference_point.y, reference_point.z);
        drawSphere(this->length,60,60,color);
        glPopMatrix();
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
};
//class with general quadratic equation inherits from Object
class Quadratic : public Object
{
public:
    double parameters[10];
    Quadratic(double params[10],struct point reference_point, double length, double width, double height, double color[3], double coEfficients[4], int shine)
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
// point light class
class PointLight
{
public:
    struct point position;
    double color[3];
    PointLight(struct point p, double color[3])
    {
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->position = p;
    }
    void draw()
    {

    }
};
//spot light class
class SpotLight
{
public:
    struct point position;
    double color[3];
    struct point direction;
    double angle;
    SpotLight(struct point p, double color[3], struct point dir, double angle)
    {
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->position = p;
        this->angle = angle;
        this->direction = dir;
    }
    void draw()
    {

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
        

    
};


