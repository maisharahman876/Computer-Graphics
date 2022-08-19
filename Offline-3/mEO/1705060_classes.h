#include<bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
#define INF 999999
#define pi acos(-1.0)
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
        // cout<<"origin: "<<origin.x<<" "<<origin.y<<" "<<origin.z<<endl;
        // cout<<"direction: "<<direction.x<<" "<<direction.y<<" "<<direction.z<<endl;
    }

};
void drawSphere(double radius,int slices,int stacks,double *color)
{
    struct point points[40][40];
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
        //draw a point
        glPushMatrix();
        glTranslatef(position.x,position.y,position.z);
        drawSphere(1,30,30,color);
        glPopMatrix();

    }
    double intersect(Ray r, double *color,int l)
    {

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
        //draw a point
        glPushMatrix();
        glTranslatef(position.x,position.y,position.z);
        drawSphere(1,10,10,color);
        glPopMatrix();


    }
    double intersect(Ray r, double *color,int l)
    {
        return 0.0;
    }
};
class Object
{
public:
    struct point reference_point;
    double height, width, length;
    double *color;
    double *coEfficients ;
    int shine ;
    vector<Object*> objects;
    vector<PointLight*> pointLights;
    vector<SpotLight*> spotLights;
    int recursion_level;

    virtual void draw() {}
    virtual double intersect(Ray r, double *color,int l)
    {
        return 0.0;
    }
    void setColor(double *color)
    {
        this->color = color;
        //cout<<color[0]<<" "<<color[1]<<" "<<color[2]<<endl;
    }
    void setCoEfficients(double *coEfficients)
    {
        this->coEfficients = coEfficients;
    }
    void setShine(int shine)
    {
        this->shine = shine;
    }
    void setObjectsAndLights(vector<Object*> objects, vector<PointLight*> pointLights, vector<SpotLight*> spotLights,int recursion_level)

    {
        this->objects = objects;
        this->pointLights = pointLights;
        this->spotLights = spotLights;
        this->recursion_level = recursion_level;

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
        drawSphere(this->length,10,10,color);
        glPopMatrix();
    }
    double intersect(Ray r, double *color,int l)
    {
        double a,b,c,d;
        double t1,t2,t_min;
        //  //a=Rd.Rd
        //  a=1;
        //  //b=2*Rd*R0
        //  b=2*(r.origin.x*r.direction.x+r.origin.y*r.direction.y+r.origin.z*r.direction.z);
        //  c=r.origin.x*r.origin.x+r.origin.y*r.origin.y+r.origin.z*r.origin.z-this->length*this->length;
        a = pow(r.direction.x,2) + pow(r.direction.y,2) + pow(r.direction.z,2);
        //b=2*Rd*(R0-Rc)
        b=2*(r.origin.x*r.direction.x + r.origin.y*r.direction.y + r.origin.z*r.direction.z - (reference_point.x*r.direction.x + reference_point.y*r.direction.y + reference_point.z*r.direction.z));
        //b = 2*(r.origin.x*r.direction.x + r.origin.y*r.direction.y + r.origin.z*r.direction.z);
        //c=R0.R0+Rc.Rc-2*Rc.R0-Rr^2
        c=pow(r.origin.x,2) + pow(r.origin.y,2) + pow(r.origin.z,2)+pow(reference_point.x,2)+pow(reference_point.y,2)+pow(reference_point.z,2)-2*(r.origin.x*reference_point.x + r.origin.y*reference_point.y + r.origin.z*reference_point.z)-pow(length,2);

        d = b*b - 4*a*c;

        if(d<0)
        {
            t_min=INF;
        }
        else
        {
            t1 = (-b + sqrt(d))/(2*a);
            t2 = (-b - sqrt(d))/(2*a);
            if(t1<t2)
            {
                t_min = t1;
            }
            else
            {
                t_min = t2;
            }
        }

        if(l==0)
            return t_min;
        color[0] = this->color[0]*this->coEfficients[0];
        color[1] = this->color[1]*this->coEfficients[0];
        color[2] = this->color[2]*this->coEfficients[0];
        //calculate the intersection point
        struct point intersection_point;
        intersection_point.x = r.origin.x + t_min*r.direction.x;
        intersection_point.y = r.origin.y + t_min*r.direction.y;
        intersection_point.z = r.origin.z + t_min*r.direction.z;
        //calculate normal
        struct point normal;
        normal.x = intersection_point.x - reference_point.x;
        normal.y = intersection_point.y - reference_point.y;
        normal.z = intersection_point.z - reference_point.z;
        //normalize normal
        double normal_length = sqrt(pow(normal.x,2)+pow(normal.y,2)+pow(normal.z,2));
        normal.x = normal.x/normal_length;
        normal.y = normal.y/normal_length;
        normal.z = normal.z/normal_length;
        //dot product of -normal and ray direction
        double dot_product = -normal.x*r.direction.x - normal.y*r.direction.y - normal.z*r.direction.z;
        //if dot product is negative, reverse the normal
        if(dot_product<0)
        {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        // for each point light pl in pointLights
        for(int i=0; i<pointLights.size(); i++)
        {


            struct point light_direction;
            light_direction.x = intersection_point.x - pointLights[i]->position.x;
            light_direction.y = intersection_point.y - pointLights[i]->position.y;
            light_direction.z = intersection_point.z - pointLights[i]->position.z;

            //normalize light_direction
            double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
            light_direction.x = light_direction.x/light_direction_length;
            light_direction.y = light_direction.y/light_direction_length;
            light_direction.z = light_direction.z/light_direction_length;
            Ray ray1(pointLights[i]->position,light_direction);

            //check if the object is in shadow
            double tmin = INF,t3;
            for(int j=0; j<objects.size(); j++)
            {
                double *color1 = new double[3];
                color1[0] = 0;
                color1[1] = 0;
                color1[2] = 0;
                t3 = objects[j]->intersect(ray1,color1,0);
                if(t3<tmin&&t3>0)
                {
                    tmin = t3;
                }
            }
            struct point Tmin_point;
            Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
            Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
            Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
            //distance betn tmin_point and light
            double Tmin_dist = sqrt(pow(Tmin_point.x-pointLights[i]->position.x,2)+pow(Tmin_point.y-pointLights[i]->position.y,2)+pow(Tmin_point.z-pointLights[i]->position.z,2));
            //distance betn intersection_point and light
            double intersection_dist = sqrt(pow(intersection_point.x-pointLights[i]->position.x,2)+pow(intersection_point.y-pointLights[i]->position.y,2)+pow(intersection_point.z-pointLights[i]->position.z,2));
            if(intersection_dist-0.0000001>Tmin_dist)
            {
                continue;
            }
            //calculate lambertValue using normal, rayl
            double lambertValue = 0;
            lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
            //calculate reflected ray
            struct point reflected_ray_direction;
            //2*(L.N)*N-L

            reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
            reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
            reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
            Ray reflected_ray(intersection_point,reflected_ray_direction);

            //calculate phongValue using normal, rayl, rayr
            double phongValue = 0;
            phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
            //calculate the color
            color[0] = color[0] + this->coEfficients[1]*pointLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*pointLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
            color[1] = color[1] + this->coEfficients[1]*pointLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*pointLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
            color[2] = color[2] + this->coEfficients[1]*pointLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*pointLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


        }
        for(int i=0; i<spotLights.size(); i++)
        {


            struct point light_direction;
            light_direction.x = intersection_point.x - spotLights[i]->position.x;
            light_direction.y = intersection_point.y - spotLights[i]->position.y;
            light_direction.z = intersection_point.z - spotLights[i]->position.z;
            //if spotlight angle > cutoff angle, ignore the light
            if(acos(spotLights[i]->direction.x*light_direction.x+spotLights[i]->direction.y*light_direction.y+spotLights[i]->direction.z*light_direction.z)>spotLights[i]->angle*pi/(180.0))
            {
                continue;
            }

            //normalize light_direction
            double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
            light_direction.x = light_direction.x/light_direction_length;
            light_direction.y = light_direction.y/light_direction_length;
            light_direction.z = light_direction.z/light_direction_length;
            Ray ray1(pointLights[i]->position,light_direction);

            //check if the object is in shadow
            double tmin = INF,t3;
            for(int j=0; j<objects.size(); j++)
            {
                double *color1 = new double[3];
                color1[0] = 0;
                color1[1] = 0;
                color1[2] = 0;
                t3 = objects[j]->intersect(ray1,color1,0);
                if(t3<tmin&&t3>0)
                {
                    tmin = t3;
                }
            }
            struct point Tmin_point;
            Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
            Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
            Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
            //distance betn tmin_point and light
            double Tmin_dist = sqrt(pow(Tmin_point.x-spotLights[i]->position.x,2)+pow(Tmin_point.y-spotLights[i]->position.y,2)+pow(Tmin_point.z-spotLights[i]->position.z,2));
            //distance betn intersection_point and light
            double intersection_dist = sqrt(pow(intersection_point.x-spotLights[i]->position.x,2)+pow(intersection_point.y-spotLights[i]->position.y,2)+pow(intersection_point.z-spotLights[i]->position.z,2));
            if(intersection_dist-0.0000001>Tmin_dist)
            {
                continue;
            }
            //calculate lambertValue using normal, rayl
            double lambertValue = 0;
            lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
            //calculate reflected ray
            struct point reflected_ray_direction;
            //2*(L.N)*N-L

            reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
            reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
            reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
            Ray reflected_ray(intersection_point,reflected_ray_direction);

            //calculate phongValue using normal, rayl, rayr
            double phongValue = 0;
            phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
            //calculate the color
            color[0] = color[0] + this->coEfficients[1]*spotLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*spotLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
            color[1] = color[1] + this->coEfficients[1]*spotLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*spotLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
            color[2] = color[2] + this->coEfficients[1]*spotLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*spotLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


        }
        if(l>recursion_level)
            return t_min;
        struct point reflection_dir;
        //L.N
        double dot = r.direction.x*normal.x+r.direction.y*normal.y+r.direction.z*normal.z;
        reflection_dir.x=r.direction.x-normal.x*2*dot;
        reflection_dir.y=r.direction.y-normal.y*2*dot;
        reflection_dir.z=r.direction.z-normal.z*2*dot;

        //moving the point a little forward to avoid self-intersection
        struct point reflection_point;
        reflection_point.x = intersection_point.x + reflection_dir.x*0.00001;
        reflection_point.y = intersection_point.y + reflection_dir.y*0.00001;
        reflection_point.z = intersection_point.z + reflection_dir.z*0.00001;

        Ray reflection_ray(reflection_point,reflection_dir);
        double *dummycolor =new double[3];
        dummycolor[0]=0;
        dummycolor[1]=0;
        dummycolor[2]=0;
        double tMin = INF;
        int nearest = -1;
        for(int k=0; k<objects.size(); k++)
        {
            double t=objects[k]->intersect(reflection_ray,color,0);

            if(t<tMin&&t>0)
            {
                tMin=t;
                nearest=k;
            }
        }
        double *reflectedcolor =new double[3];
        reflectedcolor[0]=0;
        reflectedcolor[1]=0;
        reflectedcolor[2]=0;
        if(nearest!=-1)
        {

            tMin=objects[nearest]->intersect(reflection_ray,reflectedcolor,l+1);
        }
        color[0]=min(color[0]+this->coEfficients[3]*reflectedcolor[0],1.0);
        color[1]=min(color[1]+this->coEfficients[3]*reflectedcolor[1],1.0);
        color[2]=min(color[2]+this->coEfficients[3]*reflectedcolor[2],1.0);
        return t_min;




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
        double A;
        double At,Ag,Ab;
        double gamma,beta,t_min;
        A=(p1.x-p2.x)*((p1.y-p3.y)*(r.direction.z)-(p1.z-p3.z)*(r.direction.y))-(p1.x-p3.x)*((p1.y-p2.y)*(r.direction.z)-(p1.z-p2.z)*(r.direction.y))+r.direction.x*((p1.y-p2.y)*(p1.z-p3.z)-(p1.z-p2.z)*(p1.y-p3.y));
        At=(p1.x-p2.x)*((p1.y-p3.y)*(p1.z-r.origin.z)-(p1.z-p3.z)*(p1.y-r.origin.y))-(p1.x-p3.x)*((p1.y-p2.y)*(p1.z-r.origin.z)-(p1.z-p2.z)*(p1.y-r.origin.y))+(p1.x-r.origin.x)*((p1.y-p2.y)*(p1.z-p3.z)-(p1.z-p2.z)*(p1.y-p3.y));
        Ab=(p1.x-r.origin.x)*((p1.y-p3.y)*(r.direction.z)-(p1.z-p3.z)*(r.direction.y))-(p1.x-p3.x)*((p1.y-r.origin.y)*(r.direction.z)-(p1.z-r.origin.z)*(r.direction.y))+r.direction.x*((p1.y-r.origin.y)*(p1.z-p3.z)-(p1.z-r.origin.z)*(p1.y-p3.y));
        Ag=(p1.x-p2.x)*((p1.y-r.origin.y)*(r.direction.z)-(p1.z-r.origin.z)*(r.direction.y))-(p1.x-r.origin.x)*((p1.y-p2.y)*(r.direction.z)-(p1.z-p2.z)*(r.direction.y))+r.direction.x*((p1.y-p2.y)*(p1.z-r.origin.z)-(p1.z-p2.z)*(p1.y-r.origin.y));

        if(A==0)
        {
            return INF;
        }
        else
        {
            beta=Ab/A;
            gamma=Ag/A;
            if(beta>0&&gamma>0&&beta+gamma<1)
            {
                t_min=At/A;

            }
            else
            {
                return INF;
            }

        }
        if(l==0)
            return t_min;
        color[0] = this->color[0]*this->coEfficients[0];
        color[1] = this->color[1]*this->coEfficients[0];
        color[2] = this->color[2]*this->coEfficients[0];
        //calculate the intersection point
        struct point intersection_point;
        intersection_point.x = r.origin.x + t_min*r.direction.x;
        intersection_point.y = r.origin.y + t_min*r.direction.y;
        intersection_point.z = r.origin.z + t_min*r.direction.z;
        //calculate normal
        struct point normal;
        //(p2-P1)cross (p3-P1)
        normal.x = (p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y);
        normal.y = (p2.z-p1.z)*(p3.x-p1.x)-(p2.x-p1.x)*(p3.z-p1.z);
        normal.z = (p2.x-p1.x)*(p3.y-p1.y)-(p2.y-p1.y)*(p3.x-p1.x);
        //normalize normal
        double normal_length = sqrt(pow(normal.x,2)+pow(normal.y,2)+pow(normal.z,2));
        normal.x = normal.x/normal_length;
        normal.y = normal.y/normal_length;
        normal.z = normal.z/normal_length;
        //dot product of -normal and ray direction
        double dot_product = -normal.x*r.direction.x - normal.y*r.direction.y - normal.z*r.direction.z;
        //if dot product is negative, reverse the normal
        if(dot_product<0)
        {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        // for each point light pl in pointLights
        for(int i=0; i<pointLights.size(); i++)
        {


            struct point light_direction;
            light_direction.x = intersection_point.x - pointLights[i]->position.x;
            light_direction.y = intersection_point.y - pointLights[i]->position.y;
            light_direction.z = intersection_point.z - pointLights[i]->position.z;

            //normalize light_direction
            double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
            light_direction.x = light_direction.x/light_direction_length;
            light_direction.y = light_direction.y/light_direction_length;
            light_direction.z = light_direction.z/light_direction_length;
            Ray ray1(pointLights[i]->position,light_direction);

            //check if the object is in shadow
            double tmin = INF,t3;
            for(int j=0; j<objects.size(); j++)
            {
                double *color1 = new double[3];
                color1[0] = 0;
                color1[1] = 0;
                color1[2] = 0;
                t3 = objects[j]->intersect(ray1,color1,0);
                if(t3<tmin&&t3>0)
                {
                    tmin = t3;
                }
            }
            struct point Tmin_point;
            Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
            Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
            Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
            //distance betn tmin_point and light
            double Tmin_dist = sqrt(pow(Tmin_point.x-pointLights[i]->position.x,2)+pow(Tmin_point.y-pointLights[i]->position.y,2)+pow(Tmin_point.z-pointLights[i]->position.z,2));
            //distance betn intersection_point and light
            double intersection_dist = sqrt(pow(intersection_point.x-pointLights[i]->position.x,2)+pow(intersection_point.y-pointLights[i]->position.y,2)+pow(intersection_point.z-pointLights[i]->position.z,2));
            if(intersection_dist-0.0000001>Tmin_dist)
            {
                continue;
            }
            //calculate lambertValue using normal, rayl
            double lambertValue = 0;
            lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
            //calculate reflected ray
            struct point reflected_ray_direction;
            //2*(L.N)*N-L

            reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
            reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
            reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
            Ray reflected_ray(intersection_point,reflected_ray_direction);

            //calculate phongValue using normal, rayl, rayr
            double phongValue = 0;
            phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
            //calculate the color
            color[0] = color[0] + this->coEfficients[1]*pointLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*pointLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
            color[1] = color[1] + this->coEfficients[1]*pointLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*pointLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
            color[2] = color[2] + this->coEfficients[1]*pointLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*pointLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


        }
        for(int i=0; i<spotLights.size(); i++)
        {


            struct point light_direction;
            light_direction.x = intersection_point.x - spotLights[i]->position.x;
            light_direction.y = intersection_point.y - spotLights[i]->position.y;
            light_direction.z = intersection_point.z - spotLights[i]->position.z;
            //if spotlight angle > cutoff angle, ignore the light
            if(acos(spotLights[i]->direction.x*light_direction.x+spotLights[i]->direction.y*light_direction.y+spotLights[i]->direction.z*light_direction.z)>spotLights[i]->angle*pi/(180.0))
            {
                continue;
            }

            //normalize light_direction
            double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
            light_direction.x = light_direction.x/light_direction_length;
            light_direction.y = light_direction.y/light_direction_length;
            light_direction.z = light_direction.z/light_direction_length;
            Ray ray1(pointLights[i]->position,light_direction);

            //check if the object is in shadow
            double tmin = INF,t3;
            for(int j=0; j<objects.size(); j++)
            {
                double *color1 = new double[3];
                color1[0] = 0;
                color1[1] = 0;
                color1[2] = 0;
                t3 = objects[j]->intersect(ray1,color1,0);
                if(t3<tmin&&t3>0)
                {
                    tmin = t3;
                }
            }
            struct point Tmin_point;
            Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
            Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
            Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
            //distance betn tmin_point and light
            double Tmin_dist = sqrt(pow(Tmin_point.x-spotLights[i]->position.x,2)+pow(Tmin_point.y-spotLights[i]->position.y,2)+pow(Tmin_point.z-spotLights[i]->position.z,2));
            //distance betn intersection_point and light
            double intersection_dist = sqrt(pow(intersection_point.x-spotLights[i]->position.x,2)+pow(intersection_point.y-spotLights[i]->position.y,2)+pow(intersection_point.z-spotLights[i]->position.z,2));
            if(intersection_dist-0.0000001>Tmin_dist)
            {
                continue;
            }
            //calculate lambertValue using normal, rayl
            double lambertValue = 0;
            lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
            //calculate reflected ray
            struct point reflected_ray_direction;
            //2*(L.N)*N-L

            reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
            reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
            reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
            Ray reflected_ray(intersection_point,reflected_ray_direction);

            //calculate phongValue using normal, rayl, rayr
            double phongValue = 0;
            phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
            //calculate the color
            color[0] = color[0] + this->coEfficients[1]*spotLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*spotLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
            color[1] = color[1] + this->coEfficients[1]*spotLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*spotLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
            color[2] = color[2] + this->coEfficients[1]*spotLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*spotLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


        }
        if(l>recursion_level)
            return t_min;
        struct point reflection_dir;
        //L.N
        double dot = r.direction.x*normal.x+r.direction.y*normal.y+r.direction.z*normal.z;
        reflection_dir.x=r.direction.x-normal.x*2*dot;
        reflection_dir.y=r.direction.y-normal.y*2*dot;
        reflection_dir.z=r.direction.z-normal.z*2*dot;

        //moving the point a little forward to avoid self-intersection
        struct point reflection_point;
        reflection_point.x = intersection_point.x + reflection_dir.x*0.00001;
        reflection_point.y = intersection_point.y + reflection_dir.y*0.00001;
        reflection_point.z = intersection_point.z + reflection_dir.z*0.00001;

        Ray reflection_ray(reflection_point,reflection_dir);
        double *dummycolor =new double[3];
        dummycolor[0]=0;
        dummycolor[1]=0;
        dummycolor[2]=0;
        double tMin = INF;
        int nearest = -1;
        for(int k=0; k<objects.size(); k++)
        {
            double t=objects[k]->intersect(reflection_ray,color,0);

            if(t<tMin&&t>0)
            {
                tMin=t;
                nearest=k;
            }
        }
        double *reflectedcolor =new double[3];
        reflectedcolor[0]=0;
        reflectedcolor[1]=0;
        reflectedcolor[2]=0;
        if(nearest!=-1)
        {

            tMin=objects[nearest]->intersect(reflection_ray,reflectedcolor,l+1);
        }
        color[0]=min(color[0]+this->coEfficients[3]*reflectedcolor[0],1.0);
        color[1]=min(color[1]+this->coEfficients[3]*reflectedcolor[1],1.0);
        color[2]=min(color[2]+this->coEfficients[3]*reflectedcolor[2],1.0);

        return t_min;


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
        double A,B,C,D,E,F,G,H,I,J;
        A=parameters[0];
        B=parameters[1];
        C=parameters[2];
        D=parameters[3];
        E=parameters[4];
        F=parameters[5];
        G=parameters[6];
        H=parameters[7];
        I=parameters[8];
        J=parameters[9];
        //Ax^2+By^2+Cz^2+Dxy+Exz+Fyz+Gx+Hy+Iz+J=0
        //using px=Ro+tRd
        //transforn it into at^2+bt+c=0
        double a,b,c,d;
        a=A*r.direction.x*r.direction.x+B*r.direction.y*r.direction.y+C*r.direction.z*r.direction.z+D*r.direction.x*r.direction.y+E*r.direction.x*r.direction.z+F*r.direction.y*r.direction.z;

        b=2*(A*r.origin.x*r.direction.x+B*r.origin.y*r.direction.y+C*r.origin.z*r.direction.z);
        b+=D*(r.origin.x*r.direction.y+r.origin.y*r.direction.x)+E*(r.origin.x*r.direction.z+r.origin.z*r.direction.x)+F*(r.origin.y*r.direction.z+r.origin.z*r.direction.y);
        b+=G*r.direction.x+H*r.direction.y+I*r.direction.z;

        c=A*r.origin.x*r.origin.x+B*r.origin.y*r.origin.y+C*r.origin.z*r.origin.z+D*r.origin.x*r.origin.y+E*r.origin.x*r.origin.z+F*r.origin.y*r.origin.z;
        c+=G*r.origin.x+H*r.origin.y+I*r.origin.z+J;

        d=b*b-4*a*c;
        double t_min,t_max;
        if(a==0)
        {
            t_max=INF;
            if(b==0)
            {
                t_min=INF;
            }
            else
            {
                t_min=-c/b;
            }
        }
        else
        {
            if(d<0)
            {
                t_min=INF;
                t_max=INF;
            }
            else if(d==0)
            {
                t_min=-b/(2*a);
                t_max=INF;
            }
            else
            {
                t_min=(-b-sqrt(d))/(2*a);
                t_max=(-b+sqrt(d))/(2*a);

            }
        }
        if(t_min>t_max)
        {
            double temp=t_min;
            t_min=t_max;
            t_max=temp;
        }
        if(t_min<0)
            t_min=INF;

        //clipping
        if(t_min!=INF&&t_max!=INF)
        {
            if(t_min>0)
            {
                struct point intersect;
                intersect.x=r.origin.x+t_min*r.direction.x;
                intersect.y=r.origin.y+t_min*r.direction.y;
                intersect.z=r.origin.z+t_min*r.direction.z;
                if((this->length!=0&&(intersect.x<this->reference_point.x||intersect.x>this->reference_point.x+this->length))||(this->width!=0&&(intersect.y<this->reference_point.y||intersect.y>this->reference_point.y+this->width))||(this->height!=0&&(intersect.z<this->reference_point.z||intersect.z>this->reference_point.z+this->height)))
                {
                    t_min=INF;
                }
            }
            if(t_max>0)
            {
                struct point intersect;
                intersect.x=r.origin.x+t_max*r.direction.x;
                intersect.y=r.origin.y+t_max*r.direction.y;
                intersect.z=r.origin.z+t_max*r.direction.z;
                if((this->length!=0&&(intersect.x<this->reference_point.x||intersect.x>this->reference_point.x+this->length))||(this->width!=0&&(intersect.y<this->reference_point.y||intersect.y>this->reference_point.y+this->width))||(this->height!=0&&(intersect.z<this->reference_point.z||intersect.z>this->reference_point.z+this->height)))
                {
                    t_max=INF;
                }

            }
            if(t_min>t_max)
                t_min=t_max;
        }
        else if(t_min!=INF&&t_min>0)
        {
            struct point intersect;
            intersect.x=r.origin.x+t_min*r.direction.x;
            intersect.y=r.origin.y+t_min*r.direction.y;
            intersect.z=r.origin.z+t_min*r.direction.z;
            if((this->length!=0&&(intersect.x<this->reference_point.x||intersect.x>this->reference_point.x+this->length))||(this->width!=0&&(intersect.y<this->reference_point.y||intersect.y>this->reference_point.y+this->width))||(this->height!=0&&(intersect.z<this->reference_point.z||intersect.z>this->reference_point.z+this->height)))
            {
                t_min=INF;
            }

        }
        if(l==0)
            return t_min;
        color[0] = this->color[0]*this->coEfficients[0];
        color[1] = this->color[1]*this->coEfficients[0];
        color[2] = this->color[2]*this->coEfficients[0];
        struct point intersection_point;
        intersection_point.x=r.origin.x+t_min*r.direction.x;
        intersection_point.y=r.origin.y+t_min*r.direction.y;
        intersection_point.z=r.origin.z+t_min*r.direction.z;
        struct point normal;
        //2Ax+Dy+Ez+G
        normal.x = 2*A*intersection_point.x+D*intersection_point.y+E*intersection_point.z+G;
        normal.y = 2*B*intersection_point.y+D*intersection_point.x+F*intersection_point.z+H;
        normal.z = 2*C*intersection_point.z+E*intersection_point.x+F*intersection_point.y+I;
        //normalize normal
        double normal_length = sqrt(pow(normal.x,2)+pow(normal.y,2)+pow(normal.z,2));
        normal.x = normal.x/normal_length;
        normal.y = normal.y/normal_length;
        normal.z = normal.z/normal_length;
        //dot product of -normal and ray direction
        double dot_product = -normal.x*r.direction.x - normal.y*r.direction.y - normal.z*r.direction.z;
        //if dot product is negative, reverse the normal
        if(dot_product<0)
        {
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        // for each point light pl in pointLights
        for(int i=0; i<pointLights.size(); i++)
        {


            struct point light_direction;
            light_direction.x = intersection_point.x - pointLights[i]->position.x;
            light_direction.y = intersection_point.y - pointLights[i]->position.y;
            light_direction.z = intersection_point.z - pointLights[i]->position.z;

            //normalize light_direction
            double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
            light_direction.x = light_direction.x/light_direction_length;
            light_direction.y = light_direction.y/light_direction_length;
            light_direction.z = light_direction.z/light_direction_length;
            Ray ray1(pointLights[i]->position,light_direction);

            //check if the object is in shadow
            double tmin = INF,t3;
            for(int j=0; j<objects.size(); j++)
            {
                double *color1 = new double[3];
                color1[0] = 0;
                color1[1] = 0;
                color1[2] = 0;
                t3 = objects[j]->intersect(ray1,color1,0);
                if(t3<tmin&&t3>0)
                {
                    tmin = t3;
                }
            }
            struct point Tmin_point;
            Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
            Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
            Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
            //distance betn tmin_point and light
            double Tmin_dist = sqrt(pow(Tmin_point.x-pointLights[i]->position.x,2)+pow(Tmin_point.y-pointLights[i]->position.y,2)+pow(Tmin_point.z-pointLights[i]->position.z,2));
            //distance betn intersection_point and light
            double intersection_dist = sqrt(pow(intersection_point.x-pointLights[i]->position.x,2)+pow(intersection_point.y-pointLights[i]->position.y,2)+pow(intersection_point.z-pointLights[i]->position.z,2));
            if(intersection_dist-0.0000001>Tmin_dist)
            {
                continue;
            }
            //calculate lambertValue using normal, rayl
            double lambertValue = 0;
            lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
            //calculate reflected ray
            struct point reflected_ray_direction;
            //2*(L.N)*N-L

            reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
            reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
            reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
            Ray reflected_ray(intersection_point,reflected_ray_direction);

            //calculate phongValue using normal, rayl, rayr
            double phongValue = 0;
            phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
            //calculate the color
            color[0] = color[0] + this->coEfficients[1]*pointLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*pointLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
            color[1] = color[1] + this->coEfficients[1]*pointLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*pointLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
            color[2] = color[2] + this->coEfficients[1]*pointLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*pointLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


        }
        for(int i=0; i<spotLights.size(); i++)
        {


            struct point light_direction;
            light_direction.x = intersection_point.x - spotLights[i]->position.x;
            light_direction.y = intersection_point.y - spotLights[i]->position.y;
            light_direction.z = intersection_point.z - spotLights[i]->position.z;
            //if spotlight angle > cutoff angle, ignore the light
            if(acos(spotLights[i]->direction.x*light_direction.x+spotLights[i]->direction.y*light_direction.y+spotLights[i]->direction.z*light_direction.z)>spotLights[i]->angle*pi/(180.0))
            {
                continue;
            }
            //normalize light_direction
            double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
            light_direction.x = light_direction.x/light_direction_length;
            light_direction.y = light_direction.y/light_direction_length;
            light_direction.z = light_direction.z/light_direction_length;
            Ray ray1(pointLights[i]->position,light_direction);

            //check if the object is in shadow
            double tmin = INF,t3;
            for(int j=0; j<objects.size(); j++)
            {
                double *color1 = new double[3];
                color1[0] = 0;
                color1[1] = 0;
                color1[2] = 0;
                t3 = objects[j]->intersect(ray1,color1,0);
                if(t3<tmin&&t3>0)
                {
                    tmin = t3;
                }
            }
            struct point Tmin_point;
            Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
            Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
            Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
            //distance betn tmin_point and light
            double Tmin_dist = sqrt(pow(Tmin_point.x-spotLights[i]->position.x,2)+pow(Tmin_point.y-spotLights[i]->position.y,2)+pow(Tmin_point.z-spotLights[i]->position.z,2));
            //distance betn intersection_point and light
            double intersection_dist = sqrt(pow(intersection_point.x-spotLights[i]->position.x,2)+pow(intersection_point.y-spotLights[i]->position.y,2)+pow(intersection_point.z-spotLights[i]->position.z,2));
            if(intersection_dist-0.0000001>Tmin_dist)
            {
                continue;
            }
            //calculate lambertValue using normal, rayl
            double lambertValue = 0;
            lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
            //calculate reflected ray
            struct point reflected_ray_direction;
            //2*(L.N)*N-L

            reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
            reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
            reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
            Ray reflected_ray(intersection_point,reflected_ray_direction);

            //calculate phongValue using normal, rayl, rayr
            double phongValue = 0;
            phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
            //calculate the color
            color[0] = color[0] + this->coEfficients[1]*spotLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*spotLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
            color[1] = color[1] + this->coEfficients[1]*spotLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*spotLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
            color[2] = color[2] + this->coEfficients[1]*spotLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*spotLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


        }
        if(l>recursion_level)
            return t_min;
        struct point reflection_dir;
        //L.N
        double dot = r.direction.x*normal.x+r.direction.y*normal.y+r.direction.z*normal.z;
        reflection_dir.x=r.direction.x-normal.x*2*dot;
        reflection_dir.y=r.direction.y-normal.y*2*dot;
        reflection_dir.z=r.direction.z-normal.z*2*dot;

        //moving the point a little forward to avoid self-intersection
        struct point reflection_point;
        reflection_point.x = intersection_point.x + reflection_dir.x*0.00001;
        reflection_point.y = intersection_point.y + reflection_dir.y*0.00001;
        reflection_point.z = intersection_point.z + reflection_dir.z*0.00001;

        Ray reflection_ray(reflection_point,reflection_dir);
        double *dummycolor =new double[3];
        dummycolor[0]=0;
        dummycolor[1]=0;
        dummycolor[2]=0;
        double tMin = INF;
        int nearest = -1;
        for(int k=0; k<objects.size(); k++)
        {
            double t=objects[k]->intersect(reflection_ray,color,0);

            if(t<tMin&&t>0)
            {
                tMin=t;
                nearest=k;
            }
        }
        double *reflectedcolor =new double[3];
        reflectedcolor[0]=0;
        reflectedcolor[1]=0;
        reflectedcolor[2]=0;
        if(nearest!=-1)
        {

            tMin=objects[nearest]->intersect(reflection_ray,reflectedcolor,l+1);
        }
        color[0]=min(color[0]+this->coEfficients[3]*reflectedcolor[0],1.0);
        color[1]=min(color[1]+this->coEfficients[3]*reflectedcolor[1],1.0);
        color[2]=min(color[2]+this->coEfficients[3]*reflectedcolor[2],1.0);
        return t_min;
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
        this->coEfficients=new double[3];
        this->coEfficients[0] = 0.5;
        this->coEfficients[1] = 0.5;
        this->coEfficients[2] = 0.5;
        this->coEfficients[3] = 0.5;

        this->shine = 4.0;
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
        double t_min=INF;
        struct point normal;
        normal.x=0;
        normal.y=0;
        normal.z=1;

        //if ray origin and normal dot product negative, reverse normal direction
        if(r.origin.x*normal.x+r.origin.y*normal.y+r.origin.z*normal.z<0)
        {
            normal.x=-normal.x;
            normal.y=-normal.y;
            normal.z=-normal.z;
        }
        if(r.direction.x*normal.x+r.direction.y*normal.y+r.direction.z*normal.z!=0.0)
            t_min=(-1.0)*(normal.x*r.origin.x+normal.y*r.origin.y+normal.z*r.origin.z)/(normal.x*r.direction.x+normal.y*r.direction.y+normal.z*r.direction.z);
        struct point intersect;
        intersect.x=r.origin.x+t_min*r.direction.x;
        intersect.y=r.origin.y+t_min*r.direction.y;
        intersect.z=r.origin.z+t_min*r.direction.z;
        if(intersect.x<-this->width||intersect.x>this->width||intersect.y<-this->length||intersect.y>this->length)

        {
            t_min=INF;
        }


        if(l==0)
            return t_min;
        if(t_min!=INF&&t_min>0)
        {
            double i,j;
            i=intersect.x+this->width;
            i=floor(i/this->tile_size);
            j=intersect.y+this->length;
            j=floor(j/this->tile_size);
            color[0] = (int)(i+j)%2*0.3;
            color[1] = (int)(i+j)%2*0.3;
            color[2] = (int)(i+j)%2*0.3;
            struct point intersection_point;
            intersection_point.x=intersect.x;
            intersection_point.y=intersect.y;
            intersection_point.z=intersect.z;

            for(int i=0; i<pointLights.size(); i++)
            {



                struct point light_direction;
                light_direction.x = intersection_point.x - pointLights[i]->position.x;
                light_direction.y = intersection_point.y - pointLights[i]->position.y;
                light_direction.z = intersection_point.z - pointLights[i]->position.z;

                //normalize light_direction
                double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
                light_direction.x = light_direction.x/light_direction_length;
                light_direction.y = light_direction.y/light_direction_length;
                light_direction.z = light_direction.z/light_direction_length;
                Ray ray1(pointLights[i]->position,light_direction);

                //check if the object is in shadow
                double tmin = INF,t3;
                for(int j=0; j<objects.size(); j++)
                {
                    //cout<<"object"<<endl;
                    double *color1 = new double[3];
                    color1[0] = 0;
                    color1[1] = 0;
                    color1[2] = 0;
                    t3 = objects[j]->intersect(ray1,color1,0);
                    if(t3<tmin&&t3>0)
                    {
                        tmin = t3;
                    }
                }
                struct point Tmin_point;
                Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
                Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
                Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
                //distance betn tmin_point and light
                double Tmin_dist = sqrt(pow(Tmin_point.x-pointLights[i]->position.x,2)+pow(Tmin_point.y-pointLights[i]->position.y,2)+pow(Tmin_point.z-pointLights[i]->position.z,2));
                //distance betn intersection_point and light
                double intersection_dist = sqrt(pow(intersection_point.x-pointLights[i]->position.x,2)+pow(intersection_point.y-pointLights[i]->position.y,2)+pow(intersection_point.z-pointLights[i]->position.z,2));
                if(intersection_dist-0.0000001>Tmin_dist)
                {
                    continue;
                }

                //calculate lambertValue using normal, rayl
                double lambertValue = 0;
                lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
                //calculate reflected ray
                struct point reflected_ray_direction;
                //2*(L.N)*N-L

                reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
                reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
                reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
                Ray reflected_ray(intersection_point,reflected_ray_direction);

                //calculate phongValue using normal, rayl, rayr
                double phongValue = 0;
                phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);

                //calculate the color
                color[0] = color[0] + this->coEfficients[1]*pointLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*pointLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
                color[1] = color[1] + this->coEfficients[1]*pointLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*pointLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
                color[2] = color[2] + this->coEfficients[1]*pointLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*pointLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


            }
            for(int i=0; i<spotLights.size(); i++)
            {


                struct point light_direction;
                light_direction.x = intersection_point.x - spotLights[i]->position.x;
                light_direction.y = intersection_point.y - spotLights[i]->position.y;
                light_direction.z = intersection_point.z - spotLights[i]->position.z;
                //if spotlight angle > cutoff angle, ignore the light
                if(acos(spotLights[i]->direction.x*light_direction.x+spotLights[i]->direction.y*light_direction.y+spotLights[i]->direction.z*light_direction.z)>spotLights[i]->angle*pi/(180.0))
                {
                    continue;
                }

                //normalize light_direction
                double light_direction_length = sqrt(pow(light_direction.x,2)+pow(light_direction.y,2)+pow(light_direction.z,2));
                light_direction.x = light_direction.x/light_direction_length;
                light_direction.y = light_direction.y/light_direction_length;
                light_direction.z = light_direction.z/light_direction_length;
                Ray ray1(pointLights[i]->position,light_direction);

                //check if the object is in shadow
                double tmin = INF,t3;
                for(int j=0; j<objects.size(); j++)
                {
                    double *color1 = new double[3];
                    color1[0] = 0;
                    color1[1] = 0;
                    color1[2] = 0;
                    t3 = objects[j]->intersect(ray1,color1,0);
                    if(t3<tmin&&t3>0)
                    {
                        tmin = t3;
                    }
                }
                struct point Tmin_point;
                Tmin_point.x = ray1.origin.x + tmin*ray1.direction.x;
                Tmin_point.y = ray1.origin.y + tmin*ray1.direction.y;
                Tmin_point.z = ray1.origin.z + tmin*ray1.direction.z;
                //distance betn tmin_point and light
                double Tmin_dist = sqrt(pow(Tmin_point.x-spotLights[i]->position.x,2)+pow(Tmin_point.y-spotLights[i]->position.y,2)+pow(Tmin_point.z-spotLights[i]->position.z,2));
                //distance betn intersection_point and light
                double intersection_dist = sqrt(pow(intersection_point.x-spotLights[i]->position.x,2)+pow(intersection_point.y-spotLights[i]->position.y,2)+pow(intersection_point.z-spotLights[i]->position.z,2));
                if(intersection_dist-0.0000001>Tmin_dist)
                {
                    continue;
                }
                //calculate lambertValue using normal, rayl
                double lambertValue = 0;
                lambertValue=-(normal.x*light_direction.x+normal.y*light_direction.y+normal.z*light_direction.z);
                //calculate reflected ray
                struct point reflected_ray_direction;
                //2*(L.N)*N-L

                reflected_ray_direction.x = 2*normal.x*lambertValue+light_direction.x;
                reflected_ray_direction.y = 2*(normal.y*lambertValue)+light_direction.y;
                reflected_ray_direction.z = 2*(normal.z*lambertValue)+light_direction.z;
                Ray reflected_ray(intersection_point,reflected_ray_direction);

                //calculate phongValue using normal, rayl, rayr
                double phongValue = 0;
                phongValue=-(r.direction.x*reflected_ray_direction.x+r.direction.y*reflected_ray_direction.y+r.direction.z*reflected_ray_direction.z);
                //calculate the color
                color[0] = color[0] + this->coEfficients[1]*spotLights[i]->color[0]*max(lambertValue,0.0)*color[0] + this->coEfficients[2]*spotLights[i]->color[0]*pow(max(phongValue, 0.0), this->shine)*color[0];
                color[1] = color[1] + this->coEfficients[1]*spotLights[i]->color[1]*max(lambertValue,0.0)*color[1] + this->coEfficients[2]*spotLights[i]->color[1]*pow(max(phongValue, 0.0), this->shine)*color[1];
                color[2] = color[2] + this->coEfficients[1]*spotLights[i]->color[2]*max(lambertValue,0.0)*color[2] + this->coEfficients[2]*spotLights[i]->color[2]*pow(max(phongValue, 0.0), this->shine)*color[2];


            }
            if(l>recursion_level)
                return t_min;
            struct point reflection_dir;
            //L.N
            double dot = r.direction.x*normal.x+r.direction.y*normal.y+r.direction.z*normal.z;
            reflection_dir.x=r.direction.x-normal.x*2*dot;
            reflection_dir.y=r.direction.y-normal.y*2*dot;
            reflection_dir.z=r.direction.z-normal.z*2*dot;

            //moving the point a little forward to avoid self-intersection
            struct point reflection_point;
            reflection_point.x = intersection_point.x + reflection_dir.x*0.00001;
            reflection_point.y = intersection_point.y + reflection_dir.y*0.00001;
            reflection_point.z = intersection_point.z + reflection_dir.z*0.00001;

            Ray reflection_ray(reflection_point,reflection_dir);
            double *dummycolor =new double[3];
            dummycolor[0]=0;
            dummycolor[1]=0;
            dummycolor[2]=0;
            double tMin = INF;
            int nearest = -1;
            for(int k=0; k<objects.size(); k++)
            {
                double t=objects[k]->intersect(reflection_ray,color,0);

                if(t<tMin&&t>0)
                {
                    tMin=t;
                    nearest=k;
                }
            }
            double *reflectedcolor =new double[3];
            reflectedcolor[0]=0;
            reflectedcolor[1]=0;
            reflectedcolor[2]=0;
            if(nearest!=-1)
            {

                tMin=objects[nearest]->intersect(reflection_ray,reflectedcolor,l+1);
            }
            color[0]=min(color[0]+this->coEfficients[3]*reflectedcolor[0],1.0);
            color[1]=min(color[1]+this->coEfficients[3]*reflectedcolor[1],1.0);
            color[2]=min(color[2]+this->coEfficients[3]*reflectedcolor[2],1.0);

        }


        return t_min;

    }



};





