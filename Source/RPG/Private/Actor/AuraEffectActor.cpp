#include "Actor/AuraEffectActor.h"
#include"AbilitySystemInterface.h"
#include"AbilitySystem/AuraAttributeSet.h"
#include"AbilitySystemBlueprintLibrary.h"
#include"AbilitySystem/AuraAbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();


}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamepalyEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)return;

	check(GamepalyEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GamepalyEffectClass, 1.0f, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovePolicy == EffectRemovePolicy::RemoveOnEndOverlay)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlay(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, InstantGamepalyEffectClass);
	}

	if (DurationEffectApplicationPolicy == EffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlay(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, InstantGamepalyEffectClass);
	}

	if (DurationEffectApplicationPolicy == EffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EffectApplicationPolicy::ApplyOnEndOverlay)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovePolicy == EffectRemovePolicy::RemoveOnEndOverlay)
	{
		UAbilitySystemComponent *TargetASC=	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!TargetASC)return;
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

