#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <iostream>

#include "image.h"
#include "matrix.h"

#include <set>
#include <thread>

using namespace std;

// Comparator for matches
// const void *a, *b: pointers to the matches to compare.
// returns: result of comparison, 0 if same, 1 if a > b, -1 if a < b.
int match_compare(const void *a, const void *b)
  {
  Match *ra = (Match *)a;
  Match *rb = (Match *)b;
  if (ra->distance < rb->distance) return -1;
  else if (ra->distance > rb->distance) return  1;
  else return 0;
  }


// Place two images side by side on canvas, for drawing matching pixels.
// const Image& a, b: images to place.
// returns: image with both a and b side-by-side.
Image both_images(const Image& a, const Image& b)
  {
  assert(a.c==b.c);
  Image both(a.w + b.w, a.h > b.h ? a.h : b.h, a.c);
  
  for(int k = 0; k < both.c; ++k)
    for(int j = 0; j < a.h; ++j)
      for(int i = 0; i < a.w; ++i)
        both(i, j, k) = a(i, j, k);
  
  for(int k = 0; k < both.c; ++k)
    for(int j = 0; j < b.h; ++j)
      for(int i = 0; i < b.w; ++i)
        both(i+a.w, j, k) = b(i, j, k);
  return both;
  }

// Draws lines between matching pixels in two images.
// const Image& a, b: two images that have matches.
// const vector<Match>& matches: array of matches between a and b.
// int inliers: number of inliers at beginning of matches, drawn in green.
// returns: image with matches drawn between a and b on same canvas.
Image draw_matches(const Image& a, const Image& b, const vector<Match>& matches, const vector<Match>& inliers)
  {
  Image both = both_images(a, b);
  
  for(int i = 0; i < (int)matches.size(); ++i)
    {
    int bx = matches[i].a->p.x; 
    int ex = matches[i].b->p.x; 
    int by = matches[i].a->p.y;
    int ey = matches[i].b->p.y;
    for(int j = bx; j < ex + a.w; ++j)
      {
      int r = (float)(j-bx)/(ex+a.w - bx)*(ey - by) + by;
      both.set_pixel(j, r, 0, 1);
      both.set_pixel(j, r, 1, 0);
      both.set_pixel(j, r, 2, 0);
      }
    }
  for(int i = 0; i < (int)inliers.size(); ++i)
    {
    int bx = inliers[i].a->p.x; 
    int ex = inliers[i].b->p.x; 
    int by = inliers[i].a->p.y;
    int ey = inliers[i].b->p.y;
    for(int j = bx; j < ex + a.w; ++j)
      {
      int r = (float)(j-bx)/(ex+a.w - bx)*(ey - by) + by;
      both.set_pixel(j, r, 0, 0);
      both.set_pixel(j, r, 1, 1);
      both.set_pixel(j, r, 2, 0);
      }
    }
  return both;
  }

// Draw the matches with inliers in green between two images.
// const Image& a, b: two images to match.
// vector<Match> m: matches
// Matrix H: the current homography
// thresh: for thresholding inliers
Image draw_inliers(const Image& a, const Image& b, const Matrix& H, const vector<Match>& m, float thresh)
  {
  vector<Match> inliers = model_inliers(H, m, thresh);
  Image lines = draw_matches(a, b, m, inliers);
  return lines;
  }

// Find corners, match them, and draw them between two images.
// const Image& a, b: images to match.
// float sigma: gaussian for harris corner detector. Typical: 2
// float thresh: threshold for corner/no corner. Typical: 1-5
// int nms: window to perform nms on. Typical: 3
Image find_and_draw_matches(const Image& a, const Image& b, float sigma, float thresh, int window, int nms, int corner_method)
  {
  vector<Descriptor> ad= harris_corner_detector(a, sigma, thresh, window, nms, corner_method);
  vector<Descriptor> bd= harris_corner_detector(b, sigma, thresh, window, nms, corner_method);
  vector<Match> m = match_descriptors(ad, bd);
  
  
  Image A=mark_corners(a, ad);
  Image B=mark_corners(b, bd);
  Image lines = draw_matches(A, B, m, {});
  
  return lines;
  }

// HW5 2.1
// Calculates L1 distance between two floating point arrays.
// vector<float>& a,b: arrays to compare.
// returns: l1 distance between arrays (sum of absolute differences).
float l1_distance(const vector<float>& a,const vector<float>& b)
  {
  assert(a.size()==b.size() && "Arrays must have same size\n");
  float total = 0.0;
  float diff = 0.0;
  // TODO: return the correct number.
  for (int i = 0; i < a.size(); i++)
  {
      diff = abs(a[i] - b[i]);
      total = total + diff;
  }
  //cout << "l1_distance: total diff = " << total << endl;
  //NOT_IMPLEMENTED();
  
  return total;
  }

// HW5 2.2a
// Finds best matches between descriptors of two images.
// const vector<Descriptor>& a, b: array of descriptors for pixels in two images.
// returns: best matches found. For each element in a[] find the index of best match in b[]
vector<int> match_descriptors_a2b(const vector<Descriptor>& a, const vector<Descriptor>& b)
  {
  vector<int> ind;
  for(int j=0;j<(int)a.size();j++)
    {
    int bind = -1; // <- find the best match (-1: no match)
    float best_distance=1e10f;  // <- best distance
    float cur_distance = 0;
    // TODO: find the best 'bind' descriptor in b that best matches a[j]
    // TODO: put your code here:
    //cout << a[j] << endl;
    for (int i = 0; i < (int)b.size(); i++)
    {
        cur_distance = l1_distance(a[j].data , b[i].data);
        //cur_distance = 4;
        if (cur_distance < best_distance)
        {
            best_distance = cur_distance;
            bind = i;
        }
    }
    ind.push_back(bind);
    //NOT_IMPLEMENTED();
    }
  return ind;
  
  }


// HW5 2.2b
// Finds best matches between descriptors of two images.
// const vector<Descriptor>& a, b: array of descriptors for pixels in two images.
// returns: best matches found. each descriptor in a should match with at most
//          one other descriptor in b.
vector<Match> match_descriptors(const vector<Descriptor>& a, const vector<Descriptor>& b)
  {
  if(a.size()==0 || b.size()==0)return {};
  
  vector<Match> m;
  
  // TODO: use match_descriptors_a2b(a,b) and match_descriptors_a2b(b,a)
  // and populate `m` with good matches!
  vector<int> ab = match_descriptors_a2b(a,b); // list of index matches in b for a's values
  vector<int> ba = match_descriptors_a2b(b,a); // list of index matches in a for b's values
  //cout << ab << endl;
  for (int i = 0; i < a.size(); i++)
  {
      //cout << "i="<<i<<", ab[i]="<<ab[i]<<", ba[ab[i]]="<<ba[ab[i]]<<endl;
      //cout << "i=" << i << " : ";
      if (ba[ab[i]] == i)
      {
          //cout << "SUCCESS: Match found with i=" << i << ", ab[i]=" << ab[i] << " and ba[ab[i]]=" << ba[ab[i]] << endl;
          // if the value of ba at the index that ab points to is i,
          // then we have a mutual match!
          float dist = l1_distance(a[i].data, b[ab[i]].data);
          //Match mut;
          //mut.a = &a[i];
          //m.push_back(Match(&a[i], &b[ab[i]], dist));
          Match mut = Match(&a[i], &b[ab[i]], dist);
          m.push_back(mut);
      }
      else
      {
          //cout << "NOPE: Match not found with i=" << i << ", ab[i]=" << ab[i] << " and ba[ab[i]]=" << ba[ab[i]] << endl;
          //cout << "Now comparing b to a: ba[ab[i]]=" << ba[ab[i]] << " and ab[ba[ab[i]]]=" << ab[ba[ab[i]]] << endl;
          //int temp =
      }
  }
  //NOT_IMPLEMENTED();
  
  return m;
  }


// HW5 3.1
// Apply a projective transformation to a point.
// const Matrix& H: homography to project point.
// const Point& p: point to project.
// returns: point projected using the homography.
Point project_point(const Matrix& H, const Point& p)
  {
  // TODO: project point p with homography H.
  // Remember that homogeneous coordinates are equivalent up to scalar.
  // Have to divide by.... something...
  double x = ( (1+H(0,0))*p.x + H(0,1)*p.y + H(0,2) ) / ( H(2,0)*p.x + H(2,1)*p.y + 1 );
  double y = ( H(1,0)*p.x + (1+H(1,1))*p.y + H(1,2) ) / ( H(2,0)*p.x + H(2,1)*p.y + 1 );
  //NOT_IMPLEMENTED();
  
  
  return Point(x,y);
  }

// HW5 3.2a
// Calculate L2 distance between two points.
// const Point& p, q: points.
// returns: L2 distance between them.
double point_distance(const Point& p, const Point& q)
  {
  // TODO: should be a quick one.
    double diff = pow((p.x - q.x), 2) + pow((p.y - q.y), 2);
  //NOT_IMPLEMENTED();
  return diff;
  }

// HW5 3.2b
// Count number of inliers in a set of matches. Should also bring inliers
// to the front of the array.
// const Matrix& H: homography between coordinate systems.
// const vector<Match>& m: matches to compute inlier/outlier.
// float thresh: threshold to be an inlier.
// returns: inliers whose projected point falls within thresh of their match in the other image.
vector<Match> model_inliers(const Matrix& H, const vector<Match>& m, float thresh)
  {
  vector<Match> inliers;
  // TODO: fill inliers
  // i.e. distance(H*a.p, b.p) < thresh
  float dist = 0;
  for (int i = 0; i < m.size(); i++)
  {
      Point a = project_point(H, m[i].a->p);
      dist = point_distance(a, m[i].b->p);
      if (dist<thresh) 
      { 
          inliers.push_back(m[i]);
      }
  }
  
  //NOT_IMPLEMENTED();
  
  return inliers;
  }

// HW5 3.3
// Randomly shuffle matches for RANSAC.
// vector<Match>& m: matches to shuffle in place.
void randomize_matches(vector<Match>& m)
  {
  // TODO: implement Fisher-Yates to shuffle the array.
  // You might want to use the swap function like:
  // swap(m[0],m[1]) which swaps the first and second element
    int len = m.size();
    int rando = 0;
    for (int i = len - 1; i > 0; i--)
    {
        rando = rand() % i + 1;
        swap(m[rando], m[i]);

    }
  
  //NOT_IMPLEMENTED();
  }

// HW5 3.4
// Computes homography between two images given matching pixels.
// const vector<Match>& matches: matching points between images.
// int n: number of matches to use in calculating homography.
// returns: matrix representing homography H that maps image a to image b.
Matrix compute_homography_ba(const vector<Match>& matches)
  {
  if(matches.size()<4)printf("Need at least 4 points for homography! %zu supplied\n",matches.size());
  if(matches.size()<4)return Matrix::identity(3,3);
  
  Matrix M(matches.size()*2,8);
  Matrix b(matches.size()*2);
  //M.print();
  
  for(int i = 0; i < (int)matches.size(); ++i)
    {
    double mx = matches[i].a->p.x;
    double my = matches[i].a->p.y;
    
    double nx = matches[i].b->p.x;
    double ny = matches[i].b->p.y;
    // TODO: fill in the matrices M and b.
    // each round of this loop is one match M[i] and b[i]

    // M is all the matches
    
    //M(i*2, 0) = nx;
    //M(i*2, 1) = ny;
    M(i * 2, 0) = mx;
    M(i * 2, 1) = my;
    M(i*2, 2) = 1;
    M(i*2, 3) = 0;
    M(i*2, 4) = 0;
    M(i*2, 5) = 0;
    //M(i*2, 6) = -1 * mx * nx;
    //M(i*2, 7) = -1 * mx * ny;
    M(i * 2, 6) = -1 * nx * mx;
    M(i * 2, 7) = -1 * nx * my;

    M(i*2, 8) = 0;
    M(i*2, 9) = 0;
    M(i*2, 10) = 0;
    //M(i*2, 11) = nx;
    //M(i*2, 12) = ny;
    M(i * 2, 11) = mx;
    M(i * 2, 12) = my;
    M(i*2, 13) = 1;
    //M(i*2, 14) = -1 * my * nx;
    //M(i*2, 15) = -1 * my * ny;
    M(i * 2, 14) = -1 * ny * mx;
    M(i * 2, 15) = -1 * ny * my;
    
    /*
    M(0,i*2) = nx;
    M(1,i*2) = ny;
    M(2,i*2) = 1;
    M(3,i*2) = 0;
    M(4,i*2) = 0;
    M(5,i*2) = 0;
    M(6,i*2) = -1 * mx * nx;
    M(7,i*2) = -1 * mx * ny;

    M(8,i*2) = 0;
    M(9,i*2) = 0;
    M(10,i*2) = 0;
    M(11,i*2) = nx;
    M(12,i*2) = ny;
    M(13,i*2) = 1;
    M(14,i*2) = -1 * my * nx;
    M(15,i*2) = -1 * my * ny;
    */
    
    /*
    M(i, 0, 0) = nx;
    M(i, 0, 1) = ny;


    M[i][0][0] = nx;
    M[i][0][1] = ny;
    M[i][0][2] = 1;
    M[i][0][3] = 0;
    M[i][0][4] = 0;
    M[i][0][5] = 0;
    */

    // b is distance
    b(i*2,0) = mx - nx;
    b(i*2,1) = my - ny;

    //M.print();
    //b.print();
    // Do I need to flip my indexes i and 0 above?  I guess, in C++, it will probably displace the same amount regardless?
    // Could just consider doing b(i) and b(i+1), too- no? Prob not
    // Also, I picked m as my x^ and y^, when m could very easily have been n- we'll see if that matters
    //NOT_IMPLEMENTED();
    
    }
  
  
  
  Matrix a = solve_system(M, b);
  
  Matrix Hba(3, 3);
  // TODO: fill in the homography H based on the result in a.
  // reverse what we did in the previous point calculation?
  Hba(0, 0) = 1+a(0);
  Hba(0, 1) = a(1);
  Hba(0, 2) = a(2);
  Hba(1, 0) = a(3);
  Hba(1, 1) = 1+a(4);
  Hba(1, 2) = a(5);
  Hba(2, 0) = a(6);
  Hba(2, 1) = a(7);
  Hba(2, 2) = 1;

  //a.print();
  //Hba.print();

  //Did I do this right?  Maybe remove the 1+'s
  //NOT_IMPLEMENTED();
  
  return Hba;
  }

// HW5 3.5
// Perform RANdom SAmple Consensus to calculate homography for noisy matches.
// vector<Match> m: set of matches.
// float thresh: inlier/outlier distance threshold.
// int k: number of iterations to run.
// int cutoff: inlier cutoff to exit early.
// returns: matrix representing most common homography between matches.
Matrix RANSAC(vector<Match> m, float thresh, int k, int cutoff)
  {
  if(m.size()<4)
    {
    //printf("Need at least 4 points for RANSAC! %zu supplied\n",m.size());
    return Matrix::identity(3,3);
    }
  
  int best = 0;
  Matrix Hba = Matrix::translation_homography(256, 0);
  // TODO: fill in RANSAC algorithm.
  vector<Match> comp(4);
  Matrix test = Matrix::translation_homography(256, 0);
  int score = 0;
  vector<Match> inliers;
  for (int i = 0; i < k; i++)
  {
      randomize_matches(m);
      for (int j = 0; j < 4; j++) { comp[j] = m[j]; }
      test = compute_homography_ba(comp);
      inliers = model_inliers(test, m, thresh);
      score = inliers.size();
      if (score > thresh) return test;
      if (score > best)
      {
          best = score;
          Hba = test;  //this might be a problem with addressing (how to copy?)
      }
      
  }

  // for k iterations:
  //     shuffle the matches
  //     compute a homography with a few matches (how many?? 4, right?)
  //     if new homography is better than old (how can you tell?):
  //         compute updated homography using all inliers
  //         remember it and how good it is
  //         if it's better than the cutoff:
  //             return it immediately
  // if we get to the end return the best homography
  //NOT_IMPLEMENTED();
  
  return Hba;
  }


Image trim_image(const Image& a)
  {
  int minx=a.w-1;
  int maxx=0;
  int miny=a.h-1;
  int maxy=0;
  
  for(int q3=0;q3<a.c;q3++)for(int q2=0;q2<a.h;q2++)for(int q1=0;q1<a.w;q1++)if(a(q1,q2,q3))
    {
    minx=min(minx,q1);
    maxx=max(maxx,q1);
    miny=min(miny,q2);
    maxy=max(maxy,q2);
    }
  
  if(maxx<minx || maxy<miny)return a;
  
  Image b(maxx-minx+1,maxy-miny+1,a.c);
  
  for(int q3=0;q3<a.c;q3++)for(int q2=miny;q2<=maxy;q2++)for(int q1=minx;q1<=maxx;q1++)
    b(q1-minx,q2-miny,q3)=a(q1,q2,q3);
  
  return b;
  }

// HW5 3.6
// Stitches two images together using a projective transformation.
// const Image& a, b: images to stitch.
// Matrix H: homography from image a coordinates to image b coordinates.
// float acoeff: blending coefficient
// returns: combined image stitched together.
Image combine_images(const Image& a, const Image& b, const Matrix& Hba, float ablendcoeff)
  {
  TIME(1);
  Matrix Hinv=Hba.inverse();
  
  // Project the corners of image b into image a coordinates.
  Point c1 = project_point(Hinv, Point(0,0));
  Point c2 = project_point(Hinv, Point(b.w-1, 0));
  Point c3 = project_point(Hinv, Point(0, b.h-1));
  Point c4 = project_point(Hinv, Point(b.w-1, b.h-1));

  
  // Find top left and bottom right corners of image b warped into image a.
  Point topleft, botright;
  botright.x = max(c1.x, max(c2.x, max(c3.x, c4.x)));
  botright.y = max(c1.y, max(c2.y, max(c3.y, c4.y)));
  topleft.x = min(c1.x, min(c2.x, min(c3.x, c4.x)));
  topleft.y = min(c1.y, min(c2.y, min(c3.y, c4.y)));
  
  // Find how big our new image should be and the offsets from image a.
  int dx = min(0, (int)topleft.x);
  int dy = min(0, (int)topleft.y);
  int w = max(a.w, (int)botright.x) - dx;
  int h = max(a.h, (int)botright.y) - dy;
  
  //printf("%d %d %d %d\n",dx,dy,w,h);
  
  // Can disable this if you are making very big panoramas.
  // Usually this means there was an error in calculating H.
  if(w > 15000 || h > 4000)
    {
    printf("Can't make such big panorama :/ (%d %d)\n",w,h);
    return Image(100,100,1);
    }
  
  Image c(w, h, a.c);
  
  // Paste image a into the new image offset by dx and dy.
  for(int k = 0; k < a.c; ++k)
    for(int j = 0; j < a.h; ++j)
      for(int i = 0; i < a.w; ++i)
        {
        // TODO: fill in.
          c(i - dx, j - dy, k) = a(i, j, k); // may be -dx and -dy, but I'm not sure
          //c(i - dx, j - dy, k) = 200;
        //NOT_IMPLEMENTED();
        }
  
  // TODO: Blend in image b as well.
  // You should loop over some points in the new image (which? all?)
  // and see if their projection from a coordinates to b coordinates falls
  // inside of the bounds of image b. If so, use bilinear interpolation to
  // estimate the value of b at that projection, then fill in image c.
  
  // When doing cylindrical and spherical, how do we cope with the missing 
  // image values due to the warping process? Consider skipping them? 
  // How do we know they are empty? Look in the image class for the function
  // "is_nonempty_patch" and try to figure out why it might be useful.
  // The member 
  
  // TODO: Put your code here.
  Point p=c1;
  // c(wn, hn, cn) = b.pixel_bilinear(wi, hi, cn);  // will eventually call something like this
  //for (int cn = 0; cn < b.c; ++cn) // can put this lower near the end
    //{
      for (int hn = 0; hn < b.h; ++hn)
          for (int wn = 0; wn < b.w; ++wn)
          {
              // find out if b pixel lands in c
              p = project_point(Hinv, Point(wn, hn));
              if (p.x > 0 && p.x < c.w && p.y>0 && p.y < c.h)
              {
                  // if so, find out if there is already an a pixel there and place it or blend it (bilinear)
                  if (c(p.x, p.y, 0) == 0)
                  {
                      for (int cn = 0; cn < c.c; ++cn)
                      {
                          c(p.x, p.y, cn) = b.pixel_bilinear(p.x, p.y, cn); // (I think I need to flip it) do I need to flip to project c onto b for bilinear to work?
                          //c(p.x, p.y, cn) = b(p.x, p.y, cn);
                          //c(p.x, p.y, cn) = 50;
                      }
                  }
                  else  // if not, just place the b pixel there (bilinear
                  {
                      for (int cn = 0; cn < c.c; ++cn)
                      {
                          c(p.x, p.y, cn) = (1-ablendcoeff)*b.pixel_bilinear(p.x, p.y, cn) + ablendcoeff*c(p.x,p.y,cn); 
                          //c(p.x, p.y, cn) = 100;
                      }
                      
                  }
              }
              

          }
      //}
  //NOT_IMPLEMENTED();
  
  
  // We trim the image so there are as few as possible black pixels.
  return trim_image(c);
  }

// Create a panoramam between two images.
// const Image& a, b: images to stitch together.
// float sigma: gaussian for harris corner detector. Typical: 2
// float thresh: threshold for corner/no corner. Typical: 1-5
// int nms: window to perform nms on. Typical: 3
// float inlier_thresh: threshold for RANSAC inliers. Typical: 2-5
// int iters: number of RANSAC iterations. Typical: 1,000-50,000
// int cutoff: RANSAC inlier cutoff. Typical: 10-100
Image panorama_image(const Image& a, const Image& b, float sigma, int corner_method, float thresh, int window, int nms, float inlier_thresh, int iters, int cutoff, float acoeff)
  {
  // Calculate corners and descriptors
  vector<Descriptor> ad;
  vector<Descriptor> bd;
  
  // doing it multithreading...
  thread tha([&](){ad = harris_corner_detector(a, sigma, thresh, window, nms, corner_method);});
  thread thb([&](){bd = harris_corner_detector(b, sigma, thresh, window, nms, corner_method);});
  tha.join();
  thb.join();
  
  // Find matches
  vector<Match> m = match_descriptors(ad, bd);
  
  // Run RANSAC to find the homography
  Matrix Hba = RANSAC(m, inlier_thresh, iters, cutoff);
  
  // Stitch the images together with the homography
  return combine_images(a, b, Hba, acoeff);
  }

// HW5 4.1
// Project an image onto a cylinder.
// const Image& im: image to project.
// float f: focal length used to take image (in pixels).
// returns: image projected onto cylinder, then flattened.
Image cylindrical_project(const Image& im, float f)
  {
  //TODO: project image onto a cylinder
  double hfov=atan(im.w/(2*f));
  double vfov=im.h/2./f;
  
  // For your convenience we have computed the output size
  Image c(im.w/cos(hfov),im.h/cos(hfov),im.c);
  
  NOT_IMPLEMENTED();
  
  return c;
  }

// HW5 4.2
// Project an image onto a cylinder.
// const Image& im: image to project.
// float f: focal length used to take image (in pixels).
// returns: image projected onto cylinder, then flattened.
Image spherical_project(const Image& im, float f)
  {
  //TODO: project image onto a sphere
  double hfov=atan(im.w/(2*f));
  double vfov=atan(im.h/(2*f));
  
  // For your convenience we have computed the output size
  Image c(im.w/cos(hfov),im.h/cos(hfov),im.c);
  
  NOT_IMPLEMENTED();
  
  return c;
  }
