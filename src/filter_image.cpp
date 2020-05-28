#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image& im)
  {
    //int divNum = im.w * im.h;
    //int divNum = std.accumulate(im.begin(),im.end(),0);
    int divNum = 0;
    for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        for (int cn=0;cn<im.c;cn++)
        {
          divNum += im(wn,hn,cn);;
        }
      }
    }
    //int numChannels = im.c;
    if (divNum!=0)
    {
      for (int wn=0;wn<im.w;wn++)
      {
        for (int hn=0;hn<im.h;hn++)
        {
          for (int cn=0;cn<im.c;cn++)
          {
            
            im(wn,hn,cn) = im(wn,hn,cn) / divNum;
          }
        }
      }
    }
  // TODO: Normalize each channel
  //NOT_IMPLEMENTED();
  
  
  }

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w)
  {
  assert(w%2); // w needs to be odd
  Image im(w,w,1);
  for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        im(wn,hn) = 1;
      }
    }
  l1_normalize(im);

  // TODO: Implement the filter
  //NOT_IMPLEMENTED();
  
  return im;
  }

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image& im, const Image& filter, bool preserve)
  {
  assert(filter.c==1);
  Image temp(im);
  Image ret(im); // Do I need to do a special thing to make a new image?
  //start at wn and hn = (filter.w-1/2)
  int ofs = (filter.w-1)/2; // Offset to start the convolution from current pixel
  cout<<"convolve_image debug:"<<endl;
  cout<<"ret.w="<<ret.w<<endl;
  cout<<"ofs="<<ofs<<endl;
  /*
  if (preserve)
  {
    ret = new Image(im);
  }*/
  // This is the case when we need to use the function clamped_pixel(x,y,c).
  // Otherwise you'll have to manually check whether the filter goes out of bounds
  
  // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
  // TODO: Do the convolution operator
  // TODO: add preserve
  for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        /*
        if (!preserve) 
        {

        }
        else
        */
        for (int cn=0;cn<im.c;cn++)
        {
          float pixVal = 0;
          int wInd = 0;
          for (int fnw=wn-ofs;fnw<=wn+ofs;fnw++)
          {
            int hInd = 0;
            for (int fnh=hn-ofs;fnh<=hn+ofs;fnh++)
            {
              //cout<<"Convolving (wn,hn,cn)=("<<wn<<","<<hn<<","<<cn<<")"
              //<<"and (wInd,hInd)="<<wInd<<","<<hInd<<")"<<endl;
              pixVal += im.clamped_pixel(fnw,fnh,cn)*filter(wInd,hInd,0);
              hInd++;
            }
            wInd++;
          }
          temp(wn,hn,cn) = pixVal;
        }
      }
    }
    if(!preserve)
    {
      ret = Image(im.w,im.h,1);
      for (int wn=0;wn<im.w;wn++)
      {
        for (int hn=0;hn<im.h;hn++)
        {
          ret(wn,hn,0) = temp(wn,hn,0) + temp(wn,hn,1) + temp(wn,hn,2);
        }
      }
    }
    else
    {
      ret = temp;
    }
    

  //NOT_IMPLEMENTED();
  // TODO: probably should just establish im's channel number here and then average if !preserve
  // Make sure to return ret and not im. This is just a placeholder
  return ret;
  }

void print_filter(const Image& im)
{
  cout<<"filter:"<<endl;
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      cout<<"("<<wn<<","<<hn<<")="<<im(wn,hn,0)<<", ";
    }
    cout<<endl;
  }
}

void print_image(const Image& im)
{
  cout<<"filter:"<<endl;
  for (int wn=0;wn<im.w;wn++)
  {
    for (int hn=0;hn<im.h;hn++)
    {
      for (int cn=0;cn<im.c;cn++)
      {
      cout<<"("<<wn<<","<<hn<<","<<cn<<")="<<im(wn,hn,cn)<<", ";
      }
    }
    cout<<endl;
  }
}

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter()
  {
  // TODO: Implement the filter
  Image im(3,3,1);
  im(0,1) = -1;
  im(1,0) = -1;
  im(2,1) = -1;
  im(1,2) = -1;
  im(1,1) = 4;
  cout<<"make_highpass_filter: high pass filter made"<< endl;
  print_filter(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  l1_normalize(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  print_filter(im);
  //NOT_IMPLEMENTED();
  
  return im;
  
  }

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter()
  {
  // TODO: Implement the filter
  Image im(3,3,1);
  im(0,1) = -1;
  im(1,0) = -1;
  im(2,1) = -1;
  im(1,2) = -1;
  im(1,1) = 5;
  print_filter(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  l1_normalize(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  print_filter(im);
  //NOT_IMPLEMENTED();
  
  return im;
  
  }

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter()
  {
  // TODO: Implement the filter
  Image im(3,3,1);
  im(0,0) = -2;
  im(0,1) = -1;
  im(0,2) = 0;
  im(1,0) = -1;
  im(1,1) = 1;
  im(1,2) = 1;
  im(2,0) = 0;
  im(2,1) = 1;
  im(2,2) = 2;
  print_filter(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  l1_normalize(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  print_filter(im);
  //NOT_IMPLEMENTED();
  
  return im;
  
  }

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma)
  {
  // TODO: Implement the filter
  //double pi = 3.1415926536;
  int w = sigma*6;
  if (w%2==0) w+=1; // make it odd
  int mid = (w-1)/2;
  Image im(w,w,1);
  for (int x=0;x<w;x++)
  {
    for (int y=0;y<w;y++)
    {
      //calculate the x and y distance from center
      int x2 = abs(mid-x);
      int y2 = abs(mid-y);
      //compute the value
      im(x,y) = (1/(2*M_PI*sigma*sigma))*exp(-(x2*x2+y2*y2)/(2*sigma*sigma));
    }
  }
  l1_normalize(im);
  //NOT_IMPLEMENTED();
  
  return im;
  
  }


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  Image c(a);
  // TODO: Implement addition
  for (int wn=0;wn<c.w;wn++)
    {
      for (int hn=0;hn<c.h;hn++)
      {
        for (int cn=0;cn<c.c;cn++)
        {
          c(wn,hn,cn) = a(wn,hn,cn) + b(wn,hn,cn);
        }
      }
    }
  
  //NOT_IMPLEMENTED();
  
  return c;
  
  }

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  Image c(a);
  // TODO: Implement subtraction
  for (int wn=0;wn<c.w;wn++)
    {
      for (int hn=0;hn<c.h;hn++)
      {
        for (int cn=0;cn<c.c;cn++)
        {
          c(wn,hn,cn) = a(wn,hn,cn) - b(wn,hn,cn);
        }
      }
    }
  /*
  cout<<"sub_image printing images:"<<endl<<"a:"<<endl;
  print_filter(a);
  cout<<"b:"<<endl;
  print_filter(b);
  cout<<"c:"<<endl;
  print_filter(c);
  */
  //NOT_IMPLEMENTED();
  
  return c;
  
  }

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter()
  {
  // TODO: Implement the filter
  Image im(3,3,1);
  im(0,0) = -1;
  im(0,1) = -2;
  im(0,2) = -1;
  im(1,0) = 0;
  im(1,1) = 0;
  im(1,2) = 0;
  im(2,0) = 1;
  im(2,1) = 2;
  im(2,2) = 1;
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  l1_normalize(im);
  cout<<im(0,0)<<","<<im(0,1)<<","<<im(1,1)<<","<<endl;
  //NOT_IMPLEMENTED();
  
  return im;
  }

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter()
  {
  // TODO: Implement the filter
  Image im(3,3,1);
  im(0,0) = -1;
  im(0,1) = 0;
  im(0,2) = 1;
  im(1,0) = -2;
  im(1,1) = 0;
  im(1,2) = 2;
  im(2,0) = -1;
  im(2,1) = 0;
  im(2,2) = 1;
  cout<<im(0,0)<<","<<im(1,0)<<","<<im(1,1)<<","<<endl;
  l1_normalize(im);
  cout<<im(0,0)<<","<<im(1,0)<<","<<im(1,1)<<","<<endl;
  //NOT_IMPLEMENTED();
  
  return im;
  }

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image& im)
  {
  assert(im.w*im.h); // assure we have non-empty image
  
  // TODO: Normalize the features for each channel
  //print_image(im);
  // find range and min
  int range = 0;
  int min = im(0,0,0);
  int max = im(0,0,0);
  int val = im(0,0,0);
  for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        for (int cn=0;cn<im.c;cn++)
        {
          val = im(wn,hn,cn);
          if (val<min) { min = val;}
          if (val>max) { max = val;}
        }
      }
    }
  range = max - min;
  // if range is 0, set everything to 0
  if (range == 0)
  {
    for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        for (int cn=0;cn<im.c;cn++)
        {
          im(wn,hn,cn) = 0;
        }
      }
    }
  }
  else
  {
    for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        for (int cn=0;cn<im.c;cn++)
        {
          im(wn,hn,cn) = (im(wn,hn,cn) - min) / range;
        }
      }
    }
  }
  //print_image(im);
  
  // if range is not 0, then subtract min and divide by range
  //NOT_IMPLEMENTED();
  
  }


// Normalizes features across all channels
void feature_normalize_total(Image& im)
  {
  assert(im.w*im.h*im.c); // assure we have non-empty image
  
  int nc=im.c;
  im.c=1;im.w*=nc;
  
  feature_normalize(im);
  
  im.w/=nc;im.c=nc;
  
  }


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image,Image> sobel_image(const Image& im)
  {
  // TODO: Your code here
  //cout<<"Generating sobel_image"<<endl;
  Image gx = make_gx_filter();
  Image gy = make_gy_filter();
  Image cx = convolve_image(im,gx,false);
  Image cy = convolve_image(im,gy,false);
  Image mag = cx;
  Image the = cy;
  for (int wn=0;wn<mag.w;wn++)
    {
      for (int hn=0;hn<mag.h;hn++)
      {
        float x = cx(wn,hn,0);
        float y = cy(wn,hn,0);
        // set mag to sqrt(cx2+cy2) at each point
        mag(wn,hn,0) = sqrt(x*x + y*y); 
        // set the to atan(cx/cy) at each point
        the(wn,hn,0) = atan2(y,x)/(2*M_PI)+0.5;

      }
    }
  feature_normalize(mag);
  //cout<<"finished feature_normalize of magnitude image"<<endl;
  //NOT_IMPLEMENTED();
  //cout<<"sobel_image generated"<<endl;
  return {mag,the};
  }


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image& im)
  {
  
  // TODO: Your code here

  //Image im = load_image("data/dog.jpg");
  Image gx = make_gx_filter();
  Image gy = make_gy_filter();
  pair<Image,Image> res = sobel_image(im);
  Image mag = res.first;
  Image theta = res.second;
  Image hsv = im;

  feature_normalize(mag);
  feature_normalize(theta);

  for (int wn=0;wn<im.w;wn++)
    {
      for (int hn=0;hn<im.h;hn++)
      {
        hsv(wn,hn,1) = mag(wn,hn,0);
        hsv(wn,hn,2) = theta(wn,hn,0);
      }
    }
  Image rgb = hsv;
  hsv_to_rgb(rgb);
  //NOT_IMPLEMENTED();
  

  return rgb;
  }


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image& im, float sigma1, float sigma2)
  {
  Image bf=im;
  
  // TODO: Your bilateral code
  NOT_IMPLEMENTED();
  
  return bf;
  }



// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }
void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }
void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image& a, const Image& b) { return sub_image(a,b); }
Image operator+(const Image& a, const Image& b) { return add_image(a,b); }
