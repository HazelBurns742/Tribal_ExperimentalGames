// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardData.h"
#include "CardManager.generated.h"


UCLASS(Blueprintable)
class TRIBAL_EXPERIMENTAL_API ACardManager : public AActor
{
	GENERATED_BODY()

public:
	ACardManager();
	
protected: 
	virtual void BeginPlay() override; 

public:
	void LoadCardData(const FString& FilePath); //Func loads card data from JSON
	UCardData* RandomCard(); //Func gets random card
	UCardData* SetHandOfCards();


private:
	TArray<UCardData*> CardDataList; //Array holds all loaded card data
};
