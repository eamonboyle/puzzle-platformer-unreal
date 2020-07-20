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
class UPanelWidget;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
    GENERATED_BODY()

public:
    UMainMenu(const FObjectInitializer& ObjectInitializer);

    void SetServerList(TArray<FString> ServerNames);

    void SelectIndex(uint32 Index);

protected:
    virtual bool Initialize() override;

private:
    TSubclassOf<class UUserWidget> ServerRowClass;
    
    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

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
    UPanelWidget* ServerList;

    UFUNCTION()
    void HostServer();

    UFUNCTION()
    void OpenJoinMenu();

    UFUNCTION()
    void CancelJoinMenu();

    UFUNCTION()
    void QuitPressed();

    UFUNCTION()
    void JoinServer();

    TOptional<uint32> SelectedIndex;
};
