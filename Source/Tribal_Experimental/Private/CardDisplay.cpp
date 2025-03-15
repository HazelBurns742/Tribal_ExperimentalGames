// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDisplay.h"
#include "CardWidget.h"  
#include "Components/HorizontalBox.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "CardData.h"
#include "TurnManager.h"
#include "Kismet/GameplayStatics.h"

UCardDisplay::UCardDisplay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCardDisplay::NativeConstruct()
{
	Super::NativeConstruct(); 
    UHorizontalBox* FoundBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("CardContainerWidget")));
    if (FoundBox)
    {
        CardContainer = FoundBox;
        UE_LOG(LogTemp, Display, TEXT("Successfully found CardContainerWidget"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find CardContainerWidget"));
    }

    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UCardDisplay::OnConfirmClicked);
    }
}

UCardWidget* UCardDisplay::CreateCardWidget(const FCardDataToReplicate& CardData)
{
    if (!CardWidgetClass) {
        UE_LOG(LogTemp, Error, TEXT("CardWidgetClass is null!"));
        return nullptr;
    }

    UCardWidget* NewCardWidget = CreateWidget<UCardWidget>(GetWorld(), CardWidgetClass);

    if(NewCardWidget)
    {
        NewCardWidget->SetCardVariables(CardData.Name, CardData.Points, CardData.Health, CardData.Combat);
        UE_LOG(LogTemp, Display, TEXT("Created card widget: %s"), *CardData.Name);

        NewCardWidget->SetCardDisplayReference(this);
    } 
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create card widget"));
    }

    return NewCardWidget;
}


void UCardDisplay::UpdateCardDisplay(const TArray<FCardDataToReplicate>& HandOfCards)
{
    if (CardContainer)
    {
        CardContainer->ClearChildren();
        UE_LOG(LogTemp, Display, TEXT("Updating card display with %d cards"), HandOfCards.Num());
    }
    for (const FCardDataToReplicate& CardData : HandOfCards)
    {
        UCardWidget* NewCardWidget = CreateCardWidget(CardData);
        if (NewCardWidget)
        {
            CardContainer->AddChildToHorizontalBox(NewCardWidget);
            UE_LOG(LogTemp, Display, TEXT("Added card: %s to the container"), *CardData.Name);
        }

        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create card widget for %s"), *CardData.Name);
        }
    }
}

void UCardDisplay::RemoveCardFromDisplay(UCardWidget* CardToRemove) {
    if (CardContainer) {
        CardContainer->RemoveChild(CardToRemove);
        UE_LOG(LogTemp, Display, TEXT("Removed card from the display"));
    }
};

void UCardDisplay::OnConfirmClicked()
{
    if (LastClickedCard)
    {
        UE_LOG(LogTemp, Warning, TEXT("Card selected, do logic?"));
        RemoveCardFromDisplay(LastClickedCard);

        //Find turn manager in scene
        AActor* FoundTurnManager = UGameplayStatics::GetActorOfClass(GetWorld(), ATurnManager::StaticClass());
        ATurnManager* TurnManager = Cast<ATurnManager>(FoundTurnManager);

        if (TurnManager) {
            //Pass card name to turn manager
            UTextBlock* NameToPass = LastClickedCard->CardNameText;
            FText NameToPassText = NameToPass->GetText();
            UE_LOG(LogTemp, Warning, TEXT("Passing card name to turn manager"));
            TurnManager->ClientChoseCard(NameToPassText.ToString());
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("CANT FIND TURN MANAGER IN SCENE TO PASS CHOSEN CARD NAME"));
        }
        //Hide menu (Unset visbility)
        SetVisibility(ESlateVisibility::Hidden);
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        //Set input to game (not UI)
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No card selected!"));
    }
}

void UCardDisplay::SetCardDisplayVisible() {

    //Show menu (Set visibility)
     SetVisibility(ESlateVisibility::Visible);
     APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

     //Set input to UI (not gaem)
     FInputModeUIOnly InputMode;
     PlayerController->SetInputMode(InputMode);
    
}