#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GL/glew.h>

#include <map>
#include <string>

class Shader
{
public:

    Shader();
    Shader(const std::string& vertex,
           const std::string& fragment);

    ~Shader();
    void Load(const std::string& vertex,
              const std::string& fragment);

    void Bind();
    void UnBind();

    unsigned int GetAttribute(const std::string& name);
    unsigned int GetUniform(const std::string& name);
    unsigned int GetHandle();

private:

    // disallow copy
    Shader(const Shader&);
    Shader& operator=(const Shader&);
    
    unsigned int progID; // OpenGL shader program ID
    std::map<std::string, unsigned int> attributes;
    std::map<std::string, unsigned int> uniforms;

    std::string ReadFile(const std::string& path);
    unsigned int CompileVertexShader(const std::string& vertex);
    unsigned int CompileFragmentShader(const std::string& fragment);
    bool LinkShaders(unsigned int vertex, unsigned int fragment);
    void PopulateAttributes();
    void PopulateUniforms();
};

#endif // SHADER_H_INCLUDED

