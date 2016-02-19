#pragma once
#include "Renderer.h"
#include "RenderObject.h"
#include <vector>


#pragma comment(lib, "nclgl.lib")

void main(void) 
{
	Window w = Window(800, 600);
	Renderer r(w);

	Mesh*	m = Mesh::LoadMeshFile("cube.asciimesh");
	vector<Shader*> shaders = vector<Shader*>(8);
	shaders[0] = new Shader("Shaders/BasicVert.glsl", "Shaders/BasicFrag.glsl");
	shaders[1] = new Shader("Shaders/SinExpandVert.glsl", "Shaders/BasicFrag.glsl");//A vertex shader which modulates the scale of objects over time, using a uniform variable.
	shaders[2] = new Shader("Shaders/BasicVert.glsl", "Shaders/TexBlendSineFrag.glsl");//A fragment shader which blends back and forth between two textures.
	shaders[3] = new Shader("Shaders/BasicVert.glsl", "Shaders/BasicFrag.glsl", "Shaders/PosSineGeom.glsl");//A geometry shader using a position-based sine wave to move vertices.
	shaders[4] = new Shader("Shaders/BasicVert.glsl", "Shaders/ThreeTexBlendFrag.glsl");//Perform a non-trivial blend between two textures, perhaps using a third, greyscale texture to determine blend levels.
	shaders[5] = new Shader("Shaders/BasicVert.glsl", "Shaders/BasicFrag.glsl", "Shaders/WireframeGeom.glsl");//Take in the vertices of the cube, and output new primitives from them (points, lines, or triangles forming new cubes)
	shaders[6] = new Shader("Shaders/PhongVert.glsl", "Shaders/PhongFrag.glsl");//Calculate per - fragment lighting(using light information accessed from uniform variables)
	shaders[7] = new Shader("Shaders/BasicVert.glsl", "Shaders/TransparentWhiteFrag.glsl");//Make part of the cube semi-transparent, while still rendering correctly from all orientations
	int shaderInd = 0;


	RenderObject o(m,shaders[shaderInd]);
	o.SetModelMatrix(Matrix4::Translation(Vector3(0,0,-10)) * Matrix4::Scale(Vector3(1,1,1)));
	o.AddTexture(Renderer::LoadTexture("smiley.png"));
	o.AddTexture(Renderer::LoadTexture("perlin.png"));
	o.AddTexture(Renderer::LoadTexture("noise.png"));
	r.AddRenderObject(o);

	Vector3 lightCol = Vector3(1, 1, 1);
	Vector3 lightPos = Vector3(100.0f, 100.0f, 0);
	float lightRad = 3000.0f;

	r.SetProjectionMatrix(Matrix4::Perspective(1, 100, 1.33f, 45.0f));
	r.SetMainLight(lightCol, lightPos, lightRad);
	r.SetViewMatrix(Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, -10)));


	while(w.UpdateWindow()) 
	{
		//float msec = w.GetTimer()->GetTimedMS();

		if (Keyboard::KeyTriggered(KeyboardKeys::KEY_RIGHT))
		{
			shaderInd = (++shaderInd) % shaders.size();
			o.SetShader(shaders[shaderInd]);
		}
		if (Keyboard::KeyTriggered(KeyboardKeys::KEY_LEFT))
		{
			--shaderInd;
			if (shaderInd < 0)
				shaderInd = shaders.size() - 1;
			o.SetShader(shaders[shaderInd]);
		}
		
		//Matrix4 p = Matrix4();
		//p.ToIdentity();
		//p.SetPositionVector(lightPos);
		//lightPos = (Matrix4::Rotation(0.1f * msec, Vector3(0, 1, 0)) * p).GetPositionVector();
		//r.SetMainLight(lightCol, lightPos, lightRad);
		o.SetModelMatrix(o.GetModelMatrix() * Matrix4::Rotation(0.1f * 100, Vector3(1, 1, 1)));

		r.UpdateScene(100);
		r.ClearBuffers();
		r.RenderScene();
		r.SwapBuffers();
	}
	delete m;
	for (int i = 0; i < shaders.size(); ++i)
		delete shaders[i];
}