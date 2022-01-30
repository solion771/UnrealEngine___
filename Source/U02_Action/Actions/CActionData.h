#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CAction.h"
#include "CActionData.generated.h"



UCLASS()
class U02_ACTION_API UCActionData : public UDataAsset
{
	GENERATED_BODY()



public:
	void BeginPlay(class ACharacter* InOwnerCharacter,class UCAction** OutAction);

private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InName);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

};
