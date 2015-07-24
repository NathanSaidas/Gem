#include "Application.h"


#include <glew-1.10.0\include\GL\glew.h>
#include <GLFW_3.0.4\include\GLFW\glfw3.h>


#include "../Engine.h"
#include "../Graphics/Color.h"
#include "../Graphics/Geometry.h"
#include "../Graphics/GraphicEnums.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Shader.h"
#include "../Graphics/VertexAttributes.h"

//Error Handling
//#include "../Core/Debug.h"
//#include "../Core/Errors/CoreErrors.h"

#include "BaseWindow.h"

using namespace Gem::Debugging;
using namespace Gem::Reflection;

namespace Gem
{

    namespace Prototype
    {
        void ExecuteTest();
        void Terminate();
    }

#pragma region GLFW Callback Handling

    //List of private methods used in the Input class for processing events. These must be initialized by Application::Execute before being used.
    Method<Input, void, KeyCode>           PROCESS_KEY_DOWN_METHOD          = Method<Input, void, KeyCode>();
    Method<Input, void, KeyCode>           PROCESS_KEY_UP_METHOD            = Method<Input, void, KeyCode>();
    Method<Input, void, MouseButton>       PROCESS_MOUSE_BUTTON_DOWN_METHOD = Method<Input, void, MouseButton>();
    Method<Input, void, MouseButton>       PROCESS_MOUSE_BUTTON_UP_METHOD   = Method<Input, void, MouseButton>();
    Method<Input, void, Float32, Float32>  PROCESS_MOUSE_MOVE_METHOD        = Method<Input, void, Float32, Float32>();
    Method<Input, void, Float32>           PROCESS_MOUSE_SCROLL_METHOD      = Method<Input, void, Float32>();
    Method<Input, void, UInt32>            PROCESS_CHARACTER                = Method<Input, void, UInt32>();

    void OnMouseButtonEvent(GLFWwindow* aWindow, int button, int action, int mods)
    {
        Input* input = Input::GetInstance();
        if (GLFW_PRESS == action)
        {
            PROCESS_MOUSE_BUTTON_DOWN_METHOD(input, (MouseButton)button);
        }
        else if (GLFW_RELEASE)
        {
            PROCESS_MOUSE_BUTTON_UP_METHOD(input, (MouseButton)button);
        }
    }

    void OnMousePositionEvent(GLFWwindow* aWindow, double x, double y)
    {
        Input* input = Input::GetInstance();
        PROCESS_MOUSE_MOVE_METHOD(input, static_cast<Float32>(x), static_cast<Float32>(y));
    }

    void OnMouseEnter(GLFWwindow* aWindow, int entered)
    {
        if (entered == GL_TRUE)
        {

        }
        else if (entered == GL_FALSE)
        {

        }
    }

    void OnScroll(GLFWwindow* aWindow, double x, double y)
    {
        Input* input = Input::GetInstance();
        PROCESS_MOUSE_SCROLL_METHOD(input, static_cast<Float32>(y));
    }

    void OnKeyPress(GLFWwindow* aWindow, int key, int scancode, int action, int mods)
    {
        if (key != 340)
        {
            int i = 0;

        }

        Input* input = Input::GetInstance();
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            PROCESS_KEY_DOWN_METHOD(input, (KeyCode)key);
        }
        else if (action == GLFW_RELEASE)
        {
            PROCESS_KEY_UP_METHOD(input, (KeyCode)key);
        }
    }

    void OnCharPress(GLFWwindow* aWindow, UInt32 character)
    {
        Input* input = Input::GetInstance();
        PROCESS_CHARACTER(input, character);
    }
#pragma endregion


    RDEFINE_CLASS(Application, object)

    Application* Application::s_Instance = nullptr;

    Application* Application::GetInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new Application();
        }
        return s_Instance;
    }
    void Application::Terminate()
    {
        if (s_Instance != nullptr)
        {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    Application::Application()
    {

    }

    Application::~Application()
    {

    }
    SInt32 Application::Execute()
    {
        Application* app = GetInstance();

        //Initialize Subsystems (Memory, Reflection)
        Memory::MemoryManager::Initialize();
        Reflection::Runtime::Compile(nullptr);
        Time::Initialize();
        Input::Initialize();

        //Initialize Window Management API
        if (!glfwInit())
        {
            Debug::Error("GLFW", "Failed to initialize GLFW.");
            Memory::MemoryManager::Terminate();
            Reflection::Runtime::Terminate();
            Input::Terminate();
            return EXIT_FAILURE;
        }
        Input* input = Input::GetInstance();
        Type type = input->GetType();

        //Get Address of ProcessKeyDown function
        MethodInfo<Input, void, KeyCode>* processKeyDown = dynamic_cast<MethodInfo<Input, void, KeyCode>*>(type.GetMethodInfo("ProcessKeyDown"));
        if (processKeyDown != nullptr)
        {
            PROCESS_KEY_DOWN_METHOD = processKeyDown->GetMethod();
        }
        //Get Address of ProcessKeyUp function
        MethodInfo<Input, void, KeyCode>* processKeyUp = dynamic_cast<MethodInfo<Input, void, KeyCode>*>(type.GetMethodInfo("ProcessKeyUp"));
        if (processKeyUp != nullptr)
        {
            PROCESS_KEY_UP_METHOD = processKeyUp->GetMethod();
        }
        //Get Address of ProcessMouseDown function
        MethodInfo<Input, void, MouseButton>* processMouseButtonDown = dynamic_cast<MethodInfo<Input, void, MouseButton>*>(type.GetMethodInfo("ProcessMouseDown"));
        if (processMouseButtonDown != nullptr)
        {
            PROCESS_MOUSE_BUTTON_DOWN_METHOD = processMouseButtonDown->GetMethod();
        }
        //Get Address of ProcessMouseUp function
        MethodInfo<Input, void, MouseButton>* processMouseButtonUp = dynamic_cast<MethodInfo<Input, void, MouseButton>*>(type.GetMethodInfo("ProcessMouseUp"));
        if (processMouseButtonUp != nullptr)
        {
            PROCESS_MOUSE_BUTTON_UP_METHOD = processMouseButtonUp->GetMethod();
        }
        //Get Address of ProcessMouseMove function
        MethodInfo<Input, void, Float32, Float32>* processMouseMove = dynamic_cast<MethodInfo<Input, void, Float32, Float32>*>(type.GetMethodInfo("ProcessMouseMove"));
        if (processMouseMove != nullptr)
        {
            PROCESS_MOUSE_MOVE_METHOD = processMouseMove->GetMethod();
        }
        //Get Address of ProcessMouseScroll function
        MethodInfo<Input, void, Float32>* processMouseScroll = dynamic_cast<MethodInfo<Input, void, Float32>*>(type.GetMethodInfo("ProcessMouseScroll"));
        if (processMouseScroll != nullptr)
        {
            PROCESS_MOUSE_SCROLL_METHOD = processMouseScroll->GetMethod();
        }
        //Get Address of ProcessCharacter function
        MethodInfo<Input, void, UInt32>* processChar = dynamic_cast<MethodInfo<Input, void, UInt32>*>(type.GetMethodInfo("ProcessChar"));
        if (processChar != nullptr)
        {
            PROCESS_CHARACTER = processChar->GetMethod();
        }

        //Create the main window.
        app->m_MainWindow = MEM_POOL_ALLOC_T(BaseWindow, "Gem Framework");
        app->m_MainWindow->Open();
        
        glfwMakeContextCurrent((GLFWwindow*)app->m_MainWindow->GetHandle());

        if (glewInit() != GLEW_OK)
        {
            Debug::Error("Glew", "Failed to load glew.");
            Memory::MemoryManager::Terminate();
            Reflection::Runtime::Terminate();
            Input::Terminate();
            return EXIT_FAILURE;
        }


        //Game Loop
        while (!ShouldWindowClose(app->m_MainWindow))
        {
            Time::Update();
            Input::GetInstance()->ListenForFirstKey();
            glfwPollEvents();

            Prototype::ExecuteTest();

            app->m_MainWindow->SwapBuffer();
            Input::GetInstance()->Update();
            Memory::MemoryManager::GetInstance()->ResetFrame();
        }

        Prototype::Terminate();

        app->m_MainWindow->Close();
        MEM_POOL_DEALLOC_T(app->m_MainWindow, BaseWindow);

        glfwTerminate();

        //Set as empty methods before input cleanup.
        PROCESS_KEY_DOWN_METHOD             = Method<Input, void, KeyCode>();
        PROCESS_KEY_UP_METHOD               = Method<Input, void, KeyCode>();
        PROCESS_MOUSE_BUTTON_DOWN_METHOD    = Method<Input, void, MouseButton>();
        PROCESS_MOUSE_BUTTON_UP_METHOD      = Method<Input, void, MouseButton>();
        PROCESS_MOUSE_MOVE_METHOD   = Method<Input, void, Float32, Float32>();
        PROCESS_MOUSE_SCROLL_METHOD = Method<Input, void, Float32>();

        Input::Terminate();
        Reflection::Runtime::Terminate();
        Memory::MemoryManager::Terminate();
        system("pause");
        return EXIT_SUCCESS;
    }

    bool Application::ShouldWindowClose(BaseWindow* aWindow)
    {
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::ShouldWindowClose");
            error.Log("Gem");
            return false;
        }

        if (aWindow->GetHandle() != nullptr)
        {
            return glfwWindowShouldClose((GLFWwindow*)aWindow->GetHandle());
        }
        return false;
    }

    BaseWindow* Application::GetMainWindow()
    {
        return GetInstance()->m_MainWindow;
    }

    bool Application::OpenWindow(BaseWindow* aWindow)
    {
        //TODO(Nathan): Check if this is the main thread.
        //If this is not the main thread return false.

        //Exit: Missing reference
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::OpenWindow");
            error.Log("Gem");
            return false;
        }

        //Exit: Window is already open.
        if (aWindow->IsOpen())
        {
            return true;
        }

        //Set Window Hints
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RED_BITS, (SInt32)aWindow->GetRedBits());
        glfwWindowHint(GLFW_GREEN_BITS, (SInt32)aWindow->GetGreenBits());
        glfwWindowHint(GLFW_BLUE_BITS, (SInt32)aWindow->GetBlueBits());
        glfwWindowHint(GLFW_ALPHA_BITS, (SInt32)aWindow->GetAlphaBits());
        glfwWindowHint(GLFW_DEPTH_BITS, (SInt32)aWindow->GetDepthBits());
        glfwWindowHint(GLFW_STENCIL_BITS, (SInt32)aWindow->GetStencilBits());
        glfwWindowHint(GLFW_RESIZABLE, (SInt32)aWindow->IsResizeable());
        GLFWmonitor* monitor = nullptr;
        if (aWindow->IsFullscreen())
        {
            monitor = glfwGetPrimaryMonitor();
        }
        //Create the window.
        GLFWwindow* windowHandle = glfwCreateWindow(aWindow->GetWidth(), aWindow->GetHeight(), aWindow->GetName().c_str(), monitor, nullptr);
        if (windowHandle != nullptr)
        {
            //Register callbacks for the window to process input.
            glfwSetMouseButtonCallback(windowHandle, OnMouseButtonEvent);
            glfwSetCursorPosCallback(windowHandle, OnMousePositionEvent);
            glfwSetCursorEnterCallback(windowHandle, OnMouseEnter);
            glfwSetScrollCallback(windowHandle, OnScroll);
            glfwSetKeyCallback(windowHandle, OnKeyPress);
            glfwSetCharCallback(windowHandle, OnCharPress);

            //Use reflection to set the value of the handle to the windowHandle created.
            Type type = aWindow->GetType();
            Reflection::MemberInfo memberInfo = type.GetMember("m_Handle");
            void** member = (void**)memberInfo.GetOffsetPointer(aWindow);
            if (member != nullptr)
            {
                (*member) = windowHandle;
            }
            return true;
        }
        return false;
    }
    bool Application::CloseWindow(BaseWindow* aWindow)
    {
        //Validate the window object.

        //Exit: Missing reference
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::CloseWindow");
            error.Log("Gem");
            return false;
        }

        //Exit: Invalid Object
        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::CloseWindow");
            error.Log("Gem");
            return false;
        }

        //Notify the window about its destruction.
        aWindow->OnDestroy();
        //Release window resources.
        glfwDestroyWindow((GLFWwindow*)aWindow->GetHandle());
        //Set window handle to null with Reflection.
        Type type = aWindow->GetType();
        Reflection::MemberInfo memberInfo = type.GetMember("m_Handle");
        void** member = (void**)memberInfo.GetOffsetPointer(aWindow);
        if (member != nullptr)
        {
            (*member) = nullptr;
            return true;
        }
        return false;
    }
    bool Application::SetWindowSize(BaseWindow* aWindow)
    {
        //Exit: Missing reference
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::SetWindowSize");
            error.Log("Gem");
            return false;
        }

        //Exit: Invalid Object
        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::SetWindowSize");
            error.Log("Gem");
            return false;
        }
        GLFWwindow* window = (GLFWwindow*)aWindow->GetHandle();
        glfwSetWindowSize(window, (SInt32)aWindow->GetWidth(), (SInt32)aWindow->GetHeight());
        return true;
    }

    bool Application::ChangeFullscreenWindow(BaseWindow* aWindow)
    {
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::ChangeFullscreenWindow");
            error.Log("Gem");
            return false;
        }

        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::ChangeFullscreenWindow");
            error.Log("Gem");
            return false;
        }

        //To change a window to fullscreen it must be closed and re-opened with the fullscreen flag set.
        //The same applies vice-versa.
        CloseWindow(aWindow);
        OpenWindow(aWindow);
        return false;
    }

    void Application::SwapBuffer(BaseWindow* aWindow)
    {
        if (aWindow == nullptr)
        {
            ArgumentNull error = ArgumentNull("aWindow", GET_TRACE(0), "Application::SwapBuffer");
            error.Log("Gem");
            return;
        }

        if (!aWindow->IsOpen())
        {
            InvalidArgument error = InvalidArgument("aWindow", GET_TRACE(0), "Application::SwapBuffer");
            error.Log("Gem");
            return;
        }

        GLFWwindow* window = (GLFWwindow*)aWindow->GetHandle();
        glfwSwapBuffers(window);
    }


    //Write Test Code Here.
    namespace Prototype
    {
        struct Variables
        {
            Pointer<Mesh> mesh;
            Pointer<Shader> shader;
        };
        //Prototype Globals...
        bool g_Initialized = false;
        
        Variables* variables = nullptr;
        GLuint vertexBuffer;

        static const GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
        };

        Vector2 ConvertToClip(Float32 x, Float32 y)
        {
            Float32 width = Application::GetMainWindow()->GetWidth();
            Float32 height = Application::GetMainWindow()->GetHeight();

            Vector2 position = Vector2(x, y) - Vector2(width, height);
            position.x /= width;
            position.y /= height;
            return position;
        }
        


        //Initialize testing variables...
        void Initialize()
        {

            // get version info
            const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
            const GLubyte* version = glGetString(GL_VERSION); // version as a string
            Debug::LogFormat("Gem", nullptr, "Renderer: %s\n", renderer);
            Debug::LogFormat("Gem", nullptr, "OpenGL version supported %s\n", version);


            // Generate 1 buffer, put the resulting identifier in vertexbuffer
            glGenBuffers(1, &vertexBuffer);

            // The following commands will talk about our 'vertexbuffer' buffer
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

            // Give our vertices to OpenGL.
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


            variables = new Variables();

            Array<Vector3> positions;
            Array<Vector3> normals;
            Array<Vector2> texCoords;
            Array<Color> colors;
            Array<UInt16> indicies;
            //
            positions.Allocate(6);
            normals.Allocate(6);
            texCoords.Allocate(6);
            colors.Allocate(6);
            indicies.Allocate(6);
            //
            Float32 left = 0.0f;
            Float32 right = 200.0f;
            Float32 top = 0.0f;
            Float32 bottom = 350.0f;
            //

            Vector2 topLeft = ConvertToClip(left, top);
            Vector2 bottomRight = ConvertToClip(right, bottom);

            left = topLeft.x;
            right = bottomRight.y;
            top = topLeft.y;
            bottom = bottomRight.y;
            
            //positions[0] = Vector3(-1.0f, -1.0f, 0.0f); //topleft
            //positions[1] = Vector3(1.0f, -1.0f, 0.0f); //bottom right
            //positions[2] = Vector3(0.0f, 1.0f, 0.0f); //bottom left
            
            positions[0] = Vector3(left,    top, 0.0f); //topleft
            positions[1] = Vector3(right,   bottom, 0.0f); //bottom right
            positions[2] = Vector3(left,    bottom, 0.0f); //bottom left
            
            positions[3] = Vector3(left,    top, 0.0f); //top left
            positions[4] = Vector3(right,   top, 0.0f); //top right
            positions[5] = Vector3(right,   bottom, 0.0f); //bottom right
            //
            texCoords[0] = Vector2(0.0f, 0.0f); //topleft
            texCoords[1] = Vector2(1.0f, 1.0f);	//bottom right
            texCoords[2] = Vector2(0.0f, 1.0f);	//bottom left
            //
            texCoords[3] = Vector2(0.0f, 0.0f);	//top left
            texCoords[4] = Vector2(1.0f, 0.0f);	//top right
            texCoords[5] = Vector2(1.0f, 1.0f);	//bottom right
            //
            normals[0] = normals[1] = normals[2] = normals[3] = normals[4] = normals[5] = Vector3::Back();
            colors[0] = colors[1] = colors[2] = colors[3] = colors[4] = colors[5] = Color::White();
            //
            indicies[0] = 0;
            indicies[1] = 1;
            indicies[2] = 2;
            indicies[3] = 3;
            indicies[4] = 4;
            indicies[5] = 5;
            //
            variables->mesh->SetPositions(positions);
            variables->mesh->SetNormals(normals);
            variables->mesh->SetTexCoords(texCoords);
            variables->mesh->SetColors(colors);
            variables->mesh->SetIndices(indicies);
            if (!variables->mesh->Upload())
            {
                Debug::Error("Graphics", "Failed to upload mesh plane to GPU");
            }

            //
            Directory currentDir = Directory::GetCurrent();
            currentDir.Back(2);
            if (!currentDir.Change("Assets/Shaders/"))
            {
                Debug::Error("Gem", "Invalid change path.");
            }
            else
            {
                std::string path = currentDir.GetPath();
                path.append("Prototype.glsl");
                variables->shader->Load(path);
                variables->shader->SetName(path);
            }
            //
            //
            if (!variables->mesh->IsUploaded())
            {
                Debug::Error("Gem", "Failed to load mesh.");
            }
            //
            if (!variables->shader->IsUploaded())
            {
                Debug::Error("Gem", "Failed to load shader.");
            }
            else
            {
                Debug::LogFormat("Gem", nullptr, "Loaded shader %s", variables->shader->GetName().c_str());
            }
        }

        void Terminate()
        {
            delete variables;
        }

        //Excute the test.
        void ExecuteTest()
        {
            if (!g_Initialized)
            {
                g_Initialized = true;
                Initialize();
            }

            //glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(0);

            // 1rst attribute buffer : vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );
            
            // Draw the triangle !
            glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
            
            glDisableVertexAttribArray(0);
            
            
            
            

            
            //Use this shader for drawing...
            variables->shader->UseShader();
            
            //Bind Buffers....
            glBindBuffer((GLenum)BufferTarget::Array, variables->mesh->GetVBO());
            glBindBuffer((GLenum)BufferTarget::ElementArray, variables->mesh->GetIBO());
            
            
            //Get Vertex Attribute Locations...
            GLint a_Position = variables->shader->GetAttributeLocation("a_Position");
            GLint a_Color = variables->shader->GetAttributeLocation("a_Color");
            
            
            //Linking/Enabling Vertex Attributes for Drawing
            //glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (GLvoid*)offsetof(s, m));
            //glEnableVertexAttribArray(loc);
            
            glVertexAttribPointer(a_Position, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)offsetof(VertexAttribute,position));
            glEnableVertexAttribArray(a_Position);
            
            glVertexAttribPointer(a_Color, 4, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), VERTEX_ATTRIB_COLOR);
            glEnableVertexAttribArray(a_Color);
            
            //Official Draw Call...
            glDrawElements((GLenum)PrimitiveMode::Triangles, variables->mesh->GetIndexCount(), GL_UNSIGNED_SHORT, nullptr);

            //glDrawArrays(GL_TRIANGLES, 0, 3);
            
            glDisableVertexAttribArray(a_Position);
            glDisableVertexAttribArray(a_Color);
            glBindBuffer((GLenum)BufferTarget::Array, 0);
            glBindBuffer((GLenum)BufferTarget::ElementArray, 0);


            //Req: Shader
            //Req: VBO/IBO
            //Req: 

            Float32 deltaTime = Time::GetDeltaTime();
            Float32 time = Time::GetTime();

        }
    }
}