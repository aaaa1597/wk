//
// Created by jun on 2016/11/20.
//
#ifndef MQOVIEWER_TKSMATRIXVECTOR_H
#define MQOVIEWER_TKSMATRIXVECTOR_H

/**************/
/* TKSVector2 */
/**************/
class TKSVector2
{
public:
    union { float x; float s; float u; };
    union { float y; float t; float v; };

public:
    TKSVector2():x(0),y(0){}
    TKSVector2(float ax, float ay):x(ax),y(ay){}
    TKSVector2(const TKSVector2 &src) : x(src.x), y(src.y) {}
    ~TKSVector2() {};
    /* =演算子 */
    TKSVector2 &operator=(const TKSVector2 &rhs);
    /* 添え字演算子 */
    float operator[](int n);
    /* 単項演算子 */
    TKSVector2& operator+=(const TKSVector2& v);
    TKSVector2& operator-=(const TKSVector2& v);
    TKSVector2& operator*=(float k);
    TKSVector2& operator/=(float k);
    TKSVector2 operator+()const;
    TKSVector2 operator-()const;
    float DotProduct(const TKSVector2& v);
//	TKSVector2& CrossProduct(const TKSVector2& v);
    /* 比較演算子 */
    bool operator==(const TKSVector2& v) const;
    bool operator!=(const TKSVector2& v) const;
    /* べクトルの大きさ */
    float magnitude() const;
    float power() const;
    /* 正規化 */
    void normalize();
};
/* 2項演算子 */
TKSVector2 operator+(const TKSVector2& u, const TKSVector2& v);		/* TKSVector2+TKSVector2 */
TKSVector2 operator-(const TKSVector2& u, const TKSVector2& v);		/* TKSVector2-TKSVector2 */
TKSVector2 operator*(float k, const  TKSVector2& v);						/* float*TKSVector2 */
TKSVector2 operator*(const TKSVector2& v, float k);						/* TKSVector2*float */
TKSVector2 operator/(const TKSVector2& v, float k);						/* TKSVector2/float */
float operator*(const TKSVector2& u, const TKSVector2& v);				/* 内積 TKSVector2*TKSVector2 */
float DotProduct(const TKSVector2& u, const TKSVector2& v);				/* 内積 DotProduct(TKSVector2,TKSVector2) */
//TKSVector2 CrossProduct(const TKSVector2& u, const TKSVector2& v);	/* 外積 CrossProduct(TKSVector2,TKSVector2) */
float angle(const TKSVector2& u, const TKSVector2& v);					/* 2つのベクトルのなす角度 */

/**************/
/* TKSVector3 */
/**************/
class TKSVector3
{
public:
    union { float x; float s; float r; };
    union { float y; float t; float b; };
    union { float z; float u; float g; };

public:
    TKSVector3():x(0),y(0),z(0) {}
    TKSVector3(float ax, float ay, float az):x(ax),y(ay),z(az) {}
    TKSVector3(const TKSVector3 &src):x(src.x),y(src.y),z(src.z) {}
    ~TKSVector3(){};
    /* =演算子 */
    TKSVector3 &operator=(const TKSVector3 &rhs);
    /* 添え字演算子 */
    float &operator[](int n);
    /* 単項演算子 */
    TKSVector3& operator+=(const TKSVector3& v);
    TKSVector3& operator-=(const TKSVector3& v);
    TKSVector3& operator*=(float k);
    TKSVector3& operator/=(float k);
    TKSVector3 operator+()const;
    TKSVector3 operator-()const;
    float DotProduct(const TKSVector3& v);
    TKSVector3 CrossProduct(const TKSVector3& v);
    /* 比較演算子 */
    bool operator==(const TKSVector3& v) const;
    bool operator!=(const TKSVector3& v) const;
    /* べクトルの大きさ */
    float magnitude() const;
    float power() const;
    /* 正規化 */
    void normalize();
};
/* 2項演算子 */
TKSVector3 operator+(const TKSVector3& u, const TKSVector3& v);	/* TKSVector3+TKSVector3 */
TKSVector3 operator-(const TKSVector3& u, const TKSVector3& v);	/* TKSVector3-TKSVector3 */
TKSVector3 operator*(float k, const  TKSVector3& v);				/* float*TKSVector3 */
TKSVector3 operator*(const TKSVector3& v, float k);					/* TKSVector3*float */
TKSVector3 operator*(const TKSVector3& v0, const TKSVector3& v1);		/* TKSVector3*TKSVector3 */
TKSVector3 operator/(const TKSVector3& v, float k);					/* TKSVector3/float */
float DotProduct(const TKSVector3& u, const TKSVector3& v);			/* 内積 DotProduct(TKSVector3,TKSVector3) */
TKSVector3 CrossProduct(const TKSVector3& u, const TKSVector3& v);	/* 外積 CrossProduct(TKSVector3,TKSVector3) */
float angle(const TKSVector3& u, const TKSVector3& v);				/* 2つのベクトルのなす角度 */

/**************/
/* TKSVector4 */
/**************/
class TKSVector4
{
public:
    union { float x; float r; float diffuse; };
    union { float y; float g; float ambient; };
    union { float z; float b; float emission; };
    union { float w; float a; float specular; };

public:
    TKSVector4():x(0),y(0),z(0),w(0) {}
    TKSVector4(float ax, float ay, float az, float aw):x(ax),y(ay),z(az),w(aw) {}
    TKSVector4(const TKSVector4 &src):x(src.x),y(src.y),z(src.z),w(src.w) {}
    ~TKSVector4(){};
    /* =演算子 */
    TKSVector4 &operator=(const TKSVector4 &rhs);
    /* 添え字演算子 */
    float &operator[](int n);
    /* 単項演算子 */
    TKSVector4& operator+=(const TKSVector4& v);
    TKSVector4& operator-=(const TKSVector4& v);
    TKSVector4& operator*=(float k);
    TKSVector4& operator/=(float k);
    TKSVector4 operator+()const;
    TKSVector4 operator-()const;
    float DotProduct(const TKSVector4& v);
//	TKSVector4& CrossProduct(const TKSVector4& v);
    /* 比較演算子 */
    bool operator==(const TKSVector4& v) const;
    bool operator!=(const TKSVector4& v) const;
    /* べクトルの大きさ */
    float magnitude() const;
    float power() const;
    /* 正規化 */
    void normalize();
};
/* 2項演算子 */
TKSVector4 operator+(const TKSVector4& u, const TKSVector4& v);		/* TKSVector4+TKSVector4 */
TKSVector4 operator-(const TKSVector4& u, const TKSVector4& v);		/* TKSVector4-TKSVector4 */
TKSVector4 operator*(float k, const  TKSVector4& v);						/* float*TKSVector4 */
TKSVector4 operator*(const TKSVector4& v, float k);						/* TKSVector4*float */
TKSVector4 operator/(const TKSVector4& v, float k);						/* TKSVector4/float */
float operator*(const TKSVector4& u, const TKSVector4& v);				/* 内積 TKSVector4*TKSVector4 */
float DotProduct(const TKSVector4& u, const TKSVector4& v);				/* 内積 DotProduct(TKSVector4,TKSVector4) */
//TKSVector3 CrossProduct(const TKSVector3& u, const TKSVector3& v);	/* 外積 CrossProduct(TKSVector4) */
float angle(const TKSVector4& u, const TKSVector4& v);					/* 2つのベクトルのなす角度 */

typedef TKSVector4 Reflection4;
typedef TKSVector4 Color4;
typedef TKSVector2 UV;

/* 3角形ポリゴン情報構造体 */
typedef struct TRIANGLE {
    int         MaterialID; /* マテリアルNo. */
    int         Index[3];   /* インデックス */
    TKSVector3  Normal;     /* 法線 */
    UV          uv[3];      /* UV情報 */
} Triangle;
/* 4角形ポリゴン情報構造体 */
typedef struct QUAD {
    int         MaterialID; /* マテリアルNo. */
    int         Index[4];   /* インデックス */
    TKSVector3  Normal;     /* 法線 */
    UV          uv[4];      /* UV情報 */
} Quad;

/**************/
/* TKSMatrix4 */
/**************/
class TKSMatrix4
{
public:
    static const float IDENTITY[];
    static const std::array<float, 16> IDENTITY2;
    float mM[16] = {0};

public:
    TKSMatrix4():mM(){}
    TKSMatrix4(const TKSMatrix4 &src) { memcpy(mM, src.mM, sizeof(mM)); }
    TKSMatrix4(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14, float a15);
    ~TKSMatrix4(){};
    void setIdentity();
    /* =演算子 */
    TKSMatrix4 &operator=(const TKSMatrix4 &rhs);
    /* 単項演算子 */
    TKSMatrix4& operator+=(const TKSMatrix4& v);
    TKSMatrix4& operator-=(const TKSMatrix4& v);
    TKSMatrix4& operator*=(float k);
    TKSMatrix4& operator/=(float k);
    TKSMatrix4& operator*=(const TKSMatrix4& v);
//	TKSMatrix4& operator/=(const TKSMatrix4& v);
    float det();		/* 行列式 */
    TKSMatrix4 inverse();	/* 逆行列 */
    TKSMatrix4 operator+() const;
    TKSMatrix4 operator-() const;
    /* 添え字演算子 */
    float& operator[](int i);
    /* 比較演算子 */
    bool operator==(const TKSMatrix4& v) const;
    bool operator!=(const TKSMatrix4& v) const;
    /* ベクトル設定/取得 */
    void setTranslation(const TKSVector3 &sv);
    TKSVector3 getTranslation() const;
//	void setRotation(const TKSVector3 &m);
//	TKSVector3 getRotation() const;
    void setScale(float s);
    void setScale(float sx, float sy, float sz);
    void setScale(const TKSVector3 &s);
    TKSVector3 getScale() const;
};
/* 2項演算子 */
TKSMatrix4 operator+(const TKSMatrix4& u, const TKSMatrix4& v);/* TKSVector4+TKSVector4 */
TKSMatrix4 operator-(const TKSMatrix4& u, const TKSMatrix4& v);/* TKSVector4-TKSVector4 */
TKSMatrix4 operator*(float k, const  TKSMatrix4& v);				/* float*TKSVector4 */
TKSMatrix4 operator*(const TKSMatrix4& v, float k);				/* TKSVector4*float */
TKSMatrix4 operator/(const TKSMatrix4& v, float k);				/* TKSVector4/float */
TKSMatrix4 operator*(const TKSMatrix4& u, const TKSMatrix4& v);/* TKSVector4*TKSVector4 */
//TKSMatrix4 operator/(const TKSMatrix4& u, const TKSMatrix4& v);		/* TKSVector4/TKSVector4 */

class MatrixVector {
public:
    static void LoadIdentity2(std::array<float, 16> &M);
    static void LoadMatrix2(std::array<float, 16> &retmat, const std::array<float, 16> &a);
    static void MultMatrixf2(std::array<float, 16> &retmat, const std::array<float, 16> m);
    static void MultMatrixf2(std::array<float, 16> &retmat, const std::array<float, 16> a, const std::array<float, 16> m);
    static void normalize2(std::array<float, 3> &v);
    static void cross2(const std::array<float, 3> &v1, const std::array<float, 3> &v2, std::array<float, 3> &result);
    static std::array<float, 16> GetPerspectivef2(float fovy, float aspect, float zNear, float zFar);
    static std::array<float, 16> GetLookAtf2(float eyex, float eyey, float eyez, float tarx, float tary, float tarz, float upx, float upy, float upz);
    static void Translatef2(std::array<float, 16> &retmat, float x, float y, float z);
    static void Rotatef2(std::array<float, 16> &retmat, float angle, float x, float y, float z);
    static std::array<float, 16> GetRotatef2(float angle, float x, float y, float z);
    static void Scalef2(std::array<float, 16> &retmat, float x, float y, float z);
    static bool invertf2(std::array<float, 16> &retmat, const std::array<float, 16> &matrix);
    static void transposef2(std::array<float, 16> &retmat, const std::array<float, 16> &matrix);
    static TKSMatrix4 createRotation(float xDeg, float yDeg, float zDeg);
    static TKSMatrix4 createTranslation(float x, float y, float z, float w);
    static TKSMatrix4 createScale(float sx, float sy, float sz);
    static TKSMatrix4 createLookAt(const TKSVector3 &eyePos, const TKSVector3 &centerPos, const TKSVector3 &upDir);
    static TKSMatrix4 createFrustum(float left, float right, float bottom, float top, float zNear, float zFar);
    static TKSMatrix4 createOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
};
#endif //MQOVIEWER_TKSMATRIXVECTOR_H
