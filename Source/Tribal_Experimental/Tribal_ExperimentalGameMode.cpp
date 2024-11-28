// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tribal_ExperimentalGameMode.h"
#include "Tribal_ExperimentalPlayerController.h"
#include "Tribal_ExperimentalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATribal_ExperimentalGameMode::ATribal_ExperimentalGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATribal_ExperimentalPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}