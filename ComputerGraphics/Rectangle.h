#pragma once

#include "SceneObject.h"
#include "Color.h"

class Rect : public SceneObject
{
public:
	Rect() : SceneObject(){ }
	Rect(Transform const& transform, float width = 1.0f, float height = 1.0f) : SceneObject(transform), m_width(width), m_heigth(height) { }

public:
	void SetHeight(float value) { m_heigth = value; }
	float GetHeight() const { return m_heigth; }

	void SetWidth(float value) { m_width = value; }
	float GetWidth() const { return m_width; }

	void SetColor(Color const& value) { m_color = value; }
	Color GetColor() const { return m_color; }

private:
	float m_width = 1.0f;
	float m_heigth = 1.0f;
	Color m_color = { 0,0,0 };
};

