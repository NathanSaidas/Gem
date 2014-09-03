/*#define STB_IMAGE_IMPLEMENTATION
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>
#include <AL\al.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

void loadImage(std::string & filename)
{
    int x, y, comp;
    unsigned char * data;
    
    unsigned int rmask, gmask, bmask, amask;
    unsigned int textureHandle;
    
    FILE * file = fopen(filename.c_str(), "rb");
    if(!file)
    {
        return;
    }
    //
    data = stbi_load_from_file(file,&x,&y,&comp,0);
    //
    fclose(file);
    //
    glGenTextures(1,&textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    GLint format;
    
    switch(comp)
    {
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        //ERROR
        break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, x,y,0,format, GL_UNSIGNED_BYTE, (GLvoid*)data);
    
    stbi_image_free(data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

}*/

#include "../Core/G_Application.h"
#include "../Core/Memory/G_Memory.h"
#include "../Core/Input/G_InputAxis.h"

#include "../Core/Utilities/G_Utilities.h"
#include "../../Windows/Windows/fruit.h"
#include <vector>


//Example of a Local Pointer that is confined to the scope
template<class T>
class LocalPtr
{
public:
    LocalPtr()
    {
        m_Data = Gem::Memory::instantiate<T>();
    }
    ~LocalPtr()
    {
        m_Data = Gem::Memory::destroy<T>(m_Data);
    }

    T * ref()
    {
        return m_Data;
    }
    T data()
    {
        return *m_Data;
    }

    T operator =(T & aValue)
    {
        return (*m_Data) = aValue;
    }
    T operator =(T aValue)
    {
        return (*m_Data) = aValue;
    }
private:
    T * m_Data;
};


int main()
{
    Gem::Application::instance()->execute();
    Gem::Application::destroy();
    Gem::MemoryManager::destroy();
    //scopeTest(firstRef);

    //integer refCount = aClass->getInt(

    system("pause");

    return 0;
}
