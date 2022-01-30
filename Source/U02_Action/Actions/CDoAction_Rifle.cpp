#include "CDoAction_Rifle.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "CAim.h"
#include "CThrow.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"

ACDoAction_Rifle::ACDoAction_Rifle()
{
	CHelpers::GetClass<UCameraShake>(&CameraShakeClass, "Blueprint'/Game/Actions/BP_CameraShake_Rifle.BP_CameraShake_Rifle_C'");
	CHelpers::GetAsset<USoundCue>(&FireSoundCue, "SoundCue'/Game/Sounds/S_RifleShoot_Cue.S_RifleShoot_Cue'");

	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "ParticleSystem'/Game/Particles_Rifle/Particles/VFX_Muzzleflash.VFX_Muzzleflash'");
	CHelpers::GetAsset<UParticleSystem>(&EjectParticle, "ParticleSystem'/Game/Particles_Rifle/Particles/VFX_Eject_bullet.VFX_Eject_bullet'");
	CHelpers::GetAsset<UParticleSystem>(&ImpactParticle, "ParticleSystem'/Game/Particles_Rifle/Particles/VFX_Impact_Default.VFX_Impact_Default'");

}

void ACDoAction_Rifle::BeginPlay()
{
	Super::BeginPlay();

	
	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);
}

void ACDoAction_Rifle::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	Begin_DoAction();
	End_DoAction();

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Rifle::Begin_DoAction()
{
	USkeletalMeshComponent* mesh = CHelpers::GetComponent<USkeletalMeshComponent>(attachment);

	FVector location = mesh->GetSocketLocation("MuzzleFlash");
	FVector muzzleLocation = mesh->GetSocketLocation("MuzzleFlash");

	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();

	//ÃÑ¾Ë direction
	FVector start, end, direction;
	ACPlayer* player = Cast<ACPlayer>(GetOwner());
	player->GetAimRay(start, end, direction);

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(direction.Rotation()));

	//APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	//controller->PlayerCameraManager->PlayCameraShake(CameraShakeClass);
	

	UGameplayStatics::SpawnEmitterAttached(FlashParticle, mesh, "MuzzleFlash", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	UGameplayStatics::SpawnEmitterAttached(EjectParticle, mesh, "EjectBullet", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSoundCue, location);
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Rifle::OnThrowBeginOverlap);

}

void ACDoAction_Rifle::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Rifle::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	Aim->Tick(Deltatime);
}

void ACDoAction_Rifle::OnAim()
{
	Aim->On();
}

void ACDoAction_Rifle::OffAim()
{
	Aim->Off();
}

void ACDoAction_Rifle::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);

}
