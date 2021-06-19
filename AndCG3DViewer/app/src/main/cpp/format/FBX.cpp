//
// Created by jun on 2021/06/15.
//

#ifdef __ANDROID__
#include <android/log.h>
#include <zlib.h>
#else   /* __ANDROID__ */
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/ComDef.h"
#include "../../../../../../WinCG3DVewer/WinCG3DVewer/zlibsrc/zlib.h"
#endif  /* __ANDROID__ */
#include <functional>
#include <cassert>
#include <sstream>
#include <algorithm>
#include "FBX.h"

namespace fbx {

template<typename T> T				General::getData() const { T ret; return ret; }
template<> std::int16_t				General::getData() const { assert(datatype==Type::Int16);		return Int16	; }
template<> bool						General::getData() const { assert(datatype==Type::Bool);		return Bool		; }
template<> std::int32_t				General::getData() const { assert(datatype==Type::Int32);		return Int32	; }
template<> float					General::getData() const { assert(datatype==Type::Float);		return Float	; }
template<> double					General::getData() const { assert(datatype==Type::Double);		return Double	; }
template<> std::int64_t				General::getData() const { assert(datatype==Type::Int64);		return Int64	; }
template<> std::vector<char>		General::getData() const { assert(datatype==Type::Bin);			return Bin		; }
template<> std::string				General::getData() const { assert(datatype==Type::Str);			return Str		; }
template<> std::vector<float>		General::getData() const { assert(datatype==Type::AryFloat);	return AryFloat	; }
template<> std::vector<std::int32_t>General::getData() const { assert(datatype==Type::AryInt32);	return AryInt32	; }
template<> std::vector<double>		General::getData() const { assert(datatype==Type::AryDouble);	return AryDouble; }
template<> std::vector<std::int64_t>General::getData() const { assert(datatype==Type::AryInt64);	return AryInt64	; }
template<> std::vector<byte>		General::getData() const { assert(datatype==Type::AryBool);		return AryBool	; }
template<> std::vector<signed char>	General::getData() const { assert(datatype==Type::AryByte);		return AryByte	; }

//template<typename T> void General::setData(const T& setval) {}
//template<> void General::setData(const std::int16_t				& setval) { datatype == Type::Int16;		Int16 = setval; }
//template<> void General::setData(const bool						& setval) { datatype == Type::Bool;			Bool = setval; }
//template<> void General::setData(const std::int32_t				& setval) { datatype == Type::Int32;		Int32 = setval; }
//template<> void General::setData(const float					& setval) { datatype == Type::Float;		Float = setval; }
//template<> void General::setData(const double					& setval) { datatype == Type::Double;		Double= setval; }
//template<> void General::setData(const std::int64_t				& setval) { datatype == Type::Int64;		Int64 = setval; }
//template<> void General::setData(const std::vector<char>		& setval) { datatype == Type::Bin;			Bin = setval; }
//template<> void General::setData(const std::string				& setval) { datatype == Type::Str;			Str = setval; }
//template<> void General::setData(const std::vector<float>		& setval) { datatype == Type::AryFloat;		AryFloat = setval; }
//template<> void General::setData(const std::vector<std::int32_t>& setval) { datatype == Type::AryInt32;		AryInt32 = setval; }
//template<> void General::setData(const std::vector<double>		& setval) { datatype == Type::AryDouble;	AryDouble = setval; }
//template<> void General::setData(const std::vector<std::int64_t>& setval) { datatype == Type::AryInt64;		AryInt64 = setval; }
//template<> void General::setData(const std::vector<byte>		& setval) { datatype == Type::AryBool;		AryBool = setval; }
//template<> void General::setData(const std::vector<signed char>	& setval) { datatype == Type::AryByte;		AryByte = setval; }

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
		ret.Str = std::string(tmpstr.begin(), tmpstr.end());
	}
			break;
	case 'f':	ret.AryFloat = FbxUtil::readArray<float>(ios);			break;
	case 'i':	ret.AryInt32 = FbxUtil::readArray<std::int32_t>(ios);	break;
	case 'd':	ret.AryDouble= FbxUtil::readArray<double>(ios);			break;
	case 'l':	ret.AryInt64 = FbxUtil::readArray<std::int64_t>(ios);	break;
	case 'b':	ret.AryBool  = FbxUtil::readArray<byte>(ios);			break;
	case 'c':	ret.AryByte  = FbxUtil::readArray<signed char>(ios);	break;
	default:  throw std::runtime_error("no impliment!! nonType");		break;
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
        if (std::memcmp(util.NullRecord(), &(nullrecord[0]), util.NullRecordLen()) != 0)
            throw std::runtime_error("UnSpported Format NullRecord!! 値は0しかサポートしません");
	}

	/* 終了位置チェック */
	if(ibs.tellg() != end_offset)
		throw std::runtime_error("UnSpported Format ibs.tellg() != end_offset!! end_offset=" + std::to_string(end_offset));

    return retFbxGeneral;
}

double FbxUtil::getPropDouble(const FbxElem &elem, const std::string &key) {
	double ret = 0;
	auto finded = std::find_if(elem.elems.begin(), elem.elems.end(), [&key](const FbxElem& subelm) {
					if (subelm.elems.size() == 0) return false;
					if (subelm.props.at(0).getData<std::string>() == key)
						return true;
					return false;
				});

	//for(const FbxElem &item : elem.elems) {
	//	const General &gen = item.props.at(0);

	//	auto findit = std::find_if(item.props.begin(), item.props.end(), [&key, &ret](const General &cgen) {
	//		General* pgen = const_cast<General*>(&cgen);
	//		std::string ssss = pgen->getData2<std::string>();
	//		return pgen->getData2<std::string>() == key;
	//	});
	//	//if (findit != item.elems.end()) {
	//	//	const General reGeneral = (findit->props.at(0));
	//	//	//ret = reGeneral.getData<double>();
	//	//	break;
	//	//}
	//};

			
	//if(elem.props[0])

	return 0;
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
	if (retbuf.size() != mNullRecordLen)
		throw std::runtime_error("UnSpported Format NullRecord!! NullRecordのサイズは13or25しかサポートしていません。NullRecordサイズ="+std::to_string(retbuf.size()));

	return retbuf;
}

//General FbxUtil::readProp(std::istream &iostream) {
//	General ret;
//	char typebuf;
//	iostream.read(&typebuf, sizeof(char));
//	ret.datatype = (General::Type)typebuf;
//
//	switch (typebuf) {
//		case 'Y': iostream.read(reinterpret_cast<char*>(&ret.Int16),     sizeof(ret.Int16));     break;
//		case 'C': iostream.read(reinterpret_cast<char*>(&ret.Bool),      sizeof(ret.Bool));      break;
//		case 'I': iostream.read(reinterpret_cast<char*>(&ret.Int32),     sizeof(ret.Int32));     break;
//		case 'F': iostream.read(reinterpret_cast<char*>(&ret.Float),     sizeof(ret.Float));     break;
//		case 'D': iostream.read(reinterpret_cast<char*>(&ret.Double),    sizeof(ret.Double));    break;
//		case 'L': iostream.read(reinterpret_cast<char*>(&ret.Int64),     sizeof(ret.Int64));     break;
//		case 'R': {
//			int size;
//			iostream.read(reinterpret_cast<char*>(&size), sizeof(size));
//			ret.Bin.resize(size);
//			iostream.read(reinterpret_cast<char*>(ret.Bin.data()), size);
//			}
//			break;
//		case 'S': {
//				int size;
//				iostream.read(reinterpret_cast<char*>(&size), sizeof(size));
//				std::vector<char> tmpstr; tmpstr.resize(size);
//				iostream.read(reinterpret_cast<char*>(tmpstr.data()), size);
//				ret.Str = std::string(tmpstr.begin(), tmpstr.end());
//			}
//			break;
//		case 'f':	ret.AryFloat = FbxUtil::readArray<float>		(iostream);	break;
//		case 'i':	ret.AryInt32 = FbxUtil::readArray<std::int32_t>	(iostream);	break;
//		case 'd':	ret.AryDouble= FbxUtil::readArray<double>		(iostream);	break;
//		case 'l':	ret.AryInt64 = FbxUtil::readArray<std::int64_t>	(iostream);	break;
//		case 'b':	ret.AryBool  = FbxUtil::readArray<byte>			(iostream);	break;
//		case 'c':	ret.AryByte  = FbxUtil::readArray<signed char>	(iostream);	break;
//		default:  throw std::runtime_error("no impliment!! nonType");			break;
//	}
//
//	return ret;
//}

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
		if (ret1 != 0)
			throw std::runtime_error("fail uncompress()!! 解凍失敗!! ret1=" + std::to_string(ret1));

		/* 解答結果チェック2 */
		if((dstlen*sizeof(T)) != actualdstsize)
			throw std::runtime_error("fail uncompress()!! 解凍後のサイズが合わない!! "
									 "dstlen*sizeof(T)(" + std::to_string(dstlen*sizeof(T)) + ""
									 ")!=actualdstsize(" + std::to_string(actualdstsize) + ")");
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
		ret += item.toString(hierarchy+1) + ",";
	}
	ret += ")\n";

	ret += intent + "SubElems数(" + std::to_string(elems.size()) + ")\n";
	for (FbxElem item : elems) {
        ret += item.toString(hierarchy+1);
    }

    return ret;
}

std::string General::toString(int hierarchy) {
    std::string intent(hierarchy, '\t');
    std::string ret = intent;

    std::ostringstream oss;
    switch (datatype) {
        case (Type)'Y': ret = std::to_string(Int16) ; break;
        case (Type)'C': ret = std::to_string(Bool)  ; break;
        case (Type)'I': ret = std::to_string(Int32) ; break;
        case (Type)'F': ret = std::to_string(Float) ; break;
        case (Type)'D': ret = std::to_string(Double); break;
        case (Type)'L': ret = std::to_string(Int64) ; break;
        case (Type)'R':
            for(size_t lp=0; lp<((Bin.size()>10)?10:Bin.size());lp++)
                oss << Bin[lp] << ',';
            ret = oss.str();
            ret += (Bin.size()>10)?"...":"";
            break;
        case (Type)'S': ret = Str;                    break;
        case (Type)'f':
            for(size_t lp=0; lp<((AryFloat.size()>10)?10:AryFloat.size());lp++)
                oss << AryFloat[lp] << ',';
            ret = oss.str();
            ret += (AryFloat.size()>10)?"...":"";
            break;
        case (Type)'i':
            for(size_t lp=0; lp<((AryInt32.size()>10)?10:AryInt32.size()); lp++)
                oss << AryInt32[lp] << ',';
            ret = oss.str();
            ret += (AryInt32.size()>10)?"...":"";
            break;
        case (Type)'d':
            for(size_t lp=0; lp<((AryDouble.size()>10)?10:AryDouble.size()); lp++)
                oss << AryDouble[lp] << ',';
            ret = oss.str();
            ret += (AryDouble.size()>10)?"...":"";
            break;
        case (Type)'l':
            for(size_t lp=0; lp<((AryInt64.size()>10)?10:AryInt64.size()); lp++)
                oss << AryInt64[lp] << ',';
            ret = oss.str();
            ret += (AryInt64.size()>10)?"...":"";
            break;
        case (Type)'b':
            for(size_t lp=0; lp<((AryBool.size()>10)?10:AryBool.size()); lp++)
                oss << (unsigned char)AryBool[lp] << ',';
            ret = oss.str();
            ret += (AryBool.size()>10)?"...":"";
            break;
        case (Type)'c':
            for(size_t lp=0; lp<((AryByte.size()>10)?10:AryByte.size()); lp++)
                oss << AryByte[lp] << ',';
            ret = oss.str();
            ret += (AryByte.size()>10)?"...":"";
            break;
        default:  throw std::runtime_error("no impliment!! unKnownnType");               break;
    }
    return ret;
}

}   /* namespace fbx */
