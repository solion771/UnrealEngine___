#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CActionComponent.h"
#include "CUserWidget_Weapon.generated.h"

UCLASS()
class U02_ACTION_API UCUserWidget_Weapon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void ChangeWeapon(EActionType Type);
};
