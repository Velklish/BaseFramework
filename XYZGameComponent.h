#pragma once
#include <SimpleMath.h>
#include "Forward.h"

namespace BaseFramework 
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
		virtual void Update(DirectX::SimpleMath::Matrix m_world, DirectX::SimpleMath::Matrix m_view, DirectX::SimpleMath::Matrix m_proj) = 0;
		virtual void Draw() = 0;
		virtual void ClearResources() = 0;
		virtual void Rotate(RotDirection direction, float angle) = 0;
		virtual void Translate(float x, float y, float z) = 0;
		
		virtual DirectX::SimpleMath::Matrix GetWorld() = 0;
	};
}
