#include "Program.h"
#include "../File.h"

namespace GL {

    Program::Program() {
        create();
    }

    Program::Program(const std::string& vsPath, const std::string& fsPath) {
        Program();

        Shader vertexShader(vsPath, Shader::Type::VertexShader);
        Shader fragmentShader(fsPath, Shader::Type::FragmentShader);
        load(vertexShader, fragmentShader);
    }

    Program::Program(const Shader& vertexShader, const Shader& fragmentShader) {
        Program();

        load(vertexShader, fragmentShader);
    }

    Program::~Program() {
        destroy();
    }

    Uniform& Program::operator[](const std::string& uniformName) {
        return _uniforms[uniformName];
    }
    
    void Program::load(const Shader& vertexShader, const Shader& fragmentShader) {
        if(_linked == false) {
            _linked = link(vertexShader, fragmentShader);
            mapUniforms(vertexShader);
            mapUniforms(fragmentShader);
        }
    }

    void Program::use() {
        if(_linked)
            glUseProgram(_programID);
    }

    void Program::unbind() {
        glUseProgram(0);
    }

    GLuint Program::getID() const {
        return _programID;
    }

    GLuint Program::getUniformLocation(const std::string& uniformName) {
        return _uniforms[uniformName].getLocation();
    }

    const Uniform& Program::getUniform(const std::string& uniformName) {
        return _uniforms[uniformName];
    }

    void Program::create() {
        _programID = glCreateProgram();
        _linked = false;
    }

    void Program::destroy() {
        glDeleteProgram(_programID);
    }

    bool Program::link(const Shader& vertexShader, const Shader& fragmentShader) {
        std::vector<char> programErrorMsg;
        GLint result;
        GLint infoLen;

        // Create and link program
        glAttachShader(_programID, vertexShader.getID());
        glAttachShader(_programID, fragmentShader.getID());
        glLinkProgram(_programID);

        // Check for errors
        glGetProgramiv(_programID, GL_LINK_STATUS, &result);
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLen);
        programErrorMsg.resize(infoLen);
        glGetProgramInfoLog(_programID, infoLen, NULL, &programErrorMsg[0]);

        if(result != GL_TRUE) {
            std::string errorMsg = std::string("Program linking failed:") + std::string(&programErrorMsg[0]);
            throw Util::Exception::FatalError(errorMsg.c_str());
        }
        
        return true;
    }

    void Program::mapUniforms(const Shader& shader) {
        std::stringstream stream;
        std::string uniformName;
        std::string uniformType;

        stream.str(shader._code);
        while(stream.good()) {
            stream >> uniformName;
            if(uniformName == "uniform") {
                stream >> uniformType;
                stream >> uniformName;

                if(uniformName[uniformName.size()-1] == ';')
                    uniformName.erase(uniformName.size() - 1);

                _uniforms[uniformName] = Uniform(glGetUniformLocation(_programID, uniformName.c_str()));
            }
        }
    }

}
