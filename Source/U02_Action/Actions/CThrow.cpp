#include "CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	InitialLifeSpan = 5.0f;

	Projectile->InitialSpeed = ThrowSpeed;
	Projectile->MaxSpeed = ThrowSpeed;
	Projectile->ProjectileGravityScale = 0.0f;

}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();
	
	Projectile->InitialSpeed = ThrowSpeed;
	Projectile->MaxSpeed = ThrowSpeed;
	Sphere->OnComponentBeginOverlap;

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components);
	for (USphereComponent* component : components)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);
	}



}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	CheckTrue(OtherActor == GetOwner());

	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Orange, OtherActor->GetName());

	if(!!Explosion)
	{
		FTransform transform = ExplosionTransform;
		transform.AddToTranslation(bFromSweep ? SweepResult.Location :GetActorLocation());
		transform.SetRotation(SweepResult.ImpactNormal.Rotation().Quaternion());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);


	Destroy();
}



