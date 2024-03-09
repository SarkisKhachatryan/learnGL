//
//  master.hpp
//  gl
//
//  Created by Sargis Khachatryan on 09.03.24.
//

#ifndef master_hpp
#define master_hpp

#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

namespace master {

int master();

} // namespace master


#endif /* master_hpp */
