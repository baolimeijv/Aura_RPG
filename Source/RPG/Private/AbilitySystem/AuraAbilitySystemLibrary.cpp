#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include"AbilitySystemComponent.h"
#include"Game/AuraGameModeBase.h"

void UAuraAbilitySystemLibrary::InitializedDefaultAttributes(const UObject*WorldContextObject,ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode=Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGameMode)return; 

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassesInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttribnutesContextHandle = ASC->MakeEffectContext();
	PrimaryAttribnutesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle= ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttribnutesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttribnutesContextHandle = ASC->MakeEffectContext();
	SecondaryAttribnutesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttribnutesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttribnutesContextHandle = ASC->MakeEffectContext();
	VitalAttribnutesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttribnutesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGameMode)return;

	UCharacterClassesInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	for (TSubclassOf<UGameplayAbility>AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

}
