#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <string>
#include "./utilities/MalinMath.h"
using namespace Malin;

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
	std::vector<Vec3f> texCoord_;
	std::vector<std::vector<int> > texs_;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);
	std::vector<int> texture(int idx);
	Vec3f tex(int i);
};

#endif //__MODEL_H__