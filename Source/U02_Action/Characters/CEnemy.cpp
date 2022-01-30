#include "CEnemy.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"

ACEnemy::ACEnemy()
{
	//-----------------------------------------------------
	//Create SceneComp
	//-----------------------------------------------------
	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());


	//-----------------------------------------------------
	//Create ActorComp
	//-----------------------------------------------------
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	
	//-----------------------------------------------------
	//Comp Settings
	//-----------------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/EnemyCharacter/Grux.Grux'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	//Abort By Damage
	Action->AbortByDamage();

	//Adjust Health Widget
	Status->SubHealth(damage);

	//Set Dead State
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0;
	}

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy::BeginPlay()
{
	
	UMaterialInstanceConstant* head;
	UMaterialInstanceConstant* torso;
	UMaterialInstanceConstant* legs;
	UMaterialInstanceConstant* accessory1;
	UMaterialInstanceConstant* accessory2;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&head, "MaterialInstanceConstant'/Game/ParagonGrux/Characters/Heroes/Grux/Materials/MI_Grux_Head.MI_Grux_Head'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&torso, "MaterialInstanceConstant'/Game/ParagonGrux/Characters/Heroes/Grux/Materials/MI_Grux_Torso.MI_Grux_Torso'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&legs, "MaterialInstanceConstant'/Game/ParagonGrux/Characters/Heroes/Grux/Materials/MI_Grux_LegsHands.MI_Grux_LegsHands'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&accessory1, "MaterialInstanceConstant'/Game/ParagonGrux/Characters/Heroes/Grux/Materials/MI_Grux_Accessory.MI_Grux_Accessory'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&accessory2, "MaterialInstanceConstant'/Game/ParagonGrux/Characters/Heroes/Grux/Materials/MI_Grux_Accessory.MI_Grux_Accessory'");

	HeadMaterial = UMaterialInstanceDynamic::Create(head, this);
	TorsoMaterial = UMaterialInstanceDynamic::Create(torso, this);
	LegsMaterial = UMaterialInstanceDynamic::Create(legs, this);
	Accessory1Material = UMaterialInstanceDynamic::Create(accessory1, this);
	Accessory2Material = UMaterialInstanceDynamic::Create(accessory2, this);

	GetMesh()->SetMaterial(0, HeadMaterial);
	GetMesh()->SetMaterial(1, TorsoMaterial);
	GetMesh()->SetMaterial(2, LegsMaterial);
	GetMesh()->SetMaterial(3, Accessory1Material);
	GetMesh()->SetMaterial(4, Accessory2Material);


	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	//ActionComponent에서 APlayer로 Casting하는 부분이 있어 Enemy로 받을수 있는 Component 필요함
	//Action->SetUnarmedMode();

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetName());
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetControllerText(GetController()->GetName());
	
	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	NameWidget->SetVisibility(bDrawName);
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	HeadMaterial->SetScalarParameterValue("UseLight", State->IsHittedMode() ? 1 : 0);
	TorsoMaterial->SetScalarParameterValue("UseLight", State->IsHittedMode() ? 1 : 0);
	LegsMaterial->SetScalarParameterValue("UseLight", State->IsHittedMode() ? 1 : 0);
	Accessory1Material->SetScalarParameterValue("UseLight", State->IsHittedMode() ? 1 : 0);
	Accessory2Material->SetScalarParameterValue("UseLight", State->IsHittedMode() ? 1 : 0);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{

	switch (InNewType)
	{
	case EStateType::Hitted: Hitted();	break;
	case EStateType::Dead:	Dead();	break;
	}
}

void ACEnemy::Hitted()
{

	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());

	//Force SetMove
	Status->SetMove();

	//Play Hit Montage
	Montages->PlayHitted();

	//Launch Character
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LaunchValue, true, false);

	//Change Logo Material
	ChangeColor(FLinearColor::Red);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetColor", 1.0f, false);
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());
	CLog::Print("Dead");

	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDead();
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();
	Destroy();
}

void ACEnemy::ResetColor()
{
	HeadMaterial->SetScalarParameterValue("UseLight",0);
	TorsoMaterial->SetScalarParameterValue("UseLight",0);
	LegsMaterial->SetScalarParameterValue("UseLight",0);
	Accessory1Material->SetScalarParameterValue("UseLight",0);
	Accessory2Material->SetScalarParameterValue("UseLight",0);
}