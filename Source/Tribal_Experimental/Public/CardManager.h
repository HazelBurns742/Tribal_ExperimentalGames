// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardData.h"
#include "CardManager.generated.h" 
/**
 * 
 */

UCLASS(Blueprintable)
class TRIBAL_EXPERIMENTAL_API ACardManager: public AActor
{

	GENERATED_BODY()

public:

	ACardManager();
	
protected:
	virtual void BeginPlay() override;

public:
	void LoadCardData(const FString & FilePath);

	UCardData* GetRandomCard(); 

private: 
	TArray<UCardData*> CardDataList; 

};
