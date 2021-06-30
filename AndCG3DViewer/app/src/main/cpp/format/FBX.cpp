#pragma warning(disable : 4819)
//
// Created by jun on 2021/06/15.
//
#ifdef __ANDROID__
#include <android/log.h>
#include <zlib.h>
#else   /* __ANDROID__ */
#define NOMINMAX
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/zlibsrc/zlib.h"
#endif  /* __ANDROID__ */
#include <functional>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cassert>
#include "../CG3DCom.h"
#include "FBX.h"

namespace fbx {

template<typename T> T				General::getData() const { T ret; return ret; }
template<> std::int16_t				General::getData() const { return Int16		; }
template<> bool						General::getData() const { return Bool		; }
template<> std::int32_t				General::getData() const { return Int32		; }
template<> float					General::getData() const { return Float		; }
template<> double					General::getData() const { return Double	; }
template<> std::int64_t				General::getData() const { return Int64		; }
template<> std::vector<char>		General::getData() const { return Bin		; }
template<> std::string				General::getData() const { return Str		; }
template<> std::vector<float>		General::getData() const { return AryFloat	; }
template<> std::vector<std::int32_t>General::getData() const { return AryInt32	; }
template<> std::vector<double>		General::getData() const { return AryDouble	; }
template<> std::vector<std::int64_t>General::getData() const { return AryInt64	; }
template<> std::vector<byte>		General::getData() const { return AryBool	; }
template<> std::vector<signed char>	General::getData() const { return AryByte	; }

template<typename T> void General::swapData(T &&t) {}
template<> void General::swapData(std::int16_t				&&t) { assert((datatype==Type::Int16)	&& "aaaa error!! 型不一致なのでswapできない!!"); Int16 = t; }
template<> void General::swapData(bool						&&t) { assert((datatype==Type::Bool)	&& "aaaa error!! 型不一致なのでswapできない!!"); Bool = t; }
template<> void General::swapData(std::int32_t				&&t) { assert((datatype==Type::Int32)	&& "aaaa error!! 型不一致なのでswapできない!!"); Int32 = t; }
template<> void General::swapData(float						&&t) { assert((datatype==Type::Float)	&& "aaaa error!! 型不一致なのでswapできない!!"); Float = t; }
template<> void General::swapData(double					&&t) { assert((datatype==Type::Double)	&& "aaaa error!! 型不一致なのでswapできない!!"); Double = t; }
template<> void General::swapData(std::int64_t				&&t) { assert((datatype==Type::Int64)	&& "aaaa error!! 型不一致なのでswapできない!!"); Int64 = t; }
template<> void General::swapData(std::vector<char>			&&t) { assert((datatype==Type::Bin)		&& "aaaa error!! 型不一致なのでswapできない!!"); Bin.clear();      std::vector<char>        ().swap(Bin)      ; Bin      = std::move(t); }
template<> void General::swapData(std::string				&&t) { assert((datatype==Type::Str)		&& "aaaa error!! 型不一致なのでswapできない!!"); Str = t; }
template<> void General::swapData(std::vector<float>		&&t) { assert((datatype==Type::AryFloat)&& "aaaa error!! 型不一致なのでswapできない!!"); AryFloat.clear(); std::vector<float>       ().swap(AryFloat) ; AryFloat = std::move(t); }
template<> void General::swapData(std::vector<std::int32_t>	&&t) { assert((datatype==Type::AryInt32)&& "aaaa error!! 型不一致なのでswapできない!!"); AryInt32.clear(); std::vector<std::int32_t>().swap(AryInt32) ; AryInt32 = std::move(t); }
template<> void General::swapData(std::vector<double>		&&t) { assert((datatype==Type::AryDouble)&&"aaaa error!! 型不一致なのでswapできない!!"); AryDouble.clear();std::vector<double>      ().swap(AryDouble); AryDouble= std::move(t); }
template<> void General::swapData(std::vector<std::int64_t>	&&t) { assert((datatype==Type::AryInt64)&& "aaaa error!! 型不一致なのでswapできない!!"); AryInt64.clear(); std::vector<std::int64_t>().swap(AryInt64) ; AryInt64 = std::move(t); }
template<> void General::swapData(std::vector<byte>			&&t) { assert((datatype==Type::AryBool)	&& "aaaa error!! 型不一致なのでswapできない!!"); AryBool.clear();  std::vector<byte>        ().swap(AryBool)  ; AryBool  = std::move(t); }
template<> void General::swapData(std::vector<signed char>	&&t) { assert((datatype==Type::AryByte)	&& "aaaa error!! 型不一致なのでswapできない!!"); AryByte.clear();  std::vector<signed char> ().swap(AryByte)  ; AryByte  = std::move(t); }

General General::pickData(std::istream &ios) {
	General ret;
	char typebuf;
	ios.read(&typebuf, sizeof(char));
	ret.datatype = (General::Type)typebuf;

	switch (typebuf) {
	case 'Y': ios.read(reinterpret_cast<char*>(&ret.Int16), sizeof(ret.Int16));		break;
	case 'C': ios.read(reinterpret_cast<char*>(&ret.Bool), sizeof(ret.Bool));		break;
	case 'I': ios.read(reinterpret_cast<char*>(&ret.Int32), sizeof(ret.Int32));		break;
	case 'F': ios.read(reinterpret_cast<char*>(&ret.Float), sizeof(ret.Float));		break;
	case 'D': ios.read(reinterpret_cast<char*>(&ret.Double), sizeof(ret.Double));	break;
	case 'L': ios.read(reinterpret_cast<char*>(&ret.Int64), sizeof(ret.Int64));		break;
	case 'R': {
		int size;
		ios.read(reinterpret_cast<char*>(&size), sizeof(size));
		ret.Bin.resize(size);
		ios.read(reinterpret_cast<char*>(ret.Bin.data()), size);
	}
			break;
	case 'S': {
		int size;
		ios.read(reinterpret_cast<char*>(&size), sizeof(size));
		std::vector<char> tmpstr; tmpstr.resize(size);
		ios.read(reinterpret_cast<char*>(tmpstr.data()), size);
		std::replace(tmpstr.begin(), tmpstr.end(), 0, 1);		/* 本来は"0x00,0x01"の並びだけど、std::stringの制約で"0x01,0x01"に変更した */
		ret.Str = std::string(tmpstr.begin(), tmpstr.end());
	}
			break;
	case 'f':	ret.AryFloat = FbxUtil::readArray<float>(ios);			break;
	case 'i':	ret.AryInt32 = FbxUtil::readArray<std::int32_t>(ios);	break;
	case 'd':	ret.AryDouble= FbxUtil::readArray<double>(ios);			break;
	case 'l':	ret.AryInt64 = FbxUtil::readArray<std::int64_t>(ios);	break;
	case 'b':	ret.AryBool  = FbxUtil::readArray<byte>(ios);			break;
	case 'c':	ret.AryByte  = FbxUtil::readArray<signed char>(ios);	break;
	default:  assert(false && "no impliment!! nonType");				break;
	}

	return ret;
}

bool FbxUtil::mIsInitCalled = false;

bool FbxUtil::init(Version ver) {
	if (ver < Version::FBX_FILE_VERSION_7100) {
		__android_log_print(ANDROID_LOG_DEBUG, "aaaaa", "aaaaa fbx format is notsuported!! version is too old. ver=%d", ver);
		return false;
	}

	FbxUtil::mIsInitCalled = true;
	FbxUtil& utl = FbxUtil::GetIns();
	if (ver < Version::FBX_FILE_VERSION_7500) {
		utl.mNullRecordLen = 13;
		static const char NullRecord[13] = { 0 };
		utl.mNullRecord = NullRecord;
	}
	else {
		utl.mNullRecordLen = 25;
		static const char NullRecord[25] = { 0 };
		utl.mNullRecord = NullRecord;
	}
	return true;
}

FbxElem FbxUtil::readElements(std::istream &ibs) {
	FbxElem retFbxGeneral;
	FbxUtil &util = FbxUtil::GetIns();

	/* 終了位置読込み */
	std::int64_t end_offset = util.read4or8(ibs);
	retFbxGeneral.end_offset = end_offset;
	if (end_offset == 0) {
		return retFbxGeneral;
	}

	/* プロパティ数読込み */
	std::int64_t prop_count  = util.read4or8(ibs);
	std::int64_t prop_length = util.read4or8(ibs);

	retFbxGeneral.id = FbxUtil::readcString(ibs);

	/* プロパティ読込み */
	for(int lpct = 0; lpct < prop_count; lpct++) {
		retFbxGeneral.props.push_back(General::pickData(ibs));
	}

	/* 配下Element読込み */
	if(ibs.tellg() < end_offset) {
		while (ibs.tellg() < (end_offset - util.NullRecordLen())) {
			retFbxGeneral.elems.push_back(FbxUtil::readElements(ibs));
		}
		std::vector<char> nullrecord = util.readNullRecord(ibs);
		assert(std::memcmp(util.NullRecord(), &(nullrecord[0]), util.NullRecordLen()) == 0 && 
				"UnSpported Format NullRecord!! 値は0しかサポートしません");
	}

	/* 終了位置チェック */
	assert((ibs.tellg() == end_offset) &&
		CG3D::format("UnSpported Format ibs.tellg() != end_offset!! end_offset=", end_offset).c_str());

	return retFbxGeneral;
}

double FbxUtil::getPropNumber(const FbxElem &elem, const std::string &key) {
	const std::vector<FbxElem>& subelms = elem.elems;
	auto finded = std::find_if(subelms.begin(), subelms.end(), [&key](const FbxElem& subelm) {
			assert(subelm.id == "P" && "aaaaa フォーマット不正 'P'でない!!");
			if (subelm.props.size() == 0) return false;
			if (subelm.props.at(0).getData<std::string>() == key)
				return true;
			return false;
		});

	if (finded == elem.elems.end())
		return std::numeric_limits<double>::max();	/* 見つからない。 */

	const FbxElem &findelm = *finded;
	if (findelm.props[1].getData<std::string>() == "double") {
		assert(findelm.props[2].getData<std::string>() == "Number"	&& "aaaaa フォーマット不正");
	}
	else {
		assert(findelm.props[1].getData<std::string>() == "Number"	&& "aaaaa フォーマット不正");
		assert(findelm.props[2].getData<std::string>() == ""		&& "aaaaa フォーマット不正");
	}
	assert(findelm.props[4].DataType() == General::Type::Double && "aaaaa フォーマット不正");

	return findelm.props[4].getData<double>();
}

std::int64_t FbxUtil::getPropInteger(const FbxElem &elem, const std::string& key) {
	const std::vector<FbxElem>& subelms = elem.elems;
	auto finded = std::find_if(subelms.begin(), subelms.end(), [&key](const FbxElem& subelm) {
		assert(subelm.id == "P" && "aaaaa フォーマット不正 'P'でない!!");
		if (subelm.props.size() == 0) return false;
		if (subelm.props.at(0).getData<std::string>() == key)
			return true;
		return false;
	});

	if (finded == elem.elems.end())
		return std::numeric_limits<std::int64_t>::max();	/* 見つからない。 */

	const FbxElem &findelm = *finded;
	if (findelm.props[1].getData<std::string>() == "int") {
		assert(findelm.props[2].getData<std::string>() == "Integer"	&& "aaaaa フォーマット不正");
	}
	else if (findelm.props[1].getData<std::string>() == "ULongLong") {
		assert(findelm.props[2].getData<std::string>() == ""		&& "aaaaa フォーマット不正");
	}
	assert((findelm.props[4].DataType() == General::Type::Int32 || findelm.props[4].DataType() == General::Type::Int64) && "aaaaa フォーマット不正");

	if(findelm.props[4].DataType() == General::Type::Int32)
		return findelm.props[4].getData<std::int32_t>();
	else
		return findelm.props[4].getData<std::int64_t>();
}

std::int32_t FbxUtil::getPropEnum(FbxElem &elem, const std::string &key) {
	const std::vector<FbxElem>& subelms = elem.elems;
	auto finded = std::find_if(subelms.begin(), subelms.end(), [&key](const FbxElem& subelm) {
		assert(subelm.id == "P" && "aaaaa フォーマット不正 'P'でない!!");
		if (subelm.props.size() == 0) return false;
		if (subelm.props.at(0).getData<std::string>() == key)
			return true;
		return false;
	});

	if (finded == elem.elems.end())
		return std::numeric_limits<std::int32_t>::max();	/* 見つからない。 */

	const FbxElem &findelm = *finded;
	assert(findelm.props[1].getData<std::string>() == "enum" && "aaaaa フォーマット不正");
	assert(findelm.props[2].getData<std::string>() == ""	 && "aaaaa フォーマット不正");
	assert(findelm.props[3].getData<std::string>() == ""	 && "aaaaa フォーマット不正");

	return findelm.props[4].getData<std::int32_t>();
}

std::tuple<std::string, std::string> FbxUtil::splitNameClass(const FbxElem &elm) {
	std::string ret1 = "", ret2 = "";
	std::string tmpstr = elm.props[elm.props.size()-2].getData<std::string>();
	size_t pos = tmpstr.find("\x01\x01", 0);	/* 本来は"0x00,0x01"の並びだけど、std::stringの制約で"0x01,0x01"に変更した */
	ret1 = tmpstr.substr(0, pos);
	ret2 = tmpstr.substr(pos+2);
	return { ret1, ret2 };
}

std::string FbxUtil::getElemNameEnsureClass(const FbxElem &elem, const std::string &classname) {
	std::string elemName, elemClass;
	std::tie(elemName, elemClass) = FbxUtil::splitNameClass(elem);
	assert((elemClass==classname) && "aaaaa error!! (elemClass!=classname)");
	return elemName;
}

std::tuple<std::string, std::string, std::string> FbxUtil::cg3dReadGeometryLayerInfo(std::vector<FbxElem>::const_iterator &itr) {
	std::string retName, retMapping, retRef;

	auto fbxitemitr = std::find_if(itr->elems.begin(), itr->elems.end(), [](const FbxElem& item) { return item.id == "Name"; });
	if (fbxitemitr != itr->elems.end()) {
		assert(fbxitemitr->props.size() == 1);
		assert(fbxitemitr->props[0].DataType() == General::Type::Str);
		retName = fbxitemitr->props[0].getData<std::string>();
	}

	auto mappingitr = std::find_if(itr->elems.begin(), itr->elems.end(), [](const FbxElem& item) { return item.id == "MappingInformationType"; });
	if (mappingitr != itr->elems.end()) {
		assert(mappingitr->props.size() == 1);
		assert(mappingitr->props[0].DataType() == General::Type::Str);
		retMapping = mappingitr->props[0].getData<std::string>();
	}

	auto refitr = std::find_if(itr->elems.begin(), itr->elems.end(), [](const FbxElem& item) { return item.id == "ReferenceInformationType"; });
	if (refitr != itr->elems.end()) {
		assert(refitr->props.size() == 1);
		assert(refitr->props[0].DataType() == General::Type::Str);
		retRef = refitr->props[0].getData<std::string>();
	}

	return {retName, retMapping, retRef};
}

cg::Cg3d FbxUtil::cg3dReadGeometry(const FbxElem& fbxtmpl, const FbxElem &elm, FbxImportSettings &settings) {
	cg::Mash retMesh;
	cg::Cg3d retaaa;

	CG3DMatrix4 IdentityM;
	IdentityM.setIdentity();

	const CG3DMatrix4 &geomMatCo = (settings.bakeSpaceTransform) ? settings.globalMatrix : IdentityM;
		  CG3DMatrix4 &geomMatNo = (settings.bakeSpaceTransform) ? settings.globalMatrixInvTransposed : IdentityM;
	if (settings.bakeSpaceTransform) {
		geomMatNo.normalize();
	}

	std::string elemName = FbxUtil::getElemNameEnsureClass(elm, "Geometry");

	/* Verticesキーの最初のプロパティを取得 */
	auto verticesitr = std::find_if(elm.elems.begin(), elm.elems.end(), [](const FbxElem &item){ return item.id == "Vertices"; });
	General fbxverts = (verticesitr != elm.elems.end()) ? verticesitr->props[0] : General();

	/* PolygonVertexIndexキーの最初のプロパティを取得 */
	auto polysitr = std::find_if(elm.elems.begin(), elm.elems.end(), [](const FbxElem& item) { return item.id == "PolygonVertexIndex"; });
	General fbxpolys = (polysitr != elm.elems.end()) ? polysitr->props[0] : General();

	/* Edgesキーの最初のプロパティを取得 */
	auto edgesitr = std::find_if(elm.elems.begin(), elm.elems.end(), [](const FbxElem& item) { return item.id == "Edges"; });
	General fbxedges = (edgesitr != elm.elems.end()) ? edgesitr->props[0] : General();

	if (settings.bakeSpaceTransform) {
		std::vector<double> tmpvrtxs = std::move(fbxverts.getData<std::vector<double>>());
		std::vector<double> tmpvrtxs2(tmpvrtxs.size());
		for (std::vector<double>::iterator itr = tmpvrtxs.begin(); itr != tmpvrtxs.end();) {
			CG3DVector3 v(*itr, *(itr + 1), *(itr + 2));
			CG3DVector3 v2 = geomMatCo * v;
			tmpvrtxs2.push_back(v2.x);
			tmpvrtxs2.push_back(v2.y);
			tmpvrtxs2.push_back(v2.z);
			itr += 3;
			size_t idx = std::distance(tmpvrtxs.begin(), itr);
			if (tmpvrtxs.size() - idx < 3)
				break;
		}
		fbxverts.swapData<std::vector<double>>(std::move(tmpvrtxs2));
	}

	std::vector<double> fbxvertsflat = std::move(fbxverts.getData<std::vector<double>>());

	retMesh.name  = elemName;
	retMesh.Vertexs.reserve(fbxvertsflat.size()/3);
	for (std::vector<double>::iterator itr = fbxvertsflat.begin(); itr != fbxvertsflat.end();) {
		CG3DVector3 v(*itr, *(itr+1), *(itr+2));
		retMesh.Vertexs.push_back({ .Co = v });
		itr += 3;
		size_t idx = std::distance(fbxvertsflat.begin(), itr);
		if (fbxvertsflat.size() - idx < 3)
			break;
	}

	/* Mesh::PolygonsにObjectを生成 */
	std::vector<std::int32_t> fbxpolysflat = fbxpolys.getData<std::vector<std::int32_t>>();
	if (fbxpolysflat.size() > 0) {
		retMesh.Loops.resize(fbxpolysflat.size());
		int polyloopprev = 0;
		for (int lpct = 0; lpct < fbxpolysflat.size(); lpct++) {
			int idx = fbxpolysflat[lpct];
			if (idx < 0) {
				cg::Polygon polygon;
				polygon.LoopStarts = polyloopprev;
				polygon.LoopTotals = (lpct - polyloopprev) + 1;
				polyloopprev = lpct + 1;
				retMesh.Polygons.push_back(polygon);
				idx ^= -1;
			}
			retMesh.Loops[lpct].VertexIndex = idx;
		}

		/* Mesh::Polygons::MaterialIndexに値を設定 */
		auto layerMatitr = std::find_if(elm.elems.begin(), elm.elems.end(), [](const FbxElem &item) { return item.id == "LayerElementMaterial"; });
		if (layerMatitr != elm.elems.end()) {
			auto materialsitr = std::find_if(layerMatitr->elems.begin(), layerMatitr->elems.end(), [](const FbxElem &item) { return item.id == "Materials"; });
			General fbxlayerdata = (materialsitr != layerMatitr->elems.end() && materialsitr->props.size() > 0) ? materialsitr->props[0] : General();

			std::vector<std::int32_t> fbxlayerdataIdentity = std::move( fbxlayerdata.getData<std::vector<std::int32_t>>() );
			assert(retMesh.Polygons.size() == fbxlayerdataIdentity.size());
			for(int lpct = 0; lpct <fbxlayerdataIdentity.size(); lpct++) {
				retMesh.Polygons[lpct].MaterialIndex = fbxlayerdataIdentity[lpct];
			}
		}

		/* Mesh::UvLayers::UvDataに値を設定 */
		int sposuv = 0;
		while (true) {
			auto layerUVitr = std::find_if(elm.elems.begin()+sposuv, elm.elems.end(), [](const FbxElem &item) { return item.id == "LayerElementUV"; });
			if (layerUVitr == elm.elems.end())
				break;

			sposuv = std::distance(elm.elems.begin(), layerUVitr) + 1;

			std::string name, mapping, ref;
			std::tie(name, mapping, ref) = FbxUtil::cg3dReadGeometryLayerInfo(layerUVitr);

			auto fromlayerdataitr = std::find_if(layerUVitr->elems.begin(), layerUVitr->elems.end(), [](const FbxElem &item) { return item.id == "UV"; });
			const General &fromlayerdataGeneral = fromlayerdataitr->props[0];
			std::vector<double> fromlayerdata = std::move(fromlayerdataGeneral.getData<std::vector<double>>());
			if(fromlayerdata.size() == 0)
				continue;

			auto fromlayeridxitr = std::find_if(layerUVitr->elems.begin(), layerUVitr->elems.end(), [](const FbxElem &item) { return item.id == "UVIndex"; });
			const General &fromlayeridxGeneral = fromlayeridxitr->props[0];
			std::vector<std::int32_t> fromlayeridx = std::move(fromlayeridxGeneral.getData<std::vector<std::int32_t>>());
			std::for_each(fromlayeridx.begin(), fromlayeridx.end(), [](std::int32_t &item){item*=2;});

			cg::UvLayer &touvlay = retMesh.UvLayers;
			touvlay.Name = name;
			touvlay.UvData.reserve(fromlayeridx.size());
			for (int lpct = 0; lpct < fromlayeridx.size(); lpct++)
				touvlay.UvData.push_back(CG3DVector2((float)fromlayerdata[fromlayeridx[lpct]], (float)fromlayerdata[fromlayeridx[lpct]+1]));
		}

		/* Mesh::ColorLayers::ColorDataに値を設定 */
		int sposcolor = 0;
		while (true) {
			auto layerColoritr = std::find_if(elm.elems.begin()+sposcolor, elm.elems.end(), [](const FbxElem &item) { return item.id == "LayerElementColor"; });
			if (layerColoritr == elm.elems.end())
				break;

			sposcolor = std::distance(elm.elems.begin(), layerColoritr) + 1;

			std::string name, mapping, ref;
			std::tie(name, mapping, ref) = FbxUtil::cg3dReadGeometryLayerInfo(layerColoritr);

			auto fromlayerdataitr = std::find_if(layerColoritr->elems.begin(), layerColoritr->elems.end(), [](const FbxElem &item) { return item.id == "Colors"; });
			const General &fromlayerdataGeneral = fromlayerdataitr->props[0];
			std::vector<std::int32_t> fromlayerdata = std::move(fromlayerdataGeneral.getData<std::vector<std::int32_t>>());
			if(fromlayerdata.size() == 0)
				continue;

			auto fromlayeridxitr = std::find_if(layerColoritr->elems.begin(), layerColoritr->elems.end(), [](const FbxElem &item) { return item.id == "ColorIndex"; });
			const General &fromlayeridxGeneral = fromlayeridxitr->props[0];
			std::vector<std::int32_t> fromlayeridx = std::move(fromlayeridxGeneral.getData<std::vector<std::int32_t>>());
			std::for_each(fromlayeridx.begin(), fromlayeridx.end(), [](std::int32_t &item){item*=3;});

			cg::ColorLayer &toclrlay = retMesh.ColorLayers;
			toclrlay.Name = name;
			toclrlay.ColorData.reserve(fromlayeridx.size());
			for (int lpct = 0; lpct < fromlayeridx.size(); lpct++)
				toclrlay.ColorData.push_back(CG3DVector3(fromlayerdata[fromlayeridx[lpct]], fromlayerdata[fromlayeridx[lpct]+1], fromlayerdata[fromlayeridx[lpct]+2]));
		}

		/* Mesh::Edgesに値を設定 */
		if(fbxedges.DataType() != General::Type::Empty) {
			/* edges in fact index the polygons (NOT the vertices)*/
			std::vector<std::int32_t> fbxedgessrc = fbxedges.getData<std::vector<std::int32_t>>();
			std::vector<std::int32_t> fbxedgesconvdst(fbxedgessrc.size()*2);
			std::int32_t edgeidx = 0;
			for(std::int32_t i : fbxedgessrc) {
				std::int32_t e_a = fbxpolysflat[i];
				std::int32_t e_b;
				if(e_a >= 0) {
					e_b = fbxpolysflat[i + 1];
					if(e_b < 0)
						e_b ^= -1;
				}
				else{
					/*# Last index of polygon, wrap back to the start. */
					/*# ideally we wouldn't have to search back, but it should only be 2-3 iterations.*/
					std::int32_t j = i - 1;
					while((j >= 0) && (fbxpolysflat[j] >= 0)) {
						j -= 1;
					}
					e_a ^= -1;
					e_b = fbxpolysflat[j + 1];
				}

				fbxedgesconvdst[edgeidx] = e_a;
				fbxedgesconvdst[edgeidx + 1] = e_b;
				edgeidx += 2;
			}

			retMesh.Edges.resize(fbxedgesconvdst.size()/2);
			for(int lpct = 0; lpct < retMesh.Edges.size(); lpct++)
				retMesh.Edges[lpct] = {.Vertices = CG3DVector2i(fbxedgesconvdst[lpct*2],fbxedgesconvdst[lpct*2 +1]) };
		}

		/* Mesh::Smoothに値を設定 */
		bool ok_smooth = [&elm, &mesh=retMesh](){
			auto smoothitr = std::find_if(elm.elems.begin(), elm.elems.end(), [](const FbxElem &item) { return item.id == "LayerElementSmoothing"; });
			if (smoothitr == elm.elems.end())
				return false;

			/* name名, mapping名, ref名 */
			std::string name, mapping, ref;
			std::tie(name, mapping, ref) = FbxUtil::cg3dReadGeometryLayerInfo(smoothitr);

			/* Smoothingエレメント取得 */
			auto smoothingitr = std::find_if(smoothitr->elems.begin(), smoothitr->elems.end(), [](const FbxElem &item) { return item.id == "Smoothing"; });
			if (smoothingitr == smoothitr->elems.end()) 
				return false;

			/* Smoothingエレメントのプロパティ取得 */
			const std::vector<byte> &srcfbxlayerdata = smoothingitr->props[0].getData<std::vector<byte>>();

			if (mapping == "ByEdge") {
				/* some models have bad edge data, we cant use this info...*/
				if (mesh.Edges.empty()) {
					__android_log_print(ANDROID_LOG_INFO, "aaaaa", "warning skipping sharp edges data, no valid edges...");
					return false;
				}

				std::vector<cg::Edge> &dstcg3ddata = mesh.Edges;
				dstcg3ddata.resize(srcfbxlayerdata.size());
				for(int lpct = 0; lpct < dstcg3ddata.size(); lpct++)
					dstcg3ddata[lpct].UseEdgeSharp = (srcfbxlayerdata[lpct]==(byte)0x00) ? false : true;
				/* We only set sharp edges here, not face smoothing itself...*/
				mesh.UseAutoSmooth = true;
				return false;
			}
			else if (mapping == "ByPolygon") {
				std::vector<cg::Polygon> &dstcg3ddata = mesh.Polygons;
				dstcg3ddata.resize(srcfbxlayerdata.size());
				for(int lpct = 0; lpct < dstcg3ddata.size(); lpct++)
					dstcg3ddata[lpct].UseSmooth = (srcfbxlayerdata[lpct]==(byte)0x00) ? false : true;
			}
			else {
				__android_log_print(ANDROID_LOG_INFO, "aaaaa", "warning layer %s mapping type unsupported: %s", smoothitr->id.c_str(), mapping.c_str());
				return false;
			}
		}();

	}

	std::vector<std::int32_t> fbxedgesflat = fbxedges.getData<std::vector<std::int32_t>>();
	if (fbxedgesflat.size() > 0) {
		
	}

	int aaaa = 0;

	return retaaa;
}

char FbxUtil::read1(std::istream &iostream) {
	char ret;
	iostream.read(&ret, sizeof(char));
	return ret;
}

std::int64_t FbxUtil::read4or8(std::istream &iostream) {
	std::int64_t ret = 0;
	if (mNullRecordLen == 13) {
		int tmpbuf;
		iostream.read(reinterpret_cast<char*>(&tmpbuf), sizeof(int));
		ret = tmpbuf;
	}
	else {
		std::int64_t tmpbuf;
		iostream.read(reinterpret_cast<char*>(&tmpbuf), sizeof(int));
		ret = tmpbuf;
	}
	return ret;
}

int FbxUtil::readInt(std::istream &istream) {
	int tmpbuf;
	istream.read(reinterpret_cast<char*>(&tmpbuf), sizeof(int));
	return tmpbuf;
}

/* 文字列長がcharで定義されている版 */
std::string FbxUtil::readcString(std::istream &iostream) {
	char tmpbuf;
	iostream.read(&tmpbuf, sizeof(char));
	char tmpstr[256] = { 0 };
	iostream.read(tmpstr, tmpbuf);
	return std::string(tmpstr);
}

/* 文字列長がintで定義されている版 */
std::string FbxUtil::readiString(std::istream& iostream) {
	int tmpbuf;
	iostream.read(reinterpret_cast<char*>(&tmpbuf), sizeof(int));
	char tmpstr[256] = { 0 };
	iostream.read(tmpstr, tmpbuf);
	return std::string(tmpstr);
}

std::vector<char> FbxUtil::readNullRecord(std::istream& iostream) const {
	std::vector<char> retbuf(mNullRecordLen);
	iostream.read(&(retbuf[0]), mNullRecordLen);
	/* 読込みチェック */
	assert(retbuf.size() == mNullRecordLen &&
		CG3D::format("UnSpported Format NullRecord!! NullRecordのサイズは13or25しかサポートしていません。NullRecordサイズ=", retbuf.size()).c_str());

	return retbuf;
}

template <typename T>
std::vector<T> FbxUtil::readArray(std::istream &iostream) {
	std::vector<T> ret = {};

	int dstlen, encoding, srclen;
	iostream.read(reinterpret_cast<char*>(&dstlen  ), sizeof(int));
	iostream.read(reinterpret_cast<char*>(&encoding), sizeof(int));
	iostream.read(reinterpret_cast<char*>(&srclen  ), sizeof(int));

	if (encoding == 0) {
		ret.resize(srclen);
		iostream.read(reinterpret_cast<char*>(ret.data()), srclen);
	}
	else if (encoding == 1) {
		std::vector<T> src(srclen);
		iostream.read(reinterpret_cast<char*>(src.data()), srclen);
		unsigned long actualdstsize = dstlen*sizeof(T);
		ret.resize(dstlen);
		int ret1 = uncompress((unsigned char*)ret.data(), &actualdstsize,
							  (unsigned char*)src.data(), (unsigned long)srclen);
		/* 解答結果チェック */
		assert((ret1 == 0) && 
			CG3D::format("fail uncompress()!! 解凍失敗!! ret1=", ret1).c_str());

		/* 解答結果チェック2 */
		assert((dstlen*sizeof(T) == actualdstsize) &&
			CG3D::format("fail uncompress()!! 解凍後のサイズが合わない!! ",
									 "予定サイズ=", (dstlen*sizeof(T)),
									 "実サイズ=", actualdstsize).c_str());
	}

	return ret;
}

std::string FbxElem::toString(int hierarchy) {
	std::string ret;

	std::string intent;
	for(int lpct = 0; lpct < hierarchy; lpct++)
		intent += "\t";

	ret = intent + id +"(" + std::to_string(end_offset) + ")=";

	ret += "(" + std::to_string(props.size()) + ")(";
	for (General item : props) {
		ret += item.toString() + ",";
	}
	ret += ")\n";

	ret += intent + "SubElems数(" + std::to_string(elems.size()) + ")\n";
	for (FbxElem item : elems) {
		ret += item.toString(hierarchy+1);
	}

	return ret;
}

std::string General::toString() {
	std::string ret = "'";
	ret += (char)datatype;
	ret += "':";

	std::ostringstream oss;
	switch (datatype) {
		case (Type)'Y': ret += std::to_string(Int16) ; break;
		case (Type)'C': ret += std::to_string(Bool)  ; break;
		case (Type)'I': ret += std::to_string(Int32) ; break;
		case (Type)'F': ret += std::to_string(Float) ; break;
		case (Type)'D': ret += std::to_string(Double); break;
		case (Type)'L': ret += std::to_string(Int64) ; break;
		case (Type)'R':
			for(size_t lp=0; lp<((Bin.size()>10)?10:Bin.size());lp++)
				oss << Bin[lp] << ',';
			ret += oss.str();
			ret += (Bin.size()>10)?"...":"";
			break;
		case (Type)'S': ret += Str;                    break;
		case (Type)'f':
			for(size_t lp=0; lp<((AryFloat.size()>10)?10:AryFloat.size());lp++)
				oss << AryFloat[lp] << ',';
			ret += oss.str();
			ret += (AryFloat.size()>10)?"...":"";
			break;
		case (Type)'i':
			for(size_t lp=0; lp<((AryInt32.size()>10)?10:AryInt32.size()); lp++)
				oss << AryInt32[lp] << ',';
			ret += oss.str();
			ret += (AryInt32.size()>10)?"...":"";
			break;
		case (Type)'d':
			for(size_t lp=0; lp<((AryDouble.size()>10)?10:AryDouble.size()); lp++)
				oss << AryDouble[lp] << ',';
			ret += oss.str();
			ret += (AryDouble.size()>10)?"...":"";
			break;
		case (Type)'l':
			for(size_t lp=0; lp<((AryInt64.size()>10)?10:AryInt64.size()); lp++)
				oss << AryInt64[lp] << ',';
			ret += oss.str();
			ret += (AryInt64.size()>10)?"...":"";
			break;
		case (Type)'b':
			for(size_t lp=0; lp<((AryBool.size()>10)?10:AryBool.size()); lp++)
				oss << (unsigned char)AryBool[lp] << ',';
			ret += oss.str();
			ret += (AryBool.size()>10)?"...":"";
			break;
		case (Type)'c':
			for(size_t lp=0; lp<((AryByte.size()>10)?10:AryByte.size()); lp++)
				oss << AryByte[lp] << ',';
			ret += oss.str();
			ret += (AryByte.size()>10)?"...":"";
			break;
		default:  assert(false && "no impliment!! unKnownnType"); break;
	}
	return ret;
}

double Units2FbxFactor(Scene scene) {
	if (scene.UnitSetting.System == UnitSettingSystem::None)
		return 100.0;
	else {
		return (100.0 * scene.UnitSetting.ScaleLength);
	}
}

}	/* namespace fbx */
