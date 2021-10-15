#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

GLFWwindow* window;

int main(void)
{
    
	// Initialise GLFW
    glewExperimental = true;
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
		return -1;
	}
    glfwMakeContextCurrent(window);
    
    glewExperimental=true;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    //Set escape key input
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    

    
    //Projection matrix: 45˚ field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //int width = 4;
    //int height = 3;
    //glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
    
    //Camera matrix
    //glm::mat4 View = glm::lookAt(
    //                             glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    
    /*glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(2.0f, 0.0f, 0.0f));
    glm::mat4 rotationMatrix = eulerAngleXYZ(0.0f,90.0f,0.0f);
    glm::mat4 scalingMatrix = glm::scale(glm::vec3(4.0f, 4.0f, 4.0f));
    
    glm::mat4 Model = rotationMatrix * translationMatrix * scalingMatrix * glm::mat4(1.0f);*/
    //glm::mat4 Model = glm::mat4(1.0f);
    //glm::mat4 mvp = Projection * View * Model;
    
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
    
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 0.000004f,
        0.000103f, 0.336048f,
        0.335973f, 0.335903f,
        1.000023f, 0.000013f,
        0.667979f, 0.335851f,
        0.999958f, 0.336064f,
        0.667979f, 0.335851f,
        0.336024f, 0.671877f,
        0.667969f, 0.671889f,
        1.000023f, 0.000013f,
        0.668104f, 0.000013f,
        0.667979f, 0.335851f,
        0.000059f, 0.000004f,
        0.335973f, 0.335903f,
        0.336098f, 0.000071f,
        0.667979f, 0.335851f,
        0.335973f, 0.335903f,
        0.336024f, 0.671877f,
        1.000004f, 0.671847f,
        0.999958f, 0.336064f,
        0.667979f, 0.335851f,
        0.668104f, 0.000013f,
        0.335973f, 0.335903f,
        0.667979f, 0.335851f,
        0.335973f, 0.335903f,
        0.668104f, 0.000013f,
        0.336098f, 0.000071f,
        0.000103f, 0.336048f,
        0.000004f, 0.671870f,
        0.336024f, 0.671877f,
        0.000103f, 0.336048f,
        0.336024f, 0.671877f,
        0.335973f, 0.335903f,
        0.667969f, 0.671889f,
        1.000004f, 0.671847f,
        0.667979f, 0.335851f
    };

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    
    //GLuint Texture = loadBMP_custom("uvtemplate.bmp");
    GLuint Texture = loadDDS("uvtemplate.DDS");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    
    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(TextureID, 0);
        
        //1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glEnableVertexAttribArray(vertexUVID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
                              vertexUVID,                   // The attribute we want to configure
                              2,                            // size : U+V => 2
                              GL_FLOAT,                     // type
                              GL_FALSE,                     // normalized?
                              0,                            // stride
                              (void*)0                      // array buffer offset
                              );

        
        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexUVID);
        
        //Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &TextureID);
    
    glfwTerminate();
}

