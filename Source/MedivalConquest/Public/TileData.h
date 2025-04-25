// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileData.generated.h"


UCLASS(BlueprintType) 
class MEDIVALCONQUEST_API UTileData : public UObject
{ 
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TileID; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HexID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Combat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerID;

};
