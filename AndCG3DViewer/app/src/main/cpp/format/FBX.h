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
        static General pickData(std::istream& ios);
        std::string toString(int hierarchy);

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

class FbxUtil {
public:
    static  bool                init(Version ver);
    static  FbxElem             readElements(std::istream &istream);
            int                 NullRecordLen() { return mNullRecordLen; }
            const char          *NullRecord() { return mNullRecord; }
    static  double              getPropDouble(const FbxElem &elem, const std::string& key);
    template <typename X>
    static  std::vector<X>      readArray(std::istream& iostream);
    static  FbxUtil             &GetIns() {
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
    static  char                read1(std::istream &iostream);
            std::int64_t        read4or8(std::istream &istream);
    static  int                 readInt(std::istream &istream);
    static  std::string         readcString(std::istream &iostream);
	static  std::string         readiString(std::istream &iostream);
            std::vector<char>   readNullRecord(std::istream &iostream) const;
//  static  General             readProp(std::istream &iostream);

    FbxUtil(){}
    FbxUtil(const FbxUtil&){}
    FbxUtil& operator=(const FbxUtil&) { return *this; }
};

}   /* namespace fbx */

#endif //CG3DVIEWER_FBX_H
