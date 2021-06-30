//
// Created by jun on 2021/06/23.
//
#ifndef ANDCG3DVIEWER_CG3D_H
#define ANDCG3DVIEWER_CG3D_H

#include <vector>
#include <array>
#include "MatrixVector.h"

namespace cg {

class Cg3d {
};


class Edge {
public:
	CG3DVector2i	Vertices;
	bool 			UseEdgeSharp;
};

class ColorLayer {
public:
	std::string					Name;
	std::vector<CG3DVector3>	ColorData;
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
	bool	UseSmooth;
	int		MaterialIndex;
};


class Mash {
public:
	std::string				name;
	std::vector<Vertex>		Vertexs;
	std::vector<Loop>		Loops;
	std::vector<Polygon>	Polygons;
	UvLayer					UvLayers;
	ColorLayer				ColorLayers;
	std::vector<Edge>		Edges;
	bool 					UseAutoSmooth;
};

} /* namespace cg */

#endif //ANDCG3DVIEWER_CG3D_H
