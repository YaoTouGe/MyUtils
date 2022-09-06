#if 0
#include <GL/glew.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void init();
void display();

int const fbo_width = 512;
int const fbo_height = 512;

GLuint fb, color, depth;

int test_gl(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("FBO test");
    glutDisplayFunc(display);
    glutIdleFunc(glutPostRedisplay);

    glewInit();

    init();
    glutMainLoop();

    return 0;
}

void CHECK_FRAMEBUFFER_STATUS()
{
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        /* choose different formats */
        break;

    default:
        /* programming error; will fail on all hardware */
        fputs("Framebuffer Error\n", stderr);
        exit(-1);
    }
}

float const light_dir[] = {1, 1, 1, 0};
float const light_color[] = {1, 0.95, 0.9, 1};

void init()
{
    glGenFramebuffers(1, &fb);
    glGenTextures(1, &color);
    glGenRenderbuffers(1, &depth);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 fbo_width, fbo_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo_width, fbo_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    CHECK_FRAMEBUFFER_STATUS();
}

void prepare()
{
    static float a = 0, b = 0, c = 0;

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    glViewport(0, 0, fbo_width, fbo_height);

    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);

    glTranslatef(0, 0, -5);

    glRotatef(a, 1, 0, 0);
    glRotatef(b, 0, 1, 0);
    glRotatef(c, 0, 0, 1);

    glutSolidTeapot(0.75);

    a = fmod(a + 0.1, 360.);
    b = fmod(b + 0.5, 360.);
    c = fmod(c + 0.25, 360.);
}

void final()
{
    static float a = 0, b = 0, c = 0;

    const int win_width = glutGet(GLUT_WINDOW_WIDTH);
    const int win_height = glutGet(GLUT_WINDOW_HEIGHT);
    const float aspect = (float)win_width / (float)win_height;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, win_width, win_height);

    glClearColor(1., 1., 1., 0.);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, aspect, 1, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -5);

    glRotatef(b, 0, 1, 0);

    b = fmod(b + 0.5, 360.);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, color);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_LIGHTING);

    float cube[][5] =
        {
            {-1, -1, -1, 0, 0},
            {1, -1, -1, 1, 0},
            {1, 1, -1, 1, 1},
            {-1, 1, -1, 0, 1},

            {-1, -1, 1, -1, 0},
            {1, -1, 1, 0, 0},
            {1, 1, 1, 0, 1},
            {-1, 1, 1, -1, 1},
        };
    unsigned int faces[] =
        {
            0, 1, 2, 3,
            1, 5, 6, 2,
            5, 4, 7, 6,
            4, 0, 3, 7,
            3, 2, 6, 7,
            4, 5, 1, 0};

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), &cube[0][0]);
    glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), &cube[0][3]);

    glCullFace(GL_BACK);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, faces);

    glCullFace(GL_FRONT);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, faces);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void display()
{
    prepare();
    final();

    glutSwapBuffers();
}
#else
#include <stdlib.h>
#include "opengl_test.h"
#include <math.h>
#include <limits.h>
#include <GL/glew.h>
#include <GL/glut.h> // OpenGL Graphics Utility Library
#include <stdlib.h>
#include <stdio.h>

const float PI2 = 2.0f * 3.1415926535;

GLenum runMode = GL_TRUE;

GLenum shadeModel = GL_FLAT;  // Toggles between GL_FLAT and GL_SMOOTH
GLenum polygonMode = GL_LINE; // Toggles between GL_LINE and GL_FILL

// Variables controlling the animation
float RotX = 0.0f;              // Rotational position around x-axis
float RotY = 0.0f;              // Rotational position around y-axis
float RotIncrementX = 0.0;      // Rotational increment, x-axis
float RotIncrementY = 0.0;      // Rotational increment, y-axis
const float RotIncFactor = 1.5; // Factor change in rot rate per key stroke

// Variables controlling the fineness of the polygonal mesh
int NumWraps = 10;
int NumPerWrap = 8;

// Variables controlling the size of the torus
float MajorRadius = 3.0;
float MinorRadius = 1.0;

// Mode flags
int QuadMode = 1;           // Quad/Triangle toggling
GLenum LocalMode = GL_TRUE; // Local viewer/non-local viewer mode
int Light0Flag = 1;         // Is light #0 on?
int Light1Flag = 1;         // Is light #1 on?

// Lighting values
float ambientLight[4] = {0.6, 0.6, 0.6, 1.0};
float Lt0amb[4] = {0.8, 0.8, 0.16, 1.0};
float Lt0diff[4] = {1.0, 1.0, 0.2, 1.0};
float Lt0spec[4] = {1.0, 1.0, 0.2, 1.0};
float Lt0pos[4] = {1.7 * 4.0, 0.0, 0.0, 1.0}; // 4 = MajorRadius + MinorRadius

float Lt1amb[4] = {0.0, 0.0, 0.5, 1.0};
float Lt1diff[4] = {0.0, 0.0, 0.5, 1.0};
float Lt1spec[4] = {0.0, 0.0, 1.0, 1.0};
float Lt1pos[4] = {0.0, 1.2 * 4.0, 0.0, 1.0};

// Material values
float Noemit[4] = {0.0, 0.0, 0.0, 1.0};
float Matspec[4] = {1.0, 1.0, 1.0, 1.0};
float Matnonspec[4] = {0.4, 0.05, 0.4, 1.0};
float Matshiny = 16.0;

// glutKeyboardFunc is called below to set this function to handle
//		all "normal" key presses.
void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        runMode = !runMode;
        break;
    case 's':
        runMode = GL_TRUE;
        updateScene();
        runMode = GL_FALSE;
        break;
    case 27: // Escape key
        exit(1);
    case 'r': // Reset the animation (resets everything)
        ResetAnimation();
        break;
    case '0': // Zero the rotation rates
        ZeroRotation();
        break;
    case 'f': // Shade mode toggles from flat to smooth
        ShadeModelToggle();
        break;
    case 'p': // Polygon mode toggles between fill and line
        FillModeToggle();
        break;
    case 'w': // Decrement number of wraps around torus
        WrapLess();
        break;
    case 'W': // Increment number of wraps around torus
        WrapMore();
        break;
    case 'n': // Decrement number of polys per wrap
        NumPerWrapLess();
        break;
    case 'N': // Increment number of polys per wrap
        NumPerWrapMore();
        break;
    case 'q': // Toggle between triangles and Quadrilaterals
        QuadTriangleToggle();
        break;
    case 'l': // Toggle between local and non-local viewer ('l' is for 'local')
        LocalToggle();
        break;
    case '1': // Toggle light #0 on and off
        Light0Toggle();
        break;
    case '2': // Toggle light #1 on and off
        Light1Toggle();
        break;
    }
}

// glutSpecialFunc is called below to set this function to handle
//		all "special" key presses.  See glut.h for the names of
//		special keys.
void mySpecialKeyFunc(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // Either increase upward rotation, or slow downward rotation
        KeyUp();
        break;
    case GLUT_KEY_DOWN:
        // Either increase downwardward rotation, or slow upward rotation
        KeyDown();
        break;
    case GLUT_KEY_LEFT:
        // Either increase left rotation, or slow down rightward rotation.
        KeyLeft();
        break;
    case GLUT_KEY_RIGHT:
        // Either increase right rotation, or slow down leftward rotation.
        KeyRight();
        break;
    }
}

// The routines below are coded so that the only way to change from
//	one direction of rotation to the opposite direction is to first
//  reset the animation,

void KeyUp()
{
    if (RotIncrementX == 0.0)
    {
        RotIncrementX = -0.1; // Initially, one-tenth degree rotation per update
    }
    else if (RotIncrementX < 0.0f)
    {
        RotIncrementX *= RotIncFactor;
    }
    else
    {
        RotIncrementX /= RotIncFactor;
    }
}

void KeyDown()
{
    if (RotIncrementX == 0.0)
    {
        RotIncrementX = 0.1; // Initially, one-tenth degree rotation per update
    }
    else if (RotIncrementX > 0.0f)
    {
        RotIncrementX *= RotIncFactor;
    }
    else
    {
        RotIncrementX /= RotIncFactor;
    }
}

void KeyLeft()
{
    if (RotIncrementY == 0.0)
    {
        RotIncrementY = -0.1; // Initially, one-tenth degree rotation per update
    }
    else if (RotIncrementY < 0.0)
    {
        RotIncrementY *= RotIncFactor;
    }
    else
    {
        RotIncrementY /= RotIncFactor;
    }
}

void KeyRight()
{
    if (RotIncrementY == 0.0)
    {
        RotIncrementY = 0.1; // Initially, one-tenth degree rotation per update
    }
    else if (RotIncrementY > 0.0)
    {
        RotIncrementY *= RotIncFactor;
    }
    else
    {
        RotIncrementY /= RotIncFactor;
    }
}

// Resets position and sets rotation rate back to zero.
void ResetAnimation()
{
    RotX = RotY = RotIncrementX = RotIncrementY = 0.0;
}

// Sets rotation rates back to zero.
void ZeroRotation()
{
    RotIncrementX = RotIncrementY = 0.0;
}

// Toggle between smooth and flat shading
void ShadeModelToggle()
{
    if (shadeModel == GL_FLAT)
    {
        shadeModel = GL_SMOOTH;
    }
    else
    {
        shadeModel = GL_FLAT;
    }
}

// Toggle between line mode and fill mode for polygons.
void FillModeToggle()
{
    if (polygonMode == GL_LINE)
    {
        polygonMode = GL_FILL;
    }
    else
    {
        polygonMode = GL_LINE;
    }
}

// Toggle quadrilaterial and triangle mode
void QuadTriangleToggle()
{
    QuadMode = 1 - QuadMode;
}

// Toggle from local to global mode
void LocalToggle()
{
    LocalMode = !LocalMode;
    if (LocalMode)
    {
        Lt0pos[3] = Lt1pos[3] = 1.0; // Put lights back at finite location.
    }
    else
    {
        Lt0pos[3] = Lt1pos[3] = 0.0; // Put lights at infinity too.
    }
}

// The next two routines toggle the lights on and off
void Light0Toggle()
{
    Light0Flag = 1 - Light0Flag;
}

void Light1Toggle()
{
    Light1Flag = 1 - Light1Flag;
}

// Increment number of wraps
void WrapMore()
{
    NumWraps++;
}

// Decrement number of wraps
void WrapLess()
{
    if (NumWraps > 4)
    {
        NumWraps--;
    }
}

// Increment number of segments per wrap
void NumPerWrapMore()
{
    NumPerWrap++;
}

// Decrement number segments per wrap
void NumPerWrapLess()
{
    if (NumPerWrap > 4)
    {
        NumPerWrap--;
    }
}

/*
 * issue vertex command for segment number j of wrap number i.
 */
void putVert(int i, int j)
{
    float wrapFrac = (j % NumPerWrap) / (float)NumPerWrap;
    float phi = PI2 * wrapFrac;
    float theta = PI2 * (i % NumWraps + wrapFrac) / (float)NumWraps;
    float sinphi = sin(phi);
    float cosphi = cos(phi);
    float sintheta = sin(theta);
    float costheta = cos(theta);
    float y = MinorRadius * sinphi;
    float r = MajorRadius + MinorRadius * cosphi;
    float x = sintheta * r;
    float z = costheta * r;
    glNormal3f(sintheta * cosphi, sinphi, costheta * cosphi);
    glVertex3f(x, y, z);
}

GLuint fb;
GLuint depth;
GLuint color;
int fbo_width = 800;
int fbo_height = 600;

void updateScene(void)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    
    int i, j;

    // Clear the redering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glShadeModel(shadeModel);                      // Set the shading to flat or smooth.
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode); // Set to be "wire" or "solid"
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, LocalMode);

    // Set up lights
    if (Light0Flag == 1 || Light1Flag == 1)
    {
        // Emissive spheres have no other color.
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Noemit);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Noemit);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);
    }
    if (Light0Flag == 1)
    {
        glPushMatrix();
        glTranslatef(Lt0pos[0], Lt0pos[1], Lt0pos[2]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lt0spec);
        glutSolidSphere(0.2, 5, 5);
        glPopMatrix();
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
    }
    else
    {
        glDisable(GL_LIGHT0);
    }
    if (Light1Flag == 1)
    {
        glPushMatrix();
        glTranslatef(Lt1pos[0], Lt1pos[1], Lt1pos[2]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Lt1spec);
        glutSolidSphere(0.2, 5, 5);
        glPopMatrix();
        glEnable(GL_LIGHT1);
        glLightfv(GL_LIGHT1, GL_POSITION, Lt1pos);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    // Torus Materials
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Matnonspec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Matspec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Matshiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);

    glPushMatrix(); // Save to use again next time.

    // Update the orientation of the torus, if the animation is running.
    if (runMode)
    {
        RotY += RotIncrementY;
        if (fabs(RotY) > 360.0)
        {
            RotY -= 360.0 * ((int)(RotY / 360.0));
        }
        RotX += RotIncrementX;
        if (fabs(RotX) > 360.0)
        {
            RotX -= 360.0 * ((int)(RotX / 360.0));
        }
    }
    // Set the orientation.
    glRotatef(RotX, 1.0, 0.0, 0.0);
    glRotatef(RotY, 0.0, 1.0, 0.0);

    // Draw the torus
    glColor3f(1.0, 0.5, 1.0);

    glBegin(QuadMode == 1 ? GL_QUAD_STRIP : GL_TRIANGLE_STRIP);

    for (i = 0; i < NumWraps; i++)
    {
        for (j = 0; j < NumPerWrap; j++)
        {
            putVert(i, j);
            putVert(i + 1, j);
        }
    }
    putVert(0, 0);
    putVert(1, 0);

    glEnd();

    // Draw the reference pyramid
    glTranslatef(-MajorRadius - MinorRadius - 0.3, 0.0, 0.0);
    glScalef(0.2f, 0.2f, 0.2f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-0.5, 0.0, sqrt(3.0) * 0.5);
    glVertex3f(-0.5, 0.0, -sqrt(3.0) * 0.5);
    glVertex3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, sqrt(2.0), 0.0);
    glVertex3f(-0.5, 0.0, sqrt(3.0) * 0.5);
    glVertex3f(-0.5, 0.0, -sqrt(3.0) * 0.5);
    glEnd();

    glPopMatrix(); // Restore to original matrix as set in resizeWindow()

    // Flush the pipeline, swap the buffers
    glFlush();
    glutSwapBuffers();
}

// Initialize OpenGL
void initRendering()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING); // Enable lighting calculations
    glEnable(GL_LIGHT0);   // Turn on lights (unnecessary here, since also in Animate()
    glEnable(GL_LIGHT1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); // Ambient light

    // Light 0 (Position is set in updateScene)
    glLightfv(GL_LIGHT0, GL_AMBIENT, Lt0amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Lt0spec);

    // Light 1 (Position is set in updateScene)
    glLightfv(GL_LIGHT1, GL_AMBIENT, Lt1amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Lt1diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, Lt1spec);
}

// Called when the window is resized
//		Sets up the projection view matrix (somewhat poorly, however)
void resizeWindow(int w, int h)
{
    float aspectRatio;
    glViewport(0, 0, w, h); // View port uses whole window
    h = (w == 0) ? 1 : h;
    aspectRatio = (float)w / (float)h;

    // Set up the proection view matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspectRatio, 1.0, 30.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Move system 10 units away to be able to view from the origin.
    glTranslatef(0.0, 0.0, -10.0);

    // Tilt system 15 degrees downward in order to view from above
    //   the xy-plane.
    glRotatef(15.0, 1.0, 0.0, 0.0);
}

void CHECK_FRAMEBUFFER_STATUS()
{
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        /* choose different formats */
        break;

    default:
        /* programming error; will fail on all hardware */
        fputs("Framebuffer Error\n", stderr);
        exit(-1);
    }
}

void init()
{
    glGenFramebuffers(1, &fb);
    glGenTextures(1, &color);
    glGenRenderbuffers(1, &depth);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 fbo_width, fbo_height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fbo_width, fbo_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    CHECK_FRAMEBUFFER_STATUS();
}

// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop
int test_gl(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Window position (from top corner), and size (width and hieght)
    glutInitWindowPosition(10, 60);
    glutInitWindowSize(620, 160);
    glutCreateWindow("Light Torus demo");

    // Initialize OpenGL rendering modes
    initRendering();
    resizeWindow(620, 160);

    init();

    // Set up callback functions for key presses
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialKeyFunc);

    // Set up the callback function for resizing windows
    glutReshapeFunc(resizeWindow);

    // Call this for background processing
    glutIdleFunc(updateScene);
    // Call this whenever window needs redrawing
    glutDisplayFunc(updateScene);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return (0); // This line is never reached.
}

#endif