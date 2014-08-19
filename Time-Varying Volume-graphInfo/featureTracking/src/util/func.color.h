/************************************************************************
/* Color Operation
/************************************************************************/

#ifndef FUNC_COLOR_H
#define FUNC_COLOR_H

inline unsigned char rgbToGray(unsigned char r, unsigned char g, unsigned char b) {
	return unsigned char(int(r * 299 + g * 587 + b * 114 + 500) / 1000);
// 	double R = r / (double)255.0;
// 	double G = g / (double)255.0;
// 	double B = b / (double)255.0;
// 	double y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
// 	if (y > 0.008856) return 116.0 * pow(y, 1.0 / 3.0) - 16.0;
// 	else return 903.3 * y;
};
extern unsigned int rgba2uint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
extern void RGBf2XYZ(float r, float g, float b, float& X, float& Y, float& Z);
extern void RGB2XYZ(int R, int G, int B, float& X, float& Y, float& Z);
extern void XYZ2RGBf(float X, float Y, float Z, float& r, float& g, float& b);
extern void XYZ2RGB(float X, float Y, float Z, unsigned char& R, unsigned char& G, unsigned char& B);
extern void XYZ2Lab(float X, float Y, float Z, float& L, float& a, float& b);
extern void Lab2XYZ(float L, float a, float b, float& X, float& Y, float& Z);
extern void RGB2LUVf(int R, int G, int B, float& L, float& U, float& V, bool scaled = false);

extern void RGB2Lab3f(int R, int G, int B, float& L, float& a, float& b);
extern void RGB2Lab3ub(unsigned char R, unsigned char G, unsigned char B, unsigned char& L, unsigned char& a, unsigned char& b);
extern void Lab3f2RGB3ub(float L, float a, float b, unsigned char& R, unsigned char& G, unsigned char& B);
extern void RGB3ub2HSV(unsigned char R, unsigned char G, unsigned char B, float& H, float& S, float&V);
extern void RGB2HSV(float R, float G, float B, float& H, float& S, float&V);
extern void RGB2HSL(unsigned char r, unsigned char g, unsigned char b, float& H, float& S, float&L);
extern void HSV2RGB3f(float h, float s, float v, float& r, float& g, float& b);
extern void HSV2RGB3ub(float h, float s, float v, unsigned char& R, unsigned char& G, unsigned char& B);
#endif // FUNC_COLOR_H