#pragma once

namespace mag
{
	template <typename... Args>
	class IFunctionCaller
	{
	public:
		virtual void Call(const Args&... args) = 0;
	};
}
