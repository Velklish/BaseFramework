#pragma once
#include "Forward.h"

namespace BaseFramework {
	class GameComponent
	{
	public:
		virtual void Initialize(IGame* game) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void ClearResources() = 0;
	};
}

