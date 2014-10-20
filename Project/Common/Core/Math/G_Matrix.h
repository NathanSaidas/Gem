#ifndef G_MATRIX_H
#define G_MATRIX_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "G_Vector.h"

namespace Gem
{

    class Matrix4x4 : public Object
    {
    public:
        float m11, m12, m13, m14;
        float m21, m22, m23, m24;
        float m31, m32, m33, m34;
        float m41, m42, m43, m44;

        virtual Pointer<Reflection::Type> getType() override;

        Matrix4x4(){}
        Matrix4x4(const Vector3 & aRight, const Vector3 & aUp, const Vector3 & at)
            : m11(aRight.x), m12(aUp.x), m13(at.x), m14(0.0f)
            , m21(aRight.y), m22(aUp.y), m23(at.y), m24(0.0f)
            , m31(aRight.z), m32(aUp.z), m33(at.z), m34(0.0f)
            , m41(0.0f)    , m42(0.0f) , m43(0.0f), m44(1.0f){}
        Matrix4x4(float v11, float v12, float v13, float v14,
             float v21, float v22, float v23, float v24,
             float v31, float v32, float v33, float v34,
             float v41, float v42, float v43, float v44)
        : m11(v11), m12(v12), m13(v13), m14(v14)
        , m21(v21), m22(v22), m23(v23), m24(v24)
        , m31(v31), m32(v32), m33(v33), m34(v34)
        , m41(v41), m42(v42), m43(v43), m44(v44) {}

        void setIdentity();
        void setPosition(Vector3 aPos);
        void setPosition(float x, float y, float z);
        void setFrustum(float aLeft, float aRight, float aBottom, float aTop, float aNearZ, float aFarZ);
        void setPerspective(float aFovy, float aAspect, float aNearZ, float aFarZ);
        void setLookAt(const Vector3 & aEye, const Vector3 & aUp, const Vector3 & aAt);

        void multiply(Matrix4x4 * aSrc);

        void scale(float aX, float aY, float aZ);
        void translatePreRotation(float aX, float aY, float aZ);
        void translatePostRotation(float aX, float aY, float aZ);
        void rotate(float aAngle, float aX, float aY, float aZ);

		inline static Matrix4x4  trs(Vector3 aPosition, Vector3 aRotation, Vector3 aScale)
		{
			Matrix4x4 matrix;
			matrix.setIdentity();
			//matrix.translatePostRotation(aPosition.x, aPosition.y, aPosition.z);
			matrix.setPosition(aPosition);
			matrix.scale(aScale.x, aScale.y, aScale.z);
			matrix.rotate(aRotation.x, 1.0f, 0.0f, 0.0f);
			matrix.rotate(aRotation.y, 0.0f, 1.0f, 0.0f);
			matrix.rotate(aRotation.z, 0.0f, 0.0f, 1.0f);
			
			
			
			return matrix;
		}

        Vector3 transformVector3(const Vector3 & aVec)
        {
            Vector4 result = transformVector4( Vector4(aVec.x, aVec.y, aVec.z, 1.0f));
            if(result.w)
                return Vector3(result.x/result.w, result.y/result.w, result.z/result.w);
            else
                return Vector3(result.x,result.y,result.z);
        }

        Vector4 transformVector4( const Vector4 & aVec)
        {
            return Vector4(m11 * aVec.x + m21 * aVec.y + m31 * aVec.z + m41 * aVec.w,
                           m12 * aVec.x + m22 * aVec.y + m32 * aVec.z + m42 * aVec.w,
                           m13 * aVec.x + m23 * aVec.y + m33 * aVec.z + m43 * aVec.w,
                           m14 * aVec.x + m24 * aVec.y + m44 * aVec.z + m44 * aVec.w );
        }

        void transpose()
        {
            float temp;
            temp = m12; m12 = m21; m21 = temp;
            temp = m13; m13 = m31; m31 = temp;
            temp = m14; m14 = m41; m41 = temp;
            temp = m23; m23 = m32; m32 = temp;
            temp = m24; m24 = m42; m42 = temp;
            temp = m34; m34 = m43; m43 = temp;
        }

        inline Matrix4x4 operator *(const float aValue) const
        {
            Matrix4x4 newMat;
            newMat.m11 = this->m11 * aValue;
            newMat.m12 = this->m12 * aValue;
            newMat.m13 = this->m13 * aValue;
            newMat.m14 = this->m14 * aValue;
            newMat.m21 = this->m21 * aValue;
            newMat.m22 = this->m22 * aValue;
            newMat.m23 = this->m23 * aValue;
            newMat.m24 = this->m24 * aValue;
            newMat.m31 = this->m31 * aValue;
            newMat.m32 = this->m32 * aValue;
            newMat.m33 = this->m33 * aValue;
            newMat.m34 = this->m34 * aValue;
            newMat.m41 = this->m41 * aValue;
            newMat.m42 = this->m42 * aValue;
            newMat.m43 = this->m43 * aValue;
            newMat.m44 = this->m44 * aValue;
            return newMat;
        }

        inline Matrix4x4 operator *(const Matrix4x4 aValue) const
        {
            Matrix4x4 newMat;

            newMat.m11 = this->m11 * aValue.m11 + this->m12 * aValue.m21 + this->m13 * aValue.m31 + this->m14 * aValue.m41;
            newMat.m12 = this->m11 * aValue.m12 + this->m12 * aValue.m22 + this->m13 * aValue.m32 + this->m14 * aValue.m42;
            newMat.m13 = this->m11 * aValue.m13 + this->m12 * aValue.m23 + this->m13 * aValue.m33 + this->m14 * aValue.m43;
            newMat.m14 = this->m11 * aValue.m14 + this->m12 * aValue.m24 + this->m13 * aValue.m34 + this->m14 * aValue.m44;
            newMat.m21 = this->m21 * aValue.m11 + this->m22 * aValue.m21 + this->m23 * aValue.m31 + this->m24 * aValue.m41;
            newMat.m22 = this->m21 * aValue.m12 + this->m22 * aValue.m22 + this->m23 * aValue.m32 + this->m24 * aValue.m42;
            newMat.m23 = this->m21 * aValue.m13 + this->m22 * aValue.m23 + this->m23 * aValue.m33 + this->m24 * aValue.m43;
            newMat.m24 = this->m21 * aValue.m14 + this->m22 * aValue.m24 + this->m23 * aValue.m34 + this->m24 * aValue.m44;
            newMat.m31 = this->m31 * aValue.m11 + this->m32 * aValue.m21 + this->m33 * aValue.m31 + this->m34 * aValue.m41;
            newMat.m32 = this->m31 * aValue.m12 + this->m32 * aValue.m22 + this->m33 * aValue.m32 + this->m34 * aValue.m42;
            newMat.m33 = this->m31 * aValue.m13 + this->m32 * aValue.m23 + this->m33 * aValue.m33 + this->m34 * aValue.m43;
            newMat.m34 = this->m31 * aValue.m14 + this->m32 * aValue.m24 + this->m33 * aValue.m34 + this->m34 * aValue.m44;
            newMat.m41 = this->m41 * aValue.m11 + this->m42 * aValue.m21 + this->m43 * aValue.m31 + this->m44 * aValue.m41;
            newMat.m42 = this->m41 * aValue.m12 + this->m42 * aValue.m22 + this->m43 * aValue.m32 + this->m44 * aValue.m42;
            newMat.m43 = this->m41 * aValue.m13 + this->m42 * aValue.m23 + this->m43 * aValue.m33 + this->m44 * aValue.m43;
            newMat.m44 = this->m41 * aValue.m14 + this->m42 * aValue.m24 + this->m43 * aValue.m34 + this->m44 * aValue.m44;
    
        return newMat;
        }

        bool inverse(float tolerance = 0.0001f)
        {
            // Determinants of 2x2 submatrices
	        float S0 = m11 * m22 - m12 * m21;
	        float S1 = m11 * m23 - m13 * m21;
	        float S2 = m11 * m24 - m14 * m21;
	        float S3 = m12 * m23 - m13 * m22;
	        float S4 = m12 * m24 - m14 * m22;
	        float S5 = m13 * m24 - m14 * m23;

	        float C5 = m33 * m44 - m34 * m43;
	        float C4 = m32 * m44 - m34 * m42;
	        float C3 = m32 * m43 - m33 * m42;
	        float C2 = m31 * m44 - m34 * m41;
	        float C1 = m31 * m43 - m33 * m41;
	        float C0 = m31 * m42 - m32 * m41;

	        // If determinant equals 0, there is no inverse
	        float det = S0 * C5 - S1 * C4 + S2 * C3 + S3 * C2 - S4 * C1 + S5 * C0;
	        if( fabs(det) <= tolerance )
                return false;

	        // Compute adjugate matrix
	        *this = Matrix4x4(
		         m22 * C5 - m23 * C4 + m24 * C3, -m12 * C5 + m13 * C4 - m14 * C3,
		         m42 * S5 - m43 * S4 + m44 * S3, -m32 * S5 + m33 * S4 - m34 * S3,

		        -m21 * C5 + m23 * C2 - m24 * C1,  m11 * C5 - m13 * C2 + m14 * C1,
		        -m41 * S5 + m43 * S2 - m44 * S1,  m31 * S5 - m33 * S2 + m34 * S1,

		         m21 * C4 - m22 * C2 + m24 * C0, -m11 * C4 + m12 * C2 - m14 * C0,
		         m41 * S4 - m42 * S2 + m44 * S0, -m31 * S4 + m32 * S2 - m34 * S0,

		        -m21 * C3 + m22 * C1 - m23 * C0,  m11 * C3 - m12 * C1 + m13 * C0,
		        -m41 * S3 + m42 * S1 - m43 * S0,  m31 * S3 - m32 * S1 + m33 * S0 ) * (1 / det);

	        return true;
        }
    private:

    };


    class Matrix3x3 : public Object
    {
    public:

        Matrix3x3()
        {
            setIdentity();
        }
        ~Matrix3x3(){};

        float m11, m12,m13;
        float m21, m22, m23;
        float m31, m32, m33;

        inline Vector3 operator* (Vector3 aVec) const
        {
            Vector3 result;

            result.x = m11 * aVec.x + m12 * aVec.y + m13 * aVec.z;
            result.y = m21 * aVec.x + m22 * aVec.y + m23 * aVec.z;
            result.z = m31 * aVec.x + m32 * aVec.y + m33 * aVec.z;

            return result;
        }

        void setIdentity();

        virtual Pointer<Reflection::Type> getType() override;

    };

}
GEM_CLASS(Matrix4x4,Object)
GEM_CLASS(Matrix3x3,Object)
#endif