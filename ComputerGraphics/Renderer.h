#pragma once

#include <cstdint>
#include "Color.h"

class Canvas;
class Scene;

/// <summary>
/// Renderer is responsible for "rendering" a scene, or converting 3D Objects to a 2D Canvas.
/// </summary>
class Renderer
{
public:
	Renderer(Canvas * canvas, Scene * scene) : m_canvas(canvas), m_scene(scene) { }

	void Clear();
	void Render();

	void ConvertLocalCoordinatesToCanvas(int32_t & xCoord, int32_t & yCoord);

	void SetCanvas(Canvas*  canvas) { m_canvas = canvas; }
	Canvas* GetCanvas() { return m_canvas; }

private:
	Scene* m_scene;
	Canvas * m_canvas;
	Color m_backgroundColor { 0, 0, 0 };
};

