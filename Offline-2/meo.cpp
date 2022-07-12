#include<bits/stdc++.h>
#include <fstream>

using namespace std;

double** mulMat(double **mat1, double **mat2,int R1,int R2,int C1,int C2) {
    double **rslt= new double*[4];
    for(int i = 0; i < R1; i++)
        {
            rslt[i] = new double[C2];
        }
   // printf("Multiplication of given two matrices is:\n\n");

    for (int i = 0; i < R1; i++) {
        for (int j = 0; j < C2; j++) {
            rslt[i][j] = 0.0;

            for (int k = 0; k < R2; k++) {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }

            //cout<<rslt[i][j]<<" ";
        }


    }
    return rslt;
}
class point{
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
class space
{
public:

    stack<double**> stack;
    double** current;
    space()
    {
        double** top,current;
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
    }
    void print();
    void drawTriangle(point,point,point);
    void draw(point);
    void push();
    void pop();
    void translate(double,double,double);
    void scale(double,double,double);
    void rotation(double,double,double,double);
};

void space::draw(point p)
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

    double** p1=mulMat(current,mul,4,4,4,1);
    for(int i = 0; i < 3; i++)
        {

                cout<<(double)(p1[i][0]/p1[3][0])<<" ";

        }
        cout<<endl;

}
void space::drawTriangle(point p1,point p2,point p3)
{
    draw(p1);
    draw(p2);
    draw(p3);
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
    current=stack.pop();
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

        current=mulMat(current,mat,4,4,4,4);

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

int main()
{
    point eye,look,up;
    double *fovY=new double[4];
    for(int i=0;i<4;i++)
        fovY[i]=0;
    ofstream file;
    double n;

    space s;

    file.open("output.txt",ios::out);
    freopen("scene.txt","r",stdin);

    cin>>eye.x>>eye.y>>eye.z;
    cin>>look.x>>look.y>>look.z;
    cin>>up.x>>up.y>>up.z;
    cin>>fovY[0]>>fovY[1]>>fovY[2]>>fovY[3];

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
    }
    file.close();
}
