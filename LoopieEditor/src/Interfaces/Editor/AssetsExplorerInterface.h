#pragma once 

#include "src/Interfaces/Interface.h"

namespace Loopie {
	class AssetsExplorerInterface : public Interface {
	public:
		AssetsExplorerInterface();
		~AssetsExplorerInterface() = default;
		void Render() override;
	};
}