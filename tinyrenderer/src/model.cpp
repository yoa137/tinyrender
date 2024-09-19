#include "model.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

Model::Model(const char* filename): verts_(),faces_()
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) {
		std::cerr << "fail to open file!!" << std::endl;
		return;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) {
				iss >> v.raw[i];
			}
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			
			int itrash, idx, texidx;
			iss >> trash;
			std::vector<int> f;
			std::vector<int> tex;
			while (iss >> idx >> trash >> texidx >> trash >> itrash) {
				idx--;
				f.push_back(idx);
				tex.push_back(texidx);
				//std::cout << texidx << std::endl;
			}
			faces_.push_back(f);
			texs_.push_back(tex);
			
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec3f vt;
			for (int i = 0; i < 3; i++) {
				iss >> vt.raw[i];
			}
			texCoord_.push_back(vt);
			//std::clog << texCoord_.size() << std::endl;
		}

	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model()
{
}

int Model::nverts()
{
	return (int)verts_.size();
}

int Model::nfaces()
{
	return (int)faces_.size();
}

Vec3f Model::vert(int i)
{
	return verts_[i];
}

std::vector<int> Model::face(int idx)
{
	return faces_[idx];
}

std::vector<int> Model::texture(int idx)
{
	return texs_[idx];
}

Vec3f Model::tex(int i)
{
	return texCoord_[i];
}
