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
#include <any>
#include "CG3D.h"
#include "MatVec.h"

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

	const std::map<std::pair<m::Axis, m::Axis>, std::tuple<std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>>>
	RIGHT_HAND_AXES = {
	//	Up, Forward->FBX values(tuples of(axis, sign), Up, Front, Coord).
		{{m::Axis::X,  m::Axis::_Y}, {{0, 1},  {1, 1},  {2, 1} }},
		{{m::Axis::X,  m::Axis::Y }, {{0, 1},  {1, -1}, {2, -1} }},
		{{m::Axis::X,  m::Axis::_Z}, {{0, 1},  {2, 1},  {1, -1} }},
		{{m::Axis::X,  m::Axis::Z }, {{0, 1},  {2, -1}, {1, 1} }},
		{{m::Axis::_X, m::Axis::_Y}, {{0, -1}, {1, 1},  {2, -1} }},
		{{m::Axis::_X, m::Axis::Y }, {{0, -1}, {1, -1}, {2, 1} }},
		{{m::Axis::_X, m::Axis::_Z}, {{0, -1}, {2, 1},  {1, 1} }},
		{{m::Axis::_X, m::Axis::Z }, {{0, -1}, {2, -1}, {1, -1} }},
		{{m::Axis::Y,  m::Axis::_X}, {{1, 1},  {0, 1},  {2, -1} }},
		{{m::Axis::Y,  m::Axis::X }, {{1, 1},  {0, -1}, {2, 1} }},
		{{m::Axis::Y,  m::Axis::_Z}, {{1, 1},  {2, 1},  {0, 1} }},
		{{m::Axis::Y,  m::Axis::Z }, {{1, 1},  {2, -1}, {0, -1} }},
		{{m::Axis::_Y, m::Axis::_X}, {{1, -1}, {0, 1},  {2, 1} }},
		{{m::Axis::_Y, m::Axis::X }, {{1, -1}, {0, -1}, {2, -1} }},
		{{m::Axis::_Y, m::Axis::_Z}, {{1, -1}, {2, 1},  {0, -1} }},
		{{m::Axis::_Y, m::Axis::Z }, {{1, -1}, {2, -1}, {0, 1} }},
		{{m::Axis::Z,  m::Axis::_X}, {{2, 1},  {0, 1},  {1, 1} }},
		{{m::Axis::Z,  m::Axis::X }, {{2, 1},  {0, -1}, {1, -1} }},
		{{m::Axis::Z,  m::Axis::_Y}, {{2, 1},  {1, 1},  {0, -1} }},
		{{m::Axis::Z,  m::Axis::Y }, {{2, 1},  {1, -1}, {0, 1} }},	/* Blender system! */
		{{m::Axis::_Z, m::Axis::_X}, {{2, -1}, {0, 1},  {1, -1} }},
		{{m::Axis::_Z, m::Axis::X }, {{2, -1}, {0, -1}, {1, 1} }},
		{{m::Axis::_Z, m::Axis::_Y}, {{2, -1}, {1, 1},  {0, 1} }},
		{{m::Axis::_Z, m::Axis::Y }, {{2, -1}, {1, -1}, {0, -1} }},
	};

	const std::map<std::tuple<std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>, std::pair<std::int64_t, std::int64_t>>, std::pair<m::Axis, m::Axis>>
	RIGHT_HAND_AXES_RR = {
	//	Up, Forward->FBX values(tuples of(axis, sign), Up, Front, Coord).
		{{{0,  1}, {1,  1}, {2,  1} }, {m::Axis::X,  m::Axis::_Y}},
		{{{0,  1}, {1, -1}, {2, -1} }, {m::Axis::X,  m::Axis::Y}},
		{{{0,  1}, {2,  1}, {1, -1} }, {m::Axis::X,  m::Axis::_Z}},
		{{{0,  1}, {2, -1}, {1,  1} }, {m::Axis::X,  m::Axis::Z}},
		{{{0, -1}, {1,  1}, {2, -1} }, {m::Axis::_X, m::Axis::_Y}},
		{{{0, -1}, {1, -1}, {2,  1} }, {m::Axis::_X, m::Axis::Y}},
		{{{0, -1}, {2,  1}, {1,  1} }, {m::Axis::_X, m::Axis::_Z}},
		{{{0, -1}, {2, -1}, {1, -1} }, {m::Axis::_X, m::Axis::Z}},
		{{{1,  1}, {0,  1}, {2, -1} }, {m::Axis::Y,  m::Axis::_X}},
		{{{1,  1}, {0, -1}, {2,  1} }, {m::Axis::Y,  m::Axis::X}},
		{{{1,  1}, {2,  1}, {0,  1} }, {m::Axis::Y,  m::Axis::_Z}},
		{{{1,  1}, {2, -1}, {0, -1} }, {m::Axis::Y,  m::Axis::Z}},
		{{{1, -1}, {0,  1}, {2,  1} }, {m::Axis::_Y, m::Axis::_X}},
		{{{1, -1}, {0, -1}, {2, -1} }, {m::Axis::_Y, m::Axis::X}},
		{{{1, -1}, {2,  1}, {0, -1} }, {m::Axis::_Y, m::Axis::_Z}},
		{{{1, -1}, {2, -1}, {0,  1} }, {m::Axis::_Y, m::Axis::Z}},
		{{{2,  1}, {0,  1}, {1,  1} }, {m::Axis::Z,  m::Axis::_X}},
		{{{2,  1}, {0, -1}, {1, -1} }, {m::Axis::Z,  m::Axis::X}},
		{{{2,  1}, {1,  1}, {0, -1} }, {m::Axis::Z,  m::Axis::_Y}},
		{{{2,  1}, {1, -1}, {0,  1} }, {m::Axis::Z,  m::Axis::Y}},	/* Blender system! */
		{{{2, -1}, {0,  1}, {1, -1} }, {m::Axis::_Z, m::Axis::_X}},
		{{{2, -1}, {0, -1}, {1,  1} }, {m::Axis::_Z, m::Axis::X}},
		{{{2, -1}, {1,  1}, {0,  1} }, {m::Axis::_Z, m::Axis::_Y}},
		{{{2, -1}, {1, -1}, {0, -1} }, {m::Axis::_Z, m::Axis::Y}},
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
			String   ='S',
			AryFloat ='f',
			AryInt32 ='i',
			AryDouble='d',
			AryInt64 ='l',
			AryBool  ='b',
			AryByte  ='c',
			Empty    =0x00,
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
		Type                        datatype = Type::Empty;
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

	class PrincipledBSDFWrapper {
	public:
		float						Alpha = 1;
//		ShaderImageTextureWrapper	alpha_texture;
		m::Vector3f					BaseColor;
//		ShaderImageTextureWrapper	base_color_texture;
		m::Vector3f					EmissionColor;
//		ShaderImageTextureWrapper	emission_color_texture;
		float 						EmissionStrength = 1.0f;
//		ShaderImageTextureWrapper	emission_strength_texture;
		float 						Ior = 1.45f;
//		ShaderImageTextureWrapper	ior_texture;
		bool						isReadonly = false;
//		Material					material;
		float 						Metallic = 0;
//		ShaderImageTextureWrapper	metallic_texture;
//		ShaderNodeNormalMap			node_normalmap;
//		ShaderNodeOutputMaterial	node_out;
//		ShaderNodeBsdfPrincipled	node_principled_bsdf;
//		ShaderNodeTexCoord			node_texcoords;
		float						NormalmapStrength;
//		ShaderImageTextureWrapper	normalmap_texture
		float 						Roughness = 0.5f;
//		ShaderImageTextureWrapper	roughness_texture;
		float 						Specular = 0.5f;
//		ShaderImageTextureWrapper	specular_texture;
		float						SpecularTint=0;
		float						Transmission=0;
//		ShaderImageTextureWrapper	TransmissionTexture;
		bool						UseNodes = true;

//	private:
		int							_ColSize = 300;
		std::vector<m::Vector2i>	_GridLocations = {{-300, -900}, {0, 300}, {-1500, 300}, {-300, 600}, {-300, -300}, {-600, 300}, {-600, -1500}, {-600, -600}, {-300, 300}, {-300, -600}, {-600, 0}, {-300, 0}, {-600, -300}, {300, 300}};
//		ShaderNodeNormalMap			_node_normalmap;
//		ShaderNodeTexCoord			_node_texcoords;
		int							_RowSize = 	300;
//		???							_Textures;
	};

	class FbxImportSettings {
	public:
		//std::string	report;
		std::pair<m::Axis, m::Axis>	toAxeiss;
		m::Matrix4f				globalMatrix;
		double					globalScale;
		bool					bakeSpaceTransform;
		m::Matrix4f				globalMatrixInv;
		m::Matrix4f				globalMatrixInvTransposed;
		bool					useCustomNormals;
		bool					useImageSearch;
		bool					useAlphaDecals;
		double					decalOffset;
		bool					useAnim;
		double					animOffset;
		bool					useSubsurf;
		bool					useCustomProps;
		bool					useCustomPropsEnumAsString;
		std::map<cg::Material, PrincipledBSDFWrapper>	nodalMaterialWrapMap;
		//bool					imageCache;
		bool					ignoreLeafBones;
		bool					forceConnectChildren;
		bool					automaticBoneOrientation;
		m::Matrix4f				boneCorrectionMatrix;
		bool					usePrepostRot;
	};

	class FBXTransformData {
	public:
		m::Vector3f loc;
		m::Vector3f rot;
		m::Vector3f sca;
		m::Vector3f geom_loc;
		m::Vector3f geom_rot;
		m::Vector3f geom_sca;
		m::Vector3f rot_ofs;
		m::Vector3f rot_piv;
		m::Vector3f sca_ofs;
		m::Vector3f sca_piv;
		m::Vector3f pre_rot;
		m::Vector3f pst_rot;
		std::string rot_ord;
		m::Matrix4f rot_alt_mat;
	};

	class FbxImportHelperNode {
	public:
		FbxImportHelperNode() { }
		FbxImportHelperNode(bool isRoot):isRoot(isRoot){}
		FbxImportHelperNode(const FbxElem &fbxobj, const std::any &any, const FBXTransformData &transformData, bool isBone);
	public:
		std::string fbxName;
		FbxElem fbxType;
		FbxElem fbxElem;
//		??? bl_obj = None;
		std::any cg3ddata;
//		???	bl_bone = None;					/* # Name of bone if this is a bone (this may be different to fbx_name if there was a name conflict in Blender!) */
		FBXTransformData fbxTransformData;
		bool isRoot = false;
		bool isBone = false;
		bool isArmature = false;
//		??? armature = None;				/* # For bones only, relevant armature node. */
		bool hasBoneChildren = false;		/* # True if the hierarchy below this node contains bones, important to support mixed hierarchies. */
		bool isLeaf = false;				/* # True for leaf-bones added to the end of some bone chains to set the lengths. */
		m::Matrix4f preMatrix;				/* # correction matrix that needs to be applied before the FBX transform */
		m::Matrix4f bindMatrix;				/* # for bones this is the matrix used to bind to the skin */
		m::Matrix4f matrix;
		m::Matrix4f matrixAsParent;
		m::Matrix4f matrixGeom;
		m::Matrix4f postMatrix;				/* # correction matrix that needs to be applied after the FBX transform */
		m::Matrix4f boneChildMatrix;		/* # Objects attached to a bone end not the beginning, this matrix corrects for that */
		m::Matrix4f animCompensationMatrix;	/* # a mesh moved in the hierarchy may have a different local matrix. This compensates animations for this.*/
		bool isGlobalAnimation = false;
//		??? meshes = None                      # List of meshes influenced by this bone.
//		??? clusters = []                      # Deformer Cluster nodes
//		??? armature_setup = {}                # mesh and armature matrix when the mesh was bound
//		??? _parent = None
//		??? children = []
	};

	class  RotOrderInfo {
	public:
		short axis[3];
		short parity;	/* parity of axis permutation (even=0, odd=1) - 'n' in original code */
	};

	class FbxUtil {
	public:
		static	bool			init(Version ver);
		static	FbxElem			readElements(std::istream &istream);
				int				NullRecordLen() { return mNullRecordLen; }
				const char		*NullRecord() { return mNullRecord; }
		static	double			getPropNumber(const FbxElem &elem, const std::string& key, double defaultval);
		static int64_t			getPropInteger(const FbxElem &elem, const std::string &key, std::int64_t defaultval);
		static int32_t			getPropEnum(FbxElem &elem, const std::string &key, std::int32_t defaultval);
		static	std::string		getElemNameEnsureClass(const FbxElem &fbxobj, const std::string &classname);
		template<typename X>
		static	std::vector<X>	readArray(std::istream &iostream);
		static	cg::Mesh		cg3dReadGeometry(const FbxElem &fbxtmpl, const FbxElem &elm, FbxImportSettings &settings);
		static	cg::Material	cg3dReadMaterial(const FbxElem &fbxtmpl, const FbxElem &elm, FbxImportSettings &settings);
		static	cg::Image		cg3dReadTextureImage(std::map<std::string, std::vector<char>> &AssetsData, const FbxElem &fbxobj, const std::string &modelbasepath, FbxImportSettings &settings);
		static	FBXTransformData cg3dReadObjectTransformPreprocess(const std::vector<FbxElem> &props, const FbxElem &fbxobj, const m::Matrix4f &matrix, bool aUsePrepostRot);
		static	std::tuple<m::Matrix4f, m::Matrix4f, m::Matrix4f>	readObjectTransformDo(const FBXTransformData& TransformData);
		static	FbxUtil			&GetIns() {
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
		static	bool				getPropsBool(const std::vector<FbxElem> &props, const std::string &key, bool defaultval);
		static	double				getPropNumber(const std::vector<FbxElem> &elms, const std::string &key, double defaultval);
		static	m::Vector3f			getPropColorRgb(const std::vector<FbxElem> &ElemProps, const std::string &key, const m::Vector3f &defaultval);
		static	m::Vector3f			getPropsVector3d(const std::vector<FbxElem> &props, const std::string &id, const m::Vector3f &defaultval);
		static	int32_t				getPropEnum(const std::vector<FbxElem> &Elems, const std::string &key, std::int32_t defaultval);
		static  void				cg3dReadCustomProperties(const FbxElem &elm, cg::Material &mat, const FbxImportSettings &settings);
		static  RotOrderInfo		getRotationOrderInfo(const std::string &order);
		static	m::Matrix4f			createRotation(const m::Vector3f &rot, const std::string &order);
		static	std::tuple<std::string, std::string>				splitNameClass(const FbxElem &elm);
		static  std::tuple<std::string, std::string, std::string>	cg3dReadGeometryLayerInfo(std::vector<FbxElem>::const_iterator &itr);

	private:
		FbxUtil(){}
		FbxUtil(const FbxUtil&){}
		FbxUtil& operator=(const FbxUtil&) { return *this; }
	};

	double Units2FbxFactor(Scene scene);

}   /* namespace fbx */

#endif //CG3DVIEWER_FBX_H
