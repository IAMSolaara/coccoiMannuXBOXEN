#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>

#define PI 3.1415926535

#define degToRad(angleInDegrees) ((angleInDegrees) * (PI / 180.0))
#define radToDeg(angleInRadians) ((angleInRadians) * (180.0 / PI ))

#define isWithin(v, min, max) (((v) >= (min)) && ((v) <= (max)))

#include <math.h>
#include <pbkit/pbkit.h>
#include <pbgl.h>
#include <GL/gl.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>

#include "include/glm/glm.hpp"
#include "include/camera.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

//#include "include/objImporter.h"
#include "include/OBJ_Loader.h"

/*
#include "include/sburnias.h"
#include "include/caca.h"
#include "include/risi.h"
#include "include/cock.h"
//#include "include/henSkin.h"
*/

//#define STBI_NO_SIMD
//#include "stb_image/stb_image.h"

#define RESPATH "D:\\res"
#define TEXTURESPATH  RESPATH"\\textures"
//#define TEXTURESPATH  RESPATH
#define MODELSPATH RESPATH"\\models"
#define AUDIOPATH RESPATH"\\audio"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;

uint64_t now;
uint64_t last;

uint32_t lastClock = -1;


GLfloat vertices[] = {
	//big rect
	//x     y     z      r     g     b     s     t
//face3
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//face1
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//face4
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face5                                              
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face2
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//face6
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
};

GLfloat tex2Vertices[] = {
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
};

GLfloat tex3Vertices[] = {
    -0.5f,   0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,   0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,   0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f,
    -0.5f,   0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.5f,
};

GLfloat crosshairVertices[] = {
     0.01f,   0.05f,   0.0f,
	 0.01f,  -0.05f,   0.0f,
    -0.01f,  -0.05f,   0.0f,
    -0.01f,   0.05f,   0.0f,
} ;

static GLuint load_texture(GLuint w, GLuint h, GLenum intfmt, GLenum extfmt, const void *data) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, intfmt, w, h, 0, extfmt, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double map(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void) {
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

    if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
        debugPrint("SDL_Init failed: %s\n", SDL_GetError());
        Sleep(1000);
        return 0;
    }

    if ((IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG ) & (IMG_INIT_JPG | IMG_INIT_PNG)) != (IMG_INIT_PNG | IMG_INIT_JPG) ) {
        debugPrint("IMG_Init: %s\n", IMG_GetError());
        Sleep(1000);
        return 0;
    }

   	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		debugPrint("Error initializing SDL_Mixer: \n%s\n", SDL_GetError());
        Sleep(1000);
        return 0;
	}



    if (SDL_NumJoysticks() < 1) {
        debugPrint("hook up joystick pls kthxbie");
        Sleep(1000);
        return 0;
    }

    SDL_GameController* pad = SDL_GameControllerOpen(0);
    if (pad == NULL) {
        debugPrint("Failed to open gamecontroller 0\n");
        Sleep(1000);
        return 0;
    }

    //Model hen = loadModel("D:\\res\\cube.obj");
    //debugPrint("%d", hen.totV);

    objl::Loader loader;
    auto modelLoadRes = loader.LoadFile(MODELSPATH"\\hen.obj");
    //auto modelLoadRes = loader.LoadFile("D:\\res\\cube.obj");
    auto henMesh = loader.LoadedMeshes[0];

    debugPrint("LOADED: %s\n", modelLoadRes ? "true" : "false");
    auto vertexArraySize = loader.LoadedVertices.size();
    debugPrint("Vertices: %d\n", vertexArraySize);
    debugPrint("Meshes: %d\n", loader.LoadedMeshes.size());
    debugPrint("Vertex 0 coords: %f %f %f\n",loader.LoadedVertices[0].Position.X, loader.LoadedVertices[0].Position.Y, loader.LoadedVertices[0].Position.Z );
    debugPrint("Last Vertex coords: %f %f %f\n",loader.LoadedVertices[vertexArraySize-1].Position.X, loader.LoadedVertices[vertexArraySize-1].Position.Y, loader.LoadedVertices[vertexArraySize-1].Position.Z );
    debugPrint("Mesh name: %s\n", henMesh.MeshName.c_str());
    debugPrint("Loaded material name: %s\n", henMesh.MeshMaterial.name.c_str());

    Sleep(1000);

    const int err = pbgl_init(GL_TRUE);
    if (err < 0) {
        debugPrint("pbgl_init() failed: %d\n", err);
        Sleep(5000);
        return 0;
    }
    pbgl_set_swap_interval(1);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.2f, 0.3f, 0.3f, 1.f);
    //glEnable(GL_CULL_FACE); // FIXME: disable when the Z issue is fixed
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 640.0 / 480.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    GLfloat rot_quad = 0.f;

    // automatically die in about 15 seconds
    GLuint frames = 0;
    //while (frames++ < 60 * 15) {

    //stbi_set_flip_vertically_on_load(true);

    SDL_Surface* textureSurface;
    unsigned char* textureData;
    int w,h,nChannels;

    textureSurface = IMG_Load(MODELSPATH"\\skin.png");
    textureData = (unsigned char*)textureSurface->pixels;
    w = textureSurface->w;
    h = textureSurface->h;
    GLuint henSkin = load_texture(w, h, GL_RGB, GL_RGBA, textureData);
    SDL_FreeSurface(textureSurface);

    textureSurface = IMG_Load(TEXTURESPATH"\\sburnias.png");
    textureData = (unsigned char*)textureSurface->pixels;
    w = textureSurface->w;
    h = textureSurface->h;
    GLuint texture = load_texture(w, h, GL_RGBA, GL_RGBA, textureData);
    SDL_FreeSurface(textureSurface);

    textureSurface = IMG_Load(TEXTURESPATH"\\risi.png");
    textureData = (unsigned char*)textureSurface->pixels;
    w = textureSurface->w;
    h = textureSurface->h;
    GLuint texture2 = load_texture(w, h, GL_RGBA, GL_RGBA, textureData);
    SDL_FreeSurface(textureSurface);
    
    textureSurface = IMG_Load(TEXTURESPATH"\\caca.png");
    textureData = (unsigned char*)textureSurface->pixels;
    w = textureSurface->w;
    h = textureSurface->h;
    GLuint texture3 = load_texture(w, h, GL_RGBA, GL_RGBA, textureData);
    SDL_FreeSurface(textureSurface);
    
    textureSurface = IMG_Load(TEXTURESPATH"\\cock.png");
    textureData = (unsigned char*)textureSurface->pixels;
    w = textureSurface->w;
    h = textureSurface->h;
    GLuint texture4 = load_texture(w, h, GL_RGBA, GL_RGBA, textureData);
    SDL_FreeSurface(textureSurface);


	Mix_Music* music = Mix_LoadMUS(AUDIOPATH"\\bgm.ogg");
	if (!music) {
		fprintf(stderr, "Music load error: \n%s\n", SDL_GetError());
	}

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(20);

	Mix_Chunk* sample = Mix_LoadWAV(AUDIOPATH"\\shoot.ogg");
	Mix_Chunk* succ = Mix_LoadWAV(AUDIOPATH"\\succ.ogg");


    glm::mat4 viewMatrix;
    while (1) {
		last = now;
		now = SDL_GetPerformanceCounter();

		deltaTime = ((now - last)*1000 / (double)SDL_GetPerformanceFrequency() );
		deltaTime /= 1000;

        viewMatrix = camera.GetViewMatrix();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//coobe
        glLoadIdentity();
        glMultMatrixf( (GLfloat*)&viewMatrix );
        
        glTranslatef(0.f, 0.4f, -3.5f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(rot_quad, 1.0f, 0.0f, 0.0f);
        glRotatef(rot_quad * 0.3f, 0.0f, 1.0f, 0.0f);
        glRotatef(rot_quad + 2.0f, 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBegin(GL_QUADS);
            for(int i = 0; i < 4 * 6; i++) {
                glColor3f(vertices[8*i+3],vertices[8*i+4],  vertices[8*i+5]);
                //glColor3f(1.f, 1.f, 1.f);
                glTexCoord2f(vertices[8*i+6],  vertices[8*i+7]);
                glVertex3f(vertices[8*i+0],vertices[8*i+1],  vertices[8*i+2]);
            }
        glEnd();

//skybox
        glLoadIdentity();
        glMultMatrixf( (GLfloat*)&viewMatrix );

        glTranslatef(0.f, 0.0f, -4.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(40.0f, 40.0f, 40.0f);
        glBindTexture(GL_TEXTURE_2D, texture4);

        glBegin(GL_QUADS);
            for(int i = 0; i < 4 * 6; i++) {
                glColor3f(vertices[8*i+3],vertices[8*i+4],  vertices[8*i+5]);
                //glColor3f(1.f, 1.f, 1.f);
                glTexCoord2f(vertices[8*i+6],  vertices[8*i+7]);
                glVertex3f(vertices[8*i+0],vertices[8*i+1],  vertices[8*i+2]);
            }
        glEnd();

//bottom thing
        glLoadIdentity();
        glMultMatrixf( (GLfloat*)&viewMatrix );

        glTranslatef(0.f, -0.7f, -3.5f);
        glScalef(2.0f, 2.0f, 2.0f);
        glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBegin(GL_QUADS);
            for(int i = 0; i < 4 * 1; i++) {
                glColor3f(tex2Vertices[8*i+3],tex2Vertices[8*i+4],  tex2Vertices[8*i+5]);
                //glColor3f(1.f, 1.f, 1.f);
                glTexCoord2f(tex2Vertices[8*i+6],  tex2Vertices[8*i+7]);
                glVertex3f(tex2Vertices[8*i+0],tex2Vertices[8*i+1],  tex2Vertices[8*i+2]);
            }
        glEnd();

//cacasbunra
        glLoadIdentity();
        glMultMatrixf( (GLfloat*)&viewMatrix );

        //glTranslatef(0.f, -0.7f, -3.5f);
        glRotatef( 90.f *  sin( degToRad(rot_quad) ) , 0.0f, 0.0f, 1.0f);
        glTranslatef(0.f, 0.0f, -4.5f);
        glScalef(2.0f, 2.0f, 2.0f);
        glBindTexture(GL_TEXTURE_2D, texture3);

        glBegin(GL_QUADS);
            for(int i = 0; i < 4 * 1; i++) {
                glColor3f(tex3Vertices[8*i+3],tex3Vertices[8*i+4],  tex3Vertices[8*i+5]);
                //glColor3f(1.f, 1.f, 1.f);
                glTexCoord2f(tex3Vertices[8*i+6],  tex3Vertices[8*i+7]);
                glVertex3f(tex3Vertices[8*i+0],tex3Vertices[8*i+1],  tex3Vertices[8*i+2]);
            }
        glEnd();

//hen model
        glLoadIdentity();
        glMultMatrixf( (GLfloat*)&viewMatrix );

        glTranslatef(0.f, -1.7f, -3.5f);
        glRotatef( rot_quad, 0.0f, 1.0f, 0.0f);
        glScalef(0.25f, 0.25f, 0.25f);
        glBindTexture(GL_TEXTURE_2D, henSkin);

        glBegin(GL_TRIANGLES);
            for(auto vtx : henMesh.Vertices) {
                glColor3f(1.0f, 1.0f, 1.0f);
                glTexCoord2f(vtx.TextureCoordinate.X, vtx.TextureCoordinate.Y);
                glVertex3f(vtx.Position.X, vtx.Position.Y, vtx.Position.Z);
            }
        glEnd();

//crosshair
        //glDisable(GL_DEPTH_TEST);
        glLoadIdentity();

        glTranslatef(0.f,0.f,-1.0f);

        glBegin(GL_QUADS);
            for(int i = 0; i < 4 * 1; i++) {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                glVertex3f(crosshairVertices[3*i+0],crosshairVertices[3*i+1],  crosshairVertices[3*i+2]);
            }
        glEnd();

        glRotatef( 90.0f, 0.0f, 0.0f, 1.0f);

        glBegin(GL_QUADS);
            for(int i = 0; i < 4 * 1; i++) {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                glVertex3f(crosshairVertices[3*i+0],crosshairVertices[3*i+1],  crosshairVertices[3*i+2]);
            }
        glEnd();
        //glEnable(GL_DEPTH_TEST);

        rot_quad += 1.f;

        SDL_GameControllerUpdate();
        int leftAnalogX = SDL_GameControllerGetAxis(pad,  SDL_CONTROLLER_AXIS_LEFTX);
        int leftAnalogY = SDL_GameControllerGetAxis(pad,  SDL_CONTROLLER_AXIS_LEFTY);
        int rightAnalogX = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTX);
        int rightAnalogY = SDL_GameControllerGetAxis(pad, SDL_CONTROLLER_AXIS_RIGHTY);

        //movement
        if (!isWithin(leftAnalogX, -15000, 15000)) {
            if (leftAnalogX < 0) {
                //left movement
                camera.ProcessKeyboard(LEFT, deltaTime);
            } else {
                //right movement
                camera.ProcessKeyboard(RIGHT, deltaTime);
            }
        }

        if (!isWithin(leftAnalogY, -15000, 15000)) {
            if (leftAnalogY < 0) {
                //up movement
                camera.ProcessKeyboard(FORWARD, deltaTime);
            } else {
                //down movement
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            }
        }
        
        if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
            //left movement
            camera.ProcessKeyboard(LEFT, deltaTime);
        } 
        if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
            //right movement
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }

        if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
            //left movement
            camera.ProcessKeyboard(FORWARD, deltaTime);
        } 
        if (SDL_GameControllerGetButton(pad, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
            //right movement
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }


        if (!isWithin(rightAnalogX, -5000, 5000)) {
            camera.ProcessMouseMovement(map((float)rightAnalogX, (float)-32768, (float)32767, -24.f, 24.f), 0.f);
        }

        if (!isWithin(rightAnalogY, -5000, 5000)) {
            camera.ProcessMouseMovement(0.f, map((float)rightAnalogY, (float)-32768, (float)32767, 24.f, -24.f));
        }
        

        //camera.ProcessMouseMovement(2.f, 0.f);

        pbgl_swap_buffers();

		if (rand() % 256 == 0)
			Mix_PlayChannel(-1, succ, 0);
    }

    pbgl_shutdown();
    SDL_Quit();

    return 0;
}
