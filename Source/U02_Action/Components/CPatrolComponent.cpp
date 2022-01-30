#include "CPatrolComponent.h"
#include "Global.h"
#include "Actions/CPatrolPath.h"
#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{

}

void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();


}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadiuse)
{
	OutLocation = FVector::ZeroVector;
	OutAcceptanceRadiuse = AcceptanceRadiuse;
	CheckNullResult(Path, false);

	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 count = Path->GetSpline()->GetNumberOfSplinePoints();

	//Reverse
	if (bReverse)
	{
		//Exsist Next
		if (Index > 0)
		{
			Index--;
			return;
		}

		//At Last and Loop
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		//At Last and not Loop
		Index = 1;
		bReverse = false;
		return;
	}

	//Forward
	//Exsist Next
	if (Index < count - 1)
	{
		Index++;
		return;
	}

	//At Last and Loop
	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//At Last and not Loop
	Index = count - 2;
	bReverse = true;

}

