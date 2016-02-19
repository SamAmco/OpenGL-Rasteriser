#include "Renderer.h"
#include "../nclgl/Matrix3.h"
#include <time.h>

Renderer::Renderer(Window &parent) : OGLRenderer(parent)	
{
	light.colour = Vector3(1,1,1);
	light.position = Vector3(0,0,0);
	light.radius = 0;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer(void)	{
	 
}

void	Renderer::RenderScene() 
{
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i )
	{
		Render(*(*i));
	}
}

void Renderer::SetMainLight(Vector3 colour, Vector3 position, float radius)
{
	light.colour = colour;
	light.position = position;
	light.radius = radius;
}

GLuint Renderer::LoadTexture(std::string name)
{
	GLuint t = SOIL_load_OGL_texture(name.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	if (!t)
		cout << "GLuint Renderer::LoadTexture(std::string name) FAILED TO LOAD " << name << std::endl;
	return t;
}

void	Renderer::Render(const RenderObject &o) 
{
	modelMatrix = o.GetWorldTransform();

	if(o.GetShader() && o.GetMesh()) 
	{
		GLuint program = o.GetShader()->GetShaderProgram();
		
		glUseProgram(program);

		UpdateShaderMatrices(program);

		Matrix3 rotation = Matrix3(viewMatrix);
		Vector3 invCamPos = viewMatrix.GetPositionVector();
		Vector3 camPos = rotation * -invCamPos; 

		glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*)&camPos);
		glUniform1f(glGetUniformLocation(program, "time"), ((float)clock()) / 1000.0);
		glUniform3fv(glGetUniformLocation(program, "lightColour"), 1, (float*) &light.colour);
		glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, (float*) &light.position);
		glUniform1f(glGetUniformLocation(program, "lightRadius"), light.radius);

		o.Draw();

		glUseProgram(0);
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) 
	{
		Render(*(*i));
	}
}

void	Renderer::UpdateScene(float msec) 
{
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) 
	{
		(*i)->Update(msec);
	}
}