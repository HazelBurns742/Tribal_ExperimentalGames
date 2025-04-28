// Fill out your copyright notice in the Description page of Project Settings.


#include "CardWidget.h"
#include "CardDisplay.h"
#include "Components/TextBlock.h"


void UCardWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetCardVariables(CardName, CardPoints, CardHealth, CardCombat, CardImagePath);

    if (CardButton) {
        CardButton->OnClicked.AddDynamic(this, &UCardWidget::OnCardClicked);
    }
}

void UCardWidget::SetCardVariables(const FString& Name, int32 Points, int32 Health, int32 Combat, const FString& ImagePath)
{
    CardName = Name;
    CardPoints = Points;
    CardHealth = Health;
    CardCombat = Combat;
    CardImagePath = ImagePath;

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

    if (CardImage)
    {
        UTexture2D* Texture = LoadTextureFromPath(ImagePath);
        if (Texture)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(Texture);
            Brush.ImageSize = FVector2D(128.0f, 128.0f); // or use texture dimensions
            CardImage->SetBrush(Brush);
        }
    }

    OnCardVariablesSet();
}

void UCardWidget::OnCardClicked() {
    UE_LOG(LogTemp, Warning, TEXT("Card %s clicked"), *CardName);
    CardDisplayRef->SetLastClickedCard(this);
}

UTexture2D* UCardWidget::LoadTextureFromPath(const FString& Path)
{
    if (Path.IsEmpty()) return nullptr;

    return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));
}
