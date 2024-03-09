//
//  shader_class.hpp
//  gl
//
//  Created by Sargis Khachatryan on 09.03.24.
//

#ifndef shader_class_hpp
#define shader_class_hpp

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace shader_class {

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);

    // activate the shader
    // ------------------------------------------------------------------------
    void use();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const;

    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const;

    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const;

    
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type);

};

} // namespace shader_class

#endif /* shader_class_hpp */
