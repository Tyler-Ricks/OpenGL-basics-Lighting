#include <glad.h> 
#include <glfw3.h>
#include <iostream>
#include "helpers.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

//screen properties
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//time stuff
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//object/light color stuff
glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
float incrementValue(float start, float increment) {
    start += increment;
    if (start > 1.0) {
        return(1.0f);
    }
    if (start < 0.0f) {
        return(0.0f);
    }
    return(start);
}

//cube rotation
glm::mat4 rotation = glm::mat4(1.0f); //To store total rotation
glm::mat4 newRotation = glm::mat4(1.0f);

//light moving stuff
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

//projection stuff
float fov = 45.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

    //cube rotation handling with WASD
    newRotation = glm::mat4(1.0f);
    float rotateSpeed = 40.0f * glm::radians(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        newRotation = glm::rotate(glm::mat4(1.0f), -rotateSpeed, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around global X-axis
        rotation = newRotation * rotation;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        newRotation = glm::rotate(glm::mat4(1.0f), rotateSpeed, glm::vec3(1.0f, 0.0f, 0.0f)); // Opposite global X-axis
        rotation = newRotation * rotation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        newRotation = glm::rotate(glm::mat4(1.0f), -rotateSpeed, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around global Y-axis
        rotation = newRotation * rotation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        newRotation = glm::rotate(glm::mat4(1.0f), rotateSpeed, glm::vec3(0.0f, 1.0f, 0.0f)); // Opposite global Y-axis
        rotation = newRotation * rotation;
    }

    //light cube controls with arrowkeys and leftAlt, spacebar
    float moveSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        lightPos += glm::vec3(0.0f, 0.0f, -moveSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        lightPos += glm::vec3(0.0f, 0.0f, moveSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        lightPos += glm::vec3(-moveSpeed, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        lightPos += glm::vec3(moveSpeed, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        lightPos += glm::vec3(0.0f, moveSpeed, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        lightPos += glm::vec3(0.0f, -moveSpeed, 0.0f);
    }

    //object/light color increment with some keys
    float increment = 1 / 2048.0f;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        objectColor.x = incrementValue(objectColor.x, increment);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        objectColor.x = incrementValue(objectColor.x, -increment);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        objectColor.y = incrementValue(objectColor.y, increment);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        objectColor.y = incrementValue(objectColor.y, -increment);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        objectColor.z = incrementValue(objectColor.z, increment);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        objectColor.z = incrementValue(objectColor.z, -increment);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        lightColor.x = incrementValue(lightColor.x, increment);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        lightColor.x = incrementValue(lightColor.x, -increment);
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        lightColor.y = incrementValue(lightColor.y, increment);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        lightColor.y = incrementValue(lightColor.y, -increment);
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        lightColor.z = incrementValue(lightColor.z, increment);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        lightColor.z = incrementValue(lightColor.z, -increment);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov += (float)yoffset;
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "failed to initialize glfw!" << std::endl;
        return(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "lighting", NULL, NULL);
    if (window == NULL) {
        std::cout << "window create failed" << std::endl;
        glfwTerminate();
        return(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //window resizing

    float cube_vertices[] = {
//  |   cube vertices   | Cube surfave normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int cube_VAO, cube_VBO;
    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &cube_VBO);

    glBindVertexArray(cube_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    //positon attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data for both cubes.
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    // set the vertex attribute 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader cubeShader("Shaders/vertex_shader.glsl", "Shaders/solid_shader.glsl");
    Shader lightShader("Shaders/vertex_shader.glsl", "Shaders/light_fragment.glsl");
    cubeShader.use();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    //render loop var declaractions
        
    //glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::mat4(1.0f);

    glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat3 normalMatrix;
    
    float time;
    float currentFrame;

    //render loop
    while (!glfwWindowShouldClose(window)) {
        //input checks
        processInput(window);

        //screen color and such
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        time = (float) glfwGetTime();
        currentFrame = time;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        model = glm::mat4(1.0f);

        //draw the cube object
        cubeShader.use();

        projection = glm::perspective(glm::radians(fov), (static_cast<float>(SCR_WIDTH)) / SCR_HEIGHT, 0.1f, 100.0f);
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);
        cubeShader.setVec3("viewPos", viewPos);

        
        model = rotation;
        cubeShader.setMat4("model", model);
        normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        cubeShader.setMat3("normalMatrix", normalMatrix);

        cubeShader.setVec3("lightPos", lightPos);
        cubeShader.setVec3("objectColor", objectColor);
        cubeShader.setVec3("lightColor", lightColor);

        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //draw the light source cube
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.25f));
        normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        lightShader.setMat3("normalMatrix", normalMatrix);
        lightShader.setMat4("model", model);
        lightShader.setVec3("lightColor", lightColor);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &cube_VBO);

    glfwTerminate();
    return 0;
}