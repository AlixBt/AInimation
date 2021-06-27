#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template<class T>
class AINIMATION_API State
{
public:
	virtual ~State() {}

	// This will execute when the state is entered
	virtual void EnterState(T*) = 0;

	// This is called by the AIController tick function
	virtual void ExecuteState(T*) = 0;

	// This wiil execute when the state is exited
	virtual void ExitState(T*) = 0;
};
