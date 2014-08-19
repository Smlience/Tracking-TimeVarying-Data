#define _USE_MATH_DEFINES
#include <math.h>
#include "util/func.math.h"
//////////////////////////////////////////////////////////////////////////
// Code from: 
// http://blog.sina.com.cn/s/blog_5309cefc0101cdeg.html
//
// http://www.w3.org/Graphics/Color/sRGB
static const float scRGB2XYZ[] = 
{
	0.412453f,  0.357580f,  0.180423f,  0.0f,
	0.212671f,  0.715160f,  0.072169f,  0.0f,
	0.019334f,  0.119193f,  0.950227f,  0.0f,
	0.0f,       0.0f,       0.0f,       1.0f,
};
static const float scXYZ2RGB[] =
{
	 3.240479f, -1.537150f, -0.498535f, 0.0f,
	-0.969256f,  1.875992f,  0.041556f, 0.0f,
	 0.055648f, -0.204043f,  1.057311f, 0.0f, 
	 0.0f,       0.0f,       0.0f,      1.0f,
};

void RGBf2XYZ(float r, float g, float b, float& X, float& Y, float& Z)
{
	X = r * scRGB2XYZ[0] + g * scRGB2XYZ[1] + b * scRGB2XYZ[2];
	Y = r * scRGB2XYZ[4] + g * scRGB2XYZ[5] + b * scRGB2XYZ[6];
	Z = r * scRGB2XYZ[8] + g * scRGB2XYZ[9] + b * scRGB2XYZ[10];
}

void RGB2XYZ(int R, int G, int B, float& X, float& Y, float& Z)
{
	float r = R / 255.0f;
	float g = G / 255.0f;
	float b = B / 255.0f;
	RGBf2XYZ(r, g, b, X, Y, Z);
}

void XYZ2RGBf(float X, float Y, float Z, float& r, float& g, float& b)
{
	r = X * scXYZ2RGB[0] + Y * scXYZ2RGB[1] + Z * scXYZ2RGB[2];
	g = X * scXYZ2RGB[4] + Y * scXYZ2RGB[5] + Z * scXYZ2RGB[6];
	b = X * scXYZ2RGB[8] + Y * scXYZ2RGB[9] + Z * scXYZ2RGB[10];
}

void XYZ2RGB(float X, float Y, float Z, unsigned char& R, unsigned char& G, unsigned char& B)
{
	float r, g, b;
	XYZ2RGBf(X, Y, Z, r, g, b);
	r *= 255;
	g *= 255;
	b *= 255;
	R = (unsigned char)((r < 0) ? 0 : (r < 255 ? r : 255));
	G = (unsigned char)((g < 0) ? 0 : (g < 255 ? g : 255));
	B = (unsigned char)((b < 0) ? 0 : (b < 255 ? b : 255));
}

void RGB2LUVf(int R, int G, int B, float& L, float& U, float& V, bool scaled)
{
	float X, Y, Z;
	RGB2XYZ(R, G, B, X, Y, Z);
	if (Y > 0.008856)
		L = 116.0f * pow(Y, 1.0f / 3.0f) - 16.0;
	else
		L = 903.3f * Y;

	double sum = X + 15 * Y + 3 * Z;

	if(sum != 0) U = 4 * X / sum, V = 9 * Y / sum;
	else U = 4.0, V = 9.0 / 15.0;

	U = 13 * L * (U - 0.19784977571475);
	V = 13 * L * (V - 0.46834507665248);
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 参考资料:
// http://www.easyrgb.com/index.php?X=MATH&H=02#text2
//
// VC编程实现色彩空间RGB与XYZ相互转换:
// http://www.vcsos.com/article/pageSource/120314/20120314114422.shtml
//
// VC编程实现色彩空间XYZ与LAB相互转换:
// http://www.vcsos.com/article/pageSource/120315/20120315063541.shtml
//
// http://blog.csdn.net/cay22/article/details/5535245
//////////////////////////////////////////////////////////////////////////
// Gamma 2.2
// Gamma校正
/*
float gamma(float k, float r = 2.4, float a = 0.055)
{
	if(r > 0.04045)
		k = pow((k + a) / (1.0f + a), r);
	else
		k = k / 12.92;
	return k;
}
// Gamma校正函数的反函数
float r_gamma(float x){
	if(x > pow((0.04045 + 0.055) / 1.055, 2.4))
		x = 1.055 * pow(x, 1.0f / 2.4f) - 0.055;
	else
		x = 12.92 * x;    
	return x;
}

void RGB2XYZ(int R, int G, int B, float& X, float& Y, float& Z)
{
	float r = R / 255.0;
	float g = G / 255.0;
	float b = B / 255.0;
	r = gamma(r) * 100.0;
	g = gamma(g) * 100.0;
	b = gamma(b) * 100.0;

	static float M[]={
		0.436052025, 0.385081593, 0.143087414,
		0.222491598, 0.716886060, 0.060621486,
		0.013929122, 0.097097002, 0.714185470
	};
	// From wikipedia
	// http://zh.wikipedia.org/wiki/SRGB_%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4
	// static double M[]={
	// 	0.4124, 0.3576, 0.1805,
	// 	0.2126, 0.7152, 0.0722,
	// 	0.0193, 0.1192, 0.9505
	// };
	//Observer. = 2°, Illuminant = D65
	X = r * M[0] + g * M[1] + b * M[2];
	Y = r * M[3] + g * M[4] + b * M[5];
	Z = r * M[6] + g * M[7] + b * M[8];
}

void XYZ2RGB( float X, float Y, float Z, int& R, int& G, int& B )
{
	double XYZ[]={0.0, 0.0, 0.0};

	static float inv_M[]={ 
		// 		0.436052025, 0.385081593, 0.143087414, //求逆矩阵
		// 		0.222491598, 0.716886060, 0.060621486,
		// 		0.013929122, 0.097097002, 0.714185470
		3.134051341, -1.617027711, -0.490652210,
		-0.978762730,  1.916142228,  0.033449629,
		0.071942577, -0.228971180,  1.405218305,
	};

	XYZ[0] = (inv_M[0] * X + inv_M[1] * Y + inv_M[2] * Z) / 100.0;
	XYZ[1] = (inv_M[3] * X + inv_M[4] * Y + inv_M[5] * Z) / 100.0;
	XYZ[2] = (inv_M[6] * X + inv_M[7] * Y + inv_M[8] * Z) / 100.0;

	XYZ[0] = r_gamma(XYZ[0]) > 0 ? r_gamma(XYZ[0]) * 255 : 0;
	XYZ[1] = r_gamma(XYZ[1]) > 0 ? r_gamma(XYZ[1]) * 255 : 0;
	XYZ[2] = r_gamma(XYZ[2]) > 0 ? r_gamma(XYZ[2]) * 255 : 0;

	R = XYZ[0] < 255 ? int(XYZ[0] + 0.5) : 255;
	G = XYZ[1] < 255 ? int(XYZ[1] + 0.5) : 255;
	B = XYZ[2] < 255 ? int(XYZ[2] + 0.5) : 255;
}			   
*/
//////////////////////////////////////////////////////////////////////////
/*
// 类似Gamma函数的校正函数，XYZ转LAB时用
float revise(float x){
	if(x > pow(6.0 / 29.0, 3))
		x = pow(x, 1.0f/3.0f);
	else
		x = (1.0 / 3.0) * (29.0 / 6.0) * x + (16.0 / 116.0);
	return x;
}
float r_revise(float x){
	if(x > 6.0 / 29.0)
		x = pow(x, 3.0f);
	else
		x = (x - 16.0f / 116.0f) * 3 * pow(6.0f / 29.0f, 2.0f);
	return x;
}

//REF_X 、REF_Y 、REF_Z 是参照白点CIE XYZ三色刺激值
const float REF_X=96.4221;        
const float REF_Y=100.000;
const float REF_Z=82.5221;
void XYZ2Lab(float X, float Y, float Z, float& L, float& a, float& b)
{
	float x = X / REF_X;
	float y = Y / REF_Y;
	float z = Z / REF_Z;

	x = revise(x);
	y = revise(y);
	z = revise(z);

	L = (116.0 * y) - 16.0;
	a = 500.0 * (x - y);
	b = 200.0 * (y - z);
}
void Lab2XYZ( float L, float a, float b, float& X, float& Y, float& Z )
{
	float y = (L + 16.0) / 116.0;
	float x = y + a / 500.0;
	float z = y - b / 200.0;

	y=r_revise(y);
	x=r_revise(x);
	z=r_revise(z);

	X = x*REF_X;
	Y = y*REF_Y;
	Z = z*REF_Z;
}
*/

void XYZ2Lab(float X, float Y, float Z, float& L, float& a, float& b)
{
// 	X /= 0.950456;
// 	Y /= 1.000;
// 	Z /= 1.088854;
// 	if(Y > 0.008856) L = 116.0 * pow(Y, 1.0f / 3.0f) - 16.0;
// 	else L = 903.3 * Y;
// 
// 	if(X > 0.008856) X = pow(X, 1.0f / 3.0f);
// 	else X = 7.787 * X + 16.0 / 116.0;
// 
// 	if(Y > 0.008856) Y = pow(Y, 1.0f / 3.0f);
// 	else Y = 7.787 * Y + 16.0 / 116.0;
// 
// 	if(Z > 0.008856) Z = pow(Z, 1.0f / 3.0f);
// 	else Z = 7.787 * Z + 16.0 / 116.0;
// 
// 	a = 500.0 * (X - Y);
// 	b = 200.0 * (Y - Z);
	float xr = X / 0.950456, yr = Y / 1.000, zr = Z / 1.088854;
	if(yr > 0.008856) L = 116.0 * pow(yr, 1.0f / 3.0f) - 16.0;
	else L = 903.3 * yr;

	float fxr, fyr, fzr;

	if(xr > 0.008856) fxr = pow(xr, 1.0f / 3.0f);
	else fxr = 7.787 * xr + 16.0 / 116.0;


	if(yr > 0.008856) fyr = pow(yr, 1.0f / 3.0f);
	else fyr = 7.787 * yr + 16.0 / 116.0;


	if(zr > 0.008856) fzr = pow(zr, 1.0f / 3.0f);
	else fzr = 7.787 * zr + 16.0 / 116.0;


	a = 500.0 * (fxr - fyr);
	b = 200.0 * (fyr - fzr);
}

void Lab2XYZ( float L, float a, float b, float& X, float& Y, float& Z )
{
	float P = (L + 16.0) / 116.0;

	if(L > 7.9996) Y = 1.000 * P * P * P;
	else Y = 1.000 * L / 903.3;

	float yr = Y / 1.000, fy;
	if(yr > 0.008856) fy = pow(yr, 1.0f / 3.0f);
	else fy = 7.787 * yr + 16.0 / 116.0;

	float fx = a / 500.0 + fy;
	float fz = fy - b / 200.0;

	if(fx > 0.2069) X = 0.950456 * fx * fx * fx;
	else X = 0.950456 / 7.787 * (fx - 16.0 / 116.0);

	if(fz > 0.2069) Z = 1.088854 * fz * fz * fz;
	else Z = 1.088854 / 7.787 * (fz - 16.0 / 116.0);
}

//////////////////////////////////////////////////////////////////////////
void RGB2Lab3f(int R, int G, int B, float& L, float& a, float& b)
{
	float x, y, z;
	RGB2XYZ(R, G, B, x, y, z);
	XYZ2Lab(x, y, z, L, a, b);
}

void RGB2Lab3ub( unsigned char R, unsigned char G, unsigned char B, unsigned char& L, unsigned char& a, unsigned char& b )
{
	float x, y, z;
	float lt, at, bt;
	RGB2XYZ(R, G, B, x, y, z);
	XYZ2Lab(x, y, z, lt, at, bt);
	L = lt;
	a = at + 128;
	b = bt + 128;
}

void Lab3f2RGB3ub( float L, float a, float b, unsigned char& R, unsigned char& G, unsigned char& B )
{
	float x, y, z;
	Lab2XYZ(L, a, b, x, y, z);
	XYZ2RGB(x, y, z, R, G, B);
}

unsigned int rgba2uint( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	unsigned int result = 0;
	// 	result &= a;
	// 	result <<= 8;
	// 	result &= a;
	// 	result <<= 8;
	// 	result &= a;
	// 	result <<= 8;
	result = ((((((result | a) << 8) | r) << 8) | g) << 8) | b;
	return result;
}

void RGB2HSV(float R, float G, float B, float& H, float& S, float&V)
{
	// r,g,b values are from 0 to 1
	// h = [0,360], s = [0,1], v = [0,1]
	// if s == 0, then h = -1 (undefined)
	float min, max, delta,tmp;
	tmp = R>G?G:R;
	min = tmp>B?B:tmp;
	tmp = R>G?R:G;
	max = tmp>B?tmp:B;
	V = max; // v
	delta = max - min;
	if( max != 0 )
		S = delta / max; // s
	else
	{
		// r = g = b = 0 // s = 0, v is undefined
		S = 0;
		H = 0;
		return;
	}
	if (delta == 0){
		H = 0;
		return;
	}
	else if(R == max){
		if (G >= B)
			H = (G - B) / delta; // between yellow & magenta
		else
			H = (G - B) / delta + 6.0;
	}
	else if( G == max )
		H = 2.0 + ( B - R ) / delta; // between cyan & yellow
	else if (B == max)
		H = 4.0 + ( R - G ) / delta; // between magenta & cyan
	H *= 60.0; // degrees
}

void RGB3ub2HSV(unsigned char R, unsigned char G, unsigned char B, float& H, float& S, float&V)
{
	float r = float(R) / 255.0;
	float g = float(G) / 255.0;
	float b = float(B) / 255.0;
	RGB2HSV(r, g, b, H, S, V);
}

void RGB2HSL(unsigned char r, unsigned char g, unsigned char b, float& H, float& S, float&L)
{
	float R,G,B,max_value,min_value,del_R,del_G,del_B,del_Max;
	R = r / 255.0;
	G = g / 255.0;
	B = b / 255.0;

	min_value = min(R, min(G, B));    //Min. value of RGB
	max_value = max(R, max(G, B));    //Max. value of RGB
	del_Max = max_value - min_value;        //Delta RGB value

	L = (max_value + min_value) / 2.0;

	if (del_Max == 0)           //This is a gray, no chroma...
	{
		//H = 2.0/3.0;          //Windows下S值为0时，H值始终为160（2/3*240）
		H = 0;                  //HSL results = 0 ÷ 1
		S = 0;
	}
	else                        //Chromatic data...
	{
		if (L < 0.5) S = del_Max / (max_value + min_value);
		else         S = del_Max / (2 - max_value - min_value);

		del_R = (((max_value - R) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_G = (((max_value - G) / 6.0) + (del_Max / 2.0)) / del_Max;
		del_B = (((max_value - B) / 6.0) + (del_Max / 2.0)) / del_Max;

		if      (R == max_value) H = del_B - del_G;
		else if (G == max_value) H = (1.0 / 3.0) + del_R - del_B;
		else if (B == max_value) H = (2.0 / 3.0) + del_G - del_R;

		if (H < 0)  H += 1;
		if (H > 1)  H -= 1;
	}
}

void HSV2RGB3f(float h, float s, float v, float& r, float& g, float& b)
{
	// From FvD
	if (s <= 0.0) {
		r = v;
		g = v;
		b = v;
	}
	h = fmod(h, float(2.0 * M_PI));
	if (h < 0.0)
		h += 2.0 * M_PI;
	h /= M_PI / 3.0;
	int i = int(floor(h));
	double f = h - i;
	double p = v * (1.0 - s);
	double q = v * (1.0 - (s*f));
	double t = v * (1.0 - (s*(1.0-f)));
	switch(i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:
		r = v;
		g = p;
		b = q;
		break;
	}
}

void HSV2RGB3ub( float h, float s, float v, unsigned char& R, unsigned char& G, unsigned char& B )
{
	float r, g, b;
	HSV2RGB3f(h, s, v, r, g, b);
	R = unsigned char(r * 255 + 0.5);
	G = unsigned char(r * 255 + 0.5);
	B = unsigned char(r * 255 + 0.5);
}
