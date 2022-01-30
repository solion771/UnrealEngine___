#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimeLineComponent.h"
#include "CAim.generated.h"

UCLASS()
class U02_ACTION_API UCAim : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		class UCurveFloat* Curve;

public:
	FORCEINLINE bool IsAvalidable() { return SpringArm != NULL && Camera != NULL; }

public:
	UCAim();
	void BeginPlay(class ACharacter* InCharacter);
	void Tick(float DeltaTime);

	void On();
	void Off();

private:
	void Zooming(float Value);

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bInZoom;

	FTimeline Timeline;
	FOnTimelineFloat TimelineFloat;

	class ACHUD* Hud;
};
