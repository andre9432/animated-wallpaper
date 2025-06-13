#ifndef SHADERS_H
#define SHADERS_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
	unsigned int program{ 0 };

public:
	Shader(){}

	Shader(const char* shadersPath);

	void create(const char* shadersPath);

	void setbool(const char* name, bool value);
	void setint(const char* name, int value);
	void set2i(const char* name, glm::ivec2 value);
	void setfloat(const char* name, float value);
	void set2f(const char* name, glm::vec2 value);
	void set3f(const char* name, glm::vec3 value);
	void set4f(const char* name, glm::vec4 value);
	void setmat4(const char* name, const glm::mat4& value);

	void use();
};


#endif // !SHADERS_H

