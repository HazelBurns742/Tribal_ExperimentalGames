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
	FString FilePath = FPaths::ProjectSavedDir() + TEXT("Config/CardData.json");
	LoadCardData(FilePath);
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
			const TArray<TSharedPtr<FJsonValue>>* CardsArray;
			if (JsonObject->TryGetArrayField(TEXT("TroopCards"), CardsArray)) {
				for (const TSharedPtr<FJsonValue>& CardValue : *CardsArray) {
					TSharedPtr<FJsonObject> CardObject = CardValue->AsObject();

					//Create a new card object from JSON
					UCardData* NewCardData = NewObject<UCardData>(this); 
					NewCardData->ActionPoints = CardObject->GetNumberField(TEXT("ActionPoints"));
					NewCardData->Health = CardObject->GetNumberField(TEXT("Health"));
					NewCardData->Combat = CardObject->GetNumberField(TEXT("Combat"));

					CardDataList.Add(NewCardData);
				}
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Cant deserialize JSON"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Cany load JSON"));
	}
}

UCardData* ACardManager::RandomCard() {
	//Random Card

	if (CardDataList.Num() > 0) {
		int32 RandomIndex = FMath::RandRange(0, CardDataList.Num() - 1);
		return CardDataList[RandomIndex]; 
	}

	else {
		UE_LOG(LogTemp, Warning, TEXT("No cards in the list"));
		return nullptr;
	}
}

