#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Rifle.generated.h"

UCLASS()
class U02_ACTION_API ACDoAction_Rifle : public ACDoAction
{
	GENERATED_BODY()
	
public:
	ACDoAction_Rifle();

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
	UPROPERTY(EditDefaultsOnly, Category = "Rifle")
		class USoundCue* FireSoundCue;

	UPROPERTY(EditDefaultsOnly, Category = "Rifle")
		TSubclassOf<class UCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Rifle")
		class UParticleSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Rifle")
		class UParticleSystem* EjectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Rifle")
		class UParticleSystem* ImpactParticle;

private:
	UPROPERTY()
		class UCAim* Aim;
};
