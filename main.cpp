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
const int depth  = 255;

int *zbuffer = NULL;
Vec3f light_dir(0,0,-1);
Vec3f eye_center(0,0,3);
Vec3f camera(0,0,3);
Vec3f center(0,0,0);

std::vector<float> calculVector3(Vec3f p1, Vec3f p2) {

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

Vec3f produitVectoriel(Vec3f v1, Vec3f v2) {

	Vec3f vec12;
	vec12.x = ( v1[1]*v2[2] - v1[2]*v2[1]);
	vec12.y = ( v1[2]*v2[0] - v1[0]*v2[2]);
	vec12.z = ( v1[0]*v2[1] - v1[1]*v2[0]);

	return vec12;
}


float produitScalaire(std::vector<float> v1, std::vector<float> v2) {

	float vec12 =  v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] ;

	return vec12;
}

float produitScalaire(Vec3f v1, Vec3f v2) {

	float vec12 =  v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] ;

	return vec12;
}

float norm(std::vector<float> v) {
	return std::sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

std::vector<float> normalize(std::vector<float>& v) {

	float n = 1/norm(v);

	v[0] = v[0] * (n);
	v[1] = v[1] * (n);
	v[2] = v[2] * (n);

	return v;
}

float norm(Vec3f v) {
	return std::sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

Vec3f normalize(Vec3f v) {

	float n = 1/norm(v);

	v[0] = v[0] * (n);
	v[1] = v[1] * (n);
	v[2] = v[2] * (n);

	return v;
}


Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}

Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}

Matrix lookat(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f z = normalize(eye_center);
    Vec3f x = normalize(produitVectoriel(up,z));
    Vec3f y = normalize(produitVectoriel(z,x));
    Matrix res = Matrix::identity(4);
    for (int i=0; i<3; i++) {
        res[0][i] = x[i];
        res[1][i] = y[i];
        res[2][i] = z[i];
        res[i][3] = -center[i];
    }
    return res;
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
	//if ( dy == 0) std::cout << rapport  << "    COUCOU" << std::endl; rapport = 0;
	int derror2 = std::abs(dx)*2;
	int error2 = 0;
	//int y = y0;

	int dx2 = x2-x1;
	int dy2 = y2-y1;
	int rapport2 = std::abs( ceil(  ((float)dx2/(float)dy2) - 0.5) );

	//if ( dy2 == 0) std::cout << rapport2 << "    COUCOU" << std::endl; rapport2 = 0;
	int derror3 = std::abs(dx2)*2;
	int error3 = 0;
	//int yy = y2;

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
	//int y = y0;

	int dx2 = x2-x1;
	int dy2 = y1-y2;
	int rapport2 = std::abs( ceil(  ((float)dx2/(float)dy2) - 0.5) );
	int derror3 = std::abs(dx2)*2;
	int error3 = 0;
	//int yy = y2;

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


std::vector<std::string> read(std::string name, std::vector<std::string>& listElements,  std::vector<std::string>& listTexture, std::vector<std::string>& listShade) {
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
					listElements.push_back(elements[1] + ' ' + elements[2] + ' ' + elements[3]);
				}
			}
			if (elements.size() == 5) {
				if (elements[0].compare("vt") == 0) {
						listTexture.push_back(elements[2] + ' ' + elements[3] + ' ' + elements[4]);
						//std::cout << elements[2] << "  " << elements[3] << ' ' << elements[4] << std::endl;
				}
				if (elements[0].compare("vn") == 0) {
						listShade.push_back(elements[2] + ' ' + elements[3] + ' ' + elements[4]);
						//std::cout << elements[0] << "  " << elements[2] << "  " << elements[3] << ' ' << elements[4] << std::endl;
				}
			}
		}
		fichier.close();  // on ferme le fichier
	}
	else  // sinon
		std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
	return coord;
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


void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color, TGAImage &texture, Vec3f *text, float intensite, Vec3f *ptts, float *inten) {

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
			TGAColor c1, c2,c3;
			for (int i=0; i<3; i++) {
				P.z += pts[i][2]*bc_screen[i];
			}
			P.x = (int)P.x;
			P.y = (int)P.y;
			if (zbuffer[int(P.x+P.y*width)]<P.z) {
				zbuffer[int(P.x+P.y*width)] = P.z;

				float px1 = text[0].x* bc_screen[0]+ text[1].x* bc_screen[1] + text[2].x* bc_screen[2];
				float py1 = text[0].y* bc_screen[0]+ text[1].y* bc_screen[1] + text[2].y* bc_screen[2];
		        
				// cetait inverser en y donc jinverse pour avoir la bonne coordonnee
				py1 = 1 - py1;
				
				float ya = pts[1].y + (P.x - pts[1].x) * (pts[2].y - pts[1].y) / (pts[2].x - pts[1].x);

				float yb = pts[2].y + (P.x - pts[2].x) * (pts[3].y - pts[2].y) / (pts[3].x - pts[2].x);

				//float Ia =  ((ya - pts[1].y) / (pts[0].y - pts[1].y)  ) * bc_screen[0] +  (( pts[0].y - ya) / (pts[0].y - pts[1].y)  ) * bc_screen[1] ;
				float Ia =  ((P.x - pts[1].x) / (pts[0].x - pts[1].x)  ) * bc_screen[0] +  (( pts[0].x - P.x) / (pts[0].x - pts[1].x)  ) * bc_screen[1] ;

				//float Ib =  ((yb - pts[1].y) / (pts[2].y - pts[1].y)  ) * bc_screen[2] +  (( pts[2].y - yb) / (pts[2].y - pts[1].y)  ) * bc_screen[1] ;
				float Ib =  ((P.x - pts[1].x) / (pts[2].x - pts[1].x)  ) * bc_screen[2] +  (( pts[2].x - P.x) / (pts[2].x - pts[1].x)  ) * bc_screen[1] ;

				float Ip = ((yb - P.y) / (yb - ya)) * Ia + ((P.y - ya) / (yb - ya)) * Ib;
				

				float intensitee = inten[0] * bc_screen[0] + inten[1] * bc_screen[1] + inten[2] * bc_screen[2];
				
				TGAColor c = texture.get( px1 * texture.get_width() ,   py1 * texture.get_height() );

				Vec3f n = produitVectoriel((pts[1] - pts[0]), (pts[2] - pts[0]));

				/*
				Vec3f u = n - light_dir;
				Vec3f r = light_dir + u + u;

				Vec3f spec = produitScalaire(eye , r);
				*/

				image.set(P.x, P.y, c * (-intensitee));
			}
		}
	}
} 


int main(int argc, char** argv) {
  std::vector<std::string> listElements, listTexture, listShade;
	TGAImage image(800, 800, TGAImage::RGB);
	TGAImage texture;
	texture.read_tga_file("diablo3_pose_diffuse.tga");
	//line(13,20,80,40,image,white);
	std::vector<std::string> vect = read("diablo3_pose.obj", listElements, listTexture, listShade);

	std::vector<std::string> elements;

	char delimiter = ' ';

	std::vector<std::string> d1, d2, d3, point;

	TGAColor tga;
	float *zbuffer = new float[width*height];


	Matrix ModelView  = lookat(eye_center, center, Vec3f(0,1,0));
	Matrix R = Matrix::identity(4);
	Matrix T = Matrix::identity(4);
	Matrix Projection = Matrix::identity(4);
	Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
	Projection[3][2] = -1.f/ norm(eye_center);


	for (int i=0; i < listElements.size() ; i++) {
		elements = split(listElements[i], delimiter);
		Vec3f p1, p2, p3;
		d1 = split(elements[0].c_str(), '/') ;
		d2 = split(elements[1].c_str(), '/');
		d3 = split(elements[2].c_str(), '/');

		point = split(vect[ atoi (d1[0].c_str()) - 1 ], delimiter);
		int x0 = (atof( point[0].c_str()) +1.)*width/2.;
		int y0 = (atof( point[1].c_str()) +1.)*height/2.;
		int z0 = (atof( point[2].c_str()) +1.)*depth/2.;

		p1 = { atof( point[0].c_str() ), atof( point[1].c_str()) , atof( point[2].c_str()) };

		point = split(vect[ atoi (d2[0].c_str()) - 1 ], delimiter);
		int x1 = (atof( point[0].c_str()) +1.)*width/2.;
		int y1 = (atof( point[1].c_str()) +1.)*height/2.;
		int z1 = (atof( point[2].c_str()) +1.)*depth/2.;

		p2 = { atof( point[0].c_str() ), atof( point[1].c_str()) , atof( point[2].c_str()) };

		point = split(vect[ atoi (d3[0].c_str()) - 1 ], delimiter);
		int x2 = (atof( point[0].c_str()) +1.)*width/2.;
		int y2 = (atof( point[1].c_str()) +1.)*height/2.;
		int z2 = (atof( point[2].c_str()) +1.)*depth/2.;

		p3 = { atof( point[0].c_str() ), atof( point[1].c_str()) , atof( point[2].c_str()) };



		Vec3f p0[3] = {p1, p2, p3};

		point = split( listTexture[ atoi (d1[1].c_str()) - 1 ], delimiter);
		Vec3f tex1( atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()) );


		point = split(listTexture[ atoi (d2[1].c_str()) - 1 ], delimiter);
		Vec3f tex2( atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()) );

		point = split(listTexture[ atoi (d3[1].c_str()) - 1 ], delimiter);
		Vec3f tex3( atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()) );

		Vec3f text[3] = { tex1, tex2, tex3 };

		point = split( listShade[ atoi (d1[2].c_str()) - 1 ], delimiter);
		Vec3f shade1( atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()) );


		point = split(listShade[ atoi (d2[2].c_str()) - 1 ], delimiter);
		Vec3f shade2( atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()) );

		point = split(listShade[ atoi (d3[2].c_str()) - 1 ], delimiter);
		Vec3f shade3( atof( point[0].c_str()), atof( point[1].c_str()), atof( point[2].c_str()) );

		Vec3f shade[3] = { shade1, shade2, shade3 };

		Vec3f tt1 = normalize(shade1);
	        Vec3f tt2 = normalize(shade2);	
	        Vec3f tt3 = normalize(shade3);
		float inten1 = produitScalaire(tt1, light_dir);
		float inten2 = produitScalaire(tt2, light_dir);
		float inten3 = produitScalaire(tt3, light_dir);

		float inten[3] = {inten1, inten2, inten3};
		
		R[0][0] = R[2][2] = cos(2);
		R[0][2] = -sin(2);
		R[2][0] = sin(2);

		T[0][3] = 2;

		//Vec3f t0[3] = {Vec3f(x0, y0, z0),   Vec3f(x1, y1, z1),  Vec3f(x2, y2, z2)};
		Vec3f t0[3] = { m2v(ViewPort*Projection*ModelView*v2m(p1)),  m2v(ViewPort*Projection*ModelView*v2m(p2)),  m2v(ViewPort*Projection*ModelView*v2m(p3)) };

		//std::cout << t0[0] << std::endl;

		std::vector<float> light;
		light.push_back(0);
		light.push_back(0);
		light.push_back(-1);


		std::vector<float> vec12 = calculVector3(p1, p3);
		std::vector<float> vec13 = calculVector3(p1, p2);

		std::vector<float> n = produitVectoriel(vec12, vec13);

		normalize(n);

		float intensite = produitScalaire(n,light);

		//	if (intensite > 0) {

		//std::cout << intensite << std::endl;
		// tga = TGAColor( intensite * 255, intensite * 255,  intensite * 255, 255);
		  triangle(t0, zbuffer, image, tga, texture, text, intensite, p0, inten);
		  //	}

	}

/*
	Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(150, 160),  Vec2i(70, 80)};
	filledtoptriangle(t0[0], t0[1], t0[2], image, red);
	dessin(t0[0], t0[1], t0[2], image, red);
*/

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
