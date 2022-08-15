#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
//include classes.h
#include "1705060_classes.h"

#define pi (2*acos(0.0))
using namespace std;
double posx,posy,posz;
double u[3],r[3],l[3];

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int levels,pixels,objects,point_lights,spot_lights;
int floor_width=1000,tile_width=20;



void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);


            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( 2, a,a);
        glVertex3f( 2,-a,a);
        glVertex3f(2,-a,-a);
        glVertex3f(2, a,-a);
    }
    glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
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
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
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
void drawOneEightSphere(double radius,int slices,int stacks)
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
            double a,b,c;

            a=r*cos(((double)j/(double)slices)/2*pi);

            b=r*sin(((double)j/(double)slices)/2*pi);

            c=h;

            points[i][j].x=a;
            points[i][j].y=b;
            points[i][j].z=c;

        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f(1.0,0.0,0.0);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

            }
            glEnd();
        }
    }
}
void drawCylinder(double h,double r,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;

    //generate points
    for(i=0; i<=stacks; i++)
    {

        for(j=0; j<=slices; j++)
        {


            points[i][j].x=r*cos(j*pi/(2*slices));
            points[i][j].y=r*sin(j*pi/(2*slices));
            points[i][j].z=h*i/(stacks);

        }
    }
    for(i=0; i<stacks; i++)
    {
        glColor3f(0.0,1.0,0.0);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

            }
            glEnd();
        }
    }

}

void drawBox(double n,double r)
{



    //square part
    //upper
    glPushMatrix();
    {

        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(n/2,n/2,n/2+r);
            glVertex3f(n/2,-n/2,(n/2+r));
            glVertex3f(-n/2,-n/2,(n/2+r));
            glVertex3f(-n/2,n/2,(n/2+r));
        }
        glEnd();
        //sides
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(n/2,(n/2+r),n/2);
            glVertex3f(n/2,(n/2+r),-n/2);
            glVertex3f(-n/2,(n/2+r),-n/2);
            glVertex3f(-n/2,(n/2+r),n/2);
        }
        glEnd();
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(n/2,-(n/2+r),n/2);
            glVertex3f(n/2,-(n/2+r),-n/2);
            glVertex3f(-n/2,-(n/2+r),-n/2);
            glVertex3f(-n/2,-(n/2+r),n/2);
        }
        glEnd();
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(-(n/2+r),n/2,n/2);
            glVertex3f(-(n/2+r),n/2,-n/2);
            glVertex3f(-(n/2+r),-n/2,-n/2);
            glVertex3f(-(n/2+r),-n/2,n/2);
        }
        glEnd();
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_QUADS);
        {
            glVertex3f((n/2+r),n/2,n/2);
            glVertex3f((n/2+r),n/2,-n/2);
            glVertex3f((n/2+r),-n/2,-n/2);
            glVertex3f((n/2+r),-n/2,n/2);
        }
        glEnd();
        //lower
        glColor3f(0.0,0.0,0.0);
        glBegin(GL_QUADS);
        {
            glVertex3f(n/2,n/2,-(n/2+r));
            glVertex3f(n/2,-n/2,-(n/2+r));
            glVertex3f(-n/2,-n/2,-(n/2+r));
            glVertex3f(-n/2,n/2,-(n/2+r));
        }
        glEnd();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(n/2,n/2,n/2);
        drawOneEightSphere(r,100,10);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(n/2,n/2,-n/2);
        glRotatef(90,0,1,0);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(-n/2,n/2,n/2);
        glRotatef(-90,0,1,0);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(n/2,-n/2,n/2);
        glRotatef(-90,0,0,1);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(n/2,-n/2,-n/2);
        glRotatef(180,1,0,0);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(-n/2,n/2,-n/2);
        glRotatef(-180,0,1,0);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(-n/2,-n/2,n/2);
        glRotatef(180,0,0,1);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(-n/2,-n/2,-n/2);
        glRotatef(180,0,0,1);
        glRotatef(-90,1,0,0);
        drawOneEightSphere(r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(n/2,n/2,-n/2);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(n/2,n/2,n/2);
        glRotatef(90,1,0,0);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(n/2,-n/2,-n/2);
        glRotatef(-90,1,0,0);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(-n/2,n/2,-n/2);
        glRotatef(90,0,1,0);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(n/2,n/2,n/2);
        glRotatef(-90,0,1,0);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(-n/2,n/2,-n/2);
        glRotatef(90,0,0,1);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(n/2,-n/2,-n/2);
        glRotatef(-90,0,0,1);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();
    glPushMatrix();
    {

        glTranslatef(-n/2,-n/2,-n/2);
        glRotatef(+180,0,0,1);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(-n/2,-n/2,n/2);
        glRotatef(-180,1,0,1);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(-n/2,-n/2,n/2);
        glRotatef(180,0,1,1);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(-n/2,n/2,-n/2);
        glRotatef(-90,1,0,0);
        glRotatef(-180,0,1,0);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

    glPushMatrix();
    {

        glTranslatef(-n/2,-n/2,-n/2);

        glRotatef(90,0,1,0);
        glRotatef(-90,0,0,1);
        drawCylinder(n,r,90,90);
    }
    glPopMatrix();

}

void keyboardListener(unsigned char key, int x,int y)
{
    double x1,y1,z1;
    switch(key)
    {
    case '2':
        x1=r[0],y1=r[1],z1=r[2];
        r[0]=r[0]*cos(-3*3.14/180)+l[0]*sin(-3*3.14/180);
        r[1]=r[1]*cos(-3*3.14/180)+l[1]*sin(-3*3.14/180);
        r[2]=r[2]*cos(-3*3.14/180)+l[2]*sin(-3*3.14/180);
        l[0]=l[0]*cos(-3*3.14/180)-x1*sin(-3*3.14/180);
        l[1]=l[1]*cos(-3*3.14/180)-y1*sin(-3*3.14/180);
        l[2]=l[2]*cos(-3*3.14/180)-z1*sin(-3*3.14/180);
        break;
    case '1':
        x1=r[0],y1=r[1],z1=r[2];
        r[0]=r[0]*cos(3*3.14/180)+l[0]*sin(3*3.14/180);
        r[1]=r[1]*cos(3*3.14/180)+l[1]*sin(3*3.14/180);
        r[2]=r[2]*cos(3*3.14/180)+l[2]*sin(3*3.14/180);
        l[0]=l[0]*cos(3*3.14/180)-x1*sin(3*3.14/180);
        l[1]=l[1]*cos(3*3.14/180)-y1*sin(3*3.14/180);
        l[2]=l[2]*cos(3*3.14/180)-z1*sin(3*3.14/180);
        break;
    case '3':
        x1=l[0],y1=l[1],z1=l[2];
        l[0]=l[0]*cos(3*3.14/180)+u[0]*sin(3*3.14/180);
        l[1]=l[1]*cos(3*3.14/180)+u[1]*sin(3*3.14/180);
        l[2]=l[2]*cos(3*3.14/180)+u[2]*sin(3*3.14/180);
        u[0]=u[0]*cos(3*3.14/180)-x1*sin(3*3.14/180);
        u[1]=u[1]*cos(3*3.14/180)-y1*sin(3*3.14/180);
        u[2]=u[2]*cos(3*3.14/180)-z1*sin(3*3.14/180);
        break;
    case '4':
        x1=l[0],y1=l[1],z1=l[2];
        l[0]=l[0]*cos(-3*3.14/180)+u[0]*sin(-3*3.14/180);
        l[1]=l[1]*cos(-3*3.14/180)+u[1]*sin(-3*3.14/180);
        l[2]=l[2]*cos(-3*3.14/180)+u[2]*sin(-3*3.14/180);
        u[0]=u[0]*cos(-3*3.14/180)-x1*sin(-3*3.14/180);
        u[1]=u[1]*cos(-3*3.14/180)-y1*sin(-3*3.14/180);
        u[2]=u[2]*cos(-3*3.14/180)-z1*sin(-3*3.14/180);
        break;
    case '6':
        x1=r[0],y1=r[1],z1=r[2];
        r[0]=r[0]*cos(3*3.14/180)-u[0]*sin(3*3.14/180);
        r[1]=r[1]*cos(3*3.14/180)-u[1]*sin(3*3.14/180);
        r[2]=r[2]*cos(3*3.14/180)-u[2]*sin(3*3.14/180);
        u[0]=u[0]*cos(3*3.14/180)+x1*sin(3*3.14/180);
        u[1]=u[1]*cos(3*3.14/180)+y1*sin(3*3.14/180);
        u[2]=u[2]*cos(3*3.14/180)+z1*sin(3*3.14/180);
        break;
    case '5':
        x1=r[0],y1=r[1],z1=r[2];
        r[0]=r[0]*cos(-3*3.14/180)-u[0]*sin(-3*3.14/180);
        r[1]=r[1]*cos(-3*3.14/180)-u[1]*sin(-3*3.14/180);
        r[2]=r[2]*cos(-3*3.14/180)-u[2]*sin(-3*3.14/180);
        u[0]=u[0]*cos(-3*3.14/180)+x1*sin(-3*3.14/180);
        u[1]=u[1]*cos(-3*3.14/180)+y1*sin(-3*3.14/180);
        u[2]=u[2]*cos(-3*3.14/180)+z1*sin(-3*3.14/180);
        break;
    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    double x1=u[0],y1=u[1];
    switch(key)
    {
    case GLUT_KEY_UP:		//down arrow key
        posx += l[0]*2.0;
        posy+=2.0*l[1];
        posz+=2.0*l[2];
        break;
    case GLUT_KEY_DOWN:		// up arrow key
        posx -= l[0]*2.0;
        posy-=2.0*l[1];
        posz-=2.0*l[2];
        break;

    case GLUT_KEY_RIGHT:
        posx += r[0]*2.0;
        posy+=2.0*r[1];
        posz+=2.0*r[2];
        break;
    case GLUT_KEY_LEFT:
        posx-= r[0]*2.0;
        posy-=2.0*r[1];
        posz-=2.0*r[2];
        break;

    case GLUT_KEY_PAGE_UP:
        posx += u[0]*2.0;
        posy+=2.0*u[1];
        posz+=2.0*u[2];
        break;
    case GLUT_KEY_PAGE_DOWN:
        posx -= u[0]*2.0;
        posy-=2.0*u[1];
        posz-=2.0*u[2];
        break;


    case GLUT_KEY_HOME:

        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}



void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(posx,posy,posz,	posx+l[0],posy+l[1],posz+l[2],	u[0],u[1],u[2]);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}
void loadData()
{
    //read data from scene.txt
    freopen("C:\\Users\\maish\\Documents\\GitHub\\Computer-Graphics\\Offline-3\\Ray Tracing\\scene.txt","r",stdin);
    //ifstream fin("scene.txt");
    // if(!fin)
    // {
    //     cout<<"Cannot open file"<<endl;
    //     exit(0);
    // }
    cin>>levels;
    cin>>pixels;
    cin>>objects;
    cout<<levels<<" "<<pixels<<" "<<objects<<endl;
    for(int i=0;i<objects;i++)
    {
        string s;
        cin>>s;
        if(s=="sphere")
        {
            point center;
            int shininess;
            double radius;
            double color[3];
            double coeffs[4];
            cin>>center.x>>center.y>>center.z;
            cin>>radius;
            cin>>color[0]>>color[1]>>color[2];
            cin>>coeffs[0]>>coeffs[1]>>coeffs[2]>>coeffs[3];
            cin>>shininess;
        }
        else if(s=="triangle")
        {
            point p1,p2,p3;
            double color[3];
            double coeffs[4];
            int shininess;
            cin>>p1.x>>p1.y>>p1.z;
            cin>>p2.x>>p2.y>>p2.z;
            cin>>p3.x>>p3.y>>p3.z;
            cin>>color[0]>>color[1]>>color[2];
            cin>>coeffs[0]>>coeffs[1]>>coeffs[2]>>coeffs[3];
            cin>>shininess;

        }
        else if(s=="general")
        {
            double coeffs[4];
            double color[3];
            int shininess;
            double params[10];
            double length,width,height;
            struct point cube_ref;
            cin>>params[0]>>params[1]>>params[2]>>params[3]>>params[4]>>params[5]>>params[6]>>params[7]>>params[8]>>params[9];
            cin>>cube_ref.x>>cube_ref.y>>cube_ref.z>>length>>width>>height;
            cin>>color[0]>>color[1]>>color[2];
            cin>>coeffs[0]>>coeffs[1]>>coeffs[2]>>coeffs[3];
            cin>>shininess;
        }
    }
    cin>>point_lights;
    for(int i=0;i<point_lights;i++)
    {
        struct point p;
        double color[3];
        cin>>p.x>>p.y>>p.z;
        cin>>color[0]>>color[1]>>color[2];
    }
    cin>>spot_lights;
    for(int i=0;i<spot_lights;i++)
    {
       struct point p;
        double color[3];
        struct point dir;
        double angle;
        cin>>p.x>>p.y>>p.z;
        cin>>color[0]>>color[1]>>color[2];
        cin>>dir.x>>dir.y>>dir.z;
        cin>>angle;
    }
    //fin.close();

}
void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    angle=0;
    posx=100.0;
    posy=100.0;
    posz=0.0;
    u[0]=0,u[1]=0,u[2]=1;
    r[0]=-0.907,r[1]=0.707,r[2]=0;
    l[0]=-0.707,l[1]=-0.707,l[2]=0;
    loadData();
    //box_a=50,cyl_r=25;
    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{

    glutInit(&argc,argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
