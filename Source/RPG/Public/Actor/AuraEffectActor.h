// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EffectApplicationPolicy:uint8
{
	ApplyOnOverlay,
	ApplyOnEndOverlay,
	DoNotApply
};

UENUM(BlueprintType)
enum class EffectRemovePolicy:uint8
{
	RemoveOnEndOverlay,
	DoNotRemove
};

UCLASS()
class RPG_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GamepalyEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlay(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlay(AActor* TargetActor);


	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGamepalyEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EffectApplicationPolicy InstantEffectApplicationPolicy=EffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EffectApplicationPolicy DurationEffectApplicationPolicy = EffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EffectApplicationPolicy InfiniteEffectApplicationPolicy = EffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EffectRemovePolicy InfiniteEffectRemovePolicy=EffectRemovePolicy::RemoveOnEndOverlay;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*>ActiveEffectHandles;

	
};
