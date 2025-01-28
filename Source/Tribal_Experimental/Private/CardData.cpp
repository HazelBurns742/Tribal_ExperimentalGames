// Fill out your copyright notice in the Description page of Project Settings.


#include "CardData.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

//CardData::CardData()
//{
//}
//
//CardData::~CardData()
//{
//}

UCardData::UCardData()
{
	//Default values
	CardID = 0; 
	Health = 0; 
	Combat = 0;

}

void UCardData::LoadFromJson(const FString& FilePath)
{
	//Load json file as string
	FString JsonString; 

	//If file is loaded 
	if (FFileHelper::LoadFileToString(JsonString, *FilePath)) {

		//Json reader & desearializing
		TSharedPtr<FJsonObject> JsonObject; 
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString); 
		UE_LOG(LogTemp, Warning, TEXT("Loaded JSON"))

		if (FJsonSerializer::Deserialize(Reader, JsonObject)) {

			//All card category 
			CardID = JsonObject->GetNumberField(TEXT("CardID"));
			Name = JsonObject->GetStringField(TEXT("Name")); 
			Image = JsonObject->GetStringField(TEXT("Image"));
			Model = JsonObject->GetStringField(TEXT("Model"));


			//Undefined category
			ActionPoints = JsonObject->GetNumberField(TEXT("Name"));

			//Entity Category 
			Health = JsonObject->GetNumberField(TEXT("Name"));
			Combat = JsonObject->GetNumberField(TEXT("Combat"));

			UE_LOG(LogTemp, Warning, TEXT("Deserialized JSON"))
		}

		else {
			UE_LOG(LogTemp, Warning, TEXT("Can't deserialize JSON"))
		}
	}

	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't load JSON"))
	}
}
