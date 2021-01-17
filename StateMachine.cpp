#include "StateMachine.h"
namespace Mer
{
	void StateMachine::AddState(StateRef newState, bool isReplacing)
	{
		this->_isAdding = true;
		this->_isReplacing = isReplacing;
		this->_newState = std::move(newState);//moves new state to _newState will be added to the stack in ProcessStateChanges
	}
	void StateMachine::RemoveState()
	{
		this->_isRemoving = true;
	}

	void StateMachine::ProcessStateChanges()//just decides what to do based on the bool variables
	{
		if (this->_isRemoving && !this->_states.empty())
		{
			this->_states.top()->CleanUp();
			this->_states.pop();

			if (!this->_states.empty())
			{
				this->_states.top()->Resume();
			}

			this->_isRemoving = false;
		}
		if (this->_isAdding)
		{
			if (!this->_states.empty())
			{
				if (this->_isReplacing)
				{
					this->_states.top()->CleanUp();
					this->_states.pop();
				}
				else
				{
					this->_states.top()->Pause();
				}
			}

			this->_states.push(std::move(this->_newState));
			this->_states.top()->Init();
			this->_isAdding = false;

		}
	}

	StateRef& StateMachine::GetActiveState()
	{
		return this->_states.top();
	}
}