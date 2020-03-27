// Timothy Stander
// Chess animation starter kit.

#include <stdlib.h>
//#include <stdio.h>
#include <cmath>
//#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include "../pglut.h"
//#include "glut.h"
//#include "graphics.h"


// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = {0.7, 0.1, 0.2, 1.0};
GLdouble greenMaterial[] = {0.1, 0.7, 0.4, 1.0};
GLdouble brightGreenMaterial[] = {0.1, 0.9, 0.1, 1.0};
GLdouble blueMaterial[] = {0.1, 0.2, 0.7, 1.0};
GLdouble whiteMaterial[] = {1.0, 1.0, 1.0, 1.0};

double screen_x = 600;
double screen_y = 500;
void help(std::string l,double x[]){
	std::cout<<l<<": "<<x[0]<<" "<<x[1]<<" "<<x[2]<<std::endl;
}

std::vector<std::string> splitBy(std::string s, char delim){
	std::stringstream ss(s);
	std::vector<std::string> r;
	
	while (ss){
		std::string curr;
		char c=-1;
		while (c!=delim&&ss) {
			c=ss.get();
			curr+=c;
		}
		curr.pop_back();
		r.push_back(curr);
	}
	return r;
}
std::string prettyVector(std::vector<std::string> v){
	std::string s;
	s+="[";
	for (int i=0;i<v.size();i++){
		s+='"';
		s+=v[i];
		s+="\",";
	}
	s.pop_back();
	s+="]";
	return s;
}
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];
	
	// Take the cross product of v1 and v2, to find the std::vector perpendicular to both.
	n[0] = v1[1]*v2[2] - v1[2]*v2[1];
	n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
	n[2] = v1[0]*v2[1] - v1[1]*v2[0];

	double size = std::sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= -size;
	n[1] /= -size;
	n[2] /= -size;
}
struct matrix4x4d{
	double values[16];
	matrix4x4d(){identity();};
	void identity(){
		values[0]=1;values[1]=0;values[2]=0;values[3]=0;
		values[4]=0;values[5]=1;values[6]=0;values[7]=0;
		values[8]=0;values[9]=0;values[10]=1;values[11]=0;
		values[12]=0;values[13]=0;values[14]=0;values[15]=1;
	}
	matrix4x4d& operator*=(matrix4x4d other){
		double temps[3];
		temps[0]=values[0];
		temps[1]=values[1];
		temps[2]=values[2];
		values[0]=temps[0]*other.values[0]+temps[1]*other.values[4]+temps[2]*other.values[8]+values[3]*other.values[12];
		values[1]=temps[0]*other.values[1]+temps[1]*other.values[5]+temps[2]*other.values[9]+values[3]*other.values[13];
		values[2]=temps[0]*other.values[2]+temps[1]*other.values[6]+temps[2]*other.values[10]+values[3]*other.values[14];
		values[3]=temps[0]*other.values[3]+temps[1]*other.values[7]+temps[2]*other.values[11]+values[3]*other.values[15];

		temps[0]=values[4];
		temps[1]=values[5];
		temps[2]=values[6];
		values[4]=temps[0]*other.values[0]+temps[1]*other.values[4]+temps[2]*other.values[8]+values[7]*other.values[12];
		values[5]=temps[0]*other.values[1]+temps[1]*other.values[5]+temps[2]*other.values[9]+values[7]*other.values[13];
		values[6]=temps[0]*other.values[2]+temps[1]*other.values[6]+temps[2]*other.values[10]+values[7]*other.values[14];
		values[7]=temps[0]*other.values[3]+temps[1]*other.values[7]+temps[2]*other.values[11]+values[7]*other.values[15];

		temps[0]=values[8];
		temps[1]=values[9];
		temps[2]=values[10];
		values[8]=temps[0]*other.values[0]+temps[1]*other.values[4]+temps[2]*other.values[8]+values[11]*other.values[12];
		values[9]=temps[0]*other.values[1]+temps[1]*other.values[5]+temps[2]*other.values[9]+values[11]*other.values[13];
		values[10]=temps[0]*other.values[2]+temps[1]*other.values[6]+temps[2]*other.values[10]+values[11]*other.values[14];
		values[11]=temps[0]*other.values[3]+temps[1]*other.values[7]+temps[2]*other.values[11]+values[11]*other.values[15];

		temps[0]=values[12];
		temps[1]=values[13];
		temps[2]=values[14];
		values[12]=temps[0]*other.values[0]+temps[1]*other.values[4]+temps[2]*other.values[8]+values[15]*other.values[12];
		values[13]=temps[0]*other.values[1]+temps[1]*other.values[5]+temps[2]*other.values[9]+values[15]*other.values[13];
		values[14]=temps[0]*other.values[2]+temps[1]*other.values[6]+temps[2]*other.values[10]+values[15]*other.values[14];
		values[15]=temps[0]*other.values[3]+temps[1]*other.values[7]+temps[2]*other.values[11]+values[15]*other.values[15];
		return *this;
	};
	matrix4x4d operator*(matrix4x4d other){
		matrix4x4d ret(*this);
		ret*=other;
		return ret;
	}
};
struct vector3d{
	double arr[3];
	vector3d(double x_,double y_,double z_):arr{x_,y_,z_}{};
	vector3d():arr{0,0,0}{};
	vector3d(const vector3d& v):arr{v.arr[0],v.arr[1],v.arr[2]}{}
	double mag(){return std::sqrt(arr[0]*arr[0]+arr[1]*arr[1]+arr[2]*arr[2]);};
	vector3d operator-(vector3d v){return vector3d(arr[0]-v.arr[0],arr[1]-v.arr[1],arr[2]-v.arr[2]);};
	vector3d operator+(vector3d v){return vector3d(arr[0]+v.arr[0],arr[1]+v.arr[1],arr[2]+v.arr[2]);};
	vector3d operator/(double s){return vector3d(arr[0]/s,arr[1]/s,arr[2]/s);};
	vector3d operator*(double s){return vector3d(arr[0]*s,arr[1]*s,arr[2]*s);};
	static vector3d cross(vector3d v1, vector3d v2){return vector3d(v1.arr[1]*v2.arr[2]-v1.arr[2]*v2.arr[1],v1.arr[2]*v2.arr[0]-v1.arr[0]-v2.arr[2],v1.arr[0]*v2.arr[1]-v1.arr[1]*v2.arr[0]);};
	static vector3d normalize(vector3d v){return v/v.mag();};
	static double dot(vector3d v1, vector3d v2){return v1.arr[0]*v2.arr[0]+v1.arr[1]*v2.arr[1]+v1.arr[2]*v2.arr[2];}
	vector3d& operator=(const vector3d& r){arr[0]=r.arr[0];arr[1]=r.arr[1];arr[2]=r.arr[2];return *this;}
	friend std::ostream& operator<<(std::ostream& os, const vector3d& v);
};
std::ostream& operator<<(std::ostream& os, const vector3d& v){
	return os<<"<"<<v.arr[0]<<", "<<v.arr[1]<<", "<<v.arr[2]<<">";
}
template <class myType>
std::ostream& operator<<(std::ostream& os, const std::vector<myType>& v){
	os<<"[";
	if (v.size()>0){
		for (int i=0;i<v.size()-1;i++){
			os<<v[i];
			os<<", ";
		}
		os<<v[v.size()-1];
	}
	os<<"]";
	return os;
}


struct vector2d{
	double x,y;
};

struct face{
	int v1,v2,v3,t1,t2,t3,n1,n2,n3;
};
struct Mesh{
	std::vector<vector3d> vertices;
	std::vector<vector3d> normals;
	std::vector<vector2d> textureuv;
	std::vector<face> faces;
	std::string name;
	void draw(){
		//glScaled(400,400,400);
		glBegin(GL_TRIANGLES);
		for (int i = 0;i<faces.size();i++){
			glNormal3dv(normals[faces[i].n1].arr);
			glVertex3dv(vertices[faces[i].v1].arr);

			glNormal3dv(normals[faces[i].n2].arr);
			glVertex3dv(vertices[faces[i].v2].arr);

			glNormal3dv(normals[faces[i].n3].arr);
			glVertex3dv(vertices[faces[i].v3].arr);
		}
		glEnd();
	}
};
struct Model{
	std::vector<Mesh> Meshes;
	void draw(){for(int i=0;i<Meshes.size();i++){Meshes[i].draw();}}
};
Model LoadOBJ(std::istream& in){
	Model m;
	m.Meshes.push_back(Mesh());
	int cme=0;
	Mesh& me=m.Meshes[cme];
	while (in){
		std::string line;
		getline(in,line);
		std::stringstream l(line);
		std::string type;
		l>>type;
		if (type=="#"){
		}
		else if (type=="v"){
			double x,y,z;
			l>>x>>y>>z;
			vector3d v(x*400,y*400,z*400);
			std::cout<<v<<std::endl;
			me.vertices.push_back(v);
			//std::cout<<me.vertices[me.vertices.size()-1]<<" "<<me.vertices[0]<<std::endl;
		}
		else if (type=="vn"){
			double x,y,z;
			l>>x>>y>>z;
			me.normals.push_back(vector3d(x,y,z));
		}
		else if (type=="f"){
			std::vector<std::vector<std::string>> allparts;
			while (l){
				std::string value;
				l>>value>>std::ws;
				allparts.push_back(splitBy(value,'/'));
			}
			for (int i=1;i<allparts.size()-1;i++){
				face f;
				std::stringstream(allparts[0][0])>>f.v1;
				std::stringstream(allparts[i][0])>>f.v2;
				std::stringstream(allparts[i+1][0])>>f.v3;
				
				if (abs((me.vertices[f.v1-1]-me.vertices[f.v2-1]).arr[1])>200){
					std::cout<<"f1 f2: "<<me.vertices[f.v1-1]<<me.vertices[f.v2-1]<<std::endl;
				}
				if (abs((me.vertices[f.v1-1]-me.vertices[f.v3-1]).arr[1])>200){
					std::cout<<"f1 f3: "<<me.vertices[f.v1-1]<<me.vertices[f.v3-1]<<std::endl;
				}
				if (abs((me.vertices[f.v3-1]-me.vertices[f.v2-1]).arr[1])>200){
					std::cout<<"f3 f2: "<<me.vertices[f.v3-1]<<me.vertices[f.v2-1]<<std::endl;
				}
				if (allparts[0].size()>2){
					std::stringstream(allparts[0][2])>>f.n1;
				}
				else if (f.v1<me.normals.size()){
					f.n1=f.v1;
				}
				else{
					f.n1=0;
				}
				if (allparts[i].size()>2){
					std::stringstream(allparts[i][2])>>f.n2;
				}
				else if (f.v2<me.normals.size()){
					f.n2=f.v2;
				}
				else{
					f.n2=0;
				}
				if (allparts[i+1].size()>2){
					std::stringstream(allparts[i+1][2])>>f.n3;
				}
				else if (f.v3<me.normals.size()){
					f.n3=f.v3;
				}
				else{
					f.n3=0;
				}
				f.v1-=1;
				f.v2-=1;
				f.v3-=1;
				f.n1-=1;
				f.n2-=1;
				f.n3-=1;
				me.faces.push_back(f);
			}
		}
		else{
			std::cout<<"I don't recognize '"<<type<<"'"<<std::endl;
		}
	}/*
	for (int i=0;i<m.Meshes.size();i++){
		me=m.Meshes[i];
		for (int j=0;j<me.faces.size();j++){
			face& f = me.faces[j];
			vector3d A=me.vertices[f.v2]-me.vertices[f.v1];
			vector3d B=me.vertices[f.v3]-me.vertices[f.v1];
			vector3d C=vector3d::normalize(vector3d::cross(A,B));
			if (f.n1==-1||f.n2==-1||f.n3==-1){
				me.normals.push_back(C);
				if (f.n1==-1){
					f.n1=me.normals.size()-1;
				}
				if (f.n2==-1){
					f.n2=me.normals.size()-1;
				}
				if (f.n3==-1){
					f.n3=me.normals.size()-1;
				}
			}
		}
	}*/
	return m;
}
struct vector9i{
	double vx,vy,vz,nx,ny,nz;
};
class OBJModel{
	public:
		OBJModel(std::istream& fin);
		void draw();
	private:
		double mX,mY,mZ;
		std::vector<vector3d> mNormals;
		std::vector<vector3d> mVerts;
		std::vector<vector9i> mTriangles;

};/*
OBJModel::OBJModel(std::istream& fin){
	while (fin){
		std::string value;
		fin>>value>>std::ws;
		std::string restofline;
		getline(fin,restofline);
		if (value=="#"){
		}
		else if (value=="v"){
			std::vector<std::string> values=splitBy(restofline,' ');
			vector3d v;
			std::stringstream(values[0])>>v.x;
			std::stringstream(values[1])>>v.y;
			std::stringstream(values[2])>>v.z;
			v.x*=400;
			v.y*=400;
			v.z*=400;
			mVerts.push_back(v);
		}
		else if (value=="vn"){
			std::vector<std::string> values=splitBy(restofline,' ');
			vector3d v;
			std::stringstream(values[0])>>v.x;
			std::stringstream(values[1])>>v.y;
			std::stringstream(values[2])>>v.z;
			mNormals.push_back(v);
		}
		else if (value=="f"){
			std::vector<std::string> values=splitBy(restofline,' ');
			std::vector<std::vector<std::string>> allparts;
			for (int i=0;i<values.size();i++){
				value=values[i];
				allparts.push_back(splitBy(value,'/'));
			}
			for (int i=1;i<values.size()-1;i++){
				vector9i v;
				std::stringstream(allparts[0][0])>>v.vx;
				std::stringstream(allparts[i][0])>>v.vy;
				std::stringstream(allparts[i+1][0])>>v.vz;

				std::stringstream(allparts[0][2])>>v.nx;
				std::stringstream(allparts[i][2])>>v.ny;
				std::stringstream(allparts[i+1][2])>>v.nz;
				v.vx-=1;
				v.vy-=1;
				v.vz-=1;
				v.nx-=1;
				v.ny-=1;
				v.nz-=1;*//*
				double a[3];
				double b[3];
				double c[3];
				double d[3];
				lv3d(d,mVerts[v.vx]);
				a[0]=d[0];
				b[0]=d[1];
				c[0]=d[2];
				lv3d(d,mVerts[v.vy]);
				a[1]=d[0];
				b[1]=d[1];
				c[1]=d[2];
				lv3d(d,mVerts[v.vz]);
				a[2]=d[0];
				b[2]=d[1];
				c[2]=d[2];
				FindTriangleNormal(a,b,c,d);
				vector3d e;
				e.x=d[0];
				e.y=d[1];
				e.z=d[2];
				mNormals.push_back(e);
				v.nx=v.ny=v.nz=mNormals.size()-1;*//*
				mTriangles.push_back(v);
			}
		}
		else{
			std::cout<<"I don't recognize '"<<value<<"'"<<std::endl;
		}
	}
	double s=1000; // Infinity
	for(int i=0;i<mVerts.size();i++){
		if (mVerts[i].y<s){
			s=mVerts[i].y;
		}
	}
	for(int i=0;i<mVerts.size();i++){
		mVerts[i].y-=s;
	}
}

void OBJModel::draw(){
	glBegin(GL_TRIANGLES);
	for (int i = 0;i<mTriangles.size();i++){
		glNormal3dv(mNormals[mTriangles[i].nx].arr);
		glVertex3dv(mVerts[mTriangles[i].vx].arr);

		glNormal3dv(mNormals[mTriangles[i].ny].arr);
		glVertex3dv(mVerts[mTriangles[i].vy].arr);

		glNormal3dv(mNormals[mTriangles[i].nz].arr);
		glVertex3dv(mVerts[mTriangles[i].vz].arr);
	}
	glEnd();
}
*/
double GetTime()
{
	static clock_t start_time = clock();
	clock_t current_time = clock();
	double total_time = double(current_time - start_time) / CLOCKS_PER_SEC;
	return total_time;
}
/*
// Outputs a std::string of text at the specified location.
void text_output(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}
*/
// Given the three triangle points x[0],y[0],z[0],
//		x[1],y[1],z[1], and x[2],y[2],z[2],
//		Finds the normal std::vector n[0], n[1], n[2].


// Loads the given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
void DrawPiece(const char filename[])
{
	// Try to open the given file.
	char buffer[200];
	std::ifstream in(filename);
	if(!in)
	{
		std::cerr << "Error. Could not open " << filename << std::endl;
		exit(1);
	}

	double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
	int done = false;
	int verts = 0; // vertices in the current polygon
	int polygons = 0; // total polygons in this file.
	do
	{
		in.getline(buffer, 200); // get one line (point) from the file.
		int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
		done = in.eof();
		if(!done)
		{
			if(count == 3) // if this line had an x,y,z point.
			{
				verts++;
			}
			else // the line was empty. Finish current polygon and start a new one.
			{
				if(verts>=3)
				{
					glBegin(GL_POLYGON);
					double n[3];
					FindTriangleNormal(x, y, z, n);
					glNormal3dv(n);
					for(int i=0; i<verts; i++)
					{
						glVertex3d(x[i], y[i], z[i]);
					}
					glEnd(); // end previous polygon
					polygons++;
					verts = 0;
				}
			}
		}
	}
	while(!done);

	if(verts>0)
	{
		std::cerr << "Error. Extra vertices in file " << filename << std::endl;
		exit(1);
	}

}

// NOTE: Y is the UP direction for the chess pieces.
double eye[3] = {4500, 8000, -4000}; // pick a nice vantage point.
double at[3]  = {4500, 0,     4000};
//
// GLUT callback functions
//

// As t goes from t0 to t1, set v between v0 and v1 accordingly.
void Interpolate(double t, double t0, double t1,
	double & v, double v0, double v1)
{
	double ratio = (t - t0) / (t1 - t0);
	if (ratio < 0)
		ratio = 0;
	if (ratio > 1)
		ratio = 1;
	v = v0 + (v1 - v0)*ratio;
}

std::ifstream gf("King.obj");
Model gKingModel = LoadOBJ(gf);
std::ifstream kf("Knight.obj");
//Model gKnightModel = LoadOBJ(kf);
//std::ifstream pf("Pawn.obj");
//Model gPawnModel = LoadOBJ(pf);/*
/*std::ifstream qf("Queen.obj");
Model gQueenModel = LoadOBJ(qf);
std::ifstream rf("Rook.obj");
Model gRookModel = LoadOBJ(rf);
std::ifstream bf("Bishop.obj");
Model gBishopModel = LoadOBJ(rf);*/
int gC=0;
double tc=0;
double ds=0;
double ps=GetTime();
// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	double t = GetTime();
	ds=t-ps;
	ps=t;
	tc+=ds;
	gC+=1;
	if (tc>1){
		std::cout<<gC<<std::endl;
		tc=0;
		gC=0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,1,0); // Y is up!

	// Set the color for one side (white), and draw its 16 pieces.
	GLfloat mat_amb_diff1[] = {0.8f, 0.9f, 0.5f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
	/*
			v.x*=1000;
			v.y*=1000;
			v.z*=1000;
			v.x+=3000;
			v.z+=5000;*/
	/*double q;
	Interpolate(t, 0.0, 6.0, q, 4000, 2000);
	glPushMatrix();
	glTranslated(q,0,5000);
	
	glRotated(t*32,0,1,0);
	
	gRookModel.draw();
	glPopMatrix();*/


	
	
	
	glPushMatrix();
	glTranslatef(3000, 0, 1000);
	glRotated(t*64,0,1,0);
	//gBishopModel.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4000, 0, 1000);
	glRotated(t*32,0,1,0);
	gKingModel.draw();
	glPopMatrix();

	double z;
	Interpolate(t, 1.0, 3.0, z, 1000, 7000);
	glPushMatrix();
	glTranslatef(5000, 0, z);
	glRotated(t*16,0,1,0);
	//gQueenModel.draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(6000, 0, 1000);
	glRotated(t*48,0,1,0);
	//gBishopModel.draw();
	glPopMatrix();

	for(int x=1000; x<=8000; x+=1000)
	{
		glPushMatrix();
		glTranslatef(x, 0, 2000);
		glRotated(t*32,0,1,0);
		//gPawnModel.draw();
		glPopMatrix();
	}

	// Set the color for one side (black), and draw its 16 pieces.
	GLfloat mat_amb_diff2[] = {0.1f, 0.5f, 0.8f, 0.1};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff2);

	double x;
	Interpolate(t, 4.0, 6.0, x, 4000, 2000);
	glPushMatrix();
	glTranslatef(x, 0, 8000);
	glRotated(t*32,0,1,0);
	gKingModel.draw();
	glPopMatrix();

	for(int x=1000; x<=8000; x+=1000)
	{
		glPushMatrix();
		glTranslatef(x, 0, 7000);
		glRotated(t*32,0,1,0);
		//gPawnModel.draw();
		glPopMatrix();
	}
	

	GLfloat light_position[] = {(float)sin(t*2)*10000,5000,(float)cos(t*2)*10000, 0}; // light comes FROM this std::vector direction.
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light

	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}



void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) w/(GLdouble) h;
	gluPerspective( 
	/* field of view in degree */ 45.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ 100, /* Z far */ 30000.0);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w,h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	// set material's specular properties
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// set light properties
	GLfloat light_position[] = {(float)eye[0], (float)eye[1], (float)eye[2],1};
	GLfloat white_light[] = {1,1,1,1};
	GLfloat low_light[] = {.3f,.3f,.3f,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

	glEnable(GL_DEPTH_TEST); // turn on depth buffering
	glEnable(GL_LIGHTING);	 // enable general lighting
	glEnable(GL_LIGHT0);	 // enable the first light.
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv)
{
	//std::ifstream bf("Bishop.obj");
	//Model gBishopModel = LoadOBJ(bf);
	vector3d banana(1,0,0);
	std::cout<<banana<<std::endl;
	std::vector<vector3d> a;
	a.push_back(vector3d(1,1,1));
	a.push_back(vector3d(1,1,-2));
	std::cout<<a;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Shapes");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(1,1,1,1);	
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
