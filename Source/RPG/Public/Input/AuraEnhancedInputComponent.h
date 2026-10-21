// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include"AuraInputConfig.h"
#include "AuraEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	
	template<class UserClass,typename PressedFunctype,typename RealseFunctype,typename HeldFunctype>
	void BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFunctype PressedFunc, RealseFunctype RealseFunc, HeldFunctype HeldFunc);
	
};

template<class UserClass, typename PressedFunctype, typename RealseFunctype, typename HeldFunctype>
inline void UAuraEnhancedInputComponent::BindAbilityAction(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFunctype PressedFunc, RealseFunctype RealseFunc, HeldFunctype HeldFunc)
{
	check(InputConfig);

	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (RealseFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, RealseFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc,Action.InputTag);
			}

		}
	}
}
