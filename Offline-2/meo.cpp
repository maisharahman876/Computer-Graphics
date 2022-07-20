#include<bits/stdc++.h>
#include <fstream>
#define pi 3.141592653
#define axisX 0
#define axisY 1
#define axisZ 2
#define inf 99999999
#include "bitmap_image.hpp"
using namespace std;
double maximum(double a, double b, double c)
{
    if(a>=b&&a>=c)
    {
        return a;
    }
    else if(b>=a&&b>=c)
        return b;
    else
        return c;
}
double minimum(double a, double b, double c)
{
    if(a<=b&&a<=c)
    {
        return a;
    }
    else if(b<=a&&b<=c)
        return b;
    else
        return c;
}
double** mulMat(double **mat1, double **mat2,int R1,int R2,int C1,int C2)
{
    double **rslt= new double*[R1];
    for(int i = 0; i < R1; i++)
    {
        rslt[i] = new double[C2];
    }
    // printf("Multiplication of given two matrices is:\n\n");

    for (int i = 0; i < R1; i++)
    {
        for (int j = 0; j < C2; j++)
        {
            rslt[i][j] = 0.0;

            for (int k = 0; k < R2; k++)
            {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }

            //cout<<rslt[i][j]<<" ";
        }
        // cout<<endl;


    }
    /*for(int i=0;i<4;i++)
    {
        delete mat1[i];
        delete mat2[i];

    }
    delete mat1;
    delete mat2;*/
    return rslt;
}
class point
{
public:
    double x,y,z;
    point()
    {
        this->x=0.0;
        this->y=0.0;
        this->z=0.0;

    }
    point(double x,double y, double z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void printPoint()
    {
        cout<<x<<" "<<y<<" "<<z<<endl;
    }

};
class Triangle
{
public :
    point p1;
    point p2;
    point p3;
    int r,g,b;
    void print()
    {
        p1.printPoint();
        p2.printPoint();
        p3.printPoint();
    }

};
point Rodrigues(point xp, point a, double t)
{
    //a.x
    double m=xp.x*a.x+xp.y*a.y+xp.z*a.z;
    //cout<<"m="<<m<<endl;
    point n;

    /* M = m1 * i + m2 * j + m3 * k

     N = n1 * i + n2 * j + n3 * k.

     So, cross product = (m2 * n3 – m3 * n2) * i - (m1 * n3 – m3 * n1) * j + (m1 * n2 – m2 * n1) * k*/
    n.x=    a.y*xp.z-a.z*xp.y;
    n.y= -a.x*xp.z+a.z*xp.x;
    n.z=a.x*xp.y-a.y*xp.x;
    //cout<<sin(t*pi/180)<<"the aXx is"<<endl;
    //xp.printPoint();
    //result
    xp.x=cos(t*pi/180)*xp.x + (1-cos(t*pi/180))*m*a.x + sin(t*pi/180)*n.x;
    xp.y=cos(t*pi/180)*xp.y + (1-cos(t*pi/180))*m*a.y + sin(t*pi/180)*n.y;
    xp.z=cos(t*pi/180)*xp.z + (1-cos(t*pi/180))*m*a.z + sin(t*pi/180)*n.z;

    return xp;

}
double intersect(point p1,point p2,int axis1,double val,int axis2)
{
    double m=0.0;
    if(axis1==axisY&&axis2==axisX)
    {
        if((p2.y-p1.y)==0)
            m=inf;
        else
            m=(val-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;

    }
    else if (axis1==axisY&&axis2==axisZ)
    {
        if((p2.y-p1.y)==0)
            m=inf;
        else
            m=(val-p1.y)*(p2.z-p1.z)/(p2.y-p1.y)+p1.z;

    }
    else if (axis1==axisX&&axis2==axisZ)
    {
        if((p2.x-p1.x)==0)
            m=inf;
        else
            m=(val-p1.x)*(p2.z-p1.z)/(p2.x-p1.x)+p1.z;


    }
    return m;
}
class space
{
public:

    stack<double**> stack;
    double** current;
    double** V;
    double** P;
    ofstream st1,st2,st3,st4;
    vector<Triangle> objects;
    double** z_buffer;
    space()
    {

        double** top;

        top= new double*[4];
        current= new double*[4];
        for(int i = 0; i < 4; i++)
        {
            top[i] = new double[4];
            current[i] = new double[4];
            for(int j = 0; j < 4; j++)
            {
                if(i==j)
                    top[i][j]=1.0;
                else
                    top[i][j]=0.0;
                current[i][j]=top[i][j];
            }
        }
        stack.push(top);
        srand(time(0));
        //calculateView();

        st1.open("stage1.txt",ios::out);
        st2.open("stage2.txt",ios::out);
        st3.open("stage3.txt",ios::out);
        st4.open("z_buffer.txt",ios::out);
    }
    void print();
    void drawTriangle(point,point,point);
    double** draw(point);
    void push();
    void pop();
    void translate(double,double,double);
    void scale(double,double,double);
    void rotation(double,double,double,double);
    void calculateView(point eye,point look,point up);
    void calculatePerspective(double,double,double,double);
    void z_bufferAlgo(int,int,double,double,double,double);
    ~space()
    {
        for(int i = 0; i < 4; i++)
        {
            delete current[i];
            delete V[i];
            delete P[i];

        }
        delete current;
        delete V;
        delete P;
        st1.close();
        st2.close();
        st3.close();
    }

};

double** space::draw(point p)
{


    double **mul= new double*[4];
    for(int i = 0; i < 4; i++)
    {
        mul[i] = new double[1];
        mul[i][0]=0;
    }
    mul[0][0]=p.x;
    mul[1][0]=p.y;
    mul[2][0]=p.z;
    mul[3][0]=1.0;
    /*cout<<"the matrix baby:"<<endl;
    for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                cout<<current[i][jF]<<" ";
            }
            cout<<endl;
        }
    */
    double** p1=mulMat(current,mul,4,4,4,1);
    for(int i = 0; i < 3; i++)
    {

        st1 << std::setprecision(7) << std::fixed;
        st1<<(double)(p1[i][0]/p1[3][0])<<" ";

    }
    double** p2=mulMat(V,p1,4,4,4,1);
    for(int i = 0; i < 3; i++)
    {

        st2 << std::setprecision(7) << std::fixed;
        st2<<(double)(p2[i][0]/p2[3][0])<<" ";

    }
    double** p3=mulMat(P,p2,4,4,4,1);
    for(int i = 0; i < 3; i++)
    {

        st3  << std::setprecision(7) << std::fixed;
        st3<<(double)(p3[i][0]/p3[3][0])<<" ";

    }
    st1<<endl;
    st2<<endl;
    st3<<endl;
    cout<<endl;
    return p3;


}
void space::drawTriangle(point p1,point p2,point p3)
{
    double** d1=draw(p1);
    double** d2=draw(p2);
    double** d3=draw(p3);

    cout<<endl;
    point n_p1(d1[0][0]/d1[3][0],d1[1][0]/d1[3][0],d1[2][0]/d1[3][0]),n_p2(d2[0][0]/d2[3][0],d2[1][0]/d2[3][0],d2[2][0]/d2[3][0]),n_p3(d3[0][0]/d3[3][0],d3[1][0]/d3[3][0],d3[2][0]/d3[3][0]);
    Triangle t;
    t.p1=n_p1;
    t.p2=n_p2;
    t.p3=n_p3;

    t.r=rand()%255;
    t.g=rand()%255;
    t.b=rand()%255;
    objects.push_back(t);
    t.print();
    cout<<endl;
    st1<<endl;
    st2<<endl;
    st3<<endl;

}
void space::push()
{
    double** top;
    top= new double*[4];

    for(int i = 0; i < 4; i++)
    {
        top[i] = new double[4];

        for(int j = 0; j < 4; j++)
        {

            top[i][j]=current[i][j];
        }
    }
    stack.push(top);
}
void space::pop()
{
    for(int i = 0; i < 4; i++)
    {
        delete current[i];
    }
    delete current;
    current=stack.top();
    stack.pop();
}
void space::translate(double tx,double ty,double tz)
{
    double** mat;
    mat= new double*[4];

    for(int i = 0; i < 4; i++)
    {
        mat[i] = new double[4];

        for(int j = 0; j < 4; j++)
        {
            if(i==j)

                mat[i][j]=1.0;
            else
                mat[i][j]=0.0;
        }
    }
    mat[0][3]=tx;
    mat[1][3]=ty;
    mat[2][3]=tz;
    current=mulMat(current,mat,4,4,4,4);

}
void space::scale(double sx,double sy,double sz)
{
    double** mat;
    mat= new double*[4];

    for(int i = 0; i < 4; i++)
    {
        mat[i] = new double[4];

        for(int j = 0; j < 4; j++)
        {
            if(i==j)

                mat[i][j]=1.0;
            else
                mat[i][j]=0.0;
        }
    }
    mat[0][0]=sx;
    mat[1][1]=sy;
    mat[2][2]=sz;
    current=mulMat(current,mat,4,4,4,4);

}
void space::rotation(double angle,double ax,double ay,double az)
{
    double** mat;
    mat= new double*[4];

    for(int i = 0; i < 4; i++)
    {
        mat[i] = new double[4];

        for(int j = 0; j < 4; j++)
        {
            if(i==j)

                mat[i][j]=1.0;
            else
                mat[i][j]=0.0;
        }
    }
    //normalize
    double r=sqrt(ax*ax+ay*ay+az*az);
    ax=ax/r;
    ay=ay/r;
    az=az/r;
    point a(ax,ay,az);
    //calculating
    point c1,c2,c3;
    point vi(1,0,0),vj(0,1,0),vk(0,0,1);
    c1=Rodrigues(vi,a,angle);
    c2=Rodrigues(vj,a,angle);
    c3=Rodrigues(vk,a,angle);
    mat[0][0]=c1.x,mat[0][1]=c2.x,mat[0][2]=c3.x;
    mat[1][0]=c1.y,mat[1][1]=c2.y,mat[1][2]=c3.y;
    mat[2][0]=c1.z,mat[2][1]=c2.z,mat[2][2]=c3.z;
    /*for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cout<<mat[i][j]<<" ";
        }
        cout<<endl;
    }*/
    current=mulMat(current,mat,4,4,4,4);

}
void space::calculateView(point eye,point look,point up)
{

    double** T;
    double** R;
    R= new double*[4];
    T= new double*[4];
    for(int i = 0; i < 4; i++)
    {
        R[i] = new double[4];
        T[i] = new double[4];
        for(int j=0; j<4; j++)
        {
            if(i==j)
            {
                R[i][j]=1;
                T[i][j]=1;
            }
            else
            {
                R[i][j]=0;
                T[i][j]=0;
            }
        }
    }
    point l,r,u;

    //calculation of lru
    l.x=look.x-eye.x;
    l.y=look.y-eye.y;
    l.z=look.z-eye.z;
    double m=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
    l.x=l.x/m;
    l.y=l.y/m;
    l.z=l.z/m;

    r.x= l.y*up.z-l.z*up.y;
    r.y=-l.x*up.z+l.z*up.x;
    r.z= l.x*up.y-l.y*up.x;
    m=sqrt(r.x*r.x+r.y*r.y+r.z*r.z);
    r.x=r.x/m;
    r.y=r.y/m;
    r.z=r.z/m;

    u.x= r.y*l.z-r.z*l.y;
    u.y=-r.x*l.z+r.z*l.x;
    u.z= r.x*l.y-r.y*l.x;


    //populate R T matrices and calculate V
    T[0][3]=-eye.x;
    T[1][3]=-eye.y;
    T[2][3]=-eye.z;

    R[0][0]=r.x,R[0][1]=r.y,R[0][2]=r.z;
    R[1][0]=u.x,R[1][1]=u.y,R[1][2]=u.z;
    R[2][0]=-l.x,R[2][1]=-l.y,R[2][2]=-l.z;

    V= mulMat(R,T,4,4,4,4);
}
void space::print()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cout<<current[i][j]<<" ";
        }
        cout<<endl;
    }
}
void space::calculatePerspective(double fovY,double aspectRatio,double near,double far)
{

    double fovX=fovY*aspectRatio;
    double t=near*tan(fovY*pi/360);
    double r=near*tan(fovX*pi/360);
    P= new double*[4];
    for(int i = 0; i < 4; i++)
    {
        P[i] = new double[4];
        for(int j=0; j<4; j++)
            P[i][j]=0;
    }
    P[0][0]=near/r;
    P[1][1]=near/t;
    P[2][2]=-(far+near)/(far-near);
    P[2][3]=-(2*far*near)/(far-near);
    P[3][2]=-1;


}
void space::z_bufferAlgo(int w,int h,double lx, double ly, double min_z,double max_z)
{
    z_buffer= new double*[h];
    double dx,dy,top_y,left_x;
    bitmap_image image(w,h);
    dx=-2*lx/w;
    dy=-2*ly/h;
    top_y=-ly-dy/2;
    left_x=lx+dx/2;
    for(int i=0; i<h; i++)
    {
        z_buffer[i]=new double[w];
        for(int j=0; j<w; j++)
        {
            z_buffer[i][j]=max_z;
        }
    }

    for(int i=0; i<objects.size(); i++)
    {

        Triangle t=objects[i];

        double max_y=maximum(t.p1.y,t.p2.y,t.p3.y);
        double min_y=minimum(t.p1.y,t.p2.y,t.p3.y);




        max_y=max_y>top_y?top_y:max_y;
        min_y=min_y<-top_y?-top_y:min_y;
        int  top_scanline=int((top_y-max_y)/dy);
        int bottom_scanline=int((top_y-min_y)/dy);
        //cout<<"Scanliness----->"<<endl;
        //cout<<top_scanline<<" "<<bottom_scanline<<endl;
        for (int r=top_scanline; r<=bottom_scanline; r++)
        {
            double y=top_y-r*dy;

            point top_point,left_p,right_p;
            if(t.p1.y>=y&&t.p2.y<y&&t.p3.y<y)
            {
               top_point=t.p1;
                if(t.p2.x>t.p3.x)
                {
                    right_p=t.p2;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p3;
                }
            }
            else if(t.p1.y>y&&t.p2.y<=y&&t.p3.y<=y)
            {
                top_point=t.p1;
                if(t.p2.x>t.p3.x)
                {
                    right_p=t.p2;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p3;
                }

            }
            else if(t.p2.y>=y&&t.p3.y<y&&t.p1.y<y)
            {
                top_point=t.p2;
                if(t.p1.x>t.p3.x)
                {
                    right_p=t.p1;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p1;
                    right_p=t.p3;
                }

            }
            else if(t.p2.y>y&&t.p1.y<=y&&t.p3.y<=y)
            {
                top_point=t.p2;
                if(t.p1.x>t.p3.x)
                {
                    right_p=t.p1;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p1;
                    right_p=t.p3;
                }

            }
            else if(t.p3.y>=y&&t.p2.y<y&&t.p1.y<y)
            {
                top_point=t.p3;
                if(t.p2.x>t.p1.x)
                {
                    right_p=t.p2;
                    left_p=t.p1;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p1;
                }

            }
            else if(t.p3.y>y&&t.p2.y<=y&&t.p1.y<=y)
            {
                top_point=t.p3;
                if(t.p2.x>t.p1.x)
                {
                    right_p=t.p2;
                    left_p=t.p1;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p1;
                }

            }
             else if(t.p1.y<=y&&t.p2.y>y&&t.p3.y>y)
            {
                top_point=t.p1;
                if(t.p2.x>t.p3.x)
                {
                    right_p=t.p2;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p3;
                }

            }
            else if(t.p1.y<y&&t.p2.y>=y&&t.p3.y>=y)
            {
                top_point=t.p1;
                if(t.p2.x>t.p3.x)
                {
                    right_p=t.p2;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p3;
                }

            }
            else if(t.p2.y<=y&&t.p3.y>y&&t.p1.y>y)
            {
                top_point=t.p2;
                if(t.p1.x>t.p3.x)
                {
                    right_p=t.p1;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p1;
                    right_p=t.p3;
                }

            }
            else if(t.p2.y<y&&t.p1.y>=y&&t.p3.y>=y)
            {
                top_point=t.p2;
                if(t.p1.x>t.p3.x)
                {
                    right_p=t.p1;
                    left_p=t.p3;
                }
                else
                {
                    left_p=t.p1;
                    right_p=t.p3;
                }

            }
            else if(t.p3.y<=y&&t.p2.y>y&&t.p1.y>y)
            {
                top_point=t.p3;
                if(t.p2.x>t.p1.x)
                {
                    right_p=t.p2;
                    left_p=t.p1;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p1;
                }

            }
            else if(t.p3.y<y&&t.p2.y>=y&&t.p1.y>=y)
            {
                top_point=t.p3;
                if(t.p2.x>t.p1.x)
                {
                    right_p=t.p2;
                    left_p=t.p1;
                }
                else
                {
                    left_p=t.p2;
                    right_p=t.p1;
                }

            }
            else
            {
               // continue;
            }




            double m1,m2,m3;
            m1= intersect(top_point,left_p,axisY,y,axisX);
            m2=intersect(top_point,right_p,axisY,y,axisX);



            double max_x=0.0;
            double min_x=0.0;
            double zp,c,za,zb,xa,xb;
            if(m1==inf||m2==inf)
            {
                cout<<"y=="<<y<<endl;
                 cout<<"Infinity ashe m1 m2 Triangle no :"<<i<<endl;

                continue;
            }

                za=intersect(top_point,left_p,axisY,y,axisZ);
                xa=min_x=m1;

                zb=intersect(top_point,right_p,axisY,y,axisZ);
                xb=max_x=m2;
                if(m1<left_x)
                {
                    zp=intersect(top_point,left_p,axisX,left_x,axisZ);
                    min_x=left_x;
                }
                else
                    zp=za;


            if(max_x>-left_x)
                max_x=-left_x;

            c=(zb-za)*dx/(xb-xa);
            if(zb==inf||za==inf)
            {
                cout<<"Infinity ashe za zb"<<endl;
                continue;

            }


            int left_scanline=int((min_x-left_x)/dx);
            int right_scanline=int((max_x-left_x)/dx);

            for(int col=left_scanline; col<=right_scanline; col++)
            {
                if(zp<max_z&&zp>min_z)
                {
                    if(zp<z_buffer[r][col])
                    {
                        z_buffer[r][col]=zp;
                        image.set_pixel(col,r,t.r,t.g,t.b);

                    }
                }
                zp+=c;


            }

        }

    }
    for(int i=0; i<h; i++)
    {
       for(int j=0; j<w; j++)
        {

            if(z_buffer[i][j]<max_z)
            {
                st4 << std::setprecision(6) << std::fixed;
                st4<<z_buffer[i][j]<<" ";
            }

        }
        st4<<endl;
    }

    image.save_image("out.bmp");

}
int main()
{
    point eye,look,up;
    double fovY,aspectRatio,near,far,limit_x,limit_y,front_limit,rear_limit;
    int screen_width,screen_height;

    ofstream file;
    double n;
    ifstream config;



    freopen("scene.txt","r",stdin);
    config.open("config.txt",ios::in);

    cin>>eye.x>>eye.y>>eye.z;
    cin>>look.x>>look.y>>look.z;
    cin>>up.x>>up.y>>up.z;
    cin>>fovY>>aspectRatio>>near>>far;
    config>>screen_width>>screen_height;
    config>>limit_x>>limit_y>>front_limit>>rear_limit;

    space s;
    s.calculateView(eye,look,up);
    s.calculatePerspective(fovY,aspectRatio,near,far);

    /* eye.printPoint();
     look.printPoint();
     up.printPoint();
     for(int i=0;i<4;i++)
         cout<<fovY[i];

     */

    while(1)
    {
        string input;
        cin>>input;
        cout<<input<<endl;
        point p1,p2,p3;
        if(input=="triangle")
        {
            //cout<<"ashe"<<endl;
            cin>>p1.x>>p1.y>>p1.z;
            cin>>p2.x>>p2.y>>p2.z;
            cin>>p3.x>>p3.y>>p3.z;
            s.drawTriangle(p1,p2,p3);



        }
        else if(input=="push")
        {
            s.push();
        }
        else if(input=="pop")
        {
            s.pop();
        }
        else if(input=="translate")
        {
            double tx,ty,tz;
            cin>>tx>>ty>>tz;
            s.translate(tx,ty,tz);
        }
        else if(input=="scale")
        {
            double tx,ty,tz;
            cin>>tx>>ty>>tz;
            s.scale(tx,ty,tz);;
        }
        else if(input=="rotate")
        {
            double angle,tx,ty,tz;
            cin>>angle>>tx>>ty>>tz;
            s.rotation(angle,tx,ty,tz);
        }
        else if(input=="end")
        {
            break;
        }
    }
    s.z_bufferAlgo(screen_width,screen_height,limit_x,limit_y,front_limit,rear_limit);
    file.close();
    config.close();
    return 0;
}
