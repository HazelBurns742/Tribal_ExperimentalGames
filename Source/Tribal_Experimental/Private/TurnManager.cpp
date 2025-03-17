// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManager.h"
#include "CardManager.h"
#include "CardDisplay.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h" 




// Sets default values
ATurnManager::ATurnManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	//if (!RootComponent)
	//{
	//	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//	RootComp = RootComponent;
	//}
}

// Called when the game starts or when spawned
void ATurnManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("TURN MANAGER WAS SPAWNED!"));
	CardManager = GetWorld()->SpawnActor<ACardManager>(ACardManager::StaticClass());
}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

}

void ATurnManager::CreateAndAddWidgetToViewport(APlayerController* PlayerController){

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
	if (HasAuthority()) 
	{
		FClientData NewClient;
		NewClient.ClientID = FString(TEXT("Player")) + FString::FromInt(ClientNum);
		UE_LOG(LogTemp, Display, TEXT("Number of clients: %d"), ClientNum);

		UE_LOG(LogTemp, Display, TEXT("Added client %s"), *NewClient.ClientID);
		NewClient.ActionPoints = 5;

		if (CardManager != nullptr) {
			//Shuffle original deck
			TArray<FCardDataToReplicate> ClientDeck = CardManager->ReplicatedCardDataList;
			ShuffleMyDeck(ClientDeck);
			MyCardDeckPointer = 0;

			//Set shuffled deck to my deck
			NewClient.MyCardDeck = ClientDeck;
			SetClientHand(NewClient);
			Clients.Add(NewClient);

			UE_LOG(LogTemp, Warning, TEXT("Added Client: %s to Client array"), *NewClient.ClientID);
			UE_LOG(LogTemp, Warning, TEXT("Num of clients in array: %d"), Clients.Num());
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Card Manger was null, DIDNT CALL SHUFFLE OR SET HAND"));
		}
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

	if (CachedCardDisplay) {
		CachedCardDisplay->SetCardDisplayVisible();
		CachedCardDisplay->UpdateCardDisplay(Client.HandOfCards, Client.ClientID);
		UE_LOG(LogTemp, Display, TEXT("CardDisplay updated for client %d"), ClientNum);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CardDisplay BP is null"));
	}


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

void ATurnManager::ClientChoseCard(FString ChosenCardName, FString ClientID) {

	UE_LOG(LogTemp, Warning, TEXT("Turn manager recieved chosen card name: %s"), *ChosenCardName);
	UE_LOG(LogTemp, Warning, TEXT("Client ID recieved: %s"), *ClientID);
	UE_LOG(LogTemp, Warning, TEXT("Num of clients to loop through: %d"), Clients.Num());

	for (FClientData& Client : Clients) {
		UE_LOG(LogTemp, Warning, TEXT("Looping through %d clients"), Clients.Num());
		UE_LOG(LogTemp, Warning, TEXT("Comparing Client ID: %s with Passed Client ID: %s"), *Client.ClientID, *ClientID);
		if (Client.ClientID == ClientID) {
			UE_LOG(LogTemp, Warning, TEXT("Client ID matched"));

			for (FCardDataToReplicate& Card : Client.HandOfCards) {
				if (Card.Name == ChosenCardName) {
					Client.ActionPoints -= Card.Points;
					//Place cards

					//Client.HandOfCards.RemoveSingle(Card);
					UE_LOG(LogTemp, Display, TEXT("Client: %s! Card %s used! Remaining Actions: %d"), *ClientID, *Card.Name, Client.ActionPoints);
					break;
				}
			}
			break;
		}
	}


	//Get array of all clients
	//Find client with id = clientid 
	//Then do for

	//For every card in hand of cards, 
	// If handofcards card.name = CardWidget.name 
	//	actionpoints - card.points;
	//	place card? //Need to call tile logic first? 
	//	Remove card from hand of cards
}

void ATurnManager::ServerClientChoseCard_Implementation(const FString& ChosenCardName, const FString& ClientID) {
	
	UE_LOG(LogTemp, Warning, TEXT("Turn manager recieved chosen card name: %s"), *ChosenCardName);
	UE_LOG(LogTemp, Warning, TEXT("Client ID recieved: %s"), *ClientID);
	UE_LOG(LogTemp, Warning, TEXT("Num of clients to loop through: %d"), Clients.Num());

	for (FClientData& Client : Clients) {
		UE_LOG(LogTemp, Warning, TEXT("Looping through %d clients"), Clients.Num());
		UE_LOG(LogTemp, Warning, TEXT("Comparing Client ID: %s with Passed Client ID: %s"), *Client.ClientID, *ClientID);
		if (Client.ClientID == ClientID) {
			UE_LOG(LogTemp, Warning, TEXT("Client ID matched"));

			for (FCardDataToReplicate& Card : Client.HandOfCards) {
				if (Card.Name == ChosenCardName) {
					Client.ActionPoints -= Card.Points;
					//Place cards

					//Client.HandOfCards.RemoveSingle(Card);
					UE_LOG(LogTemp, Display, TEXT("Client: %s! Card %s used! Remaining Actions: %d"), *ClientID, *Card.Name, Client.ActionPoints);
					break;
				}
			}
			break;
		}
	}
}

void ATurnManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the Clients array to all clients
	DOREPLIFETIME(ATurnManager, Clients);
}