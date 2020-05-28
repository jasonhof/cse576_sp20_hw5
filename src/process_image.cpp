#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  cout << im.w << "," << im.h << "," << im.c << endl;
  // TODO: calculate the pixels of 'gray'
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      gray(wn,hn,0)=.299*im(wn,hn,0)+.587*im(wn,hn,1)+.114*im(wn,hn,2);
    }
  }
  //gray.save_image("output/gray_prod2");
  
  //NOT_IMPLEMENTED();
  
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: shift all the pixels at the specified channel
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      im(wn,hn,c) = im(wn,hn,c)+v;
    }
  }
  
  //NOT_IMPLEMENTED();
  
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      im(wn,hn,c) = im(wn,hn,c)*v;
    }
  }
  //NOT_IMPLEMENTED();
  
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      for (int cn=0;cn<im.c;cn++)
      {
        if (im(wn,hn,cn) > 1) {(im(wn,hn,cn) = 1); }
        if (im(wn,hn,cn) < 0) {(im(wn,hn,cn) = 0); }
      }
    }
  }
  
  //NOT_IMPLEMENTED();
  
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  float R,G,B,H,S,V,C,Hp = 0;
  // TODO: Convert all pixels from RGB format to HSV format
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      R = im(wn,hn,0);
      G = im(wn,hn,1);
      B = im(wn,hn,2);
      V=max(R,G,B);
      C = V-min(R,G,B);
      if(V==0) { S = 0; }
      else { S = C / V; }
      if (C==0) { H=0; }
      else
      {
        if      (V==R) { Hp = (G-B)/C; }
        else if (V==G) { Hp = ((B-R)/C)+2; }
        else if (V==B) { Hp = ((R-G)/C)+4; }
        if (Hp<0) { H = (Hp/6)+1; }
        else { H = Hp/6; }
      }

      im(wn,hn,0) = H;
      im(wn,hn,1) = S;
      im(wn,hn,2) = V;
      
      

      /*
      for (int cn=0;cn<im.c;cn++)
      {
        if (im(wn,hn,cn) > 1) {(im(wn,hn,cn) = 1); }
        if (im(wn,hn,cn) < 0) {(im(wn,hn,cn) = 0); }
      }*/
    }
  }


  //NOT_IMPLEMENTED();
  
  }

void set_pix(Image& im, int x, int y, float m, float a, float b, float c)
  {
    im(x,y,0) = a + m;
    im(x,y,1) = b + m;
    im(x,y,2) = c + m;
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  //cout<<"Converting HSV to RGB ..."<<endl;
  float H,S,V,C,X,m,R,G,B = 0;
  float frac = 1.0/6.0;
  //cout<<frac<<endl;
  // TODO: Convert all pixels from HSV format to RGB format
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      H = im(wn,hn,0);
      S = im(wn,hn,1);
      V = im(wn,hn,2);
      C = V*S;
      X = C*(1 - abs(fmod(6*H,2) -1));
      m = V-C;
      if      (H>=0      & H < frac  ) { set_pix(im,wn,hn,m,C,X,0); }
      else if (H>=frac   & H < 2*frac) { set_pix(im,wn,hn,m,X,C,0); }
      else if (H>=2*frac & H < 3*frac) { set_pix(im,wn,hn,m,0,C,X); }
      else if (H>=3*frac & H < 4*frac) { set_pix(im,wn,hn,m,0,X,C); }
      else if (H>=4*frac & H < 5*frac) { set_pix(im,wn,hn,m,X,0,C); }
      else if (H>=5*frac & H < 1     ) { set_pix(im,wn,hn,m,C,0,X); }
    }
  }
  //NOT_IMPLEMENTED();
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  // Using an alternate definition of HCL colorspace from Sarifuddin and Missaoui in 2005:
  // https://www.researchgate.net/publication/228906385_A_new_perceptually_uniform_color_space_with_associated_color_similarity_measure_for_content-based_image_and_video_retrieval
  float H,C,L,R,G,B,Q,a = 0;
  float Y = 100;
  float g = 3;
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      R = im(wn,hn,0);
      G = im(wn,hn,1);
      B = im(wn,hn,2);
      a = ( min(R,G,B) / max(R,G,B) / Y);
      Q = exp(a*g);
      L = (Q*max(R,G,B) + (1-Q)*min(R,G,B)) / 2;
      C = (Q*(abs(R-G)+abs(G-B)+abs(B-R))) / 3;
      H = atan((G-B)/(R-G));

      im(wn,hn,0) = L;
      im(wn,hn,1) = C;
      im(wn,hn,2) = H;

    }
  }


  //NOT_IMPLEMENTED();
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  
  NOT_IMPLEMENTED();
  
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
