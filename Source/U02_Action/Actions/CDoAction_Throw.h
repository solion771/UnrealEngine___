#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Throw.generated.h"

UCLASS()
class U02_ACTION_API ACDoAction_Throw : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;

public:
	virtual void DoAction()override;
	virtual void Begin_DoAction()override;
	virtual void End_DoAction()override;

	virtual void Tick(float Deltatime)override;

	virtual void OnAim()override;
	virtual void OffAim()override;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);

private:
	UPROPERTY()
		class UCAim* Aim;
};
