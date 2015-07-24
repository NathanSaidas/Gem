#ifndef GEM_PRIMITIVE_SHAPE_BUFFER_H
#define GEM_PRIMITIVE_SHAPE_BUFFER_H

//============================================================
// Date:			May,	13,		2015
// Description:		Defines the class PrimitiveShapeBuffer
//============================================================

#pragma  region CHANGE LOG
// -- Nathan Hanlan	- Refactored this class to fit in the Gem project from SchoolGameEngine
// -- Nathan Hanlan - Constructor now uses initialization list.
#pragma endregion

#include <glew-1.10.0/include/GL\glew.h>
#include "../Core/GemAPI.h"
#include "../Core/BasicTypes.h"

namespace Gem
{

    //FORCE_EXPORT_META(PrimitiveShapeBuffer);

    //This is a VBO used for drawing basic primitives. Data is reallocated each time a primitive makes a draw call.
    //Use Mesh for increased performance.
    class PrimitiveShapeBuffer : public object
    {
		RDECLARE_CLASS(PrimitiveShapeBuffer)
    public:
        // -- Initializes the PrimitiveShapeBuffer to default values
        PrimitiveShapeBuffer();
        // -- Releases existing memory from the PrimitiveShapeBuffer
        ~PrimitiveShapeBuffer();

        // -- Creates the PrimitiveShapeBuffer objects
        void Create();
        // -- Release the PrimitiveShapeBuffer objects
        void Release();
        // -- Returns true if the PrimitiveShapeBuffer is allocated
        bool IsAllocated();
        // -- Marshals data into the buffer. 
        void BindData(GLsizeiptr aAttributeCount, GLvoid * aAttributes, GLsizeiptr aIndicesSize, GLvoid * aIndices);
        // -- Returns the VBO handle
        GLuint GetVBO();
        // -- Returns the IBO handle
        GLuint GetIBO();

    private:
        GLuint m_VBO;
        GLuint m_IBO;
        bool m_Allocated;
    };

	TYPE_DEFINE(PrimitiveShapeBuffer)
}

#endif