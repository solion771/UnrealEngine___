#include "CEquipment.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
}

void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (!!Data.AnimMontage)
	{
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRatio, Data.StartSection);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* character = Cast<IICharacter>(OwnerCharacter);
	CheckNull(character);
	//character->ChangeColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
	{
		OnEquipmentDelegate.Broadcast();
	}

}

void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();

	bEquipped = true;
}

void ACEquipment::Unequip_Implementation()
{
	bEquipped = false;

	State->SetIdleMode();

	if (!!Data.AnimMontage_Unequip)
	{
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage_Unequip, Data.PlayRatio, Data.StartSection);
		CLog::Log("Have__AnimMontage_Unequip");
	}
	else
	{
		Begin_Unequip();
		End_Unequip();
		CLog::Log("Dont_Have__AnimMontage_Unequip");
	}

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

}

void ACEquipment::Begin_Unequip_Implementation()
{
	if (OnUnequipmentDelegate.IsBound())
	{
		OnUnequipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Unequip_Implementation()
{
	State->SetIdleMode();
}