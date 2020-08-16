#include <iostream>
#include <array>


#if 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "Shader.h"
#include "texture.h"
#include "vbo.h"
#include "voa.h"
#include "Camera.h"

#include "GLCall.h"
#include "Block.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera 
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timings
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// function declarations 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void drawFace(Texture* faceTextures[7], Colors C, VertexArrayObject& vao, bool drawVoid);
void drawBlock(Texture* faceTextures[7], Block B, VertexArrayObject* VAOS[6]);   

namespace coords {

    float top[]{
         -0.25f,  0.25f, -0.25f,  0.0f, 1.0f,
          0.25f,  0.25f, -0.25f,  1.0f, 1.0f,
          0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
          0.25f,  0.25f,  0.25f,  1.0f, 0.0f,
         -0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
         -0.25f,  0.25f, -0.25f,  0.0f, 1.0f
    };

    float bot[]{
         -0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
          0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
          0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
          0.25f, -0.25f,  0.25f,  1.0f, 0.0f,
         -0.25f, -0.25f,  0.25f,  0.0f, 0.0f,
         -0.25f, -0.25f, -0.25f,  0.0f, 1.0f,
    };

    float left[]{
         -0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
         -0.25f,  0.25f, -0.25f,  1.0f, 0.0f,
         -0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
         -0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
         -0.25f, -0.25f,  0.25f,  0.0f, 1.0f,
         -0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
    };

    float right[]{
          0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
          0.25f,  0.25f, -0.25f,  1.0f, 0.0f,
          0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
          0.25f, -0.25f, -0.25f,  1.0f, 1.0f,
          0.25f, -0.25f,  0.25f,  0.0f, 1.0f,
          0.25f,  0.25f,  0.25f,  0.0f, 0.0f,
    };

    float front[]{
         -0.25f, -0.25f,  0.25f,   0.0f,  0.0f,
          0.25f, -0.25f,  0.25f,  -1.0f,  0.0f,
          0.25f,  0.25f,  0.25f,  -1.0f, -1.0f,
          0.25f,  0.25f,  0.25f,  -1.0f, -1.0f,
         -0.25f,  0.25f,  0.25f,   0.0f, -1.0f,
         -0.25f, -0.25f,  0.25f,   0.0f,  0.0f,
    };

    float back[]{ -
          0.25f, -0.25f, -0.25f,   0.0f,  0.0f,
          0.25f, -0.25f, -0.25f,  -1.0f,  0.0f,
          0.25f,  0.25f, -0.25f,  -1.0f, -1.0f,
          0.25f,  0.25f, -0.25f,  -1.0f, -1.0f,
         -0.25f,  0.25f, -0.25f,   0.0f, -1.0f,
         -0.25f, -0.25f, -0.25f,   0.0f,  0.0f,
    };
}

int main()
{   
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window{ "Rcube", SCR_WIDTH, SCR_HEIGHT, true };

    // tell GLFW to capture our mouse
    window.setFramebufferSizeCallback(framebuffer_size_callback);
    window.setCursorPosCallback(mouse_callback);
    window.setMouseScrollCallback(scroll_callback);
    window.setInputMode(disabled);

    // glad: load all OpenGL function pointers
   // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
// -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader shader("vertex.shader", "fragment.shader");
    shader.use();
    shader.setInt("texture1", 0); 

    // create vaos and vbos for all faces // textures
    // ------------------------------------
    Texture::vFlip(false);
    VertexArrayObject vaoTop{ 2 }; 
    VertexBufferObject vboTop{ coords::top, sizeof(coords::top), GL_STATIC_DRAW };
    vaoTop.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); 
    vaoTop.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    VertexArrayObject vaoBot{ 2 };
    VertexBufferObject vboBot{ coords::bot, sizeof(coords::bot), GL_STATIC_DRAW };
    vaoBot.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vaoBot.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    VertexArrayObject vaoLeft{ 2 };
    VertexBufferObject vboLeft{ coords::left, sizeof(coords::left), GL_STATIC_DRAW };
    vaoLeft.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vaoLeft.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    VertexArrayObject vaoRight{ 2 };
    VertexBufferObject vboRight{ coords::right, sizeof(coords::right), GL_STATIC_DRAW };
    vaoRight.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vaoRight.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    VertexArrayObject vaoFront{ 2 };
    VertexBufferObject vboFront{ coords::front, sizeof(coords::front), GL_STATIC_DRAW };
    vaoFront.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vaoFront.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    VertexArrayObject vaoBack{ 2 };
    VertexBufferObject vboBack{ coords::back, sizeof(coords::back), GL_STATIC_DRAW };
    vaoBack.setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vaoBack.setAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    Texture blue;
    blue.bind(GL_TEXTURE_2D);
    blue.generateTexture("blueface.png", GL_RGBA);
    //blue.generateTexture("r1.png", GL_RGBA);
    Texture green;
    green.bind(GL_TEXTURE_2D);
    green.generateTexture("greenface.png", GL_RGBA);
    //green.generateTexture("r2.png", GL_RGBA);
    Texture orange;
    orange.bind(GL_TEXTURE_2D);
    orange.generateTexture("orangeface.png", GL_RGBA);
    //orange.generateTexture("r3.png", GL_RGBA);
    Texture red;
    red.bind(GL_TEXTURE_2D);
    red.generateTexture("redface.png", GL_RGBA);
    //red.generateTexture("r4.png", GL_RGBA);
    Texture white;
    white.bind(GL_TEXTURE_2D);
    white.generateTexture("whiteface.png", GL_RGBA);
    //white.generateTexture("r5.png", GL_RGBA);
    Texture yellow;
    yellow.bind(GL_TEXTURE_2D);
    yellow.generateTexture("yellowface.png", GL_RGBA);
    //yellow.generateTexture("r6.png", GL_RGBA);
    Texture black;
    black.bind(GL_TEXTURE_2D);
    black.generateTexture("void.png", GL_RGBA);

    Texture* faceTextures[7] = { &blue, &green, &orange, &red, &white, &yellow, &black };
    VertexArrayObject* VAOS[6] = { &vaoTop, &vaoBot, &vaoLeft, &vaoRight, &vaoFront, &vaoBack }; 

    Rcube<12>* rubikcube = new Rcube<12>();
    int num = 11;

    struct MovData {
        bool turn;
        Direction D; 
    };


    MovData fb[] = { {true, countercw}, {false, countercw}, {true, clockwise}, {false, countercw} };
    MovData rb[] = { {false, countercw}, {true, clockwise}, {false, clockwise}, {true, countercw} };
    int movIndex = 0; 

    /* Loop until the user closes the window */
    while (!window.getWindowShouldClose())
    {
        movIndex %= 4; 
        (*rubikcube).r = rb[movIndex].turn;
        (*rubikcube).cF = fb[movIndex].turn;

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window.getWindow());

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate shader
        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        for (int i = 0; i < (*rubikcube).cube.size; i++) {
            for (int j = 0; j < (*rubikcube).cube.size; j++) {
                for (int k = 0; k < (*rubikcube).cube.size; k++) {
                    if ((*rubikcube).cube[{i, j, k}].draw) {
                        glm::mat4 model = glm::mat4(1.0f); 
                        if ((*rubikcube).cF) {
                            rubikcube->columnF(i, num, model, fb[movIndex].D);
                        }
                        else if ((*rubikcube).cS) {
                            rubikcube->columnS(k, num, model, clockwise);
                        }
                        else if ((*rubikcube).r){
                            rubikcube->row(j, num, model, rb[movIndex].D);
                        }
                        model = glm::translate(model, glm::vec3(i * .5, j * .5, k * -.5));
                        shader.setMat4("model", model);
                        drawBlock(faceTextures, (*rubikcube).cube[{i, j, k}], VAOS); 
                    }
                }
            }
        }
        
        if(!(*rubikcube).current) movIndex++;
        (*rubikcube).updateAngle();

        /* Swap front and back buffers */
        window.swapBuffers();

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 5.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //cameraPos += cameraSpeed * cameraFront;
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        //cameraPos -= cameraSpeed * cameraFront;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        //cameraPos -= glm::normalize(cameraUp) * cameraSpeed;
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        //cameraPos += glm::normalize(cameraUp) * cameraSpeed;
        camera.ProcessKeyboard(UP, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
#endif

// select correct texture to bind and draw face
void drawFace(Texture* faceTextures[7], Colors C, VertexArrayObject& vao, bool drawVoid) {
    switch (C) {
        case BLUE:
            (*faceTextures[0]).bind(GL_TEXTURE_2D); 
            break;
        case GREEN:
            (*faceTextures[1]).bind(GL_TEXTURE_2D);
            break;
        case ORANGE:
            (*faceTextures[2]).bind(GL_TEXTURE_2D);
            break;
        case RED:
            (*faceTextures[3]).bind(GL_TEXTURE_2D);
            break;
        case WHITE:
            (*faceTextures[4]).bind(GL_TEXTURE_2D);
            break;
        case YELLOW:
            (*faceTextures[5]).bind(GL_TEXTURE_2D);
            break;
        case VOID:
            if (drawVoid) {
                (*faceTextures[6]).bind(GL_TEXTURE_2D);
            }
            else {
                return;
            }
            break;
        default:
            std::cout << "ERROR DRawFACE" << std::endl; 
            return;
    }
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
// draw block
void drawBlock(Texture* faceTextures[7], Block B, VertexArrayObject* VAOS[6]) {
    glActiveTexture(GL_TEXTURE0);
    drawFace(faceTextures, B.colors[0], *VAOS[0], B.drawVoid);
    drawFace(faceTextures, B.colors[1], *VAOS[1], B.drawVoid);
    drawFace(faceTextures, B.colors[2], *VAOS[2], B.drawVoid);
    drawFace(faceTextures, B.colors[3], *VAOS[3], B.drawVoid);
    drawFace(faceTextures, B.colors[4], *VAOS[4], B.drawVoid);
    drawFace(faceTextures, B.colors[5], *VAOS[5], B.drawVoid);
}


