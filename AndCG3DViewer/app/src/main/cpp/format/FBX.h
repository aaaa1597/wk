//
// Created by jun on 2021/06/15.
//

#ifndef CG3DVIEWER_FBX_H
#define CG3DVIEWER_FBX_H

#include <cstdint>
#include <stdexcept>
#include <istream>
#include <vector>
#include <map>
#include "CG3D.h"
#include "MatrixVector.h"

namespace fbx {

enum class Version : int {
    FBX_FILE_VERSION_6100 = 6100,
	FBX_FILE_VERSION_7000 = 7000,
	FBX_FILE_VERSION_7099 = 7099,
	FBX_FILE_VERSION_7100 = 7100,
	FBX_FILE_VERSION_7200 = 7200,
	FBX_FILE_VERSION_7300 = 7300,
	FBX_FILE_VERSION_7400 = 7400,
	FBX_FILE_VERSION_7500 = 7500,
};

const std::map<std::pair<Axis, Axis>, std::tuple<std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>>>
RIGHT_HAND_AXES = {
//	Up, Forward->FBX values(tuples of(axis, sign), Up, Front, Coord).
	{{Axis::X, Axis::_Y}, {{0,  1}, {1,  1}, {2,  1} }},
	{{Axis::X, Axis::Y }, {{0,  1}, {1, -1}, {2, -1} }},
	{{Axis::X, Axis::_Z}, {{0,  1}, {2,  1}, {1, -1} }},
	{{Axis::X, Axis::Z }, {{0,  1}, {2, -1}, {1,  1} }},
	{{Axis::_X,Axis::_Y}, {{0, -1}, {1,  1}, {2, -1} }},
	{{Axis::_X,Axis::Y }, {{0, -1}, {1, -1}, {2,  1} }},
	{{Axis::_X,Axis::_Z}, {{0, -1}, {2,  1}, {1,  1} }},
	{{Axis::_X,Axis::Z }, {{0, -1}, {2, -1}, {1, -1} }},
	{{Axis::Y, Axis::_X}, {{1,  1}, {0,  1}, {2, -1} }},
	{{Axis::Y, Axis::X }, {{1,  1}, {0, -1}, {2,  1} }},
	{{Axis::Y, Axis::_Z}, {{1,  1}, {2,  1}, {0,  1} }},
	{{Axis::Y, Axis::Z }, {{1,  1}, {2, -1}, {0, -1} }},
	{{Axis::_Y,Axis::_X}, {{1, -1}, {0,  1}, {2,  1} }},
	{{Axis::_Y,Axis::X }, {{1, -1}, {0, -1}, {2, -1} }},
	{{Axis::_Y,Axis::_Z}, {{1, -1}, {2,  1}, {0, -1} }},
	{{Axis::_Y,Axis::Z }, {{1, -1}, {2, -1}, {0,  1} }},
	{{Axis::Z, Axis::_X}, {{2,  1}, {0,  1}, {1,  1} }},
	{{Axis::Z, Axis::X }, {{2,  1}, {0, -1}, {1, -1} }},
	{{Axis::Z, Axis::_Y}, {{2,  1}, {1,  1}, {0, -1} }},
	{{Axis::Z, Axis::Y }, {{2,  1}, {1, -1}, {0,  1} }},	/* Blender system! */
	{{Axis::_Z,Axis::_X}, {{2, -1}, {0,  1}, {1, -1} }},
	{{Axis::_Z,Axis::X }, {{2, -1}, {0, -1}, {1,  1} }},
	{{Axis::_Z,Axis::_Y}, {{2, -1}, {1,  1}, {0,  1} }},
	{{Axis::_Z,Axis::Y }, {{2, -1}, {1, -1}, {0, -1} }},
};

const std::map<std::tuple<std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>>, std::pair<Axis,Axis>>
RIGHT_HAND_AXES_RR = {
//	Up, Forward->FBX values(tuples of(axis, sign), Up, Front, Coord).
	{{{0,  1}, {1,  1}, {2,  1} }, {Axis::X, Axis::_Y}},
	{{{0,  1}, {1, -1}, {2, -1} }, {Axis::X, Axis::Y}},
	{{{0,  1}, {2,  1}, {1, -1} }, {Axis::X, Axis::_Z}},
	{{{0,  1}, {2, -1}, {1,  1} }, {Axis::X, Axis::Z}},
	{{{0, -1}, {1,  1}, {2, -1} }, {Axis::_X,Axis::_Y}},
	{{{0, -1}, {1, -1}, {2,  1} }, {Axis::_X,Axis::Y}},
	{{{0, -1}, {2,  1}, {1,  1} }, {Axis::_X,Axis::_Z}},
	{{{0, -1}, {2, -1}, {1, -1} }, {Axis::_X,Axis::Z}},
	{{{1,  1}, {0,  1}, {2, -1} }, {Axis::Y, Axis::_X}},
	{{{1,  1}, {0, -1}, {2,  1} }, {Axis::Y, Axis::X}},
	{{{1,  1}, {2,  1}, {0,  1} }, {Axis::Y, Axis::_Z}},
	{{{1,  1}, {2, -1}, {0, -1} }, {Axis::Y, Axis::Z}},
	{{{1, -1}, {0,  1}, {2,  1} }, {Axis::_Y,Axis::_X}},
	{{{1, -1}, {0, -1}, {2, -1} }, {Axis::_Y,Axis::X}},
	{{{1, -1}, {2,  1}, {0, -1} }, {Axis::_Y,Axis::_Z}},
	{{{1, -1}, {2, -1}, {0,  1} }, {Axis::_Y,Axis::Z}},
	{{{2,  1}, {0,  1}, {1,  1} }, {Axis::Z, Axis::_X}},
	{{{2,  1}, {0, -1}, {1, -1} }, {Axis::Z, Axis::X}},
	{{{2,  1}, {1,  1}, {0, -1} }, {Axis::Z, Axis::_Y}},
	{{{2,  1}, {1, -1}, {0,  1} }, {Axis::Z, Axis::Y}},	/* Blender system! */
	{{{2, -1}, {0,  1}, {1, -1} }, {Axis::_Z,Axis::_X}},
	{{{2, -1}, {0, -1}, {1,  1} }, {Axis::_Z,Axis::X}},
	{{{2, -1}, {1,  1}, {0,  1} }, {Axis::_Z,Axis::_Y}},
	{{{2, -1}, {1, -1}, {0, -1} }, {Axis::_Z,Axis::Y}},
};

const std::vector<std::pair<double, int>>
FBX_FRAMERATES = {
	{        -1.0, 14}, /* Custom framerate.*/
	{       120.0,  1},
	{       100.0,  2},
	{        60.0,  3},
	{        50.0,  4},
	{        48.0,  5},
	{        30.0,  6}, /* BW NTSC. */
	{30.0 / 1.001,  9}, /* Color NTSC. */
	{        25.0, 10},
	{        24.0, 11},
	{24.0 / 1.001, 13},
	{        96.0, 15},
	{        72.0, 16},
	{60.0 / 1.001, 17},
};

const std::vector<std::pair<int, double>>
FBX_FRAMERATES_RR = {
	{14,        -1.0}, /* Custom framerate.*/
	{ 1,       120.0},
	{ 2,       100.0},
	{ 3,        60.0},
	{ 4,        50.0},
	{ 5,        48.0},
	{ 6,        30.0}, /* BW NTSC. */
	{ 9,30.0 / 1.001}, /* Color NTSC. */
	{10,        25.0},
	{11,        24.0},
	{13,24.0 / 1.001},
	{15,        96.0},
	{16,        72.0},
	{17,60.0 / 1.001},
};

enum class UnitSettingSystem : int {
	None = 0,
	METRIC,
};

class UnitSetting {
public:
	UnitSettingSystem	System = UnitSettingSystem::None;
	double				ScaleLength = 0;
};

class Render {
public:
	double	Fsp = 0;
	double	FspBase = 0;
};

class Scene {
public:
	UnitSetting UnitSetting;
	Render		Render;
};

class Context {
public:
	Scene Scene;
};

enum class byte : unsigned char {};
class General {
public:
	enum class Type : char {
		Int16    ='Y',
		Bool     ='C',
		Int32    ='I',
		Float    ='F',
		Double   ='D',
		Int64    ='L',
		Bin      ='R',
		Str      ='S',
		AryFloat ='f',
		AryInt32 ='i',
		AryDouble='d',
		AryInt64 ='l',
		AryBool  ='b',
		AryByte  ='c',
	};

public:
	General() {}
	General(const General &obj) :datatype(obj.datatype),Int16(obj.Int16),Bool(obj.Bool),Int32(obj.Int32),Float(obj.Float),Double(obj.Double),Int64(obj.Int64),Bin(obj.Bin),Str(obj.Str),AryFloat(obj.AryFloat),AryInt32(obj.AryInt32),AryDouble(obj.AryDouble),AryInt64(obj.AryInt64),AryBool(obj.AryBool),AryByte(obj.AryByte) { }
	~General() {}
	template<typename T> T getData() const;
	template<typename T> void swapData(T &&t);
	static  General     pickData(std::istream& ios);
			Type        DataType() const { return datatype;}
			std::string toString();

private:
	Type                        datatype = Type::Int16;
	std::int16_t                Int16 = 0;
	bool                        Bool = false;
	std::int32_t                Int32 = 0;
	float                       Float = 0;
	double                      Double = 0;
	std::int64_t                Int64 = 0;
	std::vector<char>           Bin;
	std::string                 Str = "";
	std::vector<float>          AryFloat;
	std::vector<std::int32_t>   AryInt32;
	std::vector<double>         AryDouble;
	std::vector<std::int64_t>   AryInt64;
	std::vector<byte>           AryBool;
	std::vector<signed char>    AryByte;
};

class FbxElem {
public:
	std::int64_t         end_offset = 0;
	std::string          id = "";
	std::vector<General> props;
	std::vector<FbxElem> elems;
	std::string toString(int hierarchy);
};

class FBXHeaderExtension {
public:
	class CreationTimeStamp {
	public:
		int Version;
		int Year;
		int Month;
		int Day;
		int Hour;
		int Minute;
		int Second;
		int Millisecond;
	};
	class OtherFlags {
		std::map<std::string, int> Flags;
	};

public:
	int FBXHeaderVersion;
	int FBXVersion;
	int EncryptionType;
	CreationTimeStamp CreationTimeStamp;
	std::string Creator;
};

class FbxImportSettings {
public:
	//std::string	report;
	std::pair<Axis, Axis>	toAxeiss;
	CG3DMatrix4				globalMatrix;
	double					globalScale;
	bool					bakeSpaceTransform;
	CG3DMatrix4				globalMatrixInv;
	CG3DMatrix4				globalMatrixInvTransposed;
	bool					useCustomNormals;
	bool					useImageSearch;
	bool					useAlphaDecals;
	double					decalOffset;
	bool					useAnim;
	double					animOffset;
	bool					useSubsurf;
	bool					useCustomProps;
	bool					useCustomPropsEnumAsString;
	//bool					nodalMaterialWrapMap;
	//bool					imageCache;
	bool					ignoreLeafBones;
	bool					forceConnectChildren;
	bool					automaticBoneOrientation;
	CG3DMatrix4				boneCorrectionMatrix;
	bool					usePrepostRot;
};

class FbxUtil {
public:
	static	bool				init(Version ver);
	static	FbxElem				readElements(std::istream &istream);
			int					NullRecordLen() { return mNullRecordLen; }
			const char			*NullRecord() { return mNullRecord; }
	static	double				getPropNumber(const FbxElem &elem, const std::string& key);
	static	std::int64_t		getPropInteger(const FbxElem &elem, const std::string& key);
	static	std::int32_t		getPropEnum(FbxElem& elem, const std::string& key);
	static	std::string			getElemNameEnsureClass(const FbxElem &fbxobj, const std::string &classname);
	template<typename X>
	static	std::vector<X>		readArray(std::istream& iostream);
	static	cg3d::Cg3d			cg3dReadGeometry(const FbxElem& fbxtmpl, const FbxElem& elm, FbxImportSettings &settings);
	static	FbxUtil				&GetIns() {
		static FbxUtil instance;
		assert((instance.mIsInitCalled) && "aaaaa FbxUtil needs to be FbxUtil::init() first!!");
		return instance;
	}

private:
	static bool mIsInitCalled;
	int         mNullRecordLen = 13;
	const char  *mNullRecord = nullptr;
	FbxUtil     *mpFbxUtil = nullptr;

private:
	static	char				read1(std::istream &iostream);
			std::int64_t		read4or8(std::istream &istream);
	static	int					readInt(std::istream &istream);
	static	std::string			readcString(std::istream &iostream);
	static	std::string			readiString(std::istream &iostream);
			std::vector<char>	readNullRecord(std::istream &iostream) const;
//	static	General				readProp(std::istream &iostream);
	static	std::tuple<std::string, std::string>                    splitNameClass(const FbxElem &elm);
    static  std::tuple<std::string, std::string, std::string> cg3dReadGeometryLayerInfo(std::vector<FbxElem>::const_iterator &itr);

private:
	FbxUtil(){}
	FbxUtil(const FbxUtil&){}
	FbxUtil& operator=(const FbxUtil&) { return *this; }
};

double Units2FbxFactor(Scene scene);

}   /* namespace fbx */

#endif //CG3DVIEWER_FBX_H
