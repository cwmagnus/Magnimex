#pragma once

#include "IFunctionCaller.h"

namespace mag
{
	template <typename... Args>
	class FunctionCaller : public IFunctionCaller<Args...>
	{
	public:
		FunctionCaller(void(*function)(Args...));

		void Call(const Args&... args) override;

	private:
		void(*_function)(Args...);
	};

	template <typename... Args>
	FunctionCaller<Args...>::FunctionCaller(void(*function)(Args...)) :
		_function(function)
	{

	}

	template <typename... Args>
	void FunctionCaller<Args...>::Call(const Args&... args)
	{
		(*_function)(args...);
	}
}