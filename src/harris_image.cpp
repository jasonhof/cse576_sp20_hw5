#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>

#include "image.h"
//#include "matrix.h"

using namespace std;
#define M_PI 3.14159265358979323846


// Create a feature descriptor for an index in an image.
// const Image& im: source image.
// int x,y: coordinates for the pixel we want to describe.
// returns: Descriptor for that index.
Descriptor describe_index(const Image& im, int x, int y, int w)
  {
  Descriptor d;
  d.p={(double)x,(double)y};
  d.data.reserve(w*w*im.c);
  
  // If you want you can experiment with other descriptors
  // This subtracts the central value from neighbors
  // to compensate some for exposure/lighting changes.
  for(int c=0;c<im.c;c++)
    {
    float cval = im.clamped_pixel(x,y,c);
    for(int dx=-w/2;dx<=w/2;dx++)for(int dy=-w/2;dy<=w/2;dy++)
      d.data.push_back(im.clamped_pixel(x+dx,y+dy,c)-cval);
    }
  return d;
  }

// Marks the spot of a point in an image.
// Image& im: image to mark.
// Point p: spot to mark in the image.
void mark_spot(Image& im, const Point& p)
  {
  int x = p.x;
  int y = p.y;
  
  for(int i = -9; i < 10; ++i)
    {
    im.set_pixel(x+i, y, 0, 1);
    im.set_pixel(x, y+i, 0, 1);
    im.set_pixel(x+i, y, 1, 0);
    im.set_pixel(x, y+i, 1, 0);
    im.set_pixel(x+i, y, 2, 1);
    im.set_pixel(x, y+i, 2, 1);
    }
  }

// Marks corners denoted by an array of descriptors.
// Image& im: image to mark.
// vector<Descriptor> d: corners in the image.
Image mark_corners(const Image& im, const vector<Descriptor>& d)
  {
  Image im2=im;
  for(auto&e1:d)mark_spot(im2,e1.p);
  return im2;
  }


// HW5 1.1b
// Creates a 1d Gaussian filter.
// float sigma: standard deviation of Gaussian.
// returns: single row Image of the filter.
Image make_1d_gaussian(float sigma)
  {
  // TODO: make separable 1d Gaussian.
  
  //NOT_IMPLEMENTED();
  int len = sigma * 6;
  if (len % 2 == 0) len += 1;
  Image lin(len,1); // set to proper dimension
  lin.data[0]=1;
  int mid = (len - 1 / 2);
  for (int x = 0; x < len; x++)
  {
      int x2 = abs(mid - x);
      lin(x, 0) = (1 / (2 * M_PI * sigma * sigma)) * exp(-(x2 * x2) / (2 * sigma * sigma));
  }
  
  
  return lin;
  }

void print_filter2(const Image& im)
{
    cout << "filter:" << endl;
    for (int wn = 0; wn < im.w; wn++)
    {
        for (int hn = 0; hn < im.h; hn++)
        {
            cout << "(" << wn << "," << hn << ")=" << im(wn, hn, 0) << ", ";
        }
        cout << endl;
    }
}

// HW5 1.1b
// Smooths an image using separable Gaussian filter.
// const Image& im: image to smooth.
// float sigma: std dev. for Gaussian.
// returns: smoothed Image.
Image smooth_image(const Image& im, float sigma)
  {
  // TODO: use two convolutions with 1d gaussian filter.
  // Hint: to make the filter from vertical to horizontal or vice versa
  // use "swap(filter.h,filter.w)"
    Image xg = make_1d_gaussian(sigma);
    Image yg = xg;
    swap(yg.h, yg.w);
  //NOT_IMPLEMENTED();
    print_filter2(xg);
    print_filter2(yg);
    cout << "got here1";
    Image ret = convolve_image(im, xg, true);
    cout << "2";
    ret = convolve_image(im, yg, true);
    cout << "3" << endl;;
    return ret;
  }

// HW5 1.1
// Calculate the structure matrix of an image.
// const Image& im im: the input image.
// float sigma: std dev. to use for weighted sum.
// returns: structure matrix. 1st channel is Ix^2, 2nd channel is Iy^2,
//          third channel is IxIy.
Image structure_matrix(const Image& im2, float sigma)
  {
  TIME(1);
  // only grayscale or rgb
  assert((im2.c==1 || im2.c==3) && "only grayscale or rgb supported");
  Image im;
  // convert to grayscale if necessary
  cout << "im2 channels = " <<im2.c << endl;
  if(im2.c==1)im=im2;
  else im=rgb_to_grayscale(im2);
  
  Image S(im.w, im.h, 3);
  cout << "S channels = " << S.c << endl;
  // TODO: calculate structure matrix for im.

  // Convolve image with Sobel filter gx, then convolve with gy
  Image gx = make_gx_filter();
  Image gy = make_gy_filter();
  Image Ix = convolve_image(im, gx, true);
  Image Iy = convolve_image(im, gy, true);
  //cout << "S1 channels = " << S1.c << endl;
  //cout << "S2 channels = " << S2.c << endl;
  // Corresponding measures - set Ix^2 to ch1, Iy^2 to ch2, and IxIy to ch3
  for (int wn = 0; wn < im.w; wn++)
  {
      for (int hn = 0; hn < im.h; hn++) 
      {
          S(wn, hn, 0) = Ix(wn, hn, 0) * Ix(wn, hn, 0);
          S(wn, hn, 1) = Iy(wn, hn, 0) * Iy(wn, hn, 0);
          S(wn, hn, 2) = Ix(wn, hn, 0) * Iy(wn, hn, 0);
      }
  }


  // Convolve image with Gaussian
  Image g = make_gaussian_filter(sigma);
  S = convolve_image(S, g, true);

  // Alternatively, do fast smoothing instead of Gaussian (not working rn)
  //S = smooth_image(S, sigma);
  //cout << "S channels = " << S.c << endl;
  
  //NOT_IMPLEMENTED();
  //S = S3;
  cout << "S channels = " << S.c << endl;
  return S;
  }


// HW5 1.2
// Estimate the cornerness of each pixel given a structure matrix S.
// const Image& im S: structure matrix for an image.
// returns: a response map of cornerness calculations.
Image cornerness_response(const Image& S, int method)
  {
  Image R(S.w, S.h);
  // TODO: fill in R, "cornerness" for each pixel using the structure matrix.
  // We'll use formulation det(S) - alpha * trace(S)^2, alpha = .06.
  // E(S) = det(S) / trace(S)
  
  NOT_IMPLEMENTED();
  
  return R;
  }


// HW5 1.3
// Perform non-max supression on an image of feature responses.
// const Image& im: 1-channel image of feature responses.
// int w: distance to look for larger responses.
// returns: Image with only local-maxima responses within w pixels.
Image nms_image(const Image& im, int w)
  {
  //TIME(1);
  Image r=im;
  // TODO: perform NMS on the response map.
  // for every pixel in the image:
  //     for neighbors within w:
  //         if neighbor response greater than pixel response:
  //             set response to be very low
  
  NOT_IMPLEMENTED();
  
  return r;
  }


// HW5 1.4
// Perform corner detection and extract features from the corners.
// const Image& im: input image.
// const Image& nms: nms image
// float thresh: threshold for cornerness.
// returns: vector of descriptors of the corners in the image.
vector<Descriptor> detect_corners(const Image& im, const Image& nms, float thresh, int window)
  {
  vector<Descriptor> d;
  //TODO: count number of responses over threshold (corners)
  //TODO: and fill in vector<Descriptor> with descriptors of corners, use describe_index.
  
  NOT_IMPLEMENTED();
  
  return d;
  }


// Perform harris corner detection and extract features from the corners.
// const Image& im: input image.
// float sigma: std. dev for harris.
// float thresh: threshold for cornerness.
// int nms: distance to look for local-maxes in response map.
// returns: vector of descriptors of the corners in the image.
vector<Descriptor> harris_corner_detector(const Image& im, float sigma, float thresh, int window, int nms, int corner_method)
  {
  // Calculate structure matrix
  Image S = structure_matrix(im, sigma);
  
  // Estimate cornerness
  Image R = cornerness_response(S,corner_method);
  
  // Run NMS on the responses
  Image Rnms = nms_image(R, nms);
  
  return detect_corners(im, Rnms, thresh, window);
  }

// Find and draw corners on an image.
// Image& im: input image.
// float sigma: std. dev for harris.
// float thresh: threshold for cornerness.
// int nms: distance to look for local-maxes in response map.
Image detect_and_draw_corners(const Image& im, float sigma, float thresh, int window, int nms, int corner_method)
  {
  vector<Descriptor> d = harris_corner_detector(im, sigma, thresh, window, nms, corner_method);
  return mark_corners(im, d);
  }
