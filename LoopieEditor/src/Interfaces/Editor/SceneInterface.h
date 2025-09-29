#pragma once 

#include "src/Interfaces/Interface.h"

namespace Loopie {
	class SceneInterface : public Interface {
	public:
		SceneInterface();
		~SceneInterface() = default;
		void Render() override;
	};
}