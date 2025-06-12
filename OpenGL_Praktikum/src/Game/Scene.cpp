#include "Scene.h"
#include <AssetManager.h>
#include "../Framework/SceneElements/cube.h"
#include "OPENGL_PRAKTIKUM_CUBEWITHNORMALSCOLORED_H.h"

float rightArmRotationAngle = 0.0f;

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene()
{}

void Scene::loadShaders() {
	// Shader laden
	m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
	m_shader = m_assets.getShaderProgram("shader");
	m_shader->use();
}


void Scene::createTransforms() {
	// Szene aufbauen

	m_root = std::make_shared<Transform>();

	m_head = std::make_shared<Transform>();
	m_head->scale(glm::vec3(2.0f)); // <- Szene kleiner skalieren!

	m_torso = std::make_shared<Transform>();
	m_torso->translate(glm::vec3(0.0f, -2.75f, 0.0f));
	m_torso->scale(glm::vec3(2.0f, 4.0f, 1.0f));

	glm::vec3 arm(1.3f, 0.0f, 0.0f);

	m_leftUpperArm = std::make_shared<Transform>();
	m_leftUpperArm->translate(glm::vec3(-0.65f, 0.5f, 0.5f));
	m_leftUpperArm->scale(glm::vec3(0.25f, 0.2f, 1.0f));
	m_leftUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.4f, 0.0f), arm);


	m_leftLowerArm = std::make_shared<Transform>();
	m_leftLowerArm->translate(glm::vec3(glm::vec3(0.0f, 0.0f, 0.8f)));
	m_leftLowerArm->scale(glm::vec3(1.0f, 1.0f, 0.5f));


	m_rightUpperArm = std::make_shared<Transform>();
	m_rightUpperArm->translate(glm::vec3(0.65f, 0.3f, 0.5f));
	m_rightUpperArm->scale(glm::vec3(0.25f, 0.2f, 1.0f));
	m_rightUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), arm);

	m_rightLowerArm = std::make_shared<Transform>();
	m_rightLowerArm->translate(glm::vec3(0.0f, 0.0f, 0.8f));
	m_rightLowerArm->scale(glm::vec3(1.0f, 1.0f, 0.5f));

	m_leftLeg = std::make_shared<Transform>();
	m_leftLeg->translate(glm::vec3(-0.25f, -0.7f, 0.0f));
	m_leftLeg->scale(glm::vec3(0.25f, 0.4f, 0.25f));

	m_rightLeg = std::make_shared<Transform>();
	m_rightLeg->translate(glm::vec3(0.25f, -0.7f, 0.0f));
	m_rightLeg->scale(glm::vec3(0.25f, 0.4f, 0.25f));

	m_lightCube = std::make_shared<Transform>();
	m_lightCube->translate(glm::vec3(4.0f, 2.0f, 4.0f));
	m_lightCube->scale(glm::vec3(0.2f, 0.2f, 0.2f));


}

void Scene::createSceneGraph() {
	// Struktur aufbauen
	m_root->addChild(m_head);
	m_head->addChild(m_torso);
	m_torso->addChild(m_leftUpperArm);
	m_leftUpperArm->addChild(m_leftLowerArm);
	m_torso->addChild(m_rightUpperArm);
	m_rightUpperArm->addChild(m_rightLowerArm);
	m_torso->addChild(m_leftLeg);
	m_torso->addChild(m_rightLeg);
	m_root->addChild(m_lightCube);
}

void Scene::configureVaoVboNoNormals() {
	// VAO + VBO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

	glBindVertexArray(0);

	// Face Culling & Depth
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}
void Scene::configureVaoVboWithNormals() {
	// VAO + VBO
	// Generate and bind VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate and bind VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalCubeVert), normalCubeVert, GL_STATIC_DRAW);

	// Position attribute (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normal attribute (location = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Color attribute (location = 2)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind VAO
	glBindVertexArray(0);

	// Enable face culling and depth testing
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}

bool Scene::init()
{
	try
	{
		loadShaders();

		configureVaoVboWithNormals();

		createTransforms();

		createSceneGraph();

		std::cout << "Scene initialization done\n";
		return true;
	}
	catch (std::exception& ex)
	{
		throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}


void Scene::configureLighting() {
	glm::vec3 lightPos(4.0f, 2.0f, 4.0f);
	//glm::vec3 lightPos = glm::vec3(m_lightCube->getMatrix() * initialLightPos);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 matSpecular(0.8f, 0.2f, 0.6f);
	float matShinyness = 0.8f;
/*
	glm::mat4 lightCubeModelMatrix = m_lightCube->getMatrix();

	glm::vec3 lightPos = glm::vec3(lightCubeModelMatrix * initialLightPos);
*/
	glm::vec3 lightCubeColor(1.0f, 1.0f, 1.0f);

	m_shader->use();
	m_shader->setUniform("lightColor", lightCubeColor);
	m_shader->setUniform("lightPos", lightPos);
	m_shader->setUniform("lightColor", lightColor);
	m_shader->setUniform("matSpecular", matSpecular);
	m_shader->setUniform("matShinyness", matShinyness);
}

void Scene::configureCamera() {
	/*fovy zB. glm::radians(90.0f)	Weitwinkel: größere Perspektivverzerrung, mehr vom Raum sichtbar
	near z.B. 1.0f	Alles näher als 1.0 wird abgeschnitten (Clipping)
	far z.B. 10.0f	Alles weiter als 10.0 wird nicht gezeichnet
	aspectRatio	Falsches Seitenverhältnis verzerrt Bild (z.B. gestaucht/gestreckt)*/

	glm::mat4 view = glm::lookAt(
		glm::vec3(-10.0f, 5.0f, 30.0f), // Kameraposition
		glm::vec3(0.0f, 0.0f, 0.0f), //Blickpunkt
		glm::vec3(0.0f, 1.0f, 0.0f) //Up-Vektor
	);

	//Seitenverhältnis
	float aspectRatio = static_cast<float>(m_window->getFrameBufferWidth()) / static_cast<float>(m_window->getFrameBufferHeight());
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		aspectRatio,
		0.1f,
		100.0f);

	m_shader->setUniform("viewMatrix", view, false);
	m_shader->setUniform("projectionMatrix", projection, false);
}

void Scene::render(float dt)
{
	// Framebuffer löschen vor dem Zeichnen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader->use();

	configureCamera();


	configureLighting();


	// Hier keine Einzelzeichnung mehr nötig – alles passiert im Renderbaum
	renderNode(m_root, glm::mat4(1.0f));
}


void Scene::renderNode(std::shared_ptr<Transform> node, const glm::mat4& parentMatrix)
{
	glm::mat4 model = parentMatrix * node->getMatrix();
	m_shader->setUniform("modelMatrix", model, false);

	glBindVertexArray(m_vao);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	for (auto& child : node->getChildren())
	{
		renderNode(child, model);
	}
}

void Scene::update(float dt)
{
	static float time = 0.0f;
	time += dt;
	m_head->rotate(glm::vec3(0.0f, glm::radians(0.5f), 0.0f));

	glm::vec3 arm(0.01 * (sinf(time)), 0, 0);
	m_leftUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), arm);
	m_rightUpperArm->rotateAroundPoint(glm::vec3(0.0f, 0.5f, 0.0f), -arm);

	float legSwing = glm::radians(15.0f) * sinf(time * 2.0f);
	m_leftLeg->rotateAroundPoint(glm::vec3(0.25f, 0.0f, 0.0f), glm::vec3(0.05f * legSwing, 0.0f, 0.0f));
	m_rightLeg->rotateAroundPoint(glm::vec3(-0.25f, 0.0f, 0.0f), glm::vec3(0.05f * (-legSwing), 0.0f, 0.0f));
}

OpenGLWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{
}

void Scene::onMouseMove(MousePosition mouseposition)
{

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{

}
void Scene::shutdown()
{

}