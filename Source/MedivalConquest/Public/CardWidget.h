// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h" 
#include "CardWidget.generated.h"

class UTextBlock;
class UCardDisplay;

UCLASS()
class MEDIVALCONQUEST_API UCardWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Card Display")
    void SetCardVariables(const FString& Name, int32 Points, int32 Health, int32 Combat);

    UFUNCTION()
    void OnCardClicked();

    void SetCardDisplayReference(UCardDisplay* Display) { CardDisplayRef = Display; }

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardNameText;

protected:
    virtual void NativeConstruct() override;

private:

    UPROPERTY(meta = (BindWidget))
    UButton* CardButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardPointsText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardHealthText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardCombatText;

    FString CardName;
    int32 CardPoints;
    int32 CardHealth;
    int32 CardCombat;

    UCardDisplay* CardDisplayRef = nullptr;
};
