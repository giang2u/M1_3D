#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);





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


void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) { 
    if (t0.y==t1.y && t0.y==t2.y) return; // I dont care about degenerate triangles 
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
    if (t0.y>t1.y) std::swap(t0, t1); 
    if (t0.y>t2.y) std::swap(t0, t2); 
    if (t1.y>t2.y) std::swap(t1, t2); 
    int total_height = t2.y-t0.y; 
    for (int i=0; i<total_height; i++) { 
        bool second_half = i>t1.y-t0.y || t1.y==t0.y; 
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y; 
        float alpha = (float)i/total_height; 
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here 
        Vec2i A =               t0 + (t2-t0)*alpha; 
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta; 
        if (A.x>B.x) std::swap(A, B); 
        for (int j=A.x; j<=B.x; j++) { 
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y 
        } 
    } 
}

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
  int width = 800;
  int height = 800;
  std::vector<std::string> elements;

  /*
    for (int i=0; i<vect.size(); i++) { 
    elements = split(vect[i], ' ');
    int x0 = (atof( elements[0].c_str()) +1.)*width/2.; 
    int y0 = (atof( elements[1].c_str()) +1.)*height/2.; 
    image.set(x0, y0, white);
    }
  */

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

      triangle(t0[0], t0[1], t0[2], image, white);

      //std::cout << x0 << ' ' << y0 << ' ' << x1 << ' ' << y1 << std::endl;
      //line(d1[0], d1[1], d3[0], d3[1], image, white);
      //line(d2[0], d2[1], d3[0], d3[1], image, white);

    }
  

	
  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  return 0;
}
