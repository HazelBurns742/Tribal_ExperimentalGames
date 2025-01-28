
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h" // Include for UObject-derived classes
#include "CardData.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class TRIBAL_EXPERIMENTAL_API UCardData : public UObject
{

	GENERATED_BODY() 

public:
	//CardData();
	//~CardData();

	UCardData(); 


	//Data for every card
	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 CardID; 


	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	FString Image;

	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	FString Model;

	//Data for UNDEFINED
	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 ActionPoints;


	//Data for Entities
	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 Health;

	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 Combat;

	// Function to load data from a JSON file
	void LoadFromJson(const FString& FilePath);
};
