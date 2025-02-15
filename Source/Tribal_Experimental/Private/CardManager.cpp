// Fill out your copyright notice in the Description page of Project Settings.


#include "CardManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "CardData.h"


ACardManager::ACardManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACardManager::BeginPlay() {
	Super::BeginPlay();
	FString FilePath = FPaths::ProjectDir() + TEXT("Config/CardDataStorage.json");
	
	//DEBUGGING
	
	UE_LOG(LogTemp, Display, TEXT("File path: %s"), *FilePath);

	if (FPaths::FileExists(FilePath)) {
		UE_LOG(LogTemp, Display, TEXT("File found: %s"), *FilePath);

		//CALLING LOAD CARD DATA
		LoadCardData(FilePath);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("File does not exist: %s"), *FilePath);
	}


}

//Load card data from JSON
void ACardManager::LoadCardData(const FString& FilePath) {

	//Load JSON file to string
	FString JsonString; 
	if (FFileHelper::LoadFileToString(JsonString, *FilePath)) {

		//Create JSON reader and parse into JSON obect
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonObject> JsonObject; 


		


		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {
			TArray<FString> CardTypes = { TEXT("TroopCards"), TEXT("MonsterCards"), TEXT("AnimalCards") };

			for (const FString& CardType: CardTypes) {
				const TArray<TSharedPtr<FJsonValue>>* CardsArray;
				if (JsonObject->TryGetArrayField(TEXT("TroopCards"), CardsArray)) {
					for (const TSharedPtr<FJsonValue>& CardValue : *CardsArray) {
						TSharedPtr<FJsonObject> CardObject = CardValue->AsObject();

						//Create a new card object from JSON
						UCardData* NewCardData = NewObject<UCardData>(this);

						//DATA FROM JSON
						//Data for every card
						NewCardData->Name = CardObject->GetStringField(TEXT("Name"));
						NewCardData->Image = CardObject->GetStringField(TEXT("Image"));
						NewCardData->Model = CardObject->GetStringField(TEXT("Model"));

						//Data for undefined
						NewCardData->Points = CardObject->GetNumberField(TEXT("Points"));

						//Data for entities
						NewCardData->Health = CardObject->GetNumberField(TEXT("Health"));
						NewCardData->Combat = CardObject->GetNumberField(TEXT("Combat"));


						UE_LOG(LogTemp, Display, TEXT("Card data loaded"));
						CardDataList.Add(NewCardData);
					}
				}
			}
			RandomCard();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Cant deserialize JSON"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Cant load JSON"));
	}
}

UCardData* ACardManager::RandomCard() {
	//Random Card

	if (CardDataList.Num() > 0) {
		int32 RandomIndex = FMath::RandRange(0, CardDataList.Num() - 1);
		UE_LOG(LogTemp, Display, TEXT("Random Card = %s"), *CardDataList[RandomIndex]->Name);
		return CardDataList[RandomIndex]; 
	}

	else {
		UE_LOG(LogTemp, Warning, TEXT("No cards in the list"));
		return nullptr;
	}
}

