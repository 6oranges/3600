#include<math.h>
#include<iostream>
#include<vector>
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
struct cvector3d{
	double x,y,z;
	double* arr=&x;
	cvector3d(double x_,double y_,double z_):x(x_),y(y_),z(z_){};
	cvector3d():x(0),y(0),z(0){};
	cvector3d(const cvector3d& v):x(v.x),y(v.y),z(v.z){}
	double mag(){return std::sqrt(x*x+y*y+z*z);};
	cvector3d operator-(cvector3d v){return cvector3d(x-v.x,y-v.y,z-v.z);};
	cvector3d operator+(cvector3d v){return cvector3d(x+v.x,y+v.y,z+v.z);};
	cvector3d operator/(double s){return cvector3d(x/s,y/s,z/s);};
	cvector3d operator*(double s){return cvector3d(x*s,y*s,z*s);};
	static cvector3d cross(cvector3d v1, cvector3d v2){return cvector3d(v1.y*v2.z-v1.z*v2.y,v1.z*v2.x-v1.x-v2.z,v1.x*v2.y-v1.y*v2.x);};
	static cvector3d normalize(cvector3d v){return v/v.mag();};
	static double dot(cvector3d v1, cvector3d v2){return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;}
	cvector3d& operator=(const cvector3d& r){x=r.x;y=r.y;z=r.z;return *this;}
	friend std::ostream& operator<<(std::ostream& os, const cvector3d& v);
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