// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardData.h"
#include "CardManager.h" 
#include "TurnManager.generated.h"

USTRUCT(BlueprintType)
struct FClientData
{
	GENERATED_BODY()

	// Client data variables

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Client Data")
	FString ClientID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Client Data") 
	TArray<UCardData*> HandOfCards; 
};



UCLASS()
class TRIBAL_EXPERIMENTAL_API ATurnManager : public AActor
{
	GENERATED_BODY() 
	
public:	
	// Sets default values for this actor's properties 
	ATurnManager();  
	 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Management")
	ACardManager* CardManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Clients")
	TArray<FClientData> AllClients; 

	UFUNCTION(BlueprintCallable, Category = "Turn Manager")
	void AddClientToSession();

	void SetClientHand(FClientData& Client);
};
