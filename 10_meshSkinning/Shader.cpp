#include <Shader.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

Shader::Shader()
{
    progID = glCreateProgram();
}
Shader::Shader(const std::string& vertex,
               const std::string& fragment)
{
    progID = glCreateProgram();
    Load(vertex, fragment);
}

Shader::~Shader()
{
    glDeleteProgram(progID);
}

void Shader::Load(const std::string& vertex,
                  const std::string& fragment)
{
    // verify the files exist
    std::ifstream v(vertex.c_str());
    bool vertFile = v.good();
    v.close();
    std::ifstream f(fragment.c_str());
    bool fragFile = f.good();
    f.close();

    std::string vSource = vertex;
    if (vertFile) {
        vSource = ReadFile(vertex);
    }

    std::string fSource = fragment;
    if (fragFile) {
        fSource = ReadFile(fragment);
    }

    unsigned int vert = CompileVertexShader(vSource);
    unsigned int frag = CompileFragmentShader(fSource);

    if (LinkShaders(vert, frag)) {
        PopulateAttributes();
        PopulateUniforms();
    }
}

void Shader::Bind()   { glUseProgram(progID); }
void Shader::UnBind() { glUseProgram(0);      }

unsigned int Shader::GetAttribute(const std::string& name)
{
    std::map<std::string, unsigned int>::iterator it =
        attributes.find(name);
    if (it == attributes.end()) {
        std::cout << "Bad attrib index: " << name << std::endl;
        return 0;
    }

    return it->second;
}

unsigned int Shader::GetUniform(const std::string& name)
{
    std::map<std::string, unsigned int>::iterator it =
        uniforms.find(name);
    if (it == uniforms.end()) {
        std::cout << "Bad uniform index: " << name << std::endl;
        return 0;
    }
    
    return it->second;

}

unsigned int Shader::GetHandle() { return progID; }

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file;
    file.open(path);
    std::stringstream contents;
    contents << file.rdbuf();
    file.close();
    return contents.str();
}

unsigned int Shader::CompileVertexShader(const std::string& vertex)
{
    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    const char* vSource = vertex.c_str();
    glShaderSource(v, 1, &vSource, NULL);
    glCompileShader(v);
    
    int success = 0;
    glGetShaderiv(v, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(v, 512, NULL, infoLog);
        std::cout << "Vertex compilation failed" << std::endl
                  << infoLog << std::endl;
        glDeleteShader(v);
        return 0;
    }

    return v;
}

unsigned int Shader::CompileFragmentShader(const std::string& fragment)
{
    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fSource = fragment.c_str();
    glShaderSource(f, 1, &fSource, NULL);
    glCompileShader(f);
    
    int success = 0;
    glGetShaderiv(f, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(f, 512, NULL, infoLog);
        std::cout << "Fragment compilation failed" << std::endl
                  << infoLog << std::endl;
        glDeleteShader(f);
        return 0;
    }

    return f;
}

bool Shader::LinkShaders(unsigned int vertex, unsigned int fragment)
{
    glAttachShader(progID, vertex);
    glAttachShader(progID, fragment);
    glLinkProgram(progID);

    int success = 0;
    glGetProgramiv(progID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(progID, 512, NULL, infoLog);
        std::cout << "Shader linking failed" << std::endl
                  << infoLog << std::endl;
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return true;
}

void Shader::PopulateAttributes()
{
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;

    // get the number of shader attributes
    glUseProgram(progID);
    glGetProgramiv(progID, GL_ACTIVE_ATTRIBUTES, &count);
    
    for (int i = 0; i < count; i++) {

        // get the attribute name
        memset(name, 0, sizeof(char) * 128);
        glGetActiveAttrib(progID, (GLuint)i, 128, &length, &size, &type, name);

        // store the name and location in the map
        int attrib = glGetAttribLocation(progID, name);
        if (attrib >= 0) {
            attributes[name] = attrib;
        }
    }

    // unbind
    glUseProgram(0);
}

void Shader::PopulateUniforms()
{
    int count = -1;
    int length;
    char name[128];
    int size;
    GLenum type;
    char testName[256];

    // get the number of shader uniforms
    glUseProgram(progID);
    glGetProgramiv(progID, GL_ACTIVE_UNIFORMS, &count);
    
    for (int i = 0; i < count; i++) {

        // get the attribute name
        memset(name, 0, sizeof(char) * 128);
        glGetActiveUniform(progID, (GLuint)i, 128, &length, &size, &type, name);

        // store the name and location in the map
        int uniform = glGetUniformLocation(progID, name);
        if (uniform >= 0) {
            // check if its an array
            std::string uniformName = name;
            std::size_t found = uniformName.find('[');
            if (found != std::string::npos) {
                uniformName.erase(uniformName.begin() + found, uniformName.end());
                unsigned int uniformIndex = 0;
                while (true) {
                    memset(testName, 0, sizeof(char) * 256);
                    sprintf(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
                    int uniformLocation = glGetUniformLocation(progID, testName);
                    if (uniformLocation < 0) {
                        break;
                    }
                    uniforms[testName] = uniformLocation;
                }
            }
            // add the original array name also
            uniforms[uniformName] = uniform;
        }
    }

    // unbind
    glUseProgram(0);
}


