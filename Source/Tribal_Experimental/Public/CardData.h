// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CardData.generated.h"

UCLASS(Blueprintable)
class TRIBAL_EXPERIMENTAL_API UCardData : public UObject
{
	GENERATED_BODY()
public:

	//Card Properties
	UCardData();


	//Data for every card
	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	FString Image;

	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	FString Model;


	//Data for UNDEFINED
	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 Points;


	//Data for Entities
	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 Health;

	UPROPERTY(BlueprintReadWrite, Category = "CardData")
	int32 Combat;


};

// Define the FCardData struct here
USTRUCT(BlueprintType)
struct FCardDataToReplicate
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Image;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Model;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Points;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Combat;

	FCardDataToReplicate()
		: Name(TEXT(""))  // Initialize Name to an empty string (or provide default values for other members)
		, Image(TEXT(""))
		, Model(TEXT(""))
		, Points(0)
		, Health(0)
		, Combat(0)
	{}

	// Constructor to initialize from UObject data
	FCardDataToReplicate(const UCardData* HostCardData)
	{
		if (HostCardData)
		{
			Name = HostCardData->Name;
			Image = HostCardData->Image;
			Model = HostCardData->Model;
			Points = HostCardData->Points;
			Health = HostCardData->Health;
			Combat = HostCardData->Combat;
		}
	}
};
