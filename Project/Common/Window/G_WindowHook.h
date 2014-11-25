#ifndef G_WINDOW_HOOK_H
#define G_WINDOW_HOOK_H

#include "../Primitives/G_PrimitiveObjects.h"
#include "../Math/G_Math.h"
#include <vector>

namespace Gem
{
    class Window;
	///Graphics Debugging / Testing
	class Mesh;
	class Texture;
	class Shader;
	class DualMesh;
	class RenderableObject;
	class Camera;
	///----------------------------

    class WindowHook : public Object
    {
    public:
        WindowHook();
        virtual ~WindowHook();
        //Gets invoked by the window
        virtual void Update();

        virtual Pointer<Reflection::Type> GetType() override;

    protected:
        bool IsFocused();
		virtual Vector2 GetWindowSize();
        virtual void OnAttachToWindow(int aHandle);
        virtual void OnDetachFromWindow(int aHandle);
		virtual void OnWindowResize(float aWidth, float aHeight);
    private:
        Window * m_Window;
        friend class WindowManager;

		///Temp stuff for adding objects
		RenderableObject * AddObject(MemoryHandle<Mesh> aMesh, MemoryHandle<Texture> aTexture, MemoryHandle<Shader> aShader);
		std::vector<MemoryHandle<RenderableObject>> m_Objects;

		///Temp Stuff for graphics
		MemoryHandle<Camera> m_Camera;
		MemoryHandle<Shader> m_SimpleBlur;
		MemoryHandle<Mesh> m_Plane;

    };
}
GEM_CLASS(WindowHook,Object)
#endif