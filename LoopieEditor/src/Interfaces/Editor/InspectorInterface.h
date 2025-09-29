#pragma once 

#include "src/Interfaces/Interface.h"

namespace Loopie {
	class InspectorInterface : public Interface {
	public:
		InspectorInterface();
		~InspectorInterface() = default;
		void Render() override;
	};
}