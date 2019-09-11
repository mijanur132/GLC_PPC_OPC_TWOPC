#include <iostream>
#include <vector>
#include<iostream>
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp>

#include "util.hpp"
#include "mesh.hpp"

using namespace std;
using namespace glm;
// Window handle.
void initGLFW();
GLFWwindow* window = nullptr;
int width = 800, height = 600;

// Shaders, location to uniform variables, and vertex array objects.
void initOpenGL();
void prepareScene();
void prepareTexture();
void prepareQuad();
// Similar as VAO and VBOs, texture in OpenGL are also unsigned integers.
GLuint texture;
GLuint tex_unit_location;

GLuint VAO, VBO;
GLuint shader;
GLuint transformLocation;
Mesh* mesh;
Mesh* mesh1;
int color_array[256][256];
int model=1;

// camera
glm::vec3 camCoords = glm::vec3(0.0, 0.0, 1.0);
bool camRot = false;
glm::vec2 camOrigin;
glm::vec2 mouseOrigin;


// GLFW window callbacks to handle keyboard and mouse input.
void scrollCallback(GLFWwindow* w, double x, double y);
void keyCallback(GLFWwindow* w, int key, int sc, int action, int mode);
void mouseButtonCallback(GLFWwindow* w, int b, int action, int mode);
void cursorPosCallback(GLFWwindow* w, double xp, double yp);
void framebufferSizeCallback(GLFWwindow* w, int width, int height);
int doesIntersect(vec3 line_orig, vec3 line_direc, vec3 v0, vec3 v1, vec3 v2);
int renderGLC(vec3 a, vec3 b, vec3 c, vec3 raya, vec3 rayb, vec3 rayc, vec3 objv0, vec3 objv1, vec3 objv2, vec3 objv3);
void cameraLoop(int model);
float zadd = 0.0f;
float yadd = 0.0f;
float xadd = 0.0f;

constexpr GLuint kTextureUnitInUse = 0;
void display();
int main() {
	std::cout << "Hello, OpenGL!" << std::endl;
	initGLFW();
	initOpenGL();
	prepareScene();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	

	/*
	glm::vec3 v0 = glm::vec3(0.0, 0.0, 0.0);;
	glm::vec3 v1 = glm::vec3(0.0, 0.0, 1.0);;
	glm::vec3 v2 = glm::vec3(1.0, 0.0, 0.0);;
	
	std::cout << "v0=" << glm::to_string(v0) << std::endl;
	std::cout << "v1=" << glm::to_string(v1) << std::endl;
	std::cout << "v2=" << glm::to_string(v2) << std::endl; 
	

	int model;
	cout << "select model= 0,1,2 ; 0 pinhole, 1 persp, 2 ortho " << endl;
	cin >> model;

	
	vec3 s1t1 = vec3(0.0, 0, 1);
	vec3 s2t2 = vec3(0.5, 0.0, 1);
	vec3 s3t3 = vec3(0.0, 0.5, 1);

	vec3 u1v1 = vec3(0.0f, 0, 0);
	vec3 u2v2 = vec3(0.25f, 0.0f, 0);
	vec3 u3v3 = vec3(0.0f, 0.25f, 0);


	if (model == 0) 
	{
		 u1v1 = vec3(0.0f, 0, 0);
		 u2v2 = vec3(0.01f, 0.0f, 0);
		 u3v3 = vec3(0.0f, 0.01f, 0);
	}

	if (model == 2)
	{
		u1v1 = vec3(0.0, 0, 0);;
		u2v2 = vec3(0.5, 0.0, 0);
		u3v3 = vec3(0.0, 0.5, 0);;
	}
	

	vec3 Ray_u1v1_s1t1 = s1t1 - u1v1;
	vec3 Ray_u2v2_s2t2 = s2t2 - u2v2;
	vec3 Ray_u3vu3_s3t3 = s3t3 - u3v3;

	vec3 objv0 = vec3(0, 0, 2); //for first triangle
	vec3 objv3 = vec3(0, .5f, 2);
	vec3 objv2 = vec3(0.5, 0.5, 2);
	vec3 objv1 = vec3(0.5f, 0, 2);

	renderGLC(s1t1, s2t2, s3t3, Ray_u1v1_s1t1, Ray_u2v2_s2t2, Ray_u3vu3_s3t3, objv0, objv1, objv2, objv3);
	*/
	cout << "Loaded with perspective camera. If you want to change press 0 for PPC, 1 for perspective, 2 for orthogonal and 3 for twisted orthogonal anytime......";
	cameraLoop(model);
	prepareQuad();
	prepareTexture();

	glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
	tex_unit_location = glGetUniformLocation(shader, "tex_unit");

	display();


	return 0;
}

void initGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(width, height, "OpenGL Demo", nullptr, nullptr);
	if (!window) {
		std::cerr << "Cannot create window";
		std::exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

void initOpenGL() {
	assert(window);
	if (gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)) == 0) {
		std::cerr << "Failed to intialize OpenGL loader" << std::endl;
		std::exit(1);
	}
	assert(glGetError() == GL_NO_ERROR);
}

void prepareScene() {
	glEnable(GL_DEPTH_TEST);
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 norm;
	};

	std::vector<Vertex> verts = {
		{{-0.433f, -0.25f, 0.0f}, {1.0, 0.0, 0.0}},
		{{ 0.433f, -0.25f, 0.0f}, {0.0, 1.0, 0.0}},
		{{ 0.0f,    0.5f,  0.0f}, {0.0, 0.0, 1.0}}
	};
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(verts[0]), verts.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, norm));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Prepares the shader
	std::vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, "sh_v.glsl"));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "sh_f.glsl"));
	shader = linkProgram(shaders);
	transformLocation = glGetUniformLocation(shader, "xform");

	assert(glGetError() == GL_NO_ERROR);
}
// GLFW window callbacks
// --------------------------------------------------------------------

void scrollCallback(GLFWwindow* w, double x, double y) {
	float offset = (y > 0) ? 0.1f : -0.1f;
	camCoords.z = glm::clamp(camCoords.z + offset, 0.1f, 10.0f);
}

void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		glfwSetWindowShouldClose(w, true);
	}
	if (key == GLFW_KEY_0 && action == GLFW_RELEASE) {
		
		cout << "PPC camera selected............................= " << zadd << endl;
		model = 0;
		cameraLoop(model);
		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}

	if (key == GLFW_KEY_0 && action == GLFW_RELEASE) {

		cout << "PPC camera selected............................= " << zadd << endl;
		model = 0;
		cameraLoop(model);
		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}
	if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {

		cout << "PPC camera selected............................= " << zadd << endl;
		model = 1;
		cameraLoop(model);
		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}
	if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {

		cout << "PPC camera selected............................= " << zadd << endl;
		model = 2;
		cameraLoop(model);
		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}
	if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {

		cout << "PPC camera selected............................= " << zadd << endl;
		model = 3;
		cameraLoop(model);
		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
		zadd = zadd + 0.25f;			
		
		cout << "Y axis value............................= " << zadd << endl;
		cameraLoop(model);
		
		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");
		
		return;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		zadd = zadd - 0.25;
		cout << "Y axis value............................= " << zadd << endl;

		cameraLoop(model);

		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		xadd = xadd + 0.25f;

		cout << "X axis value............................= " << xadd << endl;
		cameraLoop(model);

		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		xadd = xadd - 0.25f;
		cout << "X axis value............................= " << xadd << endl;

		cameraLoop(model);

		prepareTexture();
		glActiveTexture(GL_TEXTURE0 + kTextureUnitInUse);
		tex_unit_location = glGetUniformLocation(shader, "tex_unit");

		return;
	}

	

	

}

void mouseButtonCallback(GLFWwindow* w, int button, int action, int mode) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Activate rotation mode
		camRot = true;
		camOrigin = glm::vec2(camCoords);
		double xpos, ypos;
		glfwGetCursorPos(w, &xpos, &ypos);
		mouseOrigin = glm::vec2(xpos, ypos);
	} if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		camRot = false;
	}
}

void cursorPosCallback(GLFWwindow* w, double xp, double yp) {
	if (camRot) {
		float rotScale = std::fmin(width / 450.f, height / 270.f);
		glm::vec2 mouseDelta = glm::vec2(xp, yp) - mouseOrigin;
		glm::vec2 newAngle = camOrigin + mouseDelta / rotScale;
		newAngle.y = glm::clamp(newAngle.y, -90.0f, 90.0f);
		while (newAngle.x > 180.0f) newAngle.x -= 360.0f;
		while (newAngle.y < -180.0f) newAngle.y += 360.0f;
		if (glm::length(newAngle - glm::vec2(camCoords)) > std::numeric_limits<float>::epsilon()) {
			camCoords.x = newAngle.x;
			camCoords.y = newAngle.y;
		}
	}
}

void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
	::width = width;
	::height = height;
	glViewport(0, 0, width, height);
}

int doesIntersect(vec3 line_orig, vec3 line_direc, vec3 v0, vec3 v1, vec3 v2)
{
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	
	//std::cout << "v0v1=" << glm::to_string(v0v1) << std::endl;
	//std::cout << "v0v2=" << glm::to_string(v0v2) << std::endl;

	vec3 pvec = cross(line_direc, v0v2);
	vec3 tvec = line_orig - v0;
	vec3 qvec = cross(tvec, v0v1);
	float det = dot(v0v1, pvec);
	float invDet = 1 / det;
	float t = dot(v0v2, qvec) * invDet;
	float u = dot(tvec, pvec) * invDet;
	float v = dot(line_direc, qvec) * invDet;

	//std::cout << "pvec=" << glm::to_string(pvec) << "  det=" << det << endl;
	   
	// ray and triangle are parallel if det is close to 0
	if (fabs(det) < 0.0001)
	{
		//cout << "Det=" << det << endl;
		return false;
	}
	
		
	if (u < 0 || u > 1)
	{
		//cout << "u=" << u << endl;
		return false;
	}
		
	if (v < 0 || u + v > 1)
	{
		//cout << "v=" << v << endl;
		return false;
	}
		
	vec3 pointP = line_orig + t * line_direc;
	//std::cout<<"T="<<t << " pointP=" << glm::to_string(pointP) << std::endl;

	return t;
}

int renderGLC(vec3 a, vec3 b, vec3 c, vec3 raya, vec3 rayb, vec3 rayc, vec3 objv0, vec3 objv1, vec3 objv2, vec3 objv3) 
{
	memset(color_array, 0, sizeof(color_array));
	vec3 v0x = b - a;
	vec3  v1x = c - a;
	vec2 v0 = vec2(v0x.x, v0x.y);
	vec2 v1 = vec2(v1x.x, v1x.y);
	vec2 aa = vec2(a.x, a.y);
	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	int divide = 256;
	for (int i = 0; i < divide; i++)
	{
		for (int j = 0; j < divide; j++)
		{
			float x = i * 0.5 / divide;
			float y = j * 0.5 / divide;
			vec2 p = vec2(x, y);
			vec3 p3 = vec3(x, y, 1);
			
			//std::cout <<"i= "<< i<<" j= "<<j << std::endl;
			vec2 v2 = p - aa;
			
			float d20 = dot(v2, v0);
			float d21 = dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;
			if (true || u >= 0 && v >= 0 && w >= 0)
			{
				
				vec3 ray_direc = u * raya + v * rayb + w * rayc;
				int t = doesIntersect(p3, ray_direc, objv0, objv2, objv3);
				//cout << "u= " << u << " v= " << v << " w=" << w <<" T="<<t<< endl;
				if (t > 0)
				{
					color_array[divide-j][i] = 1;
				}

				int tt = doesIntersect(p3, ray_direc, objv0, objv1, objv2);
				//cout << "u= " << u << " v= " << v << " w=" << w <<" T="<<t<< endl;
				if (tt > 0)
				{
					color_array[divide - j][i] = 2;
				}
				
			}
		}
	}
	
	for (int i = 0; i < divide; i++)
	{
		for (int j = 0; j < divide; j++)
		{
			//cout << color_array[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}

void cameraLoop(int model) {


	glm::vec3 v0 = glm::vec3(0.0, 0.0, 0.0);;
	glm::vec3 v1 = glm::vec3(0.0, 0.0, 1.0);;
	glm::vec3 v2 = glm::vec3(1.0, 0.0, 0.0);;

	std::cout << "v0=" << glm::to_string(v0) << std::endl;
	std::cout << "v1=" << glm::to_string(v1) << std::endl;
	std::cout << "v2=" << glm::to_string(v2) << std::endl;
	

	/*****GLC***/
	//cout << "give Yaxis value of object. Minimum is  "<<zadd<< ":->" << endl;
	//cin >> zadd;
	
	


	vec3 s1t1 = vec3(0.0, 0, 1);
	vec3 s2t2 = vec3(0.5, 0.0, 1);
	vec3 s3t3 = vec3(0.0, 0.5, 1);

	vec3 u1v1 = vec3(0.0f, 0, 0);
	vec3 u2v2 = vec3(0.25f, 0.0f, 0);
	vec3 u3v3 = vec3(0.0f, 0.25f, 0);	

	if (model == 0)
	{
		u1v1 = vec3(0.0f, 0, 0);
		u2v2 = vec3(0.01f, 0.0f, 0);
		u3v3 = vec3(0.0f, 0.01f, 0);
	}

	if (model == 2)
	{
		u1v1 = vec3(0.0, 0, 0);;
		u2v2 = vec3(0.5, 0.0, 0);
		u3v3 = vec3(0.0, 0.5, 0);;
	}

	if (model == 3)
	{
		//this is the model from GLC paper 2004 section 7.1. It is a twisted orthographic camera. 
		//Twisted orthographic camera can be viewed as twisting parallel planes of rays in
		//	an orthographic camera along common bilinear sheets
		u1v1 = vec3(0.0, 0, 0);;
		u2v2 = vec3(0.0, 0.5, 0);
		u3v3 = vec3(0.5, 0.0, 0);;
	}

	

	vec3 Ray_u1v1_s1t1 = s1t1 - u1v1;
	vec3 Ray_u2v2_s2t2 = s2t2 - u2v2;
	vec3 Ray_u3vu3_s3t3 = s3t3 - u3v3;

	vec3 objv0 = vec3(xadd, zadd, 4); //for first triangle
	vec3 objv3 = vec3(xadd, zadd+0.5, 4);
	vec3 objv2 = vec3(xadd+0.5, zadd+0.5,4);
	vec3 objv1 = vec3(xadd+0.5f, zadd, 4);

	renderGLC(s1t1, s2t2, s3t3, Ray_u1v1_s1t1, Ray_u2v2_s2t2, Ray_u3vu3_s3t3, objv0, objv1, objv2, objv3);

	
}

void display()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.1f, 0.2f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render pass
		glUseProgram(shader);
		// Texture binding.
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(tex_unit_location, kTextureUnitInUse);

		// Notice that no transformation is sent over to the shader
		//   (and the shader doesn't require one). By default OpenGL
		//   only draws everything inside the unit cube (x, y, z in [0, 1])
		//   so the 2 triangles will occupy most of the screen (but not all).
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
		glUseProgram(0);
		assert(glGetError() == GL_NO_ERROR);

		glfwSwapBuffers(window);

	}



}

void prepareTexture() {
	
	constexpr int kImageDim = 256;
	// The image is of size 1024x1024 and every pixel has 3 channels (RGB),
	//   each channel represented by 3 unsigned 8-bit numbers.
	unsigned char * image_data = new unsigned char[kImageDim * kImageDim * 3];

	for (int row = 0; row < kImageDim; ++row) {
		for (int col = 0; col < kImageDim; ++col) {
			unsigned char *p = image_data + (row * kImageDim + col) * 3;
			p[0] = color_array[row][col]*127;
			p[1] = color_array[row][col]*127;
			p[2] = color_array[row][col]*127;
		}
	}

	// Manages the memory data in `image_data` as an OpenGL texture.
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,				// The texture is 2D. (guess what, 1D and 3D are also supported by OpenGL.)
		0,							// level of mipmapping. Just make this 0.
		GL_RGB,					// How the image should be represented.
		kImageDim, kImageDim,		// width and height of the image
		0,							// Don't worry about this.
		GL_RGB,					// The format of the image data source. `image_data` has 3 channels.
		GL_UNSIGNED_BYTE,			// Data type of the image data source. `image_data` is an array of unsigned char.
		image_data);
	// Sets wrapping and filtering of the texture. Don't worry about them - just copy/paste.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generates mipmapping for better sampling.
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	assert(glGetError() == GL_NO_ERROR);
	delete[] image_data;
}


void prepareQuad() {
	struct Vertex {
		glm::vec2 pos;
		glm::vec2 texture_uv;
	};

	// Vertices of the quad, which takes up 90% of the window both horizontally and vertically.
	std::vector<Vertex> verts = {
		{{-0.9f, -0.9f}, {0.0f, 1.0f}},	// bottom left
		{{ 0.9f, -0.9f}, {1.0f, 1.0f}},	// bottom right
		{{ 0.9f,  0.9f}, {1.0f, 0.0f}},	// top right
		{{-0.9f,  0.9f}, {0.0f, 0.0f}},	// top left
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(verts[0]), verts.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_uv));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Prepares the shader
	std::vector<GLuint> shaders;
	shaders.push_back(compileShader(GL_VERTEX_SHADER, "sh_v.glsl"));
	shaders.push_back(compileShader(GL_FRAGMENT_SHADER, "sh_f.glsl"));
	shader = linkProgram(shaders);

	assert(glGetError() == GL_NO_ERROR);
}