// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h" 
#include "Components/Button.h"
#include "CardDisplay.generated.h"

class UCardWidget;
class UHorizontalBox;

UCLASS()
class TRIBAL_EXPERIMENTAL_API UCardDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
    UCardDisplay(const FObjectInitializer& ObjectInitializer);

    UCardWidget* CreateCardWidget(const FCardDataToReplicate& CardData);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Display")
    TSubclassOf<UCardWidget> CardWidgetClass;

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Card Dsiplay")
    UHorizontalBox* CardContainer;

    UFUNCTION(BlueprintCallable, Category = "Card Dsiplay")
    void UpdateCardDisplay(const TArray<FCardDataToReplicate>& HandOfCards);

    //Confirm card
    UPROPERTY(meta = (BindWidget))
    UButton* ConfirmButton;

    UCardWidget* LastClickedCard = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Card Dsiplay")
    void OnConfirmClicked();

    UFUNCTION(BlueprintCallable, Category = "Card Dsiplay")
    void SetCardDisplayVisible();

    void SetLastClickedCard(UCardWidget* ClickedCard) { LastClickedCard = ClickedCard; }

    void RemoveCardFromDisplay(UCardWidget* CardToRemove); 

protected:
    virtual void NativeConstruct() override;
};


