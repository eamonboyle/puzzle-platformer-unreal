// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;

private:
    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CancelButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinServerButton;

    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* MenuSwitcher;

    UPROPERTY(meta = (BindWidget))
    UWidget* JoinMenu;

    UPROPERTY(meta = (BindWidget))
    UWidget* MainMenu;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* IpAddressEntry;

    UFUNCTION()
    void HostServer();

    UFUNCTION()
    void OpenJoinMenu();

    UFUNCTION()
    void CancelJoinMenu();

    UFUNCTION()
    void JoinServer();
};
