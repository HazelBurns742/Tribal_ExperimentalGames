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
	TArray<FCardDataToReplicate> MyCardDeck;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Client Data")
	TArray<FCardDataToReplicate> HandOfCards;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Client Data")
	int32 ActionPoints;
};

UCLASS(Blueprintable)
class MEDIVALCONQUEST_API ATurnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties 
	ATurnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	bool bHasSpawnedTurnManager = false;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Add a new local player (max 4 suggested) 
	UFUNCTION(BlueprintCallable)
	void AddPlayer();

	// Start the current players turn 
	UFUNCTION(BlueprintCallable)
	void StartTurn();

	// End the current players turn and move to the next 
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	//Called when a player chooses a card 
	UFUNCTION(BlueprintCallable)
	void PlayerChoseCard(FString ChosenCardName);

	// Shuffle a deck and store in manager memory 
	UFUNCTION(BlueprintCallable)
	void ShuffleMyDeck(TArray<FCardDataToReplicate> DeckToShuffle);

	// Draw cards and fill player hand 
	UFUNCTION(BlueprintCallable)
	void SetPlayerHand(FClientData& Client);

	// Total number of players, set in Blueprint before calling 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager|Settings")
	int32 amountOfPlayers = 2;

	//Current player (player's turn)
	UPROPERTY(BlueprintReadWrite, Category = "Turn Manager|State")
	int32 currentPlayer = 0;

protected:
	//Shared card manager used to distribute decks 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager")
	ACardManager* CardManager;

	// The UI widget used to display cards 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Manager")
	TSubclassOf<UCardDisplay> CardDisplayClass;

	// Active UI display widget instance 
	UPROPERTY(BlueprintReadOnly, Category = "Turn Manager")
	UCardDisplay* CachedCardDisplay;

	// All players (turn-based, local) 
	UPROPERTY(BlueprintReadOnly, Category = "Turn Manager")
	TArray<FClientData> Clients;

	// Whose turn is it? Index in Clients[] 
	UPROPERTY(BlueprintReadOnly, Category = "Turn Manager")
	int32 currentTurn;

	// Shared deck (shuffled once per game) 
	UPROPERTY(BlueprintReadOnly, Category = "Turn Manager|Deck")
	TArray<FCardDataToReplicate> MyCardDeck;

	// Pointer for drawing from deck 
	UPROPERTY(BlueprintReadOnly, Category = "Turn Manager|Deck")
	int32 MyCardDeckPointer;
};