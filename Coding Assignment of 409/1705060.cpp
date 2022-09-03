#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

class Line
{
public:
    int red, green, blue;
    int xstart, ystart, xend, yend;
    Line(int x1, int y1, int x2, int y2)
    {
        xstart = x1;
        ystart = y1;
        xend = x2;
        yend = y2;
    }
    void setColor(int r, int g, int b)
    {
        red = r;
        green = g;
        blue = b;
    }

};

void midPointLine(Line l, bitmap_image &image,int height)
{
    bool flip = false;
    int t;

    if(l.xstart>l.xend)
    {
        //swap l.xstart and l.xend
        int temp = l.xstart;
        l.xstart=l.xend;
        l.xend=temp;
        temp=l.ystart;
        l.ystart=l.yend;
        l.yend=temp;
    }
    int dx = l.xend - l.xstart;
    int dy = l.yend - l.ystart;
    if(dy<0)
    {
        dy = -dy;
        flip = true;
        t=-1;
    }
    else t=1;
    double slope;

    if(l.xend==l.xstart)
    {
        slope=999999;

    }
    else
    {
        slope=(double)(l.yend-l.ystart)/(l.xend-l.xstart);
    }
    if(slope<=-1)
    {
        l.yend=l.yend+2*(l.ystart-l.yend);
        flip=true;
        dy=l.yend-l.ystart;
        t=1;
    }
    int d,dE,dNE;
    int x,y;
    if(dy<=dx)
    {
        d = 2 * dy - dx;
        dE = 2 * dy;
        dNE = 2 * (dy - dx);
        x=l.xstart;
        y=l.ystart;

        while(x < l.xend)
        {
            //cout<<"BAL "<<d<<endl;
            if(d < 0)
            {
                d += dE;
                x++;
            }
            else
            {
                d += dNE;
                x++;
                y+=t;
            }
            //cout<<"x y yp p1 "<<x<<" "<<y<<" "<<endl;

            if(flip)
            {

                int yp=y-2*(y-l.ystart);
                // cout<<"x y yp p2 "<<x<<" "<<y<<" "<<yp<<endl;
                image.set_pixel(x,height-yp, l.red, l.green, l.blue);

            }
            else
                image.set_pixel(x,height-y, l.red, l.green, l.blue);

        }
    }
    else if(dx<dy)
    {
        d = 2 * dx - dy;
        dE = 2 * dx;
        dNE = 2 * (dx - dy);
        x=l.xstart;
        y=l.ystart;
        while(y<l.yend)
        {
            //cout<<"BAL "<<d<<endl;
            if(d < 0)
            {
                d += dE;
                y+=t;
            }
            else
            {
                d += dNE;
                x++;
                y+=t;
            }
            //cout<<"x y yp p1 "<<x<<" "<<y<<" "<<endl;

            if(flip)
            {

                int yp=y-2*(y-l.ystart);
                // cout<<"x y yp p2 "<<x<<" "<<y<<" "<<yp<<endl;
                image.set_pixel(x,height-yp, l.red, l.green, l.blue);

            }
            else
                image.set_pixel(x,height-y, l.red, l.green, l.blue);

        }
    }


}
void IntensifyPixel(int x, int y, double distance,bitmap_image &image,int height,Line l)
{
    double intensity = round(abs(distance));
    if(intensity>1)
    {
        intensity=1;
    }
    image.set_pixel(x,height-y,l.red/2+(intensity)*l.red/2,l.green/2+(intensity)*l.green/2,l.blue/2+(intensity)*l.blue/2);
}
void weightedAntiAliasing(Line l,bitmap_image &image,int height)
{
    bool flip = false;
    int t;

    if(l.xstart>l.xend)
    {
        //swap l.xstart and l.xend
        int temp = l.xstart;
        l.xstart=l.xend;
        l.xend=temp;
        temp=l.ystart;
        l.ystart=l.yend;
        l.yend=temp;
    }
    int dx = l.xend - l.xstart;
    int dy = l.yend - l.ystart;
    if(dy<0)
    {
        dy = -dy;
        flip = true;
        t=-1;
    }
    else t=1;
    double slope;

    if(l.xend==l.xstart)
    {
        slope=999999;

    }
    else
    {
        slope=(double)(l.yend-l.ystart)/(l.xend-l.xstart);
    }
    if(slope<=-1)
    {
        l.yend=l.yend+2*(l.ystart-l.yend);
        flip=true;
        dy=l.yend-l.ystart;
        t=1;
    }
    int two_v_dx = 0;
    double invDenom = 1.0/(2.0*sqrt(dx*dx+dy*dy));
    double two_dx_invDenom = 2.0*dx*invDenom;
    int two_v_dy = 0;
    // double invDenom = 1.0/(2.0*Math.sqrt(dx*dx+dy*dy));
    double two_dy_invDenom = 2.0*dy*invDenom;
    int d,dE,dNE;
    int x,y;
    if(dy<=dx)
    {
        d = 2 * dy - dx;
        dE = 2 * dy;
        dNE = 2 * (dy - dx);
        x=l.xstart;
        y=l.ystart;
        IntensifyPixel(x,y,0,image,height,l);                     // start pixel
        IntensifyPixel(x,y+1,two_dx_invDenom,image,height,l);       // neighbor above
        IntensifyPixel(x,y-1,two_dx_invDenom,image,height,l);       // neighbor below

        while(x < l.xend)
        {
            //cout<<"BAL "<<d<<endl;
            if(d < 0)
            {
                two_v_dx = d+dx;
                d += dE;

                x++;
            }
            else
            {
                two_v_dx = d-dx;
                d += dNE;
                x++;
                y+=t;
            }
            if(flip)
            {

                int yp=y-2*(y-l.ystart);
                // cout<<"x y yp p2 "<<x<<" "<<y<<" "<<yp<<endl;
                IntensifyPixel(x,yp,two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x,yp+1,two_dy_invDenom - two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x,yp-1,two_dy_invDenom + two_v_dy*invDenom,image,height,l);

            }
            else
            {
                IntensifyPixel(x,y,two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x,y+1,two_dy_invDenom - two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x,y-1,two_dy_invDenom + two_v_dy*invDenom,image,height,l);
            }
            // image.set_pixel(x,height-y, l.red, l.green, l.blue);

        }
    }
    else if(dx<dy)
    {
        d = 2 * dx - dy;
        dE = 2 * dx;
        dNE = 2 * (dx - dy);
        x=l.xstart;
        y=l.ystart;
        IntensifyPixel(x,y,0,image,height,l);                      // start pixel
        IntensifyPixel(x+1,y,two_dy_invDenom,image,height,l);     // neighbor left
        IntensifyPixel(x-1,y,two_dy_invDenom,image,height,l);     // neighbor right
        while(y<l.yend)
        {
            //cout<<"BAL "<<d<<endl;
            if(d < 0)
            {
                two_v_dy = d+dy;
                d += dE;
                y+=t;
            }
            else
            {
                two_v_dy = d-dy;
                d += dNE;
                x++;
                y+=t;
            }
            //cout<<"x y yp p1 "<<x<<" "<<y<<" "<<endl;

            if(flip)
            {

                int yp=y-2*(y-l.ystart);
                // cout<<"x y yp p2 "<<x<<" "<<y<<" "<<yp<<endl;
                IntensifyPixel(x,yp,two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x+1,yp,two_dy_invDenom - two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x-1,yp,two_dy_invDenom + two_v_dy*invDenom,image,height,l);

            }
            else
            {
                IntensifyPixel(x,y,two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x+1,y,two_dy_invDenom - two_v_dy*invDenom,image,height,l);
                IntensifyPixel(x-1,y,two_dy_invDenom + two_v_dy*invDenom,image,height,l);
            }

        }
    }

}

double getAreaCovered(int dx, int dy, int x, int y, int x1, int y1)
{

    double y_upper = y1 + 0.5;
    double y_lower = y1 - 0.5;

    double x_temp = x;
    double y_temp = y;
    double covered = 0.0;
    double step = 1.0 / 16;
    double intensity_increase = 1.0 / (16*16);
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            x_temp = x * 1.0 + step * (j + 1);
            y_temp = y * 1.0 + step * (i + 1);

            // getting d = 2*(ax+by+c) = 2*(dy*x - dx*y + dx*y1 - dy*x1) value:

            double upper_func_value = 2 * (dy * x_temp - dx * y_temp + dx * y_upper - dy * x1);
            double lower_func_value = 2 * (dy * x_temp - dx * y_temp + dx * y_lower - dy * x1);

            if (upper_func_value >= 0 && lower_func_value <= 0)
                // if (upper_func_value <= 0 && lower_func_value >= 0)

            {
                covered += intensity_increase;
            }
        }
    }

    return covered;
}
void setColor(int x,int y, double r, double g, double b,  bitmap_image &image)
{
    if(r>255)
        r=255;
    if(g>255)

        g=255;
    if(b>255)
        b=255;
    image.set_pixel(x,y,r,g,b);
}
void unweightedAntiAliasing(Line l,bitmap_image &image,int height)
{
    bool flip = false;
    int t;

    if(l.xstart>l.xend)
    {
        //swap l.xstart and l.xend
        int temp = l.xstart;
        l.xstart=l.xend;
        l.xend=temp;
        temp=l.ystart;
        l.ystart=l.yend;
        l.yend=temp;
    }
    int dx = l.xend - l.xstart;
    int dy = l.yend - l.ystart;
    if(dy<0)
    {
        dy = -dy;
        flip = true;
        t=-1;
    }
    else t=1;
    double slope;

    if(l.xend==l.xstart)
    {
        slope=999999;

    }
    else
    {
        slope=(double)(l.yend-l.ystart)/(l.xend-l.xstart);
    }
    if(slope<=-1)
    {
        l.yend=l.yend+2*(l.ystart-l.yend);
        flip=true;
        dy=l.yend-l.ystart;
        t=1;
    }
    int d,dE,dNE;
    int x,y;
    if(dy<=dx)
    {
        d = 2 * dy - dx;
        dE = 2 * dy;
        dNE = 2 * (dy - dx);
        x=l.xstart;
        y=l.ystart;

        while(x < l.xend)
        {
            //cout<<"BAL "<<d<<endl;
            if(d < 0)
            {
                d += dE;
                x++;
            }
            else
            {
                d += dNE;
                x++;
                y+=t;
            }
            //cout<<"x y yp p1 "<<x<<" "<<y<<" "<<endl;
            if(flip)
            {

                int yp=y-2*(y-l.ystart);
                // cout<<"x y yp p2 "<<x<<" "<<y<<" "<<yp<<endl;
                double intensity = getAreaCovered(dx,dy,x,yp,l.xend,l.yend);

                setColor(x,height-yp, l.red/2+(1-intensity)*l.red/2,  l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                 intensity = getAreaCovered(dx,dy,x,yp-1,l.xend,l.yend);
                setColor(x,height-yp+1,l.red/2+(1-intensity)*l.red/2,  l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                intensity = getAreaCovered(dx,dy,x,yp+1,l.xend,l.yend);
                setColor(x,height-yp-1,l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

            }
            else
            {

                double intensity = getAreaCovered(dx,dy,x,y,l.xend,l.yend);
                setColor(x,height-y, l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                 intensity = getAreaCovered(dx,dy,x,y-1,l.xend,l.yend);
                setColor(x,height-y+1, l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                intensity = getAreaCovered(dx,dy,x,y+1,l.xend,l.yend);
                setColor(x,height-y-1, l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

            }


        }
    }
    else if(dx<dy)
    {
        d = 2 * dx - dy;
        dE = 2 * dx;
        dNE = 2 * (dx - dy);
        x=l.xstart;
        y=l.ystart;

        while(y<l.yend)
        {
            //cout<<"BAL "<<d<<endl;
            if(d < 0)
            {
                d += dE;
                y+=t;
            }
            else
            {
                d += dNE;
                x++;
                y+=t;
            }
            //cout<<"x y yp p1 "<<x<<" "<<y<<" "<<endl;

            if(flip)
            {

                int yp=y-2*(y-l.ystart);
                // cout<<"x y yp p2 "<<x<<" "<<y<<" "<<yp<<endl;
                double intensity = getAreaCovered(dx,dy,x,yp,l.xend,l.yend);

                setColor(x,height-yp, l.red/2+(1-intensity)*l.red/2,  l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                 intensity = getAreaCovered(dx,dy,x,yp-1,l.xend,l.yend);
                setColor(x,height-yp+1,l.red/2+(1-intensity)*l.red/2,  l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                intensity = getAreaCovered(dx,dy,x,yp+1,l.xend,l.yend);
                setColor(x,height-yp-1,l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);
            }
            else
            {

                double intensity = getAreaCovered(dx,dy,x,y,l.xend,l.yend);
                setColor(x,height-y, l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                 intensity = getAreaCovered(dx,dy,x-1,y,l.xend,l.yend);
                setColor(x-1,height-y, l.red/2+(1-intensity)*l.red/2, l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

                intensity = getAreaCovered(dx,dy,x+1,y,l.xend,l.yend);
                setColor(x+1,height-y, l.red/2+(1-intensity)*l.red/2,  l.green/2+(1-intensity)*l.green/2,l.blue/2+(1-intensity)*l.blue/2,image);

            }

        }
    }

}
void drawLines(vector <Line> lines, int width, int height)
{
    bitmap_image image(width, height);
    bitmap_image image1(width, height);
    bitmap_image image2(width, height);
    //set color to white
    image.set_all_channels(255, 255, 255);
    image1.set_all_channels(255, 255, 255);
    image2.set_all_channels(255, 255, 255);
    for(int i = 0; i < lines.size(); i++)
    {
        midPointLine(lines[i], image,height);
        weightedAntiAliasing(lines[i], image1,height);
        unweightedAntiAliasing(lines[i], image2,height);
    }
    image.save_image("1_R.bmp");
    image1.save_image("3_RWA.bmp");
    image2.save_image("2_RUA.bmp");
}

int main()
{

    int Width,Height;
    int n;
    //read from file
    ifstream fin("input.txt");
    fin>>Width>>Height>>n;
    cout<<Width<<" "<<Height<<" "<<n<<endl;
    vector<Line> lines;
    for(int i=0; i<n; i++)
    {
        int x1,y1,x2,y2;
        fin>>x1>>y1>>x2>>y2;
        cout<<"x y "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;

        Line l(x1,y1,x2,y2);
        int r,g,b;
        fin>>r>>g>>b;
        cout << "r g b " << r << " " << g << " " << b << endl;
        l.setColor(r,g,b);
        lines.push_back(l);
    }
    drawLines(lines,Width,Height);

    fin.close();
    return 0;
}
