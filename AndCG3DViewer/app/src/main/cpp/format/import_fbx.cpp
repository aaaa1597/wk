#pragma warning(disable : 4819)
//
// Created by jun on 2021/06/14.
//
#include <vector>
#include <sstream>

#ifdef __ANDROID__
#include <android/log.h>
#else   /* __ANDROID__ */
#include "../CG3DCom.h"
#endif  /* __ANDROID__ */
#include <fstream>
#include <cassert>
#include "FBX.h"
#include "import_fbx.h"
#include "MatrixVector.h"

namespace fbx {

/**********/
/* load() */
/**********/
bool import_fbx::load(const std::vector<char> &ModelData) {
using ibinstream = std::istringstream;

	ibinstream ibs(std::string(ModelData.begin(), ModelData.end()));

	ibs.seekg(0, std::ios::end);
	size_t filesize = (size_t)ibs.tellg();
	ibs.seekg(0, std::ios::beg);
	size_t fpos = (size_t)ibs.tellg();

	/* MAGICWORD */
	const char magicword[23] = {'K','a','y','d','a','r','a',' ','F','B','X',' ','B','i','n','a','r','y',' ',' ',0x00,0x1A,0x00};
	char fbxmagic[sizeof(magicword)] = {};
	ibs.read(fbxmagic, sizeof(magicword));
	if (std::memcmp(magicword, fbxmagic, sizeof(magicword)) != 0) {
		__android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa Load(%s) Failed!! Unmatch MAGICWORD!!\n");
		return false;
	}

	/* FBXバージョン */
	int fbxversion;
	ibs.read(reinterpret_cast<char*>(&fbxversion), sizeof(int));

	__android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa FBX version(%d)\n", fbxversion);

	/* FBXUtil初期化 */
	bool ret = FbxUtil::init((Version)fbxversion);
	if (!ret) return false;


#ifndef __ANDROID__
	/**************************/
	/* ログファイル一旦全削除 */
	/**************************/
	//UTF-8からUTF-16へ変換
	const int nSize = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)"D:\\testaaaaログ\\エレメント一覧.log", -1, NULL, 0);
	BYTE* buffUtf16 = new BYTE[nSize * 2 + 2];
	::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)"D:\\testaaaaログ\\エレメント一覧.log", -1, (LPWSTR)buffUtf16, nSize);
	//UTF-16からShift-JISへ変換
	const int nSizeSJis = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)buffUtf16, -1, NULL, 0, NULL, NULL);
	BYTE* buffSJis = new BYTE[nSizeSJis * 2];
	ZeroMemory(buffSJis, nSizeSJis * 2);
	::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)buffUtf16, -1, (LPSTR)buffSJis, nSizeSJis, NULL, NULL);
	BYTE dstStr[100] = { 0 };
	memcpy(dstStr, buffSJis, nSizeSJis * 2);
	delete[] buffUtf16;
	delete[] buffSJis;
	char cDstStr[100] = {};
	memcpy(cDstStr, dstStr, sizeof(cDstStr));

	std::ofstream ofs(cDstStr);
	ofs.close();
#endif /*__ANDROID__*/

	/**********************/
	/* エレメント一括読出 */
	/**********************/
	std::vector<FbxElem> rootElem;
	while(true) {
		FbxElem allElems = FbxUtil::readElements(ibs);
		if(allElems.end_offset == 0)
			break;
#ifndef __ANDROID__
		//UTF-8からUTF-16へ変換
		const int nSize = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)"D:\\testaaaaログ\\エレメント一覧.log", -1, NULL, 0);
		BYTE* buffUtf16 = new BYTE[nSize * 2 + 2];
		::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)"D:\\testaaaaログ\\エレメント一覧.log", -1, (LPWSTR)buffUtf16, nSize);
		//UTF-16からShift-JISへ変換
		const int nSizeSJis = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)buffUtf16, -1, NULL, 0, NULL, NULL);
		BYTE* buffSJis = new BYTE[nSizeSJis * 2];
		ZeroMemory(buffSJis, nSizeSJis * 2);
		::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)buffUtf16, -1, (LPSTR)buffSJis, nSizeSJis, NULL, NULL);
		BYTE dstStr[100] = { 0 };
		memcpy(dstStr, buffSJis, nSizeSJis*2);
		delete[] buffUtf16;
		delete[] buffSJis;
		char cDstStr[100] = {};
		memcpy(cDstStr, dstStr, sizeof(cDstStr));

		std::ofstream ofs(cDstStr, std::ios::app);
//		std::ofstream ofs(cDstStr);
		ofs << allElems.toString(0);
		ofs.close();
#endif /*__ANDROID__*/
		rootElem.push_back(allElems);
	}

	/**********************/
	/* GlobalSettings取得 */
	/**********************/
	/* GlobalSettingsキーを探索 */
	std::vector<FbxElem>::iterator gsitr = std::find_if(rootElem.begin(), rootElem.end(), [](const FbxElem &item){ return item.id=="GlobalSettings"; });
	assert((gsitr != rootElem.end()) && 
		"error ありえない!! GlobalSettingsキーがない!!");
	
	std::vector<FbxElem>::iterator gsP70itr = std::find_if(gsitr->elems.begin(), gsitr->elems.end(), [](const FbxElem& item) { return item.id == "Properties70"; });
	assert((gsP70itr != gsitr->elems.end()) &&
		"error ありえない!! Properties70キーがない!!");

	FbxElem &gs  = *gsitr;
	FbxElem &gsP70 = *gsP70itr;

	double unitscale	= FbxUtil::getPropNumber(gsP70, "UnitScaleFactor");
	double unitscaleOrg	= FbxUtil::getPropNumber(gsP70, "OriginalUnitScaleFactor");
//	double globalscale	*= (unitscale / units_blender_to_fbx_factor(context.scene));
	double globalscale = 1.0f;	/* 1固定にする。 */

	/* 上軸設定取得 */
	std::int64_t axisup1 = FbxUtil::getPropInteger(gsP70, "UpAxis");
	std::int64_t axisup2 = FbxUtil::getPropInteger(gsP70, "UpAxisSign");
	std::pair<std::int64_t, std::int64_t> axisup_pair = { axisup1, axisup2 };

	/* 前軸設定取得 */
	std::int64_t axisforward1 = FbxUtil::getPropInteger(gsP70, "FrontAxis");
	std::int64_t axisforward2 = FbxUtil::getPropInteger(gsP70, "FrontAxisSign");
	std::pair<std::int64_t, std::int64_t> axisforward_pair = { axisforward1, axisforward2 };

	/* 横軸設定取得 */
	std::int64_t axiscoord1 = FbxUtil::getPropInteger(gsP70, "CoordAxis");
	std::int64_t axiscoord2 = FbxUtil::getPropInteger(gsP70, "CoordAxisSign");
	std::pair<std::int64_t, std::int64_t> axiscoord_pair = { axiscoord1, axiscoord2 };

	/* 軸設定取得キー設定 */
	std::tuple<std::pair<std::int64_t, std::int64_t>,
			   std::pair<std::int64_t, std::int64_t>,
			   std::pair<std::int64_t, std::int64_t>> axiskey = {axisup_pair, axisforward_pair, axiscoord_pair};
	std::pair<Axis, Axis> axis = RIGHT_HAND_AXES_RR.at(axiskey);
	Axis axisforward = axis.second;
	Axis axisup      = axis.first;

	/* 拡縮行列生成 */
	TKSMatrix4 ScaleM = MatrixVector::createScale(globalscale, globalscale, globalscale);
	/* 軸変換行列生成 */
	TKSMatrix4 AxisConvM = MatrixVector::createAxisConversion(axisforward, axisup);
	/* グローバル行列生成 */
	TKSMatrix4 GlocalM = MatrixVector::MultMatrix(ScaleM, AxisConvM);
	/* グローバル逆行列生成 */
	TKSMatrix4 GlocalInvM = GlocalM.inverse();
	/* グローバル逆行列の転置生成 */
	TKSMatrix4 GlocalInvTranceposeM = GlocalInvM.trancepose();

	/* Customフレームレート */
	double customfps = FbxUtil::getPropNumber(gsP70, "CustomFrameRate");

	/* TimeMode */
	int timemode = FbxUtil::getPropEnum(gsP70, "TimeMode");

	/* realfps */
	double realfps = customfps;
	auto findit = std::find_if(FBX_FRAMERATES_RR.begin(), FBX_FRAMERATES_RR.end(), [timemode](const std::pair<int, double> &item){
		return item.first == timemode;
	});
	if (findit != FBX_FRAMERATES_RR.end()) {
		if(findit->second < 0)
			realfps = 25.0f;
		else
			realfps = findit->second;
	}

	int aaaaa = 0;

	/* TODO : 呼び元に返却する必要がある */
	double scene_render_fps = round(realfps);
	double scene_render_fps_base = scene_render_fps / realfps;

	//settings = FBXImportSettings(
	//	operator.report, (axis_up, axis_forward), global_matrix, global_scale,
	//	bake_space_transform, global_matrix_inv, global_matrix_inv_transposed,
	//	use_custom_normals, use_image_search,
	//	use_alpha_decals, decal_offset,
	//	use_anim, anim_offset,
	//	use_subsurf,
	//	use_custom_props, use_custom_props_enum_as_string,
	//	nodal_material_wrap_map, image_cache,
	//	ignore_leaf_bones, force_connect_children, automatic_bone_orientation, bone_correction_matrix,
	//	use_prepost_rot,
	//	);


	return true;
}

} /* namespace fbx */

