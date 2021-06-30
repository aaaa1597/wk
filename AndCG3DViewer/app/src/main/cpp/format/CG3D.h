//
// Created by jun on 2021/06/23.
//
#ifndef ANDCG3DVIEWER_CG3D_H
#define ANDCG3DVIEWER_CG3D_H

#include <vector>
#include <array>
#include "MatVec.h"

namespace cg {

class Cg3d {
};


class Edge {
public:
	m::Vector2i	Vertices;
	bool 			UseEdgeSharp;
};

class ColorLayer {
public:
	std::string					Name;
	std::vector<m::Vector3i>	ColorData;
};

class UvLayer {
public:
    std::string					Name;
    std::vector<m::Vector2f>	UvData;
};

class Vertex {
public:
	m::Vector3f	Co;
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
