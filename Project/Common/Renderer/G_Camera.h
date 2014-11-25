#ifndef G_CAMERA_H
#define G_CAMERA_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "../Math/G_Math.h"
#include "G_FrameBuffer.h"

namespace Gem
{
    class Camera : public Object
    {
    public:
		static Camera * CurrentCamera();
        //static TestTemplate<Camera> & cam;

        //template<class T>                                                                                                                   
        //class TestReg;                                                                                                                      
        //template<>                                                                                                                          
        //class TestReg < Camera >                                                                                                              
        //{  
        //public:
        //    static const Gem::Reflection::RegistryEntry<Camera> & reg;
        //};                                                                                                                                
        //const Gem::Reflection::RegistryEntry<Camera> & TestReg<Camera>::reg = Gem::Reflection::RegistryEntry<Camera>::Instance("Camera", "Object");
		int samples;
		Vector2 blurDirection;
        
        Camera();
        ~Camera();
		void SetFBO(float aWidth, float aHeight);
		unsigned int GetRenderTexture();
		unsigned int GetDepthTexture();

        void Render();

		///Render Order / Culling
		string Name();
		int CullingMask();
		int Depth();
		bool ClearDepth();
		string Name(string & aName);
		int CullingMask(int aCullingMask);
		int Depth(int aDepth);
		bool ClearDepth(bool aFlag);

		///View Setup
		ClassUtils::Vector3 Position();
		ClassUtils::Vector3 Orientation();
		ClassUtils::Vector3 Target();
		ClassUtils::Vector3 Position(ClassUtils::Vector3 & aPosition);
		ClassUtils::Vector3 Orientation(ClassUtils::Vector3 & aRotation);
		ClassUtils::Vector3 Target(ClassUtils::Vector3 & aScale);
		ClassUtils::Matrix View();
		///Projection Setup
		ClassUtils::Matrix Projection();
		void SetProjection(float aFov, float aAspect, float aNear, float aFar);
		Rect WindowRect();
		Rect WindowRect(Rect & aWindowRec);
    private:
		static Camera * s_CurrentCamera;
		///The name of the camera for lookups
        string m_Name;
		///The objects mask the camera renders
		int m_CullingMask;
		///The depth the camera is in
		int m_Depth;
		///A flag to let the camera know whether or not to clear the depth each pass
		bool m_ClearDepth;
		///The cameras position in world space
		ClassUtils::Vector3 m_Position;
		///The cameras orientation in world space
		ClassUtils::Vector3 m_Orientation;
		///The cameras look at target in world space
		ClassUtils::Vector3 m_Target;
		///The projection matrix for the camera
		ClassUtils::Matrix m_Projection;
		
		///The area to render in.
		Rect m_WindowRect;

		MemoryHandle<FrameBuffer> m_FrameBuffer;

		void PostProcessRender();

    };
}
GEM_CLASS(Camera, Object)

#endif