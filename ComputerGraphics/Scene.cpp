#include "Scene.h"

Scene::Scene()
{
	m_objects.reserve(100);
}

Scene::~Scene()
{
	for (SceneObject * object : m_objects)
	{
		delete object;
	}
}
