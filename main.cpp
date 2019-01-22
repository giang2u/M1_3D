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
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int width = 800;
const int height = 800;



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



void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) { 
    line(t0, t1, image, color); 
    line(t1, t2, image, color); 
    line(t2, t0, image, color); 
}

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
  int derror2 = std::abs(dx)*2; 
  int error2 = 0; 
  int y = y0; 

  int dx2 = x2-x1;
  int dy2 = y2-y1;
  int rapport2 = std::abs( ceil(  ((float)dx2/(float)dy2) - 0.5) );
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
  int derror2 = std::abs(dx)*2; 
  int error2 = 0; 
  int y = y0; 

  int dx2 = x2-x1;
  int dy2 = y1-y2;
  int rapport2 = std::abs( ceil(  ((float)dx2/(float)dy2) - 0.5) );
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


TGAColor tga = TGAColor( 255, 255,  255, 255);

float intensity = n*light_dir; 
    if (intensity>0) { 
      tga = TGAColor( intensity*255, intensity*255,  intensity*255, 255);
    } 

  
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
      filledtoptriangle(t0, t1, t2, image, tga);
    }
  else if (t1.y == t2.y)
    {
      filledbottomtriangle(t0, t1, t2, image, tga);
    }
  else
    {

intenx = (t2.x - t0.x) ^ (t1.x - t0.x);
inteny = (t2.y - t0.y) ^ (t1.y - t0.y);

norm = std::sqrt(intenx * intenx + inteny * inteny);
n = 


      Vec2i t4[1] = { Vec2i(  (int) (t2.x + ((float)(t1.y - t2.y) / (float)(t0.y - t2.y)) * (t0.x - t2.x)), t1.y ) };
      filledtoptriangle(t2, t1, t4[0], image, tga);
      filledbottomtriangle(t1, t4[0], t0, image, tga);
      
    }
}


std::vector<std::string> read(std::string name, std::vector<std::string>& listElements) {
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
		
		if (elements[0] == "f") {
		  listElements.push_back(elements[1] + ' ' + elements[2] + ' ' + elements[3] );
		}
	      }
	    }
	    fichier.close();  // on ferme le fichier
	  }
        else  // sinon
	  std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
	return coord;
}


int main(int argc, char** argv) {
  std::vector<std::string> listElements;
  TGAImage image(800, 800, TGAImage::RGB);
  //line(13,20,80,40,image,white);
  std::vector<std::string> vect = read("african_head.obj", listElements);

  
  std::vector<std::string> elements;

  char delimiter = ' ';

  std::vector<std::string> d1, d2, d3, point;
  
    for (int i=0; i < listElements.size() ; i++) { 
      elements = split(listElements[i], delimiter);

      d1 = split(elements[0].c_str(), '/') ;
      d2 = split(elements[1].c_str(), '/');
      d3 = split(elements[2].c_str(), '/');

      point = split(vect[ atoi (d1[0].c_str()) - 1 ], delimiter);
      int x0 = (atof( point[0].c_str()) +1.)*width/2.; 
      int y0 = (atof( point[1].c_str()) +1.)*height/2.; 

      point = split(vect[ atoi (d2[0].c_str()) - 1 ], delimiter);
      int x1 = (atof( point[0].c_str()) +1.)*width/2.; 
      int y1 = (atof( point[1].c_str()) +1.)*height/2.; 

      point = split(vect[ atoi (d3[0].c_str()) - 1 ], delimiter);
      int x2 = (atof( point[0].c_str()) +1.)*width/2.; 
      int y2 = (atof( point[1].c_str()) +1.)*height/2.; 

      line( x0, y0, x1, y1, image, white);

      line( x2, y2, x1, y1, image, white);
      line( x0, y0, x2, y2, image, white);

      Vec2i t0[3] = {Vec2i(x0, y0),   Vec2i(x1, y1),  Vec2i(x2, y2)};
      dessin(t0[0], t0[1], t0[2], image, white);
    }
  

  Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(150, 160),  Vec2i(70, 80)};
  //filledtoptriangle(t0[0], t0[1], t0[2], image, red); 

 // dessin(t0[0], t0[1], t0[2], image, red);


  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}
