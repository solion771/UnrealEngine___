#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class U02_ACTION_API ACEnemy : public ACharacter , public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UserSettings")
		bool bDrawName = false;

	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 100.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;


private: //Actor Component
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

public:
	ACEnemy();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void ResetColor();

public:
	virtual void ChangeColor(FLinearColor InColor)override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();
	void Dead();
	void End_Dead() override;

private:
	class UMaterialInstanceDynamic* HeadMaterial;
	class UMaterialInstanceDynamic* TorsoMaterial;
	class UMaterialInstanceDynamic* LegsMaterial;
	class UMaterialInstanceDynamic* Accessory1Material;
	class UMaterialInstanceDynamic* Accessory2Material;

private:
	class AController* DamageInstigator;
};
