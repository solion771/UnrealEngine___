#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class U02_ACTION_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void ClearCombo();

public:
	virtual void DoAction()override;
	virtual void Begin_DoAction()override;
	virtual void End_DoAction()override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackerCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackerCauser, class ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
		void ResetGlobalTime();

private:
	int32 Index;

	bool bEnable;
	bool bExist;
	bool bLast;

	TArray<class ACharacter*> HittedCharacter;
};
