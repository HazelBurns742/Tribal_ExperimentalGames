// Fill out your copyright notice in the Description page of Project Settings.


#include "CardDisplay.h"
#include "CardWidget.h"  
#include "Components/HorizontalBox.h"
#include "Components/Widget.h"
#include "CardData.h"

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