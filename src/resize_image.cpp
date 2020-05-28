#include <cmath>
#include <iostream>
#include "image.h"

using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neighbor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  
  // TODO: Your code here
  /*
  float pixVal = 0;
  -.5+
  */
  

  
  // calculate x, calculate y, keep channel the same
  /*
  int xNew = x;
  int yNew = y;
  float xF, yF;
  modf(x,&xF);
  modf(y,&yF);
  xNew = xF;
  yNew = yF;
  //if (x != xNew){ xNew = round(x);}
  //if (y != yNew){ yNew = round(y);}
  */
  int xNew = round(x);
  int yNew = round(y);
  
  float pixVal = clamped_pixel(xNew,yNew,c);
  

  


  //NOT_IMPLEMENTED();
  
  
  return pixVal;
  //return 0;
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  
  
  // TODO: Your code here
  // Calculate areas
  float A1, A2, A3, A4, retVal;
  float xMinDiff, yMinDiff;
  int xMin, xMax, yMin, yMax;
  float xF, yF;
  modf(x,&xF);
  modf(y,&yF);
  xMin = xF;
  yMin = yF;
  xMax = xMin+1;
  yMax = yMin+1;
  xMinDiff = x-(float)xMin;
  yMinDiff = y-(float)yMin;
  A1 = (1-xMinDiff)*(1-yMinDiff);
  A2 = xMinDiff*(1-yMinDiff);
  A3 = (1-xMinDiff)*yMinDiff;
  A4 = xMinDiff*yMinDiff;

  // Add contributions of values
  retVal = A1*clamped_pixel(xMin,yMin,c)+
           A2*clamped_pixel(xMax,yMin,c)+
           A3*clamped_pixel(xMin,yMax,c)+
           A4*clamped_pixel(xMax,yMax,c);
  //NOT_IMPLEMENTED();
  /*
  if(x==7.625 && y<0)
  {
    cout<<A1<<","<<A2<<","<<A3<<","<<A4<<endl;
  }*/
  
  
  return retVal;
  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  
  // TODO: Your code here
  // calculate ratios of images for multiplying to find position
  float wMult = (float)im.w/(float)w;
  float hMult = (float)im.h/(float)h;
  float wInd, hInd; //float values to set within loop
  // loop over image and set ret to pixel_nearest
  for (int wn=0;wn<w;wn++)
  {
    for (int hn=0;hn<h;hn++)
    {
      for (int cn=0;cn<im.c;cn++)
      {
        //cout<<"wn="<<wn<<", hn="<<hn<<", cn="<<cn<<endl;
        //wInd = (float)wn*wMult;
        //hInd = (float)hn*hMult;
        wInd = -.5+((float)wn+.5)*wMult;
        hInd = -.5+((float)hn+.5)*hMult;
        ret(wn,hn,cn) = im.pixel_nearest(wInd,hInd,cn);
      }
      
    }
  }
  /*
  // failing at 9 0 0
  int checkVal = 9;
  float relVal = checkVal*wMult;
  cout<<"Debug nearest_resize:"<<endl;

  cout<<im.w<<endl;
  cout<<w<<endl;
  */
  /*
  cout<<wMult<<endl;
  wMult = im.w/w;
  cout<<wMult<<endl;
  */
  /*
  cout<<"checkVal="<<checkVal<<", relVal="<<relVal<<endl;
  cout<<"wMult="<<wMult<<", "<<checkVal<<"*wMult="<<checkVal*wMult<<endl;
  cout <<"im("<<checkVal-1<<",0,0)="<< im(checkVal-1,0,0)<<", im("<<checkVal<<",0,0)="<< im(checkVal,0,0)<<", im("<<checkVal+1<<",0,0)="<< im(checkVal+1,0,0)<<endl;
  cout<<"ret("<<checkVal<<",0,0)="<<ret(checkVal,0,0)<<endl;
  int yNew = relVal;
  float xF, yF;
  modf(relVal,&xF);
  cout<<"xF="<<xF<<endl;
  */
  
  //NOT_IMPLEMENTED();
  
  
  
  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  float wMult = (float)im.w/(float)w;
  float hMult = (float)im.h/(float)h;
  float wInd, hInd; //float values to set within loop
  // TODO: Your code here
  for (int wn=0;wn<w;wn++)
  {
    for (int hn=0;hn<h;hn++)
    {
      for (int cn=0;cn<im.c;cn++)
      {
        //cout<<"wn="<<wn<<", hn="<<hn<<", cn="<<cn<<endl;
        //wInd = (float)wn*wMult;
        //hInd = (float)hn*hMult;
        wInd = -.5+((float)wn+.5)*wMult;
        hInd = -.5+((float)hn+.5)*hMult;
        ret(wn,hn,cn) = im.pixel_bilinear(wInd,hInd,cn);
      }
      
    }
  }
  //NOT_IMPLEMENTED();
  
  // failing at 32 0 0
  int checkVal = 32;
  float relVal = -.5+((float)checkVal+.5)*wMult;
  /*
  cout<<"Debug bilinear_resize:"<<endl;
  cout<<"im.w="<<im.w<<", im.h="<<im.h<<", w="<<w<<", h="<<h<<endl;
  cout<<"checkVal="<<checkVal<<", relVal="<<relVal<<endl;
  cout<<"wMult="<<wMult<<", "<<checkVal<<"*wMult="<<checkVal*wMult<<endl;
  cout <<"im("<<checkVal-1<<",0,0)="<< im(checkVal-1,0,0)<<", im("<<checkVal<<",0,0)="<< im(checkVal,0,0)<<", im("<<checkVal+1<<",0,0)="<< im(checkVal+1,0,0)<<endl;
  cout <<"im("<<relVal-1<<",0,0)="<< im(relVal-1,0,0)<<", im("<<relVal<<",0,0)="<< im(relVal,0,0)<<", im("<<relVal+1<<",0,0)="<< im(relVal+1,0,0)<<endl;
  cout<<"ret("<<checkVal<<",0,0)="<<ret(checkVal,0,0)<<endl;
  */
  int yNew = relVal;
  float xF, yF;
  modf(relVal,&xF);
  int xMin = xF;
  int xMax = xMin+1;
  //cout<<"xF="<<xF<<", xMin="<<xMin<<", xMax="<<xMax<<endl;
  return ret;
  }


