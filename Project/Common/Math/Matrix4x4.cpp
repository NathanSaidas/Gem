#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

namespace Gem
{
//#ifdef _MSC_VER
//#pragma warning (push)
//#pragma warning (disable : 4018)
//#endif

	RDEFINE_CLASS(Matrix4x4, object)
    Matrix4x4::Matrix4x4() : object(), glm::mat4x4()
    {

    }
    Matrix4x4::Matrix4x4(const glm::mat4x4 & aMatrix) : object(), glm::mat4x4(aMatrix)
    {

    }
    Matrix4x4::~Matrix4x4()
    {

    }


    Matrix4x4 Matrix4x4::Transpose(const Matrix4x4 & aMatrix)                                                                           
    {
        return Matrix4x4(glm::transpose(aMatrix.Raw()));
    }
    Matrix4x4 Matrix4x4::Inverse(const Matrix4x4 & aMatrix)                                                                             
    {
        return Matrix4x4(glm::inverse(aMatrix.Raw()));
    }
    Matrix4x4 Matrix4x4::Scale(const Matrix4x4 & aMatrix, const Vector3 & aScale)                                                       
    {
        return Matrix4x4(glm::scale(aMatrix.Raw(), aScale.Raw()));
    }
    Matrix4x4 Matrix4x4::Translate(const Matrix4x4 & aMatrix, const Vector3 & aTranslation)                                             
    {
		Vector4 position = aMatrix[3];
		position.x += aTranslation.x;
		position.y += aTranslation.y;
		position.z += aTranslation.z;
		Matrix4x4 mat = aMatrix;
		mat[3] = position;
		return mat;
    }
    Matrix4x4 Matrix4x4::Rotate(const Matrix4x4 & aMatrix, const Float32 & aRotation, const Vector3 & aAxis)                            
    {
        return Matrix4x4(glm::rotate(aMatrix.Raw(), aRotation, aAxis.Raw()));
    }
    Matrix4x4 Matrix4x4::Rotate(const Matrix4x4 & aMatrix, const Vector3 & aRotation)                                                   
    {
        //Rotate ZXY
        glm::mat4x4 mat = aMatrix.Raw();
        mat = glm::rotate(mat, aRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        mat = glm::rotate(mat, aRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        mat = glm::rotate(mat, aRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        
        return Matrix4x4(mat);
    }
    Matrix4x4 Matrix4x4::Ortho(const Float32 & aLeft, const Float32 & aRight, const Float32 & aTop, const Float32 & aBottom, const Float32 & aFar, const Float32 & aNear)
    {
        return Matrix4x4(glm::ortho(aLeft, aRight, aBottom, aTop, aNear, aFar));
    }
    Matrix4x4 Matrix4x4::Perspective(const Float32 & aFOV, const Float32 & aAspectRatio, const Float32 & aNear, const Float32 & aFar)   
    {
        return Matrix4x4(glm::perspective(aFOV, aAspectRatio, aNear, aFar));
    }
    Matrix4x4 Matrix4x4::LookAt(const Vector3 & aCameraPosition, const Vector3 & aDirection, const Vector3 & aUpDirection)
    {
		//glm::mat4 mat = glm::lookAt(aCameraPosition, aTargetPosition, aUpDirection);
		//Matrix4x4 result = mat;


        Vector3 f = aDirection.Normalized();
        Vector3 s = Vector3::Cross(f, aUpDirection).Normalized();
        Vector3 u = Vector3::Cross(s, f);

        Matrix4x4 result = Matrix4x4::Identity();
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;
        result[3][0] = -Vector3::Dot(s, aCameraPosition);
        result[3][1] = -Vector3::Dot(u, aCameraPosition);
        result[3][2] = Vector3::Dot(f, aCameraPosition);
        return result;

		return result;
    }

    Matrix4x4 Matrix4x4::LookAt(const Vector3 & aCameraPosition, const Vector3 & aDirection)
    {
        Vector3 f = aDirection.Normalized();
        Vector3 s = Vector3::Cross(f, Vector3::Up()).Normalized();
        Vector3 u = Vector3::Cross(s, f);

        Matrix4x4 result = Matrix4x4::Identity();
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;
        result[3][0] = -Vector3::Dot(s, aCameraPosition);
        result[3][1] = -Vector3::Dot(u, aCameraPosition);
        result[3][2] = Vector3::Dot(f, aCameraPosition);
        return result;
    }


    Matrix4x4 Matrix4x4::Identity()
    {
        Matrix4x4 mat;
        memset(&mat, 0, sizeof(Matrix4x4));
        mat[0][0] = 1.0f;
        mat[1][1] = 1.0f;
        mat[2][2] = 1.0f;
        mat[3][3] = 1.0f;
        return mat;
    }

	Matrix4x4 Matrix4x4::TRS(const Vector3 & aPosition, const Vector3 & aRotation, const Vector3 & aScale)
	{
		Matrix4x4 mat = Matrix4x4::Identity();
		mat.Scale(aScale);
		mat.Rotate(aRotation);
		mat.Translate(aPosition);
		return mat;
	}

    Matrix4x4 Matrix4x4::TRS(const Vector3 & aPosition, const Quaternion & aRotation, const Vector3 & aScale)
    {
        Matrix4x4 mat = Matrix4x4::Identity();
        mat.Scale(aScale);
        mat *= aRotation.GetRotationMatrix();
        mat.Translate(aPosition);
        return mat;
    }

    void Matrix4x4::Transpose()                                                  
    {
        *this = Matrix4x4(glm::transpose(Raw()));
    }
    void Matrix4x4::Inverse()                                                    
    {
        *this = Matrix4x4(glm::inverse(Raw()));
    }
    void Matrix4x4::Scale(const Vector3 & aScale)                                
    {
        *this = Matrix4x4(glm::scale(Raw(),aScale.Raw()));
    }
    void Matrix4x4::Translate(const Vector3 & aTranslation)                      
    {
		Vector4 position = (*this)[3];
		position.x += aTranslation.x;
		position.y += aTranslation.y;
		position.z += aTranslation.z;
		(*this)[3] = position;
    }
    void Matrix4x4::Rotate(const Float32 & aRotation,const Float32 & aAngle, const Vector3 & aAxis)     
    {
        *this = Matrix4x4(glm::rotate(Raw(),aAngle,aAxis));
    }
    void Matrix4x4::Rotate(const Vector3 & aRotation)                            
    {
        glm::mat4x4 mat = Raw();
        mat = glm::rotate(mat, aRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        mat = glm::rotate(mat, aRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        mat = glm::rotate(mat, aRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        *this = mat;
    }
    void Matrix4x4::SetIdentity()
    {
        memset(this, 0, sizeof(Matrix4x4));
        (*this)[0][0] = 1.0f;
        (*this)[1][1] = 1.0f;
        (*this)[2][2] = 1.0f;
        (*this)[3][3] = 1.0f;
    }
    Quaternion Matrix4x4::GetRotation() const
    {
        return Quaternion(glm::quat_cast(Raw()));
    }

    glm::mat4 Matrix4x4::Raw() const
    {
        glm::mat4 mat;
        mat[0] = (*this)[0];
        mat[1] = (*this)[1];
        mat[2] = (*this)[2];
        mat[3] = (*this)[3];
        return mat;
    }
//#ifdef _MSC_VER
//#pragma warning(pop)
//#endif

}



#ifdef GLM_FORCE_RADIANS
#undef GLM_FORCE_RADIANS
#endif