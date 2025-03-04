// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManager.h"
#include "CardManager.h"



// Sets default values
ATurnManager::ATurnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurnManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}


void ATurnManager::AddClientToSession()
{
	FClientData NewClient; 
	NewClient.ClientID = FString(TEXT("Player")) + FString::FromInt(ClientNum); 
	UE_LOG(LogTemp, Display, TEXT("Number of clients: %d"), ClientNum); 

	UE_LOG(LogTemp, Display, TEXT("Added client %s"), *NewClient.ClientID);
	//SetClientHand(NewClient);
}

void ATurnManager::SetClientHand(FClientData& Client) {

	int32 DefaultHandSize = 5;  
	int32 PlayerHandSize = Client.HandOfCards.Num();

	if (PlayerHandSize < DefaultHandSize) {
		UE_LOG(LogTemp, Display, TEXT("Hand NOT full"));

		int32 NumCardsToDraw = DefaultHandSize - PlayerHandSize; 
		for (int32 i = 0; i < NumCardsToDraw; i++) {
			
			UCardData* NewCard = CardManager->RandomCard(); 
			Client.HandOfCards.Add(NewCard);
		
		}
		UE_LOG(LogTemp, Display, TEXT("Hand now full"));
	}

	//if (PlayerHandSize > DefaultHandSize) {
	// UE_LOG(LogTemp, Display, TEXT("Too many cards in hand"));
	// 
	//	int32 NumCardsToDiscard = PlayerHandSize - DefaultHandSize; 
	//	for (int32 i = 0; i < NumCardsToDiscard; i++) {
	//		bool toldToDiscard = false;
	//		if (!toldToDiscard) {
	//			//Tell player to discard
	//			toldToDiscard = true;
	//		}

	//	}
	//}
}
