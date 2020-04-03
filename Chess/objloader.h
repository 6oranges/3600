#include<math.h>
#include<iostream>
#include"vectors.h"
#include"../pglut.h"
#include<stdlib.h>
#include<string>
#include<sstream>
struct face{
	int v1,v2,v3,t1,t2,t3,n1,n2,n3;
};
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
/*
struct Mesh{
	std::vector<vector3d> vertices;
	std::vector<vector3d> normals;
	std::vector<face> faces;
	std::string name;
	void draw(){
		glScaled(400,400,400);
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
			vector3d v(x,y,z);
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
			//std::cout<<"I don't recognize '"<<type<<"'"<<std::endl;
		}
	}*//*
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
	}*//*
	return m;
}*/
struct vector9i{
	double vx,vy,vz,nx,ny,nz;
};
class OBJModel{
	public:
		OBJModel(std::istream& fin);
		OBJModel();
		void draw();
	private:
		double mX,mY,mZ;
		std::vector<cvector3d> mNormals;
		std::vector<cvector3d> mVerts;
		std::vector<vector9i> mTriangles;

};
OBJModel::OBJModel(){}
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
			cvector3d v;
			std::stringstream(values[0])>>v.x;
			std::stringstream(values[1])>>v.y;
			std::stringstream(values[2])>>v.z;
			v.x/=3;
			v.y/=3;
			v.z/=3;
			mVerts.push_back(v);
		}
		else if (value=="vn"){
			std::vector<std::string> values=splitBy(restofline,' ');
			cvector3d v;
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
				v.nz-=1;/*
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
				v.nx=v.ny=v.nz=mNormals.size()-1;*/
				mTriangles.push_back(v);
			}
		}
		else{
			//std::cout<<"I don't recognize '"<<value<<"'"<<std::endl;
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