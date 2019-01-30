#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <math.h>  
#include <limits>
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int width = 800;
const int height = 800;
const int width_tex = 1024;
const int height_tex = 1024;


std::vector<std::string> split(const std::string &chaine, char delimiteur)
{
  std::vector<std::string> elements;
  std::stringstream ss(chaine);
  std::string sousChaine;
 while (getline(ss, sousChaine, delimiteur))
 {
 elements.push_back(sousChaine);
 }
 return elements;
}


void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
} 


void line(Vec2i t0, Vec2i t1, TGAImage &image, TGAColor color) { 
  int x0 = t0.x;
  int y0 = t0.y;
  int x1 = t1.x;
  int y1 = t1.y;
  bool steep = false; 
  if (std::abs(x0-x1)<std::abs(y0-y1)) { 
    std::swap(x0, y0); 
    std::swap(x1, y1); 
    steep = true; 
  } 
  if (x0>x1) { 
    std::swap(x0, x1); 
    std::swap(y0, y1); 
  } 

  int dx = x1-x0; 
  int dy = y1-y0; 
  int derror2 = std::abs(dy)*2; 
  int error2 = 0; 
  int y = y0; 

  for (int x=x0; x<=x1; x++) { 
    if (steep) { 
      image.set(y, x, color); 
    } else { 
      image.set(x, y, color); 
    } 
    error2 += derror2; 
    if (error2 > dx) { 
      y += (y1>y0?1:-1); 
      error2 -= dx*2; 
    } 
  } 

} 



/*void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) { 
    line(t0, t1, image, color); 
    line(t1, t2, image, color); 
    line(t2, t0, image, color); 
}*/

void filledtoptriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
  if (t0.y > t1.y) {
    std::swap(t0.y, t1.y);
    std::swap(t0.x, t1.x);
  }
  if (t1.y > t2.y)  {
    std::swap(t1.y, t2.y);
    std::swap(t1.x, t2.x);
  }
  if (t0.x > t1.x) {
    std::swap(t0.y, t1.y);
    std::swap(t0.x, t1.x);
  }
  int x0 = t0.x;
  int y0 = t0.y;
  int x1 = t1.x;
  int y1 = t1.y;
  int x2 = t2.x;
  int y2 = t2.y;

  int dx = x2-x0; 
  int dy = y2-y0; 
  int rapport = std::abs( ceil( ((float)dx/(float)dy) - 0.5) );
  if ( dy == 0) std::cout << rapport  << "    COUCOU" << std::endl; rapport = 0;
  int derror2 = std::abs(dx)*2; 
  int error2 = 0; 
  int y = y0; 

  int dx2 = x2-x1;
  int dy2 = y2-y1;
  int rapport2 = std::abs( ceil(  ((float)dx2/(float)dy2) - 0.5) );
  
  if ( dy2 == 0) std::cout << rapport2 << "    COUCOU" << std::endl; rapport2 = 0;
  int derror3 = std::abs(dx2)*2; 
  int error3 = 0; 
  int yy = y2; 

  int x = x0;
  int xx = x1;

  if (rapport == 0) rapport = 1;
  if (rapport2 == 0) rapport2 = 1;
  // std::cout << rapport << "   "  << rapport2 << std::endl;


  //std::cout << x0 << ","  << y0 << "  " <<  rapport << "  " << "     " << x1 << ","  << y1 << "  "  << rapport2  <<  "  "  << x2 << std::endl;

  //std::cout << " TOPPPPPPPPP      T0  :" << t0.x << "," <<  t0.y <<  "  T1  :" << t1.x << "," << t1.y << "   T2  :" << t2.x << "," << t2.y  << std::endl;

  for (int i = y0; i <= y2; i++) {
    line(  x , i , xx , i, image, color);
    error2 += derror2;
    error3 += derror3; 

    if (error2 > dy) { 
      if ( (x2>x0 && x < x2) || (x2<x0 && x > x2) ) x += (x2>x0?rapport:-rapport); 
      error2 -= dy*2; 
    } 

    if (error3 > dy2) { 
      if ( (x2>x1 && xx < x2) || (x2<x1 && xx > x2) ) xx += (x2>x1?rapport2:-rapport2); 
      error3 -= dy2*2; 
    } 

    // std::cout << x << " " << i << " " << xx << " " << error2 << " " << error3 << " " << derror2 << " " << derror3 << " " << dy << " " << dy2<< std::endl;
  }

}

void filledbottomtriangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
  if (t0.y < t1.y) {
    std::swap(t0.y, t1.y);
    std::swap(t0.x, t1.x);
  }
  if (t1.y < t2.y)  {
    std::swap(t1.y, t2.y);
    std::swap(t1.x, t2.x);
  }
  if (t0.x > t1.x) {
    std::swap(t0.y, t1.y);
    std::swap(t0.x, t1.x);
  }
 
  //std::cout << " T0  :" << t0.x << "," <<  t0.y <<  "  T1  :" << t1.x << "," << t1.y << "   T2  :" << t2.x << "," << t2.y  << std::endl;

  int x0 = t0.x;
  int y0 = t0.y;
  int x1 = t1.x;
  int y1 = t1.y;
  int x2 = t2.x;
  int y2 = t2.y;

  int dx = x2-x0; 
  int dy = y0-y2; 
  int rapport = std::abs(  ceil( ((float)dx/(float)dy) - 0.5) );
  if ( dy == 0) std::cout << rapport  << "    COUCOU" << std::endl; rapport = 0;
  int derror2 = std::abs(dx)*2; 
  int error2 = 0; 
  int y = y0; 

  int dx2 = x2-x1;
  int dy2 = y1-y2;
  int rapport2 = std::abs( ceil(  ((float)dx2/(float)dy2) - 0.5) );
  if ( dy2 == 0) std::cout << rapport2  << "    COUCOU" << std::endl; rapport2 = 0;
  int derror3 = std::abs(dx2)*2; 
  int error3 = 0; 
  int yy = y2; 

  int x = x0;
  int xx = x1;

  if (rapport == 0) rapport = 1;
  if (rapport2 == 0) rapport2 = 1;

  for (int i = y0; i >= y2; i--) {
    line(  x , i, xx , i, image, color);
    error2 += derror2;
    error3 += derror3; 

    if (error2 > dy) { 
      if ( (x2>x0 && x < x2) || (x2<x0 && x > x2) ) x += (x2>x0?rapport:-rapport); 
      error2 -= dy*2; 
    } 

    if (error3 > dy2) { 
      if ( (x2>x1 && xx < x2) || (x2<x1 && xx > x2) ) xx += (x2>x1?rapport2:-rapport2); 
      error3 -= dy2*2; 
    } 
    //std::cout << x << " " << y << " " << xx << " " << error2 << " " << error3 << " " << derror2 << " " << derror3 << " " << dy << " " << dy2<< std::endl;
  }

}



void dessin(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {

  /* at first sort the three vertices by y-coordinate descending so t2 is the topmost vertice */
  if (t0.y > t1.y) {
    std::swap(t0.y, t1.y);
    std::swap(t0.x, t1.x);
  }
  if (t1.y > t2.y)  {
    std::swap(t1.y, t2.y);
    std::swap(t1.x, t2.x);
  }
  if (t0.y > t1.y) {
    std::swap(t0.y, t1.y);
    std::swap(t0.x, t1.x);
  }



  if (t0.y == t1.y)
    {
      filledtoptriangle(t0, t1, t2, image, color);
    }
  else if (t1.y == t2.y)
    {
      filledbottomtriangle(t0, t1, t2, image, color);
    }
  else
    {

      Vec2i t4[1] = { Vec2i(  (int) (t2.x + ((float)(t1.y - t2.y) / (float)(t0.y - t2.y)) * (t0.x - t2.x)), t1.y ) };
      filledtoptriangle(t2, t1, t4[0], image, color);
      filledbottomtriangle(t1, t4[0], t0, image, color);
      
    }
}


std::vector<std::string> read(std::string name, std::vector<std::string>& listElements,  std::vector<std::string>& listTexture) {
  std::vector<std::string> coord;
  std::ifstream fichier(name.c_str());  // on ouvre le fichier en lecture
        if(fichier)  // si l'ouverture a réussi
	  {       
	    std::string ligne;
	    char delimiter = ' ';
	    while(getline(fichier, ligne))    {
	      std::vector<std::string> elements = split(ligne, delimiter);
	      if (elements.size() == 4) {
		if (elements[0] == "v") {
		  coord.push_back(elements[1] + ' ' + elements[2] + ' ' + elements[3]);
		}
		if (elements[0].compare("vt") ) {
		  listTexture.push_back(elements[1] + ' ' + elements[2] + ' ' + elements[3]);
		}
		
		if (elements[0] == "f") {
		  listElements.push_back(elements[1] + ' ' + elements[2] + ' ' + elements[3]);
		}
	      }
	    }
	    fichier.close();  // on ferme le fichier
	  }
        else  // sinon
	  std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
	return coord;
}



std::vector<float> calculVector3(std::vector<float> p1, std::vector<float> p2) {

  std::vector<float> vec12;
  vec12.push_back( p2[0] - p1[0]);
  vec12.push_back( p2[1] - p1[1]);
  vec12.push_back( p2[2] - p1[2]);

  return vec12;
}

std::vector<float> produitVectoriel(std::vector<float> v1, std::vector<float> v2) {

  std::vector<float> vec12;
  vec12.push_back( v1[1]*v2[2] - v1[2]*v2[1]);
  vec12.push_back( v1[2]*v2[0] - v1[0]*v2[2]);
  vec12.push_back( v1[0]*v2[1] - v1[1]*v2[0]);

  return vec12;
}

float produitScalaire(std::vector<float> v1, std::vector<float> v2) {

  float vec12 =  v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] ;

  return vec12;
}

float norm(std::vector<float> v) {
  return std::sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void normalize(std::vector<float>& v) {

  float n = 1/norm(v);
  
  v[0] = v[0] * (n);
  v[1] = v[1] * (n);
  v[2] = v[2] * (n);

}

Vec3f barycentric(Vec3f t0, Vec3f t1, Vec3f t2, Vec3f p) {

  std::vector<float> v1,v2;
  v1.push_back(t2.x - t0.x);
  v1.push_back(t1.x - t0.x);
  v1.push_back(t0.x - p.x);
 
  v2.push_back(t2.y - t0.y);
  v2.push_back(t1.y - t0.y);
  v2.push_back(t0.y - p.y);

  std::vector<float> v3 = produitVectoriel(v1, v2);
  if (std::abs((int)v3[2])<1) return Vec3f(-1,1,1);
 
  return Vec3f(1.f-(v3[0]+v3[1])/v3[2], v3[1]/v3[2], v3[0]/v3[2]); 
}


void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color) { 
  Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
  Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
  Vec2f clamp(image.get_width()-1, image.get_height()-1); 
  for (int i=0; i<3; i++) { 
    for (int j=0; j<2; j++) { 
      bboxmin[j] = std::max(0.f,        std::min(bboxmin[j], pts[i][j])); 
      bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j])); 
    } 
  } 
  Vec3f P; 
  for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) { 
    for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) { 
      Vec3f bc_screen  = barycentric(pts[0], pts[1], pts[2], P); 
      if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue; 
      P.z = 0;
      for (int i=0; i<3; i++) P.z += pts[i][2]*bc_screen[i];
      if (zbuffer[int(P.x+P.y*width)]<P.z) {
	zbuffer[int(P.x+P.y*width)] = P.z;
	image.set(P.x, P.y, color);
      }
    } 
  } 
} 


TGAColor interpolate(Vec3f *pts, Vec3f p) {

  float d1 = std::sqrt( pow(pts[0][0] - p.x , 2) + pow( pts[0][1] - p.y, 2));
  float d2 = std::sqrt( pow(pts[1][0] - p.x , 2) + pow( pts[1][1] - p.y, 2));
  float d3 = std::sqrt( pow(pts[2][0] - p.x , 2) + pow( pts[2][1] - p.y, 2));

  
  return white;

}

int main(int argc, char** argv) {
  std::vector<std::string> listElements, listTexture;
  TGAImage image(800, 800, TGAImage::RGB);
  //line(13,20,80,40,image,white);
  std::vector<std::string> vect = read("african_head.obj", listElements, listTexture);

  std::vector<std::string> elements;

  char delimiter = ' ';

  std::vector<std::string> d1, d2, d3, point;



  TGAColor tga;
  float *zbuffer = new float[width*height];
  
  for (int i=0; i < listElements.size() ; i++) { 
    elements = split(listElements[i], delimiter);
    std::vector<float> p1, p2, p3;
    d1 = split(elements[0].c_str(), '/') ;
    d2 = split(elements[1].c_str(), '/');
    d3 = split(elements[2].c_str(), '/');

    point = split(vect[ atoi (d1[0].c_str()) - 1 ], delimiter);
    int x0 = (atof( point[0].c_str()) +1.)*width/2.; 
    int y0 = (atof( point[1].c_str()) +1.)*height/2.; 
    int z0 = (atof( point[2].c_str()) +1.)*height/2.; 

    p1.push_back( atof( point[0].c_str() ));
    p1.push_back( atof( point[1].c_str() ));
    p1.push_back( atof( point[2].c_str() ));
  
    point = split(vect[ atoi (d2[0].c_str()) - 1 ], delimiter);
    int x1 = (atof( point[0].c_str()) +1.)*width/2.; 
    int y1 = (atof( point[1].c_str()) +1.)*height/2.; 
    int z1 = (atof( point[2].c_str()) +1.)*height/2.; 

    p2.push_back( atof( point[0].c_str() ));
    p2.push_back( atof( point[1].c_str() ));
    p2.push_back( atof( point[2].c_str() ));
  

    point = split(vect[ atoi (d3[0].c_str()) - 1 ], delimiter);
    int x2 = (atof( point[0].c_str()) +1.)*width/2.; 
    int y2 = (atof( point[1].c_str()) +1.)*height/2.; 
    int z2 = (atof( point[2].c_str()) +1.)*height/2.; 

    p3.push_back( atof( point[0].c_str() ));
    p3.push_back( atof( point[1].c_str() ));
    p3.push_back( atof( point[2].c_str() ));


    point = split( listTexture[ atoi (d1[1].c_str()) - 1 ], delimiter);
    Vec3f tex1( (atof( point[0].c_str())+1.0)*width/2, (atof( point[1].c_str())+1.0)*width/2, (atof( point[2].c_str())+1.0)*width/2 );
    /*
    point = split(listTexture[ atoi (d2[1].c_str()) - 1 ], delimiter);
    Vec3f tex2(atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()));

    point = split(listTexture[ atoi (d3[1].c_str()) - 1 ], delimiter);
    Vec3f tex3(atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()));
    */

    //line( x0, y0, x1, y1, image, white);
    //line( x2, y2, x1, y1, image, white);
    //line( x0, y0, x2, y2, image, white);


    Vec3f t0[3] = {Vec3f(x0, y0, z0),   Vec3f(x1, y1, z1),  Vec3f(x2, y2, z2)};


    Vec3f v = barycentric(t0[0], t0[1], t0[2], tex1);
    

    std::vector<float> light;
    light.push_back(0);
    light.push_back(0);
    light.push_back(-1);
      
    //std::vector<Vec2i> sort = sortVectorSide(t0[0], t0[1], t0[2]);

    std::vector<float> vec12 = calculVector3(p1, p3);
    std::vector<float> vec13 = calculVector3(p1, p2);
    
    std::vector<float> n = produitVectoriel(vec12, vec13);

    normalize(n);

    float intensite = produitScalaire(n,light);

    if (intensite > 0) {
      
      tga = TGAColor( intensite * 255, intensite * 255,  intensite * 255, 255);
      tga = TGAColor( v.x*width_tex*height_tex, v.y*width_tex*height_tex, v.z*width_tex*height_tex, 255);
      //dessin(t0[0], t0[1], t0[2], image, tga);
      triangle(t0, zbuffer, image, tga);
    }

    

    
  }
  

  Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(150, 160),  Vec2i(70, 80)};
  //filledtoptriangle(t0[0], t0[1], t0[2], image, red); 

  //dessin(t0[0], t0[1], t0[2], image, red);


  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}
