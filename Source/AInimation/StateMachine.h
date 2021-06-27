#pragma once

#include "CoreMinimal.h"
#include "State.h"

/**
 * 
 */
template<class T>
class AINIMATION_API StateMachine
{
	T* m_pOwner;

	State<T>* m_pCurrentState;

	// A record of the last state the agent was in
	State<T>* m_pPreviousState;

	// This state logic is called every time the FSM is updated
	State<T>* m_pGlobalState;

public:
	StateMachine(T* pOwner) : m_pOwner(pOwner),
							 m_pCurrentState(NULL),
							 m_pPreviousState(NULL),
							 m_pGlobalState(NULL)
	{}

	// Use these setter methods to initialize the FSM
	void SetCurrentState(State<T>* pState) { m_pCurrentState = pState; }
	void SetPreviousState(State<T>* pState) { m_pPreviousState = pState; }
	void SetGlobalState(State<T>* pState) { m_pGlobalState = pState; }

	// Call to update the FSM
	void Update() const
	{
		// If a global state exists, call its execute method
		if (m_pGlobalState) m_pGlobalState->ExecuteState(m_pOwner);

		// Same for the current state
		if (m_pCurrentState) m_pCurrentState->ExecuteState(m_pOwner);
	}

	// Change to a new state
	void ChangeState(State<T>* pNewState)
	{
		check(pNewState);

		// Keep record of the previous state
		m_pPreviousState = m_pCurrentState;

		// We exit the current state
		m_pCurrentState->ExitState(m_pOwner);

		// We change to the new state
		m_pCurrentState = pNewState;

		// We enter the current state
		m_pCurrentState->EnterState(m_pOwner);
	}

	// Change state back to the previous state
	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	// Getters
	State<T>* GetCurrentState() const { return m_pCurrentState; };
	State<T>* GetPreviousState() const { return m_pPreviousState; };
	State<T>* GetGlobalState() const { return m_pGlobalState; };

	// Check current state type
	bool IsInState(const State<T>& state) const { return (m_pCurrentState == state); }

	~StateMachine();
};
