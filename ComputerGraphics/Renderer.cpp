#include "Renderer.h"
#include "Canvas.h"
#include "Scene.h"
#include "Rectangle.h"

#include <algorithm>

namespace
{
	bool ZSort(SceneObject const * const a, SceneObject const* const b)
	{
		return a->m_transform.z < b->m_transform.z;
	}
}

void Renderer::Clear()
{
	for (uint32_t y = 0; y < m_canvas->HEIGHT; y++)
	{
		for (uint32_t x = 0; x < m_canvas->WIDTH; x++)
		{
			m_canvas->PutPixel(x, y, m_backgroundColor);
		}
	}
}

void Renderer::Render()
{
	if (!m_canvas || !m_scene)
	{
		return;
	}

	Clear();

	Camera const & camera = m_scene->GetCamera();

	// Zort them by Z Axis
	std::vector<SceneObject *> objects = m_scene->GetObjects();
	std::sort(objects.begin(), objects.end(), ZSort);

	// Iterate through the Scene Objects
	for (SceneObject const * const object : objects)
	{
		float xCoord = object->m_transform.x - camera.m_transform.x;
		float yCoord = object->m_transform.y - camera.m_transform.y;

		// Onky suport Rectangle for now...
		Rect const * const rect = dynamic_cast<Rect const * const>(object);
		if (rect)
		{
			float left = xCoord - rect->GetWidth() / 2.0f;
			float right = left + rect->GetWidth();
			float top = yCoord - rect->GetHeight() / 2.0f;
			float bottom = top + rect->GetHeight();

			// A VERY CRUDE WAY TO DRAW A RECTANGLE
			for (float y = top; y < bottom; y++)
			{
				for (float x = left; x < right; x++)
				{
					int32_t xPixel = static_cast<uint32_t>(x);
					int32_t yPixel = static_cast<uint32_t>(y);
					ConvertLocalCoordinatesToCanvas(xPixel, yPixel);
					m_canvas->PutPixel(xPixel, yPixel, rect->GetColor());
				}
			}
		}
	}
}

void Renderer::ConvertLocalCoordinatesToCanvas(int32_t& xCoord, int32_t& yCoord)
{
	if (!m_canvas)
	{
		return;
	}

	xCoord += m_canvas->WIDTH / 2;
	yCoord += m_canvas->HEIGHT / 2;
}
