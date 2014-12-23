#ifndef G_WINDOW_HOOK_H
#define G_WINDOW_HOOK_H


#include "../Math/G_Math.h"
#include "../G_Object.h"
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

    class WindowHook : public object
    {
		G_CLASS_DEF(WindowHook)
    public:
        WindowHook();
        virtual ~WindowHook();
        //Gets invoked by the window
        virtual void Update();


    protected:
        bool IsFocused();
		virtual Vector2 GetWindowSize();
        virtual void OnAttachToWindow(int aHandle);
        virtual void OnDetachFromWindow(int aHandle);
		virtual void OnWindowResize(float aWidth, float aHeight);
    private:
        Window * m_Window;
        friend class WindowManager;

    };
}
#endif