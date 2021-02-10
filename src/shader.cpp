#include "shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::createFromString(const char *vertexCode, const char *fragmentCode)
{
    compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char *vertexLocation, const char *fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();

    compileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char *fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cout << " Failed to read!" << fileLocation << "File doesn't exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint Shader::getModelLocation()
{
    return uniformModel;
}

GLuint Shader::getProjectionLocation()
{
    return uniformProjection;
}

void Shader::useShader()
{
    if (shaderID == 0)
    {
        return;
    }
    glUseProgram(shaderID);
}

void Shader::clearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader()
{
    clearShader();
}

void Shader::addShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling the shader program: \n"
                  << shaderType << eLog << std::endl;
        return;
    }

    glAttachShader(program, theShader);
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        std::cout << "Error creating shader program!" << std::endl;
        return;
    }

    addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {0};

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: \n"
                  << eLog << std::endl;
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program: \n"
                  << eLog << std::endl;
        return;
    }

    // Grab the location of uniform variable
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}