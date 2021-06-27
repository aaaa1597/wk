//
// Created by jun on 2021/06/23.
//
#ifndef ANDCG3DVIEWER_CG3D_H
#define ANDCG3DVIEWER_CG3D_H

#include <vector>
#include <array>
#include "MatrixVector.h"

namespace cg3d {

class Cg3d {
};

class UvLayer {
public:
    std::string					Name;
    std::vector<CG3DVector2>	UvData;
};

class Vertex {
public:
	CG3DVector3	Co;
};

class Loop {
public:
	int		VertexIndex;
	double	normal;
};

class Polygon {
public:
	int		LoopStarts;
	int		LoopTotals;
	int		LoopIndices;
	bool	UseSmooths;
	int		MaterialIndex;
};


class Mash {
public:
	std::string				name;
	std::vector<Vertex>		Vertexs;
	std::vector<Loop>		Loops;
	std::vector<Polygon>	Polygons;
	std::vector<UvLayer>	UvLayers;
};

} /* namespace cg3d */

#endif //ANDCG3DVIEWER_CG3D_H
