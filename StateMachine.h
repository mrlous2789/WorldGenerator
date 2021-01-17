#pragma once
#include <memory>
#include <stack>
#include <iostream>

#include "State.h"

namespace Mer
{
	typedef std::unique_ptr<State> StateRef;

	class StateMachine
	{
	public:
		StateMachine(){ }
		~StateMachine(){ }

		void AddState(StateRef newState, bool isReplacing = true);
		void RemoveState();

		void ProcessStateChanges();

		StateRef& GetActiveState();
	private:
		std::stack<StateRef> _states;
		StateRef _newState;

		//used by process state changes to decide what to do
		bool _isRemoving = false;
		bool _isAdding = false;
		bool _isReplacing = false;
	};
}


