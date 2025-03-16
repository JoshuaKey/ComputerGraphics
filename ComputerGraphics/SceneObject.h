#pragma once

#include "Transform.h"

/// <summary>
/// Base Object for all Objects shown in the Scene
/// </summary>
class SceneObject
{
public:
	SceneObject() : m_transform() { }
	SceneObject(Transform const& transform) : m_transform(transform) { }
	virtual ~SceneObject() { }

public:
	Transform m_transform;
};

