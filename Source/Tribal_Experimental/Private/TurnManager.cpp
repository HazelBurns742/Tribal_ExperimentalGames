// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManager.h"
#include "CardManager.h"
#include "CardDisplay.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h" 




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
	CardManager = Cast<ACardManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACardManager::StaticClass()));
}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

void ATurnManager::CreateAndAddWidgetToViewport(){
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); 

	if (PlayerController) {
	CachedCardDisplay = CreateWidget<UCardDisplay>(PlayerController, CardDisplayClass);
		if (CachedCardDisplay)
		{
			CachedCardDisplay->AddToViewport();
			PlayerController->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			PlayerController->SetInputMode(InputMode);

			UE_LOG(LogTemp, Display, TEXT("CardDisplay Widget added to player viewport"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create CardDisplay widget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid player controller"));
	}
}


void ATurnManager::AddClientToSession()
{
	FClientData NewClient; 
	NewClient.ClientID = FString(TEXT("Player")) + FString::FromInt(ClientNum); 
	UE_LOG(LogTemp, Display, TEXT("Number of clients: %d"), ClientNum); 

	UE_LOG(LogTemp, Display, TEXT("Added client %s"), *NewClient.ClientID);

	if (CardManager != nullptr) {
		MyCardDeck = CardManager->ReplicatedCardDataList; // if shuffling the card list, make hard copy here
		if (!MyCardDeck[1].Name.IsEmpty()) {
			UE_LOG(LogTemp, Display, TEXT("My Card Deck [1] is %s"), *MyCardDeck[1].Name);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("MyCardDeck[1] is null at start too"));
		}
		ShuffleMyDeck(MyCardDeck);
		MyCardDeckPointer = 0;
		SetClientHand(NewClient);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Card Manger was null, DIDNT CALL SHUFFLE OR SET HAND"));
	}
}

void ATurnManager::ShuffleMyDeck(TArray<FCardDataToReplicate> DeckToShuffle) {
	int32 NumCardsInDeck = DeckToShuffle.Num();
	for (int32 i = 0; i < NumCardsInDeck - 1; i++) {
		int32 RandomIndex = FMath::RandRange(i, NumCardsInDeck -1);
		DeckToShuffle.Swap(i, RandomIndex);
	}
	MyCardDeck = DeckToShuffle;
}

void ATurnManager::SetClientHand(FClientData& Client) {
	if (MyCardDeck.Num() <= 1) {
		UE_LOG(LogTemp, Error, TEXT("My Card Deck is empty"));
		return;
	}

	//USE MY SHUFFLED DECK, USE POINTERS TO GO THROUGH THE LSIT

	int32 DefaultHandSize = 5;
	int32 PlayerHandSize = Client.HandOfCards.Num();

	if (PlayerHandSize < DefaultHandSize) {
		UE_LOG(LogTemp, Display, TEXT("Hand NOT full"));

		int32 NumCardsToDraw = DefaultHandSize - PlayerHandSize;
		for (int32 i = 0; i < NumCardsToDraw; i++) {
			// Add card to the player's hand and move the pointer
			if (MyCardDeckPointer < MyCardDeck.Num()) {
				Client.HandOfCards.Add(MyCardDeck[MyCardDeckPointer]);
				UE_LOG(LogTemp, Display, TEXT("Added card %s"), *MyCardDeck[MyCardDeckPointer].Name);
				MyCardDeckPointer++;
			}
			else {
				// If the pointer exceeds the deck size, reshuffle
				ShuffleMyDeck(MyCardDeck);
				MyCardDeckPointer = 0;
				UE_LOG(LogTemp, Display, TEXT("Reshuffled Deck"));
			}
		}
		UE_LOG(LogTemp, Display, TEXT("Hand now full"));
	}

	if (CachedCardDisplay) {
		CachedCardDisplay->UpdateCardDisplay(Client.HandOfCards);
		UE_LOG(LogTemp, Display, TEXT("CardDisplay updated for client %d"), ClientNum);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CardDisplay BP is null"));
	}



	//CardDisplay->UpdateCardDisplay(Client.HandOfCards);

	//UCardDisplay* CardDisplay = Cast<UCardDisplay>(UUserWidget::GetWidgetFromName(TEXT("CardDisplayBP")));

	//CardDisplay->UpdateCardDisplay(Client.HandOfCards);




	////if (PlayerHandSize > DefaultHandSize) {
	//// UE_LOG(LogTemp, Display, TEXT("Too many cards in hand"));
	//// 
	////	int32 NumCardsToDiscard = PlayerHandSize - DefaultHandSize; 
	////	for (int32 i = 0; i < NumCardsToDiscard; i++) {
	////		bool toldToDiscard = false;
	////		if (!toldToDiscard) {
	////			//Tell player to discard
	////			toldToDiscard = true;
	////		}

	////	}
	////}
}
