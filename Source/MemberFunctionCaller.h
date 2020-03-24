#pragma once

#include "IFunctionCaller.h"

namespace mag
{
	template <class T, typename... Args>
	class MemberFunctionCaller : public IFunctionCaller<Args...>
	{
	public:
		MemberFunctionCaller(T* object, void(T::* function)(Args...));

		void Call(const Args&... args) override;

	private:
		T* _object;
		void(T::* _function)(Args...);
	};

	template <typename T, typename... Args>
	MemberFunctionCaller<T, Args...>::MemberFunctionCaller(T* object, void(T::* function)(Args...)) :
		_object(object), _function(function)
	{

	}

	template <typename T, typename... Args>
	void MemberFunctionCaller<T, Args...>::Call(const Args&... args)
	{
		(_object->*_function)(args...);
	}
}