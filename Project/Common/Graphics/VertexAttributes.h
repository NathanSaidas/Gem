#ifndef GEM_VERTEX_ATTRIBUTE_H
#define GEM_VERTEX_ATTRIBUTE_H


#ifndef VERTEX_ATTRIB
#define VERTEX_ATTRIB(MEMBER) (void*)offsetof(Gem::VertexAttribute,MEMBER)
#endif 

#ifndef VERTEX_ATTRIB_POSITION
#define VERTEX_ATTRIB_POSITION VERTEX_ATTRIB(position)
#endif

#ifndef VERTEX_ATTRIB_NORMAL
#define VERTEX_ATTRIB_NORMAL VERTEX_ATTRIB(normal)
#endif

#ifndef VERTEX_ATTRIB_TEXCOORD
#define VERTEX_ATTRIB_TEXCOORD VERTEX_ATTRIB(texCoord)
#endif

#ifndef VERTEX_ATTRIB_COLOR
#define VERTEX_ATTRIB_COLOR VERTEX_ATTRIB(color)
#endif

namespace Gem
{
    //Describes the attributes of a vertex.
    struct VertexAttribute
    {
        float position[3];
        float normal[3];
        float texCoord[2];
        float color[4];
    };
}



#endif // GEM_VERTEX_ATTRIBUTE_H