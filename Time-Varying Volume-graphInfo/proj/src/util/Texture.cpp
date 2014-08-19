#include "util/Texture.h"
//#include "util/GPUInfoWindows.h"

//------------------------------------------------------------------------------
// Texture
//------------------------------------------------------------------------------

Texture::Texture(const Vector3i& dimensions, GLint format, GLint internalformat,
	GLenum dataType, Filter filter, bool textureRectangle)
	: dimensions_(dimensions)
	, format_(format)
	, internalformat_(internalformat)
	, dataType_(dataType)
	, filter_(filter)
	, wrapping_(REPEAT)
	, priority_(-1.f)
	, pixels_(0)
	, id_(0)
{
	init(true, textureRectangle);
}

Texture::Texture(const Vector3i& dimensions, GLint format,
	GLenum dataType, Filter filter, bool textureRectangle)
	: dimensions_(dimensions)
	, format_(format)
	, internalformat_(format)
	, dataType_(dataType)
	, filter_(filter)
	, wrapping_(REPEAT)
	, priority_(-1.f)
	, pixels_(0)
	, id_(0)
{
	init(true, textureRectangle);
}

Texture::Texture(GLubyte* data, const Vector3i& dimensions, GLint format, GLint internalformat,
	GLenum dataType, Filter filter, bool textureRectangle)
	: dimensions_(dimensions)
	, format_(format)
	, internalformat_(internalformat)
	, dataType_(dataType)
	, filter_(filter)
	, wrapping_(REPEAT)
	, priority_(-1.f)
	, pixels_(data)
	, id_(0)
{
	init(false, textureRectangle);
	arraySize_ = hmul(dimensions_) * bpp_;
}

Texture::Texture(GLubyte* data, const Vector3i& dimensions, GLint format,
	GLenum dataType, Filter filter, bool textureRectangle)
	: dimensions_(dimensions)
	, format_(format)
	, internalformat_(format)
	, dataType_(dataType)
	, filter_(filter)
	, wrapping_(REPEAT)
	, priority_(-1.f)
	, pixels_(data)
	, id_(0)
{
	init(false, textureRectangle);
	arraySize_ = hmul(dimensions_) * bpp_;
}

void Texture::init(bool allocData, bool textureRectangle) {
#ifndef GL_TEXTURE_RECTANGLE_ARB
	textureRectangle = false;
#endif

	calcType(textureRectangle);
	calcBpp();

	generateId();

	if (allocData)
		alloc();

	applyFilter();
}

int Texture::calcBpp() {
	int typeSize = 0;

	switch (dataType_) {
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		typeSize = 1;
		break;

	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
		typeSize = 2;
		break;

	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FLOAT:
		typeSize = 4;
		break;

	default:
		vxAssert(false, "unsupported dataType");
	}

	int numComponents = 0;

	switch (format_) {
	case 1:
	case GL_COLOR_INDEX:
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
	case GL_ALPHA:
	case GL_INTENSITY:
	case GL_LUMINANCE:
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_COMPONENT24:
		numComponents = 1;
		break;

	case 2:
	case GL_LUMINANCE_ALPHA:
		numComponents = 2;
		break;

	case 3:
	case GL_RGB:
	case GL_BGR:
		numComponents = 3;
		break;

	case 4:
	case GL_RGBA:
	case GL_BGRA:
	case GL_RGBA16F_ARB:
		numComponents = 4;
		break;

	default:
		vxAssert( false, "unsupported format" );
	}

	bpp_ = typeSize * numComponents;
	return bpp_;
}

int Texture::getSizeOnGPU() {
	int bpp = 0;
	switch (internalformat_) {
	case 1:
	case GL_COLOR_INDEX:
	case GL_RED:
	case GL_GREEN:
	case GL_BLUE:
	case GL_ALPHA:
	case GL_INTENSITY:
	case GL_LUMINANCE:
	case GL_DEPTH_COMPONENT:
		bpp = 1;
		break;

	case 2:
	case GL_LUMINANCE_ALPHA:
	case GL_DEPTH_COMPONENT16:
		bpp = 2;
		break;

	case GL_RGB:
	case GL_BGR:
	case GL_DEPTH_COMPONENT24:
		bpp= 3;
		break;

	case GL_RGBA:
	case GL_BGRA:
	case GL_DEPTH_COMPONENT32:
		bpp = 4;
		break;

	case GL_RGB16F_ARB:
		bpp = 6;
		break;

	case GL_RGBA16F_ARB:
		bpp = 8;
		break;

	default:
		break;
	}
	return hmul(dimensions_)*bpp;
}

GLenum Texture::calcType(bool textureRectangle) {
	if (dimensions_.z == 1)	{
		if (dimensions_.y == 1)
			type_ = GL_TEXTURE_1D;
		else
			type_ = GL_TEXTURE_2D;
	}
	else {
		type_ = GL_TEXTURE_3D;
	}

#ifdef GL_TEXTURE_RECTANGLE_ARB
	if (type_ == GL_TEXTURE_2D && textureRectangle)
		type_ = GL_TEXTURE_RECTANGLE_ARB;
#endif

	return type_;
}

void Texture::setFilter(Filter filter) {
	filter_ = filter;
	applyFilter();
}

void Texture::applyFilter() {
	bind();

	switch(filter_) {
	case NEAREST:
		glTexParameteri(type_,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(type_,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		break;

	case LINEAR:
		glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;

// 	case ANISOTROPIC:
// 		glTexParameterf(type_, GL_TEXTURE_MAX_ANISOTROPY_EXT, GPUCapsWin.getMaxTextureAnisotropy());

	case MIPMAP:
		glTexParameteri(type_,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(type_,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type_, GL_SGIS_generate_mipmap, GL_TRUE);
		break;
	}
}

void Texture::setWrapping(Wrapping w) {
	wrapping_ = w;
	applyWrapping();
}

void Texture::applyWrapping() {
	bind();
	GLint wrap = wrapping_;

	/*
	set wrapping for all diminesions
	*/
	glTexParameteri(type_, GL_TEXTURE_WRAP_S, wrap);

	if (type_ == GL_TEXTURE_2D || type_ == GL_TEXTURE_3D)
		glTexParameteri(type_, GL_TEXTURE_WRAP_T, wrap);
	if (GL_TEXTURE_3D)
		glTexParameteri(type_, GL_TEXTURE_WRAP_R, wrap);
}

void Texture::uploadTexture() {
	bind();
	
	switch(type_) {
	case GL_TEXTURE_1D:
		glTexImage1D(GL_TEXTURE_1D, 0, internalformat_, dimensions_.x, 0,
			format_, dataType_, pixels_);
		break;

	case GL_TEXTURE_2D:
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat_, dimensions_.x, dimensions_.y, 0,
			format_, dataType_, pixels_);
		break;

	case GL_TEXTURE_3D:
		glTexImage3D(GL_TEXTURE_3D, 0, internalformat_,
			dimensions_.x, dimensions_.y, dimensions_.z, 0,
			format_, dataType_, pixels_);
		break;

#ifdef GL_TEXTURE_RECTANGLE_ARB
	case GL_TEXTURE_RECTANGLE_ARB:
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, internalformat_, dimensions_.x, dimensions_.y, 0,
			format_, dataType_, pixels_);
		break;
#endif

	}
}

void Texture::downloadTexture() {
	bind();

	if (pixels_ == 0)
		alloc();

	glGetTexImage(type_, 0, format_, dataType_, pixels_);
}

GLubyte* Texture::downloadTextureToBuffer() const {
	bind();

	int arraySize = hmul(dimensions_) * bpp_;
	GLubyte* pixels = new GLubyte[arraySize];

	glGetTexImage(type_, 0, format_, dataType_, pixels);
	return pixels;
}

bool Texture::isTextureRectangle() {
#ifdef GL_TEXTURE_RECTANGLE_ARB
	return (type_ == GL_TEXTURE_RECTANGLE_ARB);
#else
	return false;
#endif
}
