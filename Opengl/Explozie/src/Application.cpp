#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
struct Particle {
    float x, y;
    float vx, vy;
    float r, g, b;
    float lifetime;
};

void createParticle(Particle& particle, float x, float y);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength);

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
static clock_t timer = 0;
static clock_t timerCheck = 0;
static bool explosion = false;
int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

    glfwSetKeyCallback(window, keyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    srand(static_cast<unsigned int>(time(nullptr)));

    Particle particles[100];
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, screenWidth, screenHeight); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment

    GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
    GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Render OpenGL here

   
        if (explosion != true)
        {
            glPushMatrix();
            glTranslatef(halfScreenWidth, halfScreenHeight, -500);
            glRotatef(rotationX, 1, 0, 0);
            glRotatef(rotationY, 0, 1, 0);
            glTranslatef(-halfScreenWidth, -halfScreenHeight, 500);
            DrawCube(halfScreenWidth, halfScreenHeight, -500, 200);
        }
        else
        {
            
            
            for (int i = 0; i < 100; i++) {
                if (particles[i].lifetime <= 0) {
                    if (i % 2 == 0)
                    {
                        createParticle(particles[i], halfScreenWidth + i/10, halfScreenHeight - i/10);
                    }
                    else
                    {
                        createParticle(particles[i], halfScreenWidth - i/10, halfScreenHeight + i/10);
                    }
                    
                }

                particles[i].x += particles[i].vx * i/10;
                particles[i].y += particles[i].vy *i/10;
                particles[i].lifetime -= 0.01f;

                glColor3f(particles[i].r, particles[i].g, particles[i].b);
                glBegin(GL_TRIANGLES);
                glVertex2f(particles[i].x - 50.0f, particles[i].y - 50.0f);
                glVertex2f(particles[i].x, particles[i].y +50.0f);
                glVertex2f(particles[i].x + 50.0f, particles[i].y - 50.0f);
                glEnd();
            }
            timerCheck = clock() - timer;
            if ((float)timerCheck / CLOCKS_PER_SEC > 2)
            {
                explosion = false;
            }
           
        }
        

        glPopMatrix();


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void createParticle(Particle& particle, float x, float y) {
    particle.x = x;
    particle.y = y;
    particle.vx = ((rand() % 200) - 100) / 100.0f;
    particle.vy = ((rand() % 200) - 100) / 100.0f;
    particle.r = 1.0f;
    particle.g = (rand() % 100) / 100.0f;
    particle.b = 0.0f;
    particle.lifetime = 1.0f;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << key << std::endl;

    const GLfloat rotationSpeed = 10;

    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_UP:
            rotationX -= rotationSpeed;
            break;
        case GLFW_KEY_DOWN:
            rotationX += rotationSpeed;
            break;
        case GLFW_KEY_RIGHT:
            rotationY += rotationSpeed;
            break;
        case GLFW_KEY_LEFT:
            rotationY -= rotationSpeed;
            break;
        case GLFW_KEY_ENTER:
            explosion = true;
            timer = clock();
            break;
        }


    }
}


void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength)
{
    GLfloat halfSideLength = edgeLength * 0.5f;

    GLfloat vertices[] =
    {
        // front face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // back face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom left

        // left face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // right face
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // bottom left

        // top face
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY + halfSideLength, centerPosZ + halfSideLength, // bottom left

        // top face
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength, // top left
        centerPosX - halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // top right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ - halfSideLength, // bottom right
        centerPosX + halfSideLength, centerPosY - halfSideLength, centerPosZ + halfSideLength  // bottom left
    };

   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glColor3f( colour[0], colour[1], colour[2] );
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glDrawArrays(GL_QUADS, 0, 24);

    glDisableClientState(GL_VERTEX_ARRAY);
}