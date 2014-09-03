#include "G_Matrix.h"
#include "G_Math.h"
#include "../Reflection/G_Reflection.h"

namespace Gem
{
    using namespace Reflection;

    Pointer<Reflection::Type> Matrix4x4::getType()
    {
        return typeOf("Matrix4x4");
    }


    void Matrix4x4::setIdentity()
    {
        m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0.0f;
        m11 = m22 = m33 = m44 = 1.0f;
    }
    void Matrix4x4::setPosition(Vector3 aPos)
    {
        m41 = aPos.x;
        m42 = aPos.y;
        m43 = aPos.z;
    }
    void Matrix4x4::setPosition(float aX, float aY, float aZ)
    {
        m41 = aX;
        m42 = aY;
        m43 = aZ;
    }

    void Matrix4x4::multiply(Matrix4x4 * aSrc)
    {
        Matrix4x4 tmp = *this;

        m11 = tmp.m11 * aSrc->m11  +  tmp.m12 * aSrc->m21  +  tmp.m13 * aSrc->m31  +  tmp.m14 * aSrc->m41;
        m12 = tmp.m11 * aSrc->m12  +  tmp.m12 * aSrc->m22  +  tmp.m13 * aSrc->m32  +  tmp.m14 * aSrc->m42;
        m13 = tmp.m11 * aSrc->m13  +  tmp.m12 * aSrc->m23  +  tmp.m13 * aSrc->m33  +  tmp.m14 * aSrc->m43;
        m14 = tmp.m11 * aSrc->m14  +  tmp.m12 * aSrc->m24  +  tmp.m13 * aSrc->m34  +  tmp.m14 * aSrc->m44;
                                                                                     
        m21 = tmp.m21 * aSrc->m11  +  tmp.m22 * aSrc->m21  +  tmp.m23 * aSrc->m31  +  tmp.m24 * aSrc->m41;
        m22 = tmp.m21 * aSrc->m12  +  tmp.m22 * aSrc->m22  +  tmp.m23 * aSrc->m32  +  tmp.m24 * aSrc->m42;
        m23 = tmp.m21 * aSrc->m13  +  tmp.m22 * aSrc->m23  +  tmp.m23 * aSrc->m33  +  tmp.m24 * aSrc->m43;
        m24 = tmp.m21 * aSrc->m14  +  tmp.m22 * aSrc->m24  +  tmp.m23 * aSrc->m34  +  tmp.m24 * aSrc->m44;

        m31 = tmp.m31 * aSrc->m11  +  tmp.m32 * aSrc->m21  +  tmp.m33 * aSrc->m31  +  tmp.m34 * aSrc->m41;
        m32 = tmp.m31 * aSrc->m12  +  tmp.m32 * aSrc->m22  +  tmp.m33 * aSrc->m32  +  tmp.m34 * aSrc->m42;
        m33 = tmp.m31 * aSrc->m13  +  tmp.m32 * aSrc->m23  +  tmp.m33 * aSrc->m33  +  tmp.m34 * aSrc->m43;
        m34 = tmp.m31 * aSrc->m14  +  tmp.m32 * aSrc->m24  +  tmp.m33 * aSrc->m34  +  tmp.m34 * aSrc->m44;

        m41 = tmp.m41 * aSrc->m11  +  tmp.m42 * aSrc->m21  +  tmp.m43 * aSrc->m31  +  tmp.m44 * aSrc->m41;
        m42 = tmp.m41 * aSrc->m12  +  tmp.m42 * aSrc->m22  +  tmp.m43 * aSrc->m32  +  tmp.m44 * aSrc->m42;
        m43 = tmp.m41 * aSrc->m13  +  tmp.m42 * aSrc->m23  +  tmp.m43 * aSrc->m33  +  tmp.m44 * aSrc->m43;
        m44 = tmp.m41 * aSrc->m14  +  tmp.m42 * aSrc->m24  +  tmp.m43 * aSrc->m34  +  tmp.m44 * aSrc->m44;
    }

    void Matrix4x4::scale(float aX, float aY, float aZ)
    {
        m11 *= aX; m12 *= aX; m13 *= aX; m14 *= aX;
        m21 *= aY; m22 *= aY; m23 *= aY; m24 *= aY;
        m31 *= aZ; m32 *= aZ; m33 *= aZ; m34 *= aZ;
    }

    void Matrix4x4::translatePreRotation(float aX, float aY, float aZ)
    {
        m41 += m11 * aX + m21 * aY + m31 * aZ;
        m42 += m12 * aX + m22 * aY + m32 * aZ;
        m43 += m13 * aX + m23 * aY + m33 * aZ;
        m44 += m14 * aX + m24 * aY + m34 * aZ;
    }

    void Matrix4x4::translatePostRotation(float aX, float aY, float aZ)
    {
        m41 += aX;
        m42 += aY;
        m43 += aZ;
    }

    void Matrix4x4::rotate(float angle, float x, float y, float z)
    {
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);
      
    sinAngle = sinf ( angle * PI / 180.0f );
    cosAngle = cosf ( angle * PI / 180.0f );
    if( mag > 0.0f )
    {
        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float oneMinusCos;
   
        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        Matrix4x4 rotMat;
        rotMat.m11 = (oneMinusCos * xx) + cosAngle;
        rotMat.m12 = (oneMinusCos * xy) - zs;
        rotMat.m13 = (oneMinusCos * zx) + ys;
        rotMat.m14 = 0.0f; 

        rotMat.m21 = (oneMinusCos * xy) + zs;
        rotMat.m22 = (oneMinusCos * yy) + cosAngle;
        rotMat.m23 = (oneMinusCos * yz) - xs;
        rotMat.m24 = 0.0f;

        rotMat.m31 = (oneMinusCos * zx) - ys;
        rotMat.m32 = (oneMinusCos * yz) + xs;
        rotMat.m33 = (oneMinusCos * zz) + cosAngle;
        rotMat.m34 = 0.0f; 

        rotMat.m41 = 0.0f;
        rotMat.m42 = 0.0f;
        rotMat.m43 = 0.0f;
        rotMat.m44 = 1.0f;

        this->multiply( &rotMat );
        }
    }

    void Matrix4x4::setFrustum(float left, float right, float bottom, float top, float nearZ, float farZ)
    {
        float deltaX = right - left;
        float deltaY = top - bottom;
        float deltaZ = farZ - nearZ;

        //assert( nearZ > 0.0f && farZ > 0.0f && deltaX > 0.0f && deltaY > 0.0f && deltaZ > 0.0f );

        m11 = 2.0f * nearZ / deltaX;
        m12 = m13 = m14 = 0.0f;

        m22 = 2.0f * nearZ / deltaY;
        m21 = m23 = m24 = 0.0f;

        m31 = (right + left) / -deltaX;
        m32 = (top + bottom) / -deltaY;
        m33 = (nearZ + farZ) / deltaZ;
        m34 = 1.0f;

        m43 = 2.0f * nearZ * farZ / -deltaZ;
        m41 = m42 = m44 = 0.0f;
    }

    void Matrix4x4::setPerspective(float fovy, float aspect, float nearZ, float farZ)
    {
        float frustumW, frustumH;
        //assert( fovy >= 1.0 && fovy <= 179);
        frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
        frustumW = frustumH * aspect;
    
        setFrustum( -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
    }
    void Matrix4x4::setLookAt(const Vector3 &eye, const Vector3 &up,const Vector3 &at)
    {
        Vector3 zaxis;
        Vector3 xaxis;
        Vector3 yaxis;
    
        zaxis = Vector3::subtract(at,eye);
        zaxis.normalize();

        //zaxis = (at - eye).Normalize();
        xaxis = Vector3::cross(up,zaxis);
        //xaxis = (up.cross(zaxis)).normalize();
        yaxis = zaxis.cross(xaxis); //.Cross(xaxis);
    
        *this = Matrix4x4( xaxis, yaxis, zaxis );
          m41 = -xaxis.dot(eye);
          m42 = -yaxis.dot(eye);
          m43 = -zaxis.dot(eye);
    }

    void Matrix3x3::setIdentity()
    {
        m12 = m13 = m21 = m23 = m31 = m32 = 0.0f;
        m11 = m22 = m33 = 0.0f;
    }

    Pointer<Reflection::Type> Matrix3x3::getType()
    {
        return typeOf("Matrix3x3");
    }
}