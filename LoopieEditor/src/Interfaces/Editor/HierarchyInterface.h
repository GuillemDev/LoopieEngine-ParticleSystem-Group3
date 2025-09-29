#pragma once 

#include "src/Interfaces/Interface.h"

namespace Loopie {
	class HierarchyInterface : public Interface {
	public:
		HierarchyInterface();
		~HierarchyInterface() = default;
		void Render() override;
	};
}