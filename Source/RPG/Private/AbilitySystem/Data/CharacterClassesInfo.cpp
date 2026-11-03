#include "AbilitySystem/Data/CharacterClassesInfo.h"

FCharacterClassDefaultInfo UCharacterClassesInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
