#ifndef SHADERDATA_H
#define SHADERDATA_H

#include <string>
#include "Vector3.h"

class ShaderData
{
public:
	float getParamF(const std::string& name) = 0;
	unsigned int getParamUint(const std::string& name) = 0;
};

#endif // SHADERDATA_H