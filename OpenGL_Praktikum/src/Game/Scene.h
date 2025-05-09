#pragma once

#include "OpenGLWindow.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>
#include "Framework/SceneElements/Transform.h"

class Scene
{
public:
	Scene(OpenGLWindow* window);
	~Scene();

	bool init();
	void shutdown();
	void render(float dt);
	void renderNode(std::shared_ptr<Transform> node, const glm::mat4& parentMatrix);
	void update(float dt);
	OpenGLWindow* getWindow();

	void onKey(Key key, Action action, Modifier modifier);
	void onMouseMove(MousePosition mouseposition);
	void onMouseButton(MouseButton button, Action action, Modifier modifier);
	void onMouseScroll(double xscroll, double yscroll);
	void onFrameBufferResize(int width, int height);

private:
	OpenGLWindow* m_window;
	AssetManager m_assets;
	ShaderProgram* m_shader;
	std::shared_ptr<Transform> m_cubeTransform;
	GLuint vaoID, vboID;
	GLuint m_vao = 0, m_vbo = 0, m_vio = 0;

	std::shared_ptr<Transform> m_root;
	std::shared_ptr<Transform> m_torso;
	std::shared_ptr<Transform> m_head;
	std::shared_ptr<Transform> m_leftUpperArm;
	std::shared_ptr<Transform> m_rightUpperArm;
	std::shared_ptr<Transform> m_leftLowerArm;
	std::shared_ptr<Transform> m_rightLowerArm;
	std::shared_ptr<Transform> m_leftLeg;
	std::shared_ptr<Transform> m_rightLeg;

};

