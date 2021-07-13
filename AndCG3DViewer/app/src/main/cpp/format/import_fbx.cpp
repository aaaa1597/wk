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
#include "CG3D.h"
#include "FBX.h"
#include "import_fbx.h"
#include "MatVec.h"

namespace fbx {
	/**********/
	/* load() */
	/**********/
	bool import_fbx::load(const std::vector<char> &ModelData) {
	//		aOperator=<bpy_struct, IMPORT_SCENE_OT_fbx("IMPORT_SCENE_OT_fbx") at 0x00000255955C5C58>
			Context		aContext;		aContext.Scene.UnitSetting.System = UnitSettingSystem::METRIC;
			std::string	aFilePath = R"(D:\Products\blender-git\dragon56-fbx\Dragon 2.5_fbx.fbx)";
			bool		aUuseManualOrientation = false;
			m::Axis		aAxisForward = m::Axis::_Z;
			m::Axis		aAxisUp = m::Axis::Y;
			double		aGlobalScale = 1.0;
			bool		aBakeSpaceTransform = false;
			bool		aUseCustomNormals = true;
			bool		aUseImageSearch = true;
			bool		aUseAlphaDecals = false;
			double		aDecalOffset = 0.0;
			bool		aUseAnim = true;
			double		aAnimOffset = 1.0;
			bool		aUseSubsurf = false;
			bool		aUseCustomProps = true;
			bool		aUseCustomPropsEnumAsString = true;
			bool		aIgnoreLeafBones = false;
			bool		aForceConnectChildren = false;
			bool		aAutomaticBoneOrientation = false;
			m::Axis		aPrimaryBoneAxis = m::Axis::Y;
			m::Axis		aSecondaryBoneAxis = m::Axis::X;
			bool		aUsePrepostRot = true;
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
			__android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa Load(%s) Failed!! Unmatch MAGICWORD!!\n", fbxmagic);
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

		/************************/
		/* 001 エレメント一括読出 */
		/************************/
		std::vector<FbxElem> rootElem;
		{
			while(true) {
				FbxElem allElems = FbxUtil::readElements(ibs);
				if(allElems.end_offset == 0)
					break;
#ifndef __ANDROID__
				//UTF-8からUTF-16へ変換
			const int nSize = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)"D:\\testaaaalog\\エレメント一覧.log", -1, NULL, 0);
			BYTE* buffUtf16 = new BYTE[nSize * 2 + 2];
			::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)"D:\\testaaaalog\\エレメント一覧.log", -1, (LPWSTR)buffUtf16, nSize);
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
		}

		/**************************/
		/* 002 GlobalSettings取得 */
		/**************************/
		/* GlobalSettingsキーを探索 */
		auto gsitr = std::find_if(rootElem.begin(), rootElem.end(), [](const FbxElem &item){ return item.id=="GlobalSettings"; });
		assert((gsitr != rootElem.end()) &&
			"error ありえない!! GlobalSettingsキーがない!!");

		auto gsP70itr = std::find_if(gsitr->elems.begin(), gsitr->elems.end(), [](const FbxElem& item) { return item.id == "Properties70"; });
		assert((gsP70itr != gsitr->elems.end()) &&
			"error ありえない!! Properties70キーがない!!");

		FbxElem &gs  = *gsitr;
		FbxElem &gsP70 = *gsP70itr;

		double unitscale	= FbxUtil::getPropNumber(gsP70, "UnitScaleFactor");
		double unitscaleOrg	= FbxUtil::getPropNumber(gsP70, "OriginalUnitScaleFactor");
		double globalscale	= aGlobalScale * (unitscale / Units2FbxFactor(aContext.Scene));

		m::Axis axisforward;
		m::Axis axisup;
		if (!aUuseManualOrientation) {
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
				std::pair<std::int64_t, std::int64_t>> axiskey = { axisup_pair, axisforward_pair, axiscoord_pair };

			std::pair<m::Axis, m::Axis> axis = RIGHT_HAND_AXES_RR.at(axiskey);
			axisforward = axis.second;
			axisup = axis.first;
		}

		/* 拡縮行列生成 */
		m::Matrix4f ScaleM = m::MatVec::createScale((float)globalscale, (float)globalscale, (float)globalscale);
		/* 軸変換行列生成 */
		m::Matrix4f AxisConvM = m::MatVec::createAxisConversion(axisforward, axisup);
		/* グローバル行列生成 */
		m::Matrix4f GlocalM = m::MatVec::MultMatrix(ScaleM, AxisConvM);
		/* グローバル逆行列生成 */
		m::Matrix4f GlocalInvM = GlocalM.getInverse();
		/* グローバル逆行列の転置生成 */
		m::Matrix4f GlocalInvTranceposeM = GlocalInvM.getTrancepose();

		m::Matrix4f BoneCorrectionMatrix;
		if (!aAutomaticBoneOrientation) {
			if((aPrimaryBoneAxis == m::Axis::Y) && (aSecondaryBoneAxis == m::Axis::X)) {
				BoneCorrectionMatrix.setIdentity();
			}
			else {
				BoneCorrectionMatrix = m::MatVec::createAxisConversion(m::Axis::X, m::Axis::Y, aSecondaryBoneAxis, aPrimaryBoneAxis);
			}
		}

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

		/* TODO : 呼び元に返却する必要がある */
		aContext.Scene.Render.Fsp		= round(realfps);
		aContext.Scene.Render.FspBase	= aContext.Scene.Render.Fsp / realfps;

		FbxImportSettings settings = {
			//.Repeat					= ???
			.toAxeiss					= {axisup, axisforward},
			.globalMatrix				= GlocalM,
			.globalScale				= globalscale,
			.bakeSpaceTransform			= aBakeSpaceTransform,
			.globalMatrixInv			= std::move(GlocalInvM),
			.globalMatrixInvTransposed	= std::move(GlocalInvTranceposeM),
			.useCustomNormals			= aUseCustomNormals,
			.useImageSearch				= aUseImageSearch,
			.useAlphaDecals				= aUseAlphaDecals,
			.decalOffset				= aDecalOffset,
			.useAnim					= aUseAnim,
			.animOffset					= aAnimOffset,
			.useSubsurf					= aUseSubsurf,
			.useCustomProps				= aUseCustomProps,
			.useCustomPropsEnumAsString = aUseCustomPropsEnumAsString,
			//.nodalMaterialWrapMap		= ???,
			//.imageCache				= ???,
			.ignoreLeafBones			= aIgnoreLeafBones,
			.forceConnectChildren		= aForceConnectChildren,
			.automaticBoneOrientation	= aAutomaticBoneOrientation,
			.boneCorrectionMatrix		= std::move(BoneCorrectionMatrix),
			.usePrepostRot				= aUsePrepostRot,
		};

		/*********************/
		/* 003 Templates取得 */
		/*********************/
		std::map<std::pair<std::string, std::string>, FbxElem> FbxTemplates = {};
		{
			/* Definitionsキーを探索 */
			auto defsitr = std::find_if(rootElem.begin(), rootElem.end(), [](const FbxElem& item) { return item.id == "Definitions"; });
			if (defsitr != rootElem.end()) {
				FbxElem &defs = *defsitr;
				for (FbxElem &fbxdef : defs.elems) {
					if (fbxdef.id == "ObjectType") {
						for (FbxElem &fbxsubdef : fbxdef.elems) {
							if (fbxsubdef.id == "PropertyTemplate") {
								assert((fbxdef.props[0].DataType() == General::Type::String) &&
									   "error ありえない!! 型がstrngでない!!");
								assert((fbxsubdef.props[0].DataType() == General::Type::String) &&
									   "error ありえない!! 型がstrngでない!!");
								std::string key1 = fbxdef.props[0].getData<std::string>();
								std::string key2 = fbxsubdef.props[0].getData<std::string>();
								std::pair<std::string, std::string> key = { key1, key2 };
								FbxTemplates.insert({ key, fbxsubdef });
							}
						}
					}
				}
			}
		}

		/*****************/
		/* 004 Nodes取得 */
		/* 参考 : http://download.autodesk.com/us/fbx/20112/FBX_SDK_HELP/index.html?url=WS73099cc142f487551fea285e1221e4f9ff8-7fda.htm,topicNumber=d0e6388 */
		/*****************/
		/* Tables: (FBX_byte_id ->[FBX_data, None or Blender_datablock]) */
		std::map<std::int64_t, std::tuple<FbxElem, cg::Mesh>> FbxTableNodes = {};
		{
			auto nodesitr = std::find_if(rootElem.begin(), rootElem.end(), [](const FbxElem& item) { return item.id == "Objects"; });
			assert((nodesitr != rootElem.end()) &&
				   "error ありえない!! Objectsキーがない!!");

			FbxElem &nodes = *nodesitr;
			for (FbxElem &fbxobj : nodes.elems) {
				assert((fbxobj.props.size() >= 3) && "error プロパティを3つ以上保持していない!!");
				assert(((fbxobj.props[0].DataType() == General::Type::Int64) && (fbxobj.props[1].DataType() == General::Type::String) && (fbxobj.props[2].DataType() == General::Type::String)) &&
					   "error プロパティがint64,string,stringの並びでない!!");
				std::int64_t fbxuuid = fbxobj.props[0].getData<std::int64_t>();
				FbxTableNodes.insert({ fbxuuid, {fbxobj, cg::Mesh()}});
			}
		}

		/***********************/
		/* 005 Connections取得 */
		/***********************/
		{
			auto consitr = std::find_if(rootElem.begin(), rootElem.end(), [](const FbxElem& item) { return item.id == "Connections"; });
			assert((consitr != rootElem.end()) &&
				   "error ありえない!! Connectionsキーがない!!");
			FbxElem &cons = *consitr;

			std::map<std::int64_t, std::map<std::int64_t, FbxElem>> FbxConnectionMap = {};
			std::map<std::int64_t, std::map<std::int64_t, FbxElem>> FbxConnectionMap_RR = {};

			for (FbxElem &fbxlink : cons.elems) {
				General &ctype = fbxlink.props[0];
				if ((fbxlink.props.size() >= 3) &&
					(fbxlink.props[1].DataType() == General::Type::Int64) && (fbxlink.props[2].DataType() == General::Type::Int64)) {
					std::int64_t csrc = fbxlink.props[1].getData<std::int64_t>();
					std::int64_t cdst = fbxlink.props[2].getData<std::int64_t>();
					FbxConnectionMap.insert({ csrc, {{cdst, fbxlink}} });
					FbxConnectionMap_RR.insert({ cdst, {{csrc, fbxlink}} });
				}
			}
		}

		/*****************/
		/* 006 Meshes取得 */
		/*****************/
		{
			//	FbxElem &fbxtmpl = FbxTemplates.at({ "Geometry", "KFbxMesh" });	/* 最新のFBX（7.4以降）では、タイプ名に「K」が使用されなくなりました。 */
			FbxElem &fbxtmpl = FbxTemplates.at({ "Geometry", "FbxMesh" });

			for(auto &FbxTableNode : FbxTableNodes) {
				//std::map<std::int64_t, std::tuple<FbxElem, cg::Cg3d>> FbxTableNodes = {};
				FbxElem &fbxobj = std::get<0>(FbxTableNode.second);
				if(fbxobj.id != "Geometry")
					continue;

				cg::Mesh &mesh = std::get<1>(FbxTableNode.second);
				if (fbxobj.props[fbxobj.props.size()-1].getData<std::string>() == "Mesh") {
					mesh = FbxUtil::cg3dReadGeometry(fbxtmpl, fbxobj, settings);
				}
			}
		}

		/********************************/
		/* 007 Materials & Textures取得 */
		/********************************/
		{
//			FbxElem &fbxtmpl = FbxTemplates.at({ "Material", "KFbxSurfacePhong" });	/* 最新のFBX（7.4以降）では、タイプ名に「K」が使用されなくなりました。 */
			FbxElem& fbxtmpl = FbxTemplates.at({ "Material", "FbxSurfacePhong" });
			for(auto &FbxTableNode : FbxTableNodes) {
				FbxElem &fbxobj = std::get<0>(FbxTableNode.second);
				if(fbxobj.id != "Material")
					continue;

				cg::Mesh &mesh = std::get<1>(FbxTableNode.second);
//				assert(mesh is None)
				cg::Material mat = FbxUtil::cg3dReadMaterial(fbxtmpl, fbxobj, settings);
//				fbx_item[1] = blen_read_material(fbxtmpl, fbxobj, settings)
			}
		}

		/***************************/
		/* 008 Cameras & Lamps取得 */
		/***************************/

		/******************************/
		/* 009 Objects & Armatures取得 */
		/******************************/

		/*********************/
		/* 010 ShapeKeys取得 */
		/*********************/

		/********************************/
		/* 011 Subdivision surfaces取得 */
		/********************************/

		/**********************/
		/* 012 Animations取得 */
		/**********************/

		/****************************/
		/* 013 Assign materials取得 */
		/****************************/

		/***************************/
		/* 014 Assign textures取得 */
		/***************************/

		/**************************************/
		/* 015 Cycles z-offset workaround取得 */
		/**************************************/

		return true;
	}
} /* namespace fbx */

