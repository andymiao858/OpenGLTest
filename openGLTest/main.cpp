//
//  main.cpp
//  openGLTest
//
//  Created by miaohuadong on 2019/5/22.
//  Copyright © 2019 miaohuadong. All rights reserved.
//

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace std;

int SCR_WIDTH = 480;
int SCR_HEIGHT = 360;

#define BMP_Header_Length 54
#define WindowWidth  SCR_WIDTH
#define WindowHeight SCR_HEIGHT


const char* vertex_shader = R"(
    #version 330 core
    layout (location = 0) in vec4 a_position;
    layout (location = 1) in vec3 color;
    // Values that stay constant for the whole mesh.
    uniform mat4 u_worldViewProjectionMatrix;

    out vec3 vertColor;

    void main(){
        gl_Position =  u_worldViewProjectionMatrix * a_position;
        vertColor = color;
    }
)";

const char* fragment_shader = R"(
    #version 330 core
    out vec4 fragColor;
    in vec3 vertColor;
    void main() {
        fragColor = vec4(vertColor, 1.0);
    }
)";

const char* pass_vert = R"(
    #version 330 core
    layout (location = 0) in vec2 a_position;
    layout (location = 1) in vec2 a_texCoord;

    out vec2 v_texCoord;

    void main() {
        gl_Position = vec4(a_position, 0.0, 1.0);
        v_texCoord = a_texCoord;
    }
)";

const char* pass_frag = R"(
    # version 330 core
    in vec2 v_texCoord;
    out vec4 fragColor;

    uniform sampler2D sampler;

    void main() {
        vec3 color = texture(sampler, v_texCoord).rgb;
        fragColor = vec4(color, 1.0);
    }
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLuint createShader(const char* vertShader, const char* fragShader) {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertShader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragShader, NULL);
    glCompileShader(fs);
    
    
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    
    glLinkProgram(shader_programme);
    return shader_programme;
}

#if 0
void grab(void)
{
    FILE*    pDummyFile;
    FILE*    pWritingFile;
    GLubyte* pPixelData;
    GLubyte  BMP_Header[BMP_Header_Length];
    GLint    i, j;
    GLint    PixelDataLength;
    
    
    i = WindowWidth * 3;
    while( i%4 != 0 )
        ++i;
    PixelDataLength = i * WindowHeight;
    
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if( pPixelData == 0 )
        exit(0);
    
    pDummyFile = fopen("bitmap1.bmp", "rb");
    if( pDummyFile == 0 )
        exit(0);
    
    pWritingFile = fopen("grab.bmp", "wb");
    if( pWritingFile == 0 )
        exit(0);
    
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = WindowWidth;
    j = WindowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);


    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight,
                 GL_BGR, GL_UNSIGNED_BYTE, pPixelData);
    
    
    fseek(pWritingFile, 0, SEEK_END);
  
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);
    
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}
#endif


void checkGLError()
{
    GLenum err;
    while( !(err = glGetError()) ){
        std::cout << err;
    }
}

int main() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // uncomment these lines if on Apple OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    /* OTHER STUFF GOES HERE NEXT */
    float points[] = {
        -0.5f, -0.5f,  0.0f, 1, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,  0.0f,  1, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.0f, 1,  0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.0f, 1,  1.0f, 1.0f, 0.0f
        
    };
    
    float border[] = {
        -0.1f, -0.1f,  0.0f, 1, 0.0f, 0.0f, 0.0f,
        -0.1f, 0.1f,  0.0f,  1, 0.0f, 0.0f, 0.0f,
        0.1f, -0.1f,  0.0f, 1,  0.0f, 0.0f, 0.0f,
        0.1f,  0.1f,  0.0f, 1,  0.0f, 0.0f, 0.0f
    };
    
    float posTexCoord[] = {
        -1.0f, -1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f,    0.0f, 0.0f,
        1.0f, -1.0f,    1.0f, 1.0f,
        1.0f, 1.0f,     1.0f, 0.0f,
    };
    
    GLuint vao[2];
    GLuint vbo1;
    glGenVertexArrays(2, vao);
    glGenBuffers(1, &vbo1);
    
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    GLuint vbo2;

    glGenBuffers(1, &vbo2);
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(border), &border, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (4 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    GLuint vao3;
    GLuint vbo3;
    glGenVertexArrays(1, &vao3);
    glGenBuffers(1, &vbo3);
    
    glBindVertexArray(vao3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posTexCoord), &posTexCoord, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    
    GLuint shader_programme = createShader(vertex_shader, fragment_shader);
    GLint matrixId = glGetUniformLocation(shader_programme, "u_worldViewProjectionMatrix");
    
    glUseProgram(shader_programme);
    
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, 0.0f, 10.0f);
    glm::mat4 matrix = proj * view * model;
    
    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &matrix[0][0]);
    
    GLuint pass_programme = createShader(pass_vert, pass_frag);
    GLint sampler = glGetUniformLocation(pass_programme, "sampler");
    
    glUseProgram(pass_programme);
    glUniform1i(sampler, 0);
    
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR FRAMEBUFFER!" << endl;
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    GLint m_viewport[4];
    glGetIntegerv( GL_VIEWPORT, m_viewport );
    printf("viewport (%d, %d)\n", m_viewport[2], m_viewport[3]);
    
    checkGLError();
    
    while(!glfwWindowShouldClose(window)) {
        
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(0);
        glClearStencil(0);
        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);
        glUseProgram(shader_programme);
        
        glStencilFunc(GL_ALWAYS, 1, 0xff);
        glStencilMask(0xff);
        glBindVertexArray(vao[1]);


        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xff);
        glStencilMask(0x00);
        glBindVertexArray(vao[0]);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glBindVertexArray(0);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_STENCIL_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(pass_programme);

        glBindVertexArray(vao3);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
//        glReadBuffer(GL_FRONT);
//        grab();

        glBindVertexArray(0);
        
        // update other events like input handling
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(window);
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    cout << "framebuffer size: (" << width << "," << height << ")" << endl;
}
