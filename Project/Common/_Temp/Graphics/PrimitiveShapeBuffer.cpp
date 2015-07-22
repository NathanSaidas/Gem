#include "PrimitiveShapeBuffer.h"
#include "Graphics.h"


namespace Gem
{
	RDEFINE_CLASS(PrimitiveShapeBuffer, object)

    PrimitiveShapeBuffer::PrimitiveShapeBuffer()
		: m_VBO(0), m_IBO(0), m_Allocated(false)
    {

    }
    PrimitiveShapeBuffer::~PrimitiveShapeBuffer()
    {
        Release();
    }

    void PrimitiveShapeBuffer::Create()
    {
        if (!m_Allocated)
        {
            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_IBO);
        }
    }
    void PrimitiveShapeBuffer::Release()
    {
        if (m_Allocated)
        {
            glDeleteBuffers(1, &m_VBO);
            glDeleteBuffers(1, &m_IBO);
            m_VBO = 0;
            m_IBO = 0;
        }
    }
    bool PrimitiveShapeBuffer::IsAllocated()
    {
        return m_Allocated;
    }

    void PrimitiveShapeBuffer::BindData(GLsizeiptr aAttributeCount, GLvoid * aAttributes, GLsizeiptr aIndicesSize, GLvoid * aIndices)
    {
        //Allocate buffers
        if (!IsAllocated())
        {
            Create();
        }

        //Store previous targets
        BufferTarget previousBoundTarget = Graphics::GetCurrentBoundTarget();
        GLuint previousBoundBuffer = Graphics::GetCurrentBoundBuffer();

        //Buffer data for the vbo
        Graphics::BindBuffer(BufferTarget::Array, m_VBO);
        Graphics::BufferData(BufferTarget::Array, aAttributeCount, aAttributes, BufferMode::DynamicDraw);

        //Buffer data for the ibo
        Graphics::BindBuffer(BufferTarget::ElementArray, m_IBO);
        Graphics::BufferData(BufferTarget::ElementArray, aIndicesSize, aIndices, BufferMode::DynamicDraw);

        //Bind the previous targets.
        Graphics::BindBuffer(previousBoundTarget, previousBoundBuffer);
    }

    GLuint PrimitiveShapeBuffer::GetVBO()
    {
        return m_VBO;
    }
    GLuint PrimitiveShapeBuffer::GetIBO()
    {
        return m_IBO;
    }
}