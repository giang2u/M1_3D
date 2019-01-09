#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <sstream>
#include <cstdlib>

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


std::vector<std::string> read(std::string name, TGAImage &image) {

  std::vector<std::string> coord;
  std::ifstream fichier(name.c_str());  // on ouvre le fichier en lecture
        if(fichier)  // si l'ouverture a réussi
	  {       
	    std::string ligne;
	    char delimiter = ' ';
	    while(getline(fichier, ligne))    {
	      //std::string token = s.substr(0, s.find(delimiter));
	      std::vector<std::string> elements = split(ligne, delimiter);
	      if (elements.size() == 4) {
		if (elements[0] == "v") {
		  coord.push_back(elements[1] + ' ' + elements[2] + ' ' + elements[3]);
		  /*
		  int x0 = (atof( elements[1].c_str()) +1.)*800/2.; 
		  int y0 = (atof( elements[2].c_str()) +1.)*800/2.; 
		  //line(x0, y0, x1, y1, image, white); 
		  image.set(x0, y0, white);
		  */
		}
		/*
		if (elements[0] == "f") {
		  std::vector<std::string> d1, d2, d3;
		  d1 = split(coord[ atoi( elements[1].c_str() )], delimiter);
		  d2 = split(coord[ atoi( elements[2].c_str() )], delimiter);
		  d3 = split(coord[ atoi( elements[3].c_str() )], delimiter);
		  line( atoi( d1[0].c_str() ), atoi( d1[1].c_str() ), atoi( d2[0].c_str() ), atoi( d2[1].c_str() ), image, white);
		  
//line(d1[0], d1[1], d3[0], d3[1], image, white);
		  //line(d2[0], d2[1], d3[0], d3[1], image, white);
		  }*/
	      }
	    }
	    fichier.close();  // on ferme le fichier
        }
        else  // sinon
	  std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
	return coord;
}


int main(int argc, char** argv) {
        TGAImage image(800, 800, TGAImage::RGB);
	//line(13,20,80,40,image,white);
	std::vector<std::string> vect = read("african_head.obj", image);
	
	
	  
	int width = 800;
	int height = 800;
	std::vector<std::string> elements;
		  
	for (int i=0; i<vect.size(); i++) { 
	  elements = split(vect[i], ' ');
	  int x0 = (atof( elements[0].c_str()) +1.)*width/2.; 
	  int y0 = (atof( elements[1].c_str()) +1.)*height/2.; 
	   image.set(x0, y0, white);
	}

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output.tga");
        return 0;
}
