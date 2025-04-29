// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDisplay.h"
#include "CardWidget.h"  
#include "Components/HorizontalBox.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "CardData.h"
#include "TurnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

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

    //Create the card widget using the card widget script
    UCardWidget* NewCardWidget = CreateWidget<UCardWidget>(GetWorld(), CardWidgetClass);

    if (NewCardWidget)
    {
        //Set the widget card data
        NewCardWidget->SetCardVariables(CardData.Name, CardData.Points, CardData.Health, CardData.Combat, CardData.Image);
        UE_LOG(LogTemp, Display, TEXT("Created card widget: %s"), *CardData.Name);

        //Set reference
        NewCardWidget->SetCardDisplayReference(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create card widget"));
    }

    return NewCardWidget;
}


void UCardDisplay::UpdateCardDisplay(const TArray<FCardDataToReplicate>& HandOfCards, FString ClientID)
{
    DislayClientIDRef = ClientID;
    if (CardContainer)
    {
        //Clear existing cards
        CardContainer->ClearChildren();
        UE_LOG(LogTemp, Display, TEXT("Updating card display with %d cards"), HandOfCards.Num());
    }
    for (const FCardDataToReplicate& CardData : HandOfCards)
    {
        //Create and add the cards in the players hand to the UI 
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
        //Removing the card the player chose from the UI
        UE_LOG(LogTemp, Warning, TEXT("Card selected, do logic?"));
        RemoveCardFromDisplay(LastClickedCard);

        ATurnManager* TurnManager = nullptr;
        TArray<AActor*> FoundActors;

        //Get the turn manager actors in the world
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurnManager::StaticClass(), FoundActors);

        //If there are any turn managers in the scene get the first one
        if (FoundActors.Num() > 0)
        {
            TurnManager = Cast<ATurnManager>(FoundActors[0]);
        }

        if (TurnManager) {
            //Pass card name to turn manager
            UTextBlock* NameToPass = LastClickedCard->CardNameText;
            FText NameToPassText = NameToPass->GetText();
            UE_LOG(LogTemp, Warning, TEXT("Passing card name to turn manager"));

            //Tell turn manager what card the player chose
            TurnManager->PlayerChoseCard(NameToPassText.ToString());
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
