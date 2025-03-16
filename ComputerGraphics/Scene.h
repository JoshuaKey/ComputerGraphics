#pragma once

#include <memory>
#include <vector>
#include "SceneObject.h"
#include "Camera.h"

/// <summary>
/// The Scene, aka "The World", is responsible for holding all Objects that may be drawn.
/// It also contains a Main Camera responsible for determine the perspective of other Objects
/// </summary>
class Scene
{
public:
	Scene();
	~Scene();

	template<typename T>
	SceneObject* AddObject()
	{
		m_objects.push_back(new T());

		return m_objects.back();
	}

	std::vector<SceneObject *> const & GetObjects() { return m_objects; }

	Camera& GetCamera() { return m_camera; }

private:
	std::vector<SceneObject *> m_objects;
	Camera m_camera;
};

