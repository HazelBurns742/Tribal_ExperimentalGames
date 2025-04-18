// Fill out your copyright notice in the Description page of Project Settings.



#include "TurnManager.h"
#include "CardManager.h"
#include "CardDisplay.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h" 




// Sets default values
ATurnManager::ATurnManager()
{
	PrimaryActorTick.bCanEverTick = true;

	currentTurn = 0; 
	amountOfPlayers; 
	currentPlayer; 

}

// Called when the game starts or when spawned
void ATurnManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("TURN MANAGER WAS SPAWNED!"));
	CardManager = GetWorld()->SpawnActor<ACardManager>(ACardManager::StaticClass());

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && CardDisplayClass) {
		CachedCardDisplay = CreateWidget<UCardDisplay>(PC, CardDisplayClass);
		if (CachedCardDisplay) {
			CachedCardDisplay->AddToViewport();
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());
			UE_LOG(LogTemp, Display, TEXT("CardDisplay Widget added to viewport"));
		}
	}
}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurnManager::AddPlayer()
{
	for (int32 i = 0; i < amountOfPlayers; ++i) {

		FClientData NewClient;
		NewClient.ClientID = FString(TEXT("Player")) + FString::FromInt(i+1);

		UE_LOG(LogTemp, Display, TEXT("Added client %s"), *NewClient.ClientID);
		NewClient.ActionPoints = 5;

		if (CardManager != nullptr) {
			//Shuffle original deck
			TArray<FCardDataToReplicate> Deck = CardManager->ReplicatedCardDataList;

			//Use random seed for each player (Different ones)
			FMath::RandInit(FDateTime::Now().GetTicks() + i);

			ShuffleMyDeck(Deck);
			MyCardDeckPointer = 0;

			//Set shuffled deck to my deck
			NewClient.MyCardDeck = Deck;
			UE_LOG(LogTemp, Display, TEXT("Deck assigned to %s:"), *NewClient.ClientID);

			SetPlayerHand(NewClient);
			Clients.Add(NewClient);

			UE_LOG(LogTemp, Warning, TEXT("Added Client: %s to Client array"), *NewClient.ClientID);
			UE_LOG(LogTemp, Warning, TEXT("Num of clients in array: %d"), Clients.Num());

		}

		else {
			UE_LOG(LogTemp, Error, TEXT("Card Manger was null, DIDNT CALL SHUFFLE OR SET HAND"));
		}
	}

	currentPlayer = 0;
	StartTurn();
}

void ATurnManager::ShuffleMyDeck(TArray<FCardDataToReplicate>& DeckToShuffle) {
	int32 NumCardsInDeck = DeckToShuffle.Num();
	for (int32 i = 0; i < NumCardsInDeck - 1; i++) {
		int32 RandomIndex = FMath::RandRange(i, NumCardsInDeck - 1);
		DeckToShuffle.Swap(i, RandomIndex);
	}
	MyCardDeck = DeckToShuffle;
}

void ATurnManager::SetPlayerHand(FClientData& Client) {
	if (Client.MyCardDeck.Num() <= 1) {
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
			if (MyCardDeckPointer < Client.MyCardDeck.Num()) {
				Client.HandOfCards.Add(Client.MyCardDeck[MyCardDeckPointer]);
				UE_LOG(LogTemp, Display, TEXT("Added card %s"), *MyCardDeck[MyCardDeckPointer].Name);
				MyCardDeckPointer++;
			}
			else {
				// If the pointer exceeds the deck size, reshuffle
				ShuffleMyDeck(Client.MyCardDeck);
				MyCardDeckPointer = 0;
				UE_LOG(LogTemp, Display, TEXT("Reshuffled Deck"));
			}
		}
		UE_LOG(LogTemp, Display, TEXT("Hand now full"));
	}
}

void ATurnManager::StartTurn() {

	UE_LOG(LogTemp, Display, TEXT("Start Turn called in script"));
	if (!Clients.IsValidIndex(currentPlayer)) return; 

	UE_LOG(LogTemp, Display, TEXT("If statement passed"));

	FClientData& CurrentClient = Clients[currentPlayer];
	CurrentClient.ActionPoints = 5;

	if (CachedCardDisplay) {
		CachedCardDisplay->UpdateCardDisplay(CurrentClient.HandOfCards, CurrentClient.ClientID);
		CachedCardDisplay->SetCardDisplayVisible();
		UE_LOG(LogTemp, Display, TEXT("CardDisplay updated for %s"), *CurrentClient.ClientID);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CardDisplay BP is null"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Turn started for %s"), *CurrentClient.ClientID);
}

void ATurnManager::EndTurn() {
	currentPlayer = (currentPlayer + 1) % amountOfPlayers;
	StartTurn();
}

void ATurnManager::PlayerChoseCard(FString ChosenCardName) {

	UE_LOG(LogTemp, Warning, TEXT("Turn manager recieved chosen card name: %s"), *ChosenCardName);
	//UE_LOG(LogTemp, Warning, TEXT("Client ID recieved: %s"), *ClientID);
	//UE_LOG(LogTemp, Warning, TEXT("Num of clients to loop through: %d"), Clients.Num());

	FClientData& CurrentClient = Clients[currentPlayer];

	for (int32 i = 0; i < CurrentClient.HandOfCards.Num(); ++i) {
		if (CurrentClient.HandOfCards[i].Name == ChosenCardName) {
			CurrentClient.ActionPoints -= CurrentClient.HandOfCards[i].Points;

			// TODO: Trigger card effect

			CurrentClient.HandOfCards.RemoveAt(i);

			if (CachedCardDisplay){
				CachedCardDisplay->UpdateCardDisplay(CurrentClient.HandOfCards, CurrentClient.ClientID);
			}

			UE_LOG(LogTemp, Display, TEXT("%s used card %s. Remaining AP: %d"),
				*CurrentClient.ClientID, *ChosenCardName, CurrentClient.ActionPoints);

			break;
		}
	}

	if (CurrentClient.ActionPoints <= 0) {
		SetPlayerHand(CurrentClient);
		EndTurn();
	}
}

void ATurnManager::UpdatePlayerUI() {

	FClientData& CurrentClient = Clients[currentPlayer];

	// Update the card display with the new hand
	if (CachedCardDisplay) {
		CachedCardDisplay->UpdateCardDisplay(CurrentClient.HandOfCards, CurrentClient.ClientID);
		CachedCardDisplay->SetCardDisplayVisible();
		UE_LOG(LogTemp, Display, TEXT("UI updated after removing card"));
	}


	else {
		UE_LOG(LogTemp, Warning, TEXT("CardDisplay BP is null, UI update failed"));
	}
}
