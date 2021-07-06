//
// Created by jun on 2021/06/23.
//
#include <tuple>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <functional> 
/*********************************/
#include<iostream>
#include<fstream>
/*********************************/
#ifdef __ANDROID__
#include <android/log.h>
#else   /* __ANDROID__ */
#endif  /* __ANDROID__ */
#include "CG3D.h"

namespace cg {
	std::tuple<bool, bool> Mash::validateArrays(bool isCleanCustomdata) {
		return Mash::validateArrays(Vertexs, Edges, Faces, Loops, Polygons, DeformVerts, isCleanCustomdata, isCleanCustomdata);
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

	std::tuple<bool, bool> Mash::validateArrays(std::vector<Vertex> &Vertexs, std::vector<Edge> &Edges, std::vector<Face> &Faces, std::vector<Loop> &Loops, std::vector<Polygon> &Polygons, std::vector<DeformVertex> &DeformVertexs, bool doVerbose, bool doFixes) {
		union {
			struct {
				int verts : 1;
				int verts_weight : 1;
				int loops_edge : 1;
			};
			int as_flag;
		} fix_flag;

		union {
			struct {
				int edges : 1;
				int faces : 1;
				/* This regroups loops and polys! */
				int polyloops : 1;
				int mselect : 1;
			};
			int as_flag;
		} free_flag;

		union {
			struct {
				int edges : 1;
			};
			int as_flag;
		} recalc_flag;

		fix_flag.as_flag = 0;
		free_flag.as_flag = 0;
		recalc_flag.as_flag = 0;

		/* Edges数と、Polygons数の判定 */
		if (Edges.size() == 0 && Polygons.size() != 0) {
			__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLogical error, %u polygons and 0 edges", Polygons.size());
			recalc_flag.edges = doFixes;
		}

		/* Vertexsの値 検証 */
		for(size_t lpct = 0; lpct < Vertexs.size(); lpct++) {
			Vertex &vert = Vertexs[lpct];
			if(std::isfinite(vert.Co.x) == false || std::isfinite(vert.Co.y) == false || std::isfinite(vert.Co.x) == false) {
				if(doFixes) {
					vert.Co.x = vert.Co.y = vert.Co.z = 0;
					fix_flag.verts = true;
				}
			}

			if(vert.No.x != 0 || vert.No.y != 0 || vert.No.x != 0) {
				/* 法線に値がある場合は、何もしなくていい */
				continue;
			}
			else {
				if(vert.Co.x != 0 || vert.Co.y != 0 || vert.Co.x != 0) {
					/* エラーログ出力して、z軸にSHRT_MAXを設定 */
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex[%zu]: is zero normal, assuming Z-up normal %s(%d)", lpct, __PRETTY_FUNCTION__, __LINE__);
					vert.No.z = SHRT_MAX;
				}
			}
		}
		/******************************/
		{
			std::ofstream outputfile("D:\\testaaaalog\\aaaavalidatelog-ore2.txt");
			outputfile << "111111111111111" << std::endl;
			for (int lpi = 0; lpi < Vertexs.size(); lpi++ )
				outputfile << CG3D::format("Vertex no[%d](%d,%d,%d) co[%d](%f,%f,%f)\n", lpi, Vertexs[lpi].No[0], Vertexs[lpi].No[1], Vertexs[lpi].No[2], lpi, Vertexs[lpi].Co[0], Vertexs[lpi].Co[1], Vertexs[lpi].Co[2]) << std::endl;
			outputfile.close();
		}
		/******************************/

		/* Edgesの値 検証 */
		std::vector<Edge> newEdges;
		newEdges.reserve(Edges.size());
		for(size_t lpct = 0;  lpct < Edges.size(); lpct++) {
			Edge &e = Edges[lpct];
			bool isRemove = false;
			if(e.Vertices.x == e.Vertices.y) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: is matching verts. %d %s(%d)", lpct, e.Vertices.x, __PRETTY_FUNCTION__, __LINE__);
				isRemove = doFixes;
			}
			if (e.Vertices.x >= (int)Vertexs.size()) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: x is index out of range, %d", lpct, e.Vertices.x);
				isRemove = doFixes;
			}
			if (e.Vertices.y >= (int)Vertexs.size()) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: y is index out of range, %d", lpct, e.Vertices.y);
				isRemove = doFixes;
			}

			bool findit = std::find_if(newEdges.begin(), newEdges.end(), [e](const Edge &e2) {
				return (e.Vertices.x == e2.Vertices.x) && (e.Vertices.y == e2.Vertices.y);
			}) != newEdges.end();
			if ((e.Vertices.x != e.Vertices.y) && findit) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tEdge[%zu]: is a duplicated=(%d,%d)", lpct, e.Vertices.x, e.Vertices.y);
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

		/******************************/
		{
			std::ofstream outputfile("D:\\testaaaalog\\aaaavalidatelog-ore2.txt");
			outputfile << "22222222222222222" << std::endl;
			for (int lpi = 0; lpi < Vertexs.size(); lpi++ )
				outputfile << CG3D::format("Edges[%d].v(%d,%d) edge_hash[%d](.entries=%p, .map=%p, .slot_mask=%d, .capacity_exp=%d, .length=%d, .dummy_count=%d)\n", lpi, Edges[lpi].Vertices.x, Edges[lpi].Vertices.y,
					lpi, newEdges[lpi].entries, newEdges[lpi].map, newEdges[lpi].slot_mask, newEdges[lpi].capacity_exp, newEdges[lpi].length, newEdges[lpi].dummy_count);

			outputfile.close();
		}
		/******************************/

		/* Facesの値 検証 */
		if( !Faces.empty() && Polygons.empty()) {
			/* TODO Facesは将来対応 */
			assert(false && "実データなしなので、動作未確認!!");
		}

		/* Polygonsの値 検証 */
		std::vector<SortPoly> sortPolygons;
		sortPolygons.reserve(Polygons.size());
		for(size_t lpi = 0; lpi < Polygons.size(); lpi++) {
			SortPoly sortPolygon;
			Polygon &poly = Polygons[lpi];

			/* index設定 */
			sortPolygon.index = lpi;

			/* Material::Indexの値 判定 */
			if(poly.MaterialIndex < 0) {
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygon[%zu] MaterialIndexが無効値になっている (%d)", lpi, Polygons[lpi].MaterialIndex);
				if(doFixes)
					poly.MaterialIndex = 0;
			}

			/* Material::LoopStart,LoopTotalの値 判定 */
			if (poly.LoopStart < 0 || poly.LoopTotal < 3) {
				/* Invalid loop data. */
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygon[%zu] に無効値 (loopstart: %d, totloop: %d) %s(%d)", lpi, poly.LoopStart, poly.LoopTotal, __PRETTY_FUNCTION__, __LINE__);
				sortPolygon.invalid = true;
			}
			else if (poly.LoopStart+poly.LoopTotal > (int)Loops.size()) {
				/* Invalid loop data. */
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa",
									"\tPolygon[%zu] uses loops out of range (loopstart: %d, loopend: %d, max nbr of loops: %zu) %s(%d)", lpi, poly.LoopStart, (poly.LoopStart + poly.LoopTotal - 1), (Loops.size() - 1), __PRETTY_FUNCTION__, __LINE__);
				sortPolygon.invalid = true;
			}
			else {
				/* Poly itself is valid, for now. */
				sortPolygon.invalid = false;
				sortPolygon.verts.reserve(poly.LoopTotal);
				sortPolygon.loopstart = poly.LoopStart;

				/* Ideally we would only have to do that once on all vertices
				 * before we start checking each poly, but several polys can use same vert,
				 * so we have to ensure here all verts of current poly are cleared. */
				/* 理想的には、すべての頂点で1回だけ実行する必要があります。
				 * 各ポリゴンのチェックを開始する前に、複数のポリゴンが同じ頂点を使用できるため、
				 * ここで現在のポリゴンのすべての頂点がクリアされていることを確認する。 */
				for (int lpj = 0; lpj < poly.LoopTotal; lpj++) {
					Loop &ml = Loops[sortPolygon.loopstart + lpj];
					if (ml.VertexIndex < (int)Vertexs.size()) {
#define	ME_VERT_TMP_TAG (1 << 2)
						Vertexs[ml.VertexIndex].flag &= ~ME_VERT_TMP_TAG;
					}
				}

				/* Test all poly's loops' vert idx. */
				for (int lpj = 0; lpj < poly.LoopTotal; lpj++) {
					Loop &ml = Loops[sortPolygon.loopstart+lpj];

					if (ml.VertexIndex >= (int)Vertexs.size()) {
						/* Invalid vert idx. */
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is invalid vert reference (%d)!!", sortPolygon.loopstart+lpj, ml.VertexIndex);
						sortPolygon.invalid = true;
					}
					else if (Vertexs[ml.VertexIndex].flag & ME_VERT_TMP_TAG) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%zu] is duplicated vert reference at corner (%d)", lpi, lpj);
						sortPolygon.invalid = true;
					}
					else {
						Vertexs[ml.VertexIndex].flag |= ME_VERT_TMP_TAG;
					}
				}

				if (sortPolygon.invalid)
					continue;

//				mloops[lpi].e ... 常に0
//				mloops[lpi].v ... Loop::VertexIndexに対応

				/* Test all poly's loops. */
				for (int lpj = 0; lpj < poly.LoopTotal; lpj++) {
					Loop &ml = Loops[sortPolygon.loopstart+lpj];
					int v1 = ml.VertexIndex;													/* v1 is prev loop vert idx */
					int v2 = Loops[sortPolygon.loopstart+(lpj+1) % poly.LoopTotal].VertexIndex;	/* v2 is current loop one. */
					bool finded = std::find_if(newEdges.begin(), newEdges.end(), [v1, v2](const Edge &e){ return (e.Vertices.x == v1 && e.Vertices.y == v2); })
											!= newEdges.end();
					if ( !finded) {
						assert(false && "実データなしなので、動作未確認!!");
						/* Edge not existing. */
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%u] needs (%d, %d) form edge but not finded.", sortPolygon.index, v1, v2);
						if (doFixes) {
							recalc_flag.edges = true;
						}
						else {
							sortPolygon.invalid = true;
						}
					}
					else if (ml.EdgeIndex >= (int)Edges.size()) {
						assert(false && "実データなしなので、動作未確認!!");
						/* Invalid edge idx.
						 * We already know from previous text that a valid edge exists, use it (if allowed)! */
						if (doFixes) {
							int prev_e = ml.EdgeIndex;
							auto findeditr = std::find_if(newEdges.begin(), newEdges.end(), [v1, v2](const Edge &e){ return (e.Vertices.x == v1 && e.Vertices.y == v2); });
							int idx = std::distance(newEdges.begin(), findeditr);
							ml.EdgeIndex = idx;
							fix_flag.loops_edge = true;
							__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is invalid edge reference (%d), fixed using edge %d", sortPolygon.loopstart+lpj, prev_e, ml.EdgeIndex);
						}
						else {
							__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is invalid edge reference (%d)", sortPolygon.loopstart+lpj, ml.EdgeIndex);
							sortPolygon.invalid = true;
						}
					}
					else {
						Edge &me = Edges[ml.EdgeIndex];
						bool isRemovedEdge = (me.Vertices.x == me.Vertices.y);
						if ( isRemovedEdge ||
							!((me.Vertices.x == v1 && me.Vertices.y == v2) || (me.Vertices.x == v2 && me.Vertices.y == v1))) {
							/* The pointed edge is invalid (tagged as removed, or vert idx mismatch),
							 * and we already know from previous test that a valid one exists,
							 * use it (if allowed)! */
							if (doFixes) {
								int prev_e = ml.EdgeIndex;
								auto findeditr = std::find_if(newEdges.begin(), newEdges.end(), [v1, v2](const Edge &e){ return (e.Vertices.x == v1 && e.Vertices.y == v2); });
								int idx = std::distance(newEdges.begin(), findeditr);
								ml.EdgeIndex = idx;
								fix_flag.loops_edge = true;
								__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%u] is invalid edge reference (%d, is_removed: %d), fixed using edge %d", sortPolygon.index, prev_e, isRemovedEdge, ml.EdgeIndex);
							}
							else {
								__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPoly[%u] has invalid edge reference (%d)", sortPolygon.index, ml.EdgeIndex);
								sortPolygon.invalid = true;
							}
						}
					}
				}

				if ( !sortPolygon.invalid) {
					/* Needed for checking polys using same verts below. */
					std::sort(sortPolygon.verts.begin(), sortPolygon.verts.end());
				}
			}

			sortPolygons.push_back(sortPolygon);
		}

		/* Second check pass, testing polys using the same verts. */
		std::sort(sortPolygons.begin(), sortPolygons.end() ,[](const SortPoly &sp1, const SortPoly &sp2){
			/* Reject all invalid polys at end of list! */
			if (sp1.invalid || sp2.invalid)
				return sp1.invalid != true;

			const int maxidx = sp1.verts.size() > sp2.verts.size() ? sp2.verts.size() : sp1.verts.size();
			for(int idx = 0; idx < maxidx; idx++) {
				if (sp1.verts[idx] != sp2.verts[idx])
					return (sp1.verts[idx] < sp2.verts[idx]);
			}

			return sp1.verts.size() < sp2.verts.size();
		});

		bool is_valid = true;
		for (int lpi = 1; lpi < Polygons.size(); lpi++) {
			SortPoly &prev_sp = sortPolygons[lpi-1];
			SortPoly &sp      = sortPolygons[lpi];
			if (sp.invalid) {
				/* Break, because all known invalid polys have been put at the end by qsort with search_poly_cmp. */
				break;
			}

			/* Test same polys. */
			if (sp.verts == prev_sp.verts) {
				if (doVerbose) {
					std::ostringstream oss;
					std::copy(sp.verts.begin(), sp.verts.end(), std::ostream_iterator<const decltype(sp.verts)::value_type&>(oss, ","));
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolygons[%u] and Polygons[%u] use same vertices (%s), considering poly %u as invalid.", prev_sp.index, sp.index, oss.str().c_str(), sp.index);
				}
				else {
					is_valid = false;
				}
				sp.invalid = true;
			}
		}

		/* Third check pass, testing loops used by none or more than one poly. */
		std::sort(sortPolygons.begin(), sortPolygons.end(), [](const SortPoly &sp1, const SortPoly &sp2){
			/* Reject all invalid polys at end of list! */
			if (sp1.invalid || sp2.invalid)
				return (sp1.invalid == false);
			/* Else, sort on loopstart. */
			return (sp1.loopstart < sp2.loopstart);
		});

		int prevend = 0;
		std::uint32_t prevsp_idx = 0;

		for (int lpi = 0; lpi < Polygons.size(); lpi++) {
			SortPoly &sp = sortPolygons[lpi];

			/* Note above prev_sp: in following code, we make sure it is always valid poly (or NULL). */
			if (sp.invalid) {
				if (doFixes) {
					free_flag.polyloops = doFixes;
					/* DO NOT REMOVE ITS LOOPS!!!
					 * As already invalid polys are at the end of the SortPoly list, the loops they
					 * were the only users have already been tagged as "to remove" during previous
					 * iterations, and we don't want to remove some loops that may be used by
					 * another valid poly! */
				}
			}
			/* Test loops users. */
			else {
				/* Unused loops. */
				if (prevend < sp.loopstart) {
					const int spos = prevend;
					for (int lpj = prevend; lpj < sp.loopstart; lpj++) {
						Loop &ml = Loops[prevend + lpj-spos];

						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoops[%d] is unused.", lpj);
						if (doFixes) {
#define INVALID_LOOP_EDGE_MARKER 4294967295u
							ml.EdgeIndex = INVALID_LOOP_EDGE_MARKER;
							free_flag.polyloops = doFixes;
						}
					}
					prevend = sp.loopstart + sp.verts.size();
					prevsp_idx = sp.index;
				}
				/* Multi-used loops. */
				else if (prevend > sp.loopstart) {
					__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tPolys %u and %u share loops from %d to %d, considering poly %u as invalid.",
							  prevsp_idx, sp.index, sp.loopstart, prevend, sp.index);
					if (doFixes) {
						free_flag.polyloops = doFixes;
						/* DO NOT REMOVE ITS LOOPS!!!
						 * They might be used by some next, valid poly!
						 * Just not updating prev_end/prev_sp vars is enough to ensure the loops
						 * effectively no more needed will be marked as "to be removed"! */
					}
				}
				else {
					prevend = sp.loopstart + sp.verts.size();
					prevsp_idx = sp.index;
				}
			}
		}
		/* We may have some remaining unused loops to get rid of! */
		if (prevend < Loops.size()) {
			const int spos = prevend;
			for (int lpj = spos; lpj < Loops.size(); lpj++) {
				Loop &ml = Loops[prevend+lpj-spos];
				__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tLoop %u is unused.", lpj);
				if (doFixes) {
					ml.EdgeIndex = INVALID_LOOP_EDGE_MARKER;
					free_flag.polyloops = doFixes;;
				}
			}
		}

		sortPolygons.clear();
		newEdges.clear();

		/* fix deform verts */
		if ( !DeformVertexs.empty()) {
			assert(false && "実データなしなので、動作未確認!!");
			for (int lpi = 0; lpi < Vertexs.size(); lpi++) {
				DeformVertex &dv = DeformVertexs[lpi];
				for (int lpj = 0; lpj < dv.dws.size(); lpj++) {
					DeformWeight &dw = dv.dws[lpj];

					/* note, greater than max defgroups is accounted for in our code, but not < 0 */
					if (!isfinite(dw.weight)) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex deform %u, group %u has weight: %f", lpi, dw.def_nr, dw.weight);
						if (doFixes) {
							dw.weight = 0.0f;
							fix_flag.verts_weight = true;
						}
					}
					else if (dw.weight < 0.0f || dw.weight > 1.0f) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex deform %u, group %u has weight: %f", lpi, dw.def_nr, dw.weight);
						if (doFixes) {
							std::function<void(float, float, float)> CLAMP = [](float a, float b, float c){
								if (a < b)		{ a = b;}
								else if (a > c)	{ a = c;}
							};
							CLAMP(dw.weight, 0.0f, 1.0f);
							fix_flag.verts_weight = true;
						}
					}

					/* Not technically incorrect since this is unsigned, however,
					 * a value over INT_MAX is almost certainly caused by wrapping an uint. */
					if (dw.def_nr >= INT_MAX) {
						__android_log_print(ANDROID_LOG_ERROR, "aaaaa", "\tVertex deform %u, has invalid group %u", lpi, dw.def_nr);
						if (doFixes) {
							dv.dws.clear();
							fix_flag.verts_weight = true;

							if (!dv.dws.empty()) {
								/* re-allocated, the new values compensate for stepping
								 * within the for loop and may not be valid */
								lpj--;
							}
							else { /* all freed */
								break;
							}
						}
					}
				}
			}
		}

		bool changed = (fix_flag.as_flag || free_flag.as_flag || recalc_flag.as_flag);

		return { is_valid, changed };
	}
} /* namespace cg */