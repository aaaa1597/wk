//
// Created by jun on 2021/06/23.
//
#include <tuple>
#include <cmath>
#ifdef __ANDROID__
#include <android/log.h>

#else   /* __ANDROID__ */
#endif  /* __ANDROID__ */
#include "CG3D.h"

namespace cg {
	bool Mash::validateArrays(bool isCleanCustomdata) {
		bool isValid, isChange;
		std::tie(isValid, isChange) = Mash::validateArrays(Vertexs, Edges, Faces, Loops, Polygons, isCleanCustomdata, isCleanCustomdata);
	}

	#pragma region /* TODO validate()実装は必要かどうかちゃんと考える。*/
/* TODO validate()実装は必要かどうかちゃんと考える。 */
//	void Mash::validate(bool isCleanCustomdata) {
//		bool isValid1 = Mash::validateAllCustomdata(Vertexs, Edges, Loops, Polygons, isCleanCustomdata, isCleanCustomdata, true);
//		bool isValid2 = Mash::validateArrays(Vertexs, Edges, Faces, Loops, Polygons, isCleanCustomdata, isCleanCustomdata, true);
//	}

//	bool Mash::validateAllCustomdata(std::vector<Vertex> &Vertexs, std::vector<Edge> &Edges, std::vector<Loop> &Loops, std::vector<Polygon> &Polygons, bool CheckMask, bool doVerbose, bool doFixes) {
//		CustomData_MeshMasks mask = {0};
//		if (CheckMask) {
//			mask = CD_MASK_MESH;
//		}
//
//		bool isValidv, isChangeV;
//		std::tie(isValidv, isChangeV) = Mash::validateCustomdata(Vertexs, mask.vmask, doVerbose, doFixes);
//
//		bool is_change_e, is_change_l, is_change_p;
//
//		return false;
//	}

//	std::tuple<bool, bool> Mash::validateCustomdata(std::vector<Vertex> &Vertex, uint64_t vmask, bool doVerbose, bool doFixes) {
//		bool isValid = true;
//		bool hasFixes = false;
//		int i = 0;
//
//		__android_log_print(ANDROID_LOG_DEBUG,"aaaaa", "%s: Checking %d CD layers...", __PRETTY_FUNCTION__ , Vertex.size());
//
////		while (i < data->totlayer) {
////			CustomDataLayer *layer = &data->layers[i];
////			bool ok = true;
////
////			if (CustomData_layertype_is_singleton(layer->type)) {
////				const int layer_tot = CustomData_number_of_layers(data, layer->type);
////				if (layer_tot > 1) {
////					PRINT_ERR("\tCustomDataLayer type %d is a singleton, found %d in Mesh structure\n",
////							  layer->type,
////							  layer_tot);
////					ok = false;
////				}
////			}
////
////			if (mask != 0) {
////				CustomDataMask layer_typemask = CD_TYPE_AS_MASK(layer->type);
////				if ((layer_typemask & mask) == 0) {
////					PRINT_ERR("\tCustomDataLayer type %d which isn't in the mask\n", layer->type);
////					ok = false;
////				}
////			}
////
////			if (ok == false) {
////				if (do_fixes) {
////					CustomData_free_layer(data, layer->type, 0, i);
////					has_fixes = true;
////				}
////			}
////
////			if (ok) {
////				if (CustomData_layer_validate(layer, totitems, do_fixes)) {
////					PRINT_ERR("\tCustomDataLayer type %d has some invalid data\n", layer->type);
////					has_fixes = do_fixes;
////				}
////				i++;
////			}
////		}
////
////		PRINT_MSG("%s: Finished (is_valid=%d)\n\n", __func__, (int)!has_fixes);
////
////		*r_change = has_fixes;
////
////		return is_valid;
////		return {is_valid, has_fixes};
//		return {true,true};
//	}
	#pragma endregion

	std::tuple<bool, bool> Mash::validateArrays(std::vector<Vertex> &Vertexs, std::vector<Edge> &Edges, std::vector<Face> &Faces, std::vector<Loop> &Loops, std::vector<Polygon> &Polygons, bool doVerbose, bool doFixes) {
		/* Vertexsの値 検証 */
		for(size_t lpct = 0; lpct < Vertexs.size(); lpct++) {
			Vertex &vert = Vertexs[lpct];
			if(std::isfinite(vert.Co.x) == false || std::isfinite(vert.Co.y) == false || std::isfinite(vert.Co.x) == false)
				vert.Co.x = vert.Co.y = vert.Co.z = 0;

			if(vert.No.x != 0 || vert.No.y != 0 || vert.No.x != 0) {
				/* 法線に値がある場合は、何もしなくていい */
				continue;
			}
			else {
				if(vert.Co.x != 0 || vert.Co.y != 0 || vert.Co.x != 0) {
					/* エラーログ出力して、z軸にSHRT_MAXを設定 */
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex[%u]: has zero normal, assuming Z-up normal", lpct);
					vert.No.z = SHRT_MAX;
				}
			}
		}

		/* Edgesの値 検証 */
		std::vector<Edge> newEdges;
		newEdges.reserve(Edges.size());
		for(size_t lpct = 0;  lpct < Edges.size(); lpct++) {
			Edge &e = Edges[lpct];
			bool isRemove = false;
			if(e.Vertices.x == e.Vertices.y) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge %u: has matching verts, both %u", lpct, e.Vertices.x);
				isRemove = doFixes;
			}
			if (e.Vertices.x >= Vertexs.size()) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge %u: v1 index out of range, %u", lpct, e.Vertices.x);
				isRemove = doFixes;
			}
			if (e.Vertices.y >= Vertexs.size()) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge %u: v2 index out of range, %u", lpct, e.Vertices.y);
				isRemove = doFixes;
			}

			bool findit = std::find_if(newEdges.begin(), newEdges.end(), [e](const Edge &e2) {
				return (e.Vertices.x == e2.Vertices.x) && (e.Vertices.y == e2.Vertices.y);
			}) != newEdges.end();
			if ((e.Vertices.x != e.Vertices.y) && findit) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge %u: is a duplicate of (%d,%d)", lpct, e.Vertices.x, e.Vertices.y);
				isRemove = doFixes;
			}

			if(isRemove) {
				/* xとyを同じ値にしてしまう。 */
				e.Vertices.y = e.Vertices.x;
			}
			else {
				newEdges.push_back({.Vertices = {e.Vertices.x, e.Vertices.y}, .UseEdgeSharp=false, .Crease=0});
			}
		}

		/* Facesの値 検証 */
		if(Faces.size() > 0 && Polygons.size()==0) {
			/* TODO Facesは将来対応 */
			assert(false && "実データなしなので、動作未確認!!");
		}

		/* Polygonsの値 検証 */
		std::vector<Polygon> sortPolygons;
		sortPolygons.reserve(Polygons.size());
		for(size_t lpct = 0; lpct < Polygons.size(); lpct++) {
			sortPolygons[lpct].LoopIndices
		}

		return {false, false};
	}
} /* namespace cg */