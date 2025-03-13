// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardData.h"
#include "CardManager.h" 
#include "TurnManager.generated.h"

class UCardDisplay; 

USTRUCT(BlueprintType)
struct FClientData
{
	GENERATED_BODY()

	// Client data variables

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Client Data")
	FString ClientID;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Client Data") 
	TArray<FCardDataToReplicate> HandOfCards; 
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

private: 
	TArray<FCardDataToReplicate> MyCardDeck;
	int32 MyCardDeckPointer; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Management")
	ACardManager* CardManager;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TurnManager")
	int32 ClientNum;

	UFUNCTION(BlueprintCallable, Category = "Turn Manager")
	void AddClientToSession();


	void SetClientHand(FClientData& Client);

	void ShuffleMyDeck(TArray<FCardDataToReplicate> DeckToShuffle); 

	//Viewports and display
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Turn Manager")
	UCardDisplay* CachedCardDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager")
	TSubclassOf<UCardDisplay> CardDisplayClass;

	UFUNCTION(BlueprintCallable, Category = "Turn Manager")
	void CreateAndAddWidgetToViewport();

};
