#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Warp.generated.h"

UCLASS()
class U02_ACTION_API ACDoAction_Warp : public ACDoAction
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;

public:
	virtual void DoAction()override;
	virtual void Begin_DoAction()override;
	virtual void End_DoAction()override;

	virtual void Tick(float Deltatime)override;

private:
	class ACPlayer* UserCursorLocation();
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator);

private:

	class UDecalComponent* Decal;
	FVector Location;

};
