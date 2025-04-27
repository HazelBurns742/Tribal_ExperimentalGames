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


    UFUNCTION(BlueprintImplementableEvent)
    void OnCardVariablesSet();

    UFUNCTION()
    void OnCardClicked();

    void SetCardDisplayReference(UCardDisplay* Display) { CardDisplayRef = Display; }

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CardNameText;


    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Card Data")
    FString CardName;
    int32 CardPoints;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Card Data")
    int32 CardHealth;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Card Data")
    int32 CardCombat;
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

    UCardDisplay* CardDisplayRef = nullptr;
};
