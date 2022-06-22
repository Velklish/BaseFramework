#pragma once
#include <SimpleMath.h>
#include "Forward.h"
#include "Transform.h"

namespace Framework 
{
	class XYZGameComponent
	{
	public:
		enum RotDirection
		{
			X,
			Y,
			Z
		};
		
		virtual void Initialize(IGame* game) = 0;
		virtual void Update(Transform transform) = 0;
		virtual void Draw() = 0;
		virtual void ClearResources() = 0;
		virtual void Rotate(RotDirection direction, float angle) = 0;
		virtual void Translate(float x, float y, float z) = 0;
		
		virtual DirectX::SimpleMath::Matrix GetWorld() = 0;
	};
}
