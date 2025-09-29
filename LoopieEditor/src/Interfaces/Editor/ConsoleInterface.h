#pragma once 

#include "src/Interfaces/Interface.h"

namespace Loopie {
	class ConsoleInterface : public Interface {
	public:
		ConsoleInterface();
		~ConsoleInterface() = default;
		void Render() override;
	};
}