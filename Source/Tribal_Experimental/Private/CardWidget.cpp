// Fill out your copyright notice in the Description page of Project Settings.


#include "CardWidget.h"
#include "Components/TextBlock.h"


void UCardWidget::NativeConstruct()
{
	Super::NativeConstruct();

    SetCardVariables(CardName, CardPoints, CardHealth, CardCombat);
}

void UCardWidget::SetCardVariables(const FString& Name, int32 Points, int32 Health, int32 Combat)
{
    CardName = Name;
    CardPoints = Points;  
    CardHealth = Health; 
    CardCombat = Combat;


    if (CardNameText) {
        CardNameText->SetText(FText::FromString(CardName));
    }

    if (CardNameText) {
        CardNameText->SetText(FText::FromString(CardName));
    }

    if (CardPointsText)
    {
        CardPointsText->SetText(FText::FromString(FString::Printf(TEXT("AP : %d"), CardPoints)));
    }
    if (CardHealthText)
    {
        CardHealthText->SetText(FText::FromString(FString::Printf(TEXT("HP : % d"), CardHealth)));
    }
    if (CardCombatText)
    {
        CardCombatText->SetText(FText::FromString(FString::Printf(TEXT("CP : %d"), CardCombat)));
    }
}
