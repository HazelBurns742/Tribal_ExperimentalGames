// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardData.h"
#include "CardManager.generated.h"


UCLASS(Blueprintable)
class MEDIVALCONQUEST_API ACardManager : public AActor
{
	GENERATED_BODY()

public:
	ACardManager();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	void LoadCardData(const FString& FilePath); //Func loads card data from JSON

	TArray<UCardData*> CardDataList; //Array holds all loaded card data

	UPROPERTY(Replicated)
	TArray<FCardDataToReplicate> ReplicatedCardDataList;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool isCardDataLoaded = false;
};
