#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
#define INF 999999
//include classes.h
#include "1705060_classes.h"
#include "bitmap_image.hpp"

#define pi 22/7
using namespace std;
double posx,posy,posz;
double u[3],r[3],l[3];


int drawgrid;
int drawaxes;
double angle;
int levels,pixels,objects,point_lights,spot_lights;
double  floor_width=1000,tile_width=20;
vector<Object*> objects_list;
vector<PointLight*> point_lights_list;
vector<SpotLight*> spot_lights_list;
int windowWidth=600,windowHeight=600;
double fovY=80;
int imgCount;

void capture()
{
    imgCount++;
    // initialize bitmap image and set background color


    bitmap_image image(pixels,pixels);
    //find the view angle from u r l
    double planeDistance = (windowHeight/2.0) /tan(fovY*pi/180/2.0);
    double *topleft=new double[3];

    //topleft = eye + l*planeDistance - r*windowWidth/2 + u*windowHeight/2
    topleft[0] = posx + l[0]*planeDistance - r[0]*windowWidth/2 + u[0]*windowHeight/2;
    topleft[1] = posy + l[1]*planeDistance - r[1]*windowWidth/2 + u[1]*windowHeight/2;
    topleft[2] = posz + l[2]*planeDistance - r[2]*windowWidth/2 + u[2]*windowHeight/2;

    double du = 1.0*windowWidth/pixels;
    double dv = 1.0*windowHeight/pixels;
    //cout<<"du "<<du<<" dv "<<dv<<endl;
// Choose middle of the grid cell
    //topleft = topleft + r*(0.5*du) - u*(0.5*dv)
    topleft[0] = topleft[0] + r[0]*(0.5*du) - u[0]*(0.5*dv);
    topleft[1] = topleft[1] + r[1]*(0.5*du) - u[1]*(0.5*dv);
    topleft[2] = topleft[2] + r[2]*(0.5*du) - u[2]*(0.5*dv);
    int nearest;
    double t, tMin;
// for i=1:imageWidth
// for j=1:imageHeight
    for(int i=0; i<pixels; i++)
    {
        for(int j=0; j<pixels; j++)
        {
            double *pixel=new double[3];
            pixel[0] = topleft[0] + r[0]*(i)*du - u[0]*(j)*dv;
            pixel[1] = topleft[1] + r[1]*(i)*du - u[1]*(j)*dv;
            pixel[2] = topleft[2] + r[2]*(i)*du - u[2]*(j)*dv;
            //cout<<"pixel "<<pixel[0]<<" "<<pixel[1]<<" "<<pixel[2]<<endl;

            struct point origin,dir;
            origin.x=posx;
            origin.y=posy;
            origin.z=posz;
            dir.x=pixel[0]-posx;
            dir.y=pixel[1]-posy;
            dir.z=pixel[2]-posz;
            //normalize dir
            double dir_length = sqrt(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
            dir.x = dir.x/dir_length;
            dir.y = dir.y/dir_length;
            dir.z = dir.z/dir_length;
            Ray ray(origin,dir);
            double *color =new double[3];
            color[0]=0;
            color[1]=0;
            color[2]=0;
            tMin = INF;
            nearest = -1;
            for(int k=0; k<objects_list.size(); k++)
            {
                double t=objects_list[k]->intersect(ray,color,0);
                
                if(t<tMin)
                {
                    tMin=t;
                    nearest=k;
                }
            }
            if(nearest!=-1)
            {

                double *color =new double[3];
                color[0]=0;
                color[1]=0;
                color[2]=0;
                tMin= objects_list[nearest]->intersect(ray,color,1);
               
                image.set_pixel(i,j,color[0]*255,color[1]*255,color[2]*255);
            }


        }
    }
    image.save_image("C:\\Users\\maish\\Documents\\GitHub\\Computer-Graphics\\Offline-3\\Ray Tracing\\Output_1"+to_string(imgCount)+".bmp");
}
// calculate curPixel using topleft,r,u,i,j,du,dv
// cast ray from eye to (curPixel-eye) direction
// double *color = new double[3]
// for each object, o in objects
// t = o.intersect(ray, dummyColor, 0)
// update t so that it stores min +ve value
// save the nearest object, on
// tmin = on->intersect(ray, color, 1)
// update image pixel (i,j)
// save image


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
    case '0':
        capture();
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

    

    for(int i=0; i<objects; i++)
    {
        objects_list[i]->draw();
    }
    for(int i=0; i<point_lights; i++)
    {
        point_lights_list[i]->draw();
    }
    for(int i=0; i<spot_lights; i++)
    {
        spot_lights_list[i]->draw();
    }
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
    Object *f=new Floor(floor_width,tile_width);
    objects_list.push_back(f);
    cin>>levels;
    cin>>pixels;
    cin>>objects;
    //cout<<levels<<" "<<pixels<<" "<<objects<<endl;
    for(int i=0; i<objects; i++)
    {
        string s;
        cin>>s;
        if(s=="sphere")
        {
            point center;
            int shininess;
            double radius;
            double *color=new double[3];
            double *coeffs=new double[4];
            cin>>center.x>>center.y>>center.z;
            cin>>radius;
            cin>>color[0]>>color[1]>>color[2];
            cin>>coeffs[0]>>coeffs[1]>>coeffs[2]>>coeffs[3];
            cin>>shininess;
            Object *sphere=new Sphere(center,radius,color,coeffs,shininess);
            objects_list.push_back(sphere);
        }
        else if(s=="triangle")
        {
            point p1,p2,p3;
            double *color=new double[3];
            double *coeffs=new double[4];
            int shininess;
            cin>>p1.x>>p1.y>>p1.z;
            cin>>p2.x>>p2.y>>p2.z;
            cin>>p3.x>>p3.y>>p3.z;
            cin>>color[0]>>color[1]>>color[2];
            cin>>coeffs[0]>>coeffs[1]>>coeffs[2]>>coeffs[3];
            cin>>shininess;
            Object *triangle=new Triangle(p1,p2,p3,color,coeffs,shininess);
            objects_list.push_back(triangle);

        }
        else if(s=="general")
        {
            double *coeffs=new double[4];
            double *color=new double[3];
            int shininess;
            double *params=new double[10];
            double length,width,height;
            struct point cube_ref;
            cin>>params[0]>>params[1]>>params[2]>>params[3]>>params[4]>>params[5]>>params[6]>>params[7]>>params[8]>>params[9];
            cin>>cube_ref.x>>cube_ref.y>>cube_ref.z>>length>>width>>height;
            cin>>color[0]>>color[1]>>color[2];
            cin>>coeffs[0]>>coeffs[1]>>coeffs[2]>>coeffs[3];
            cin>>shininess;
            Object *general=new Quadratic(params,cube_ref,length,width,height,color,coeffs,shininess);
            objects_list.push_back(general);
        }
    }
    cin>>point_lights;
    for(int i=0; i<point_lights; i++)
    {
        struct point p;
        double *color=new double[3];
        cin>>p.x>>p.y>>p.z;
        cin>>color[0]>>color[1]>>color[2];
        PointLight *pl=new PointLight(p,color);
        point_lights_list.push_back(pl);
    }
    cin>>spot_lights;
    for(int i=0; i<spot_lights; i++)
    {
        struct point p;
        double *color=new double[3];
        struct point dir;
        double angle;
        cin>>p.x>>p.y>>p.z;
        cin>>color[0]>>color[1]>>color[2];
        cin>>dir.x>>dir.y>>dir.z;
        cin>>angle;
        SpotLight *sl=new SpotLight(p,color,dir,angle);
        spot_lights_list.push_back(sl);
    }
    //fin.close();

}
void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    angle=0;
    posx=150.0;
    posy=150.0;
    posz=50.0;
    u[0]=0,u[1]=0,u[2]=1;
    r[0]=-0.907,r[1]=0.707,r[2]=0;
    l[0]=-0.707,l[1]=-0.707,l[2]=0;
    loadData();
    imgCount=0;
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
    gluPerspective(fovY,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{

    glutInit(&argc,argv);
    glutInitWindowSize(windowWidth,windowHeight);
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
