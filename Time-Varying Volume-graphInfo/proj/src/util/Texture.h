#ifndef Texture_H
#define Texture_H

#include <string>
#include <GL/glew.h>
#include "util/vector2.h"
#include "util/vector3.h"
#include "util/vector4.h"

#define vxAssert(condition, message) \
	if(!(bool(condition))) {\
		printf("%d: %s -> %s, %s\n",__LINE__, __FILE__, __FUNCTION__, message);\
		assert(condition);\
	}

class Texture {
public:

	enum Filter {
		NEAREST,
		LINEAR,
		MIPMAP,
		ANISOTROPIC
	};

	enum Wrapping {
		REPEAT = GL_REPEAT,
		CLAMP = GL_CLAMP,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT
	};

	Texture()
		: priority_(-1.f), pixels_(0), id_(0)
	{}

	//FIXME: these ctors are ambiguous due to the default params, you need to specify all
	//       arguments or the compile won't know which one you mean. joerg

	/**
	* Without data and internalformat argument, type_ is calculated by
	* dimensions and a new chunk of data will be allocated on the heap.
	*/
	Texture(const Vector3i& dimensions, GLint format = GL_LUMINANCE,
		GLenum dataType = GL_UNSIGNED_BYTE, Filter filter = LINEAR,
		bool textureRectangle = false);

	/**
	* Without data and with internalformat argument, type_ is calculated by
	* dimensions and a new chunk of data will be allocated on the heap.
	*/
	Texture(const Vector3i& dimensions, GLint format, GLint internalformat,
		GLenum dataType  = GL_UNSIGNED_BYTE, Filter filter = LINEAR,
		bool textureRectangle = false);

	/**
	* With data and without internalformat argument, type_ is calculated by
	* dimensions and no new chunk of data will be allocated on the heap.
	*/
	Texture(GLubyte* data, const Vector3i& dimensions, GLint format = GL_RGBA,
		GLenum dataType = GL_UNSIGNED_BYTE, Filter filter = LINEAR,
		bool textureRectangle = false);

	/**
	* With data and internalformat argument, type_ is calculated by
	* dimensions and no new chunk of data will be allocated on the heap.
	*/
	Texture(GLubyte* data, const Vector3i& dimensions, GLint format, GLint internalformat,
		GLenum dataType  = GL_UNSIGNED_BYTE, Filter filter = LINEAR,
		bool textureRectangle = false);

	/**
	* The destructor deletes the Texture in OpenGL.
	* Handled by the texturemanager!
	*/
	virtual ~Texture()
	{
		if (id_)
			glDeleteTextures(1, &id_);
	}

	/// allocates an appropriate buffer for the texture
	void alloc() {
		arraySize_ = hmul(dimensions_) * bpp_;
		//pixels_ = new GLubyte[arraySize_];
	}

	/// destroys the buffer for the texture and sets arraySize_ to zero
	void destroy() {
		arraySize_ = 0;
		if(pixels_)
			delete[] pixels_;
		pixels_ = 0;// so nothing really nasty can happen
	}

	/// calculates the bytes per pixel from dataType and internalformat
	int calcBpp();

	///calculates size on the GPU (using internalformat)
	int getSizeOnGPU();

	/**
	* calculates the type_ (GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D or GL_TEXTURE_RECTANGLE_ARB) from
	* dimensions_
	*
	* @param textureRectangle Determines, wether texture should be a texture rectangle
	*      GL_TEXTURE_RECTANGLE_ARB
	*/
	GLenum calcType(bool textureRectangle = false);

	/**
	* Bind the texture to the active texture unit and target.
	*
	* Note: This does not enable texturing (use enable()).
	*/
	void bind() const
	{
		glBindTexture(type_ , id_);
	}

	/**
	* Enable texturing on the active texture unit.
	*/
	void enable() const
	{
		glEnable(type_);
	}

	/**
	* Disable texturing on the active texture unit
	*/
	void disable() const
	{
		glDisable(type_);
	}

	/**
	*   Return OpenGL texture ID
	*/
	GLuint getId() const { return id_; }

	/**
	*   Set OpenGL texture ID.
	*/
	void setId(GLuint id) { id_ = id; }

	/**
	*   Generate OpenGL texture ID
	*   @return The generated ID
	*/
	GLuint generateId() {
		id_ = 0; 
		glGenTextures(1, &id_);
		return id_;
	}

	/**
	*   Returns type of the texture (GL_TEXTURE_1D...3D, GL_TEXTURE_RECTANGLE_ARB)
	*/
	GLenum getType() const { return type_; }

	Vector3i getDimensions() const { return dimensions_;}
	int getWidth() const { return dimensions_.x; };
	int getHeight() const { return dimensions_.y; }
	int getDepth() const { return dimensions_.z; }
	GLint getFormat() const { return format_; }
	GLint getInternalFormat() const { return internalformat_; }
	Filter getFilter() const { return filter_; }
	GLenum getDataType() const { return dataType_; }
	size_t getArraySize() const { return arraySize_; }
	int getVolumeIdx() const { return m_iVolumeIdx; }
	int getVolumeBrickIdx() const { return m_iVolumeBrickIdx; }
	int getVectorIdx() const { return m_iVectorIdx; }

	void setDimensions(Vector3i dimensions) { 
// 			pvxTexMg->adjustGPUMemory(-dimensions_.x * dimensions_.y * dimensions_.z);
// 			pvxTexMg->adjustGPUMemory(dimensions.x * dimensions.y * dimensions.z);
		dimensions_ = dimensions; 
	}
	void setBpp(GLubyte bpp) { bpp_ = bpp; }
	void setFormat(GLint format) { format_ = format; }
	void setInternalFormat(GLint internalformat) { internalformat_ = internalformat; }
	void setType(GLenum type) { type_ = type; }
	void setDataType(GLenum dataType) { dataType_ = dataType; }
	void setVolumeIdx(int volumeIdx) { m_iVolumeIdx = volumeIdx; }
	void setVolumeBrickIdx(int volumeBrickIdx) { m_iVolumeBrickIdx = volumeBrickIdx; }
	void setVectorIdx(int vectorIdx){ m_iVectorIdx = vectorIdx; }

	GLubyte* getPixelData() { return pixels_; }
	const GLubyte* getPixelData() const { return pixels_; }
	void setPixelData(GLubyte* pixels)
	{
		pixels_ = pixels;
		arraySize_ = pixels ? hmul(dimensions_) * bpp_ : 0;
	}

	/**
	*   Returns the Bytes Per Pixel used,
	*   usually 3 or 4 for 24/32 bits per pixel.
	*/
	GLubyte getBpp() const { return bpp_; }

	/**
	*   Set Priority of this texture in GL
	*   @param p Priority, [0...1]
	*/
	void setPriority(GLclampf p)
	{
		glPrioritizeTextures(1, &id_, &p);
	}

	/**
	*   Returns current Priority, -1 if not yet set
	*/
	GLclampf getPriority() const { return priority_; }

	/**
	*   Check if texture is in resident GL memory
	*/
	/*bool isResident() const
	{
		GLboolean resident;
		return glAreTexturesResident(1, &id_, &resident) == GL_TRUE;
	}*/
	bool isResident() const
	{
		return m_bIsResident;
	}

	/**
	*	set the resident attr 
	*/
	void setResident(bool resident)
	{
		m_bIsResident = resident;
	}

	/**
	*   Sets Filtering for Texture. Binds the texture.
	*/
	void setFilter(Filter filter);

	/**
	* Applies the stored filter once again. Binds the texture.
	*/
	void applyFilter();

	/// Set texture wrapping mode. Binds the texture.
	void setWrapping(Wrapping w);

	/// Applies the textur wrapping mode once againg. Binds the texture.
	void applyWrapping();

	Wrapping getWrapping() { return wrapping_; }

	/**
	* Upload Texture to graphics-card. Binds the texture.
	* 
	* type_, format_, internalformat_, dimensions, dataType_ and the pixels_ pointer have to
	* be set before calling this method.
	*/
	void uploadTexture();


	/**
	* Download Texture from graphics-card. Binds the texture.
	*
	* type_, format_, dimensions, dataType_ and the pixels_ pointer have to be set before
	* calling this method! Pixels will be allocated if pixels_ is a nullpointer.
	*/
	void downloadTexture();

	/**
	* Download texture from the GPU to a newly allocated buffer, to which a
	* pointer is returned.  Binds the texture.
	*
	* type_, format_, dimensions, and dataType_ have to be set before
	* calling this method!
	*/
	GLubyte* downloadTextureToBuffer() const;

	/**
	* Returns, wether texture is a texture rectangle (GL_TEXTURE_RECTANGLE_ARB)
	*/
	bool isTextureRectangle();

// 	/*
// 	1D access, always possible
// 	*/
// 	template <class T>
// 	inline T& texel(size_t i) {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( i < size_t(hmul(dimensions_)), "index out of range" );
// 		return ((T*) pixels_)[i];
// 	}
// 	template <class T>
// 	inline const T& texel(size_t i) const {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( i < size_t(hmul(dimensions_)), "index out of range" );
// 		return ((T*) pixels_)[i];
// 	}
// 
// 	/*
// 	2D access, only possible when type_ == GL_TEXTURE_2D
// 	*/
// 	template <class T>
// 	inline T& texel(size_t x, size_t y) {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( dimensions_.x * y + x < size_t(hmul(dimensions_)), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_2D, "using 2d access, but it's not a GL_TEXTURE_2D");
// 		return ((T*) pixels_)[dimensions_.x * y + x];
// 	}
// 	template <class T>
// 	inline const T& texel(size_t x, size_t y) const {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( dimensions_.x * y + x < size_t(hmul(dimensions_)), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_2D, "using 2d access, but it's not a GL_TEXTURE_2D");
// 		return ((T*) pixels_)[dimensions_.x * y + x];
// 	}
// 	template <class T>
// 	inline T& texel(const Point2i& pos) {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( dimensions_.x * pos.y + pos.x < hmul(dimensions_), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_2D, "using 2d access, but it's not a GL_TEXTURE_2D");
// 		return ((T*) pixels_)[dimensions_.x * pos.y + pos.x];
// 	}
// 	template <class T>
// 	inline const T& texel(const Point2i& pos) const {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( dimensions_.x * pos.y + pos.x < hmul(dimensions_), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_2D, "using 2d access, but it's not a GL_TEXTURE_2D");
// 		return ((T*) pixels_)[dimensions_.x * pos.y + pos.x];
// 	}
// 
// 	/*
// 	3D access, only possible when type_ == GL_TEXTURE_3D
// 	*/
// 	template <class T>
// 	inline T& texel(size_t x, size_t y, size_t z) {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( z*dimensions_.x*dimensions_.y + y*dimensions_.x + x < size_t(hmul(dimensions_)), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_3D, "using 3d access, but it's not a GL_TEXTURE_3D");
// 		return ((T*) pixels_)[z*dimensions_.x*dimensions_.y + y*dimensions_.x + x];
// 	}
// 	template <class T>
// 	inline const T& texel(size_t x, size_t y, size_t z) const {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( z*dimensions_.x*dimensions_.y + y*dimensions_.x + x < size_t(hmul(dimensions_)), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_3D, "using 3d access, but it's not a GL_TEXTURE_3D");
// 		return ((T*) pixels_)[z*dimensions_.x*dimensions_.y + y*dimensions_.x + x];
// 	}
// 	template <class T>
// 	inline T& texel(const Vector3i& pos) {
// 		vxAssert( sizeof(T) == bpp_, "sizeof(T) != bytes per pixel here" );
// 		vxAssert( pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x < hmul(dimensions_), "index out of range" );
// 		vxAssert( type_ == GL_TEXTURE_3D, "using 3d access, but it's not a GL_TEXTURE_3D");
// 		return ((T*) pixels_)[pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x];
// 	}
// 	template <class T>
// 	inline const T& texel(const Vector3i& pos) const {
// 		assert( sizeof(T) == bpp_);// "sizeof(T) != bytes per pixel here" );
// 		assert( pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x < hmul(dimensions_));//, "index out of range" );
// 		assert( type_ == GL_TEXTURE_3D);//, "using 3d access, but it's not a GL_TEXTURE_3D");
// 		return ((T*) pixels_)[pos.z*dimensions_.x*dimensions_.y + pos.y*dimensions_.x + pos.x];
// 	}

	GLubyte* pixels_;       ///< (temporary) buffer for loading image
protected:
	Vector3i dimensions_;
	GLint format_;          ///< GL_RGB...
	GLint internalformat_;  ///< GL_RGB...
	GLenum dataType_;       ///< GL_UNSIGNED_BYTE
	Filter filter_;
	Wrapping wrapping_;

	GLclampf priority_;     ///< GL texture priority [0...1]
	size_t arraySize_;
	
	GLenum type_;           ///< 1D, 2D, 3D
	GLubyte bpp_;           ///< bytes per pixel
	
	//======
	GLuint id_;             ///< OpenGL texture id
	int m_iVolumeIdx;
	int m_iVolumeBrickIdx;
	int m_iVectorIdx;
	bool m_bIsResident;
	//=====

	// used internally in the constructors
	void init(bool allocData, bool textureRectangle);
	//calculate the mul of one vector's all elements 
	int hmul(const Vector3i& dimensions) const
	{
		return dimensions.x * dimensions.y * dimensions.z;
	}

};

#endif // Texture_H
