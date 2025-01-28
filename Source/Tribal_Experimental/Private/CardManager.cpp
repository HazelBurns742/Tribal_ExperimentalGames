// Fill out your copyright notice in the Description page of Project Settings.


#include "CardManager.h"

ACardManager::ACardManager()
{
}

void ACardManager::BeginPlay()
{
	Super::BeginPlay(); 

	FString JsonFilePath = FPaths::ProjectContentDir() + TEXT("Data/CardData.json");
	LoadCardData(JsonFilePath); 
}

void ACardManager::LoadCardData(const FString& FilePath)
{
    FString JsonString;

    // If file is loaded successfully
    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString); 
        TSharedPtr<FJsonObject> JsonObject; 

        UE_LOG(LogTemp, Warning, TEXT("JSON Loaded"));
        
        if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
        {
            const TArray<TSharedPtr<FJsonValue>> CardsArray = JsonObject->GetArrayField(TEXT("cards")); 
             
            for (const TSharedPtr<FJsonValue>& CardValue : CardsArray) 
            {
                TSharedPtr<FJsonObject> CardObject = CardValue->AsObject(); 
                 
                // Create a new UCardData object
                UCardData* NewCardData = NewObject<UCardData>();  

                //All Card Data
                NewCardData->CardID = CardObject->GetNumberField(TEXT("CardID")); 
                NewCardData->Name = CardObject->GetStringField(TEXT("Name")); 
                NewCardData->Image = CardObject->GetStringField(TEXT("Image")); 
                NewCardData->Model = CardObject->GetStringField(TEXT("Model"));  


                //Uncategorised Data
                NewCardData->ActionPoints = CardObject->GetNumberField(TEXT("ActionPoints")); 

                //Entity Data
                NewCardData->Health = CardObject->GetNumberField(TEXT("Health"));  
                NewCardData->Combat = CardObject->GetNumberField(TEXT("Combat")); 

                // Add the card to the list
                CardDataList.Add(NewCardData); 

                UE_LOG(LogTemp, Warning, TEXT("JSON Deserialized"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Can't Deserialize JSON"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't Load JSON")); 
    }
}

UCardData* ACardManager::GetRandomCard()
{
    //Random card slection code

	return nullptr;
}


