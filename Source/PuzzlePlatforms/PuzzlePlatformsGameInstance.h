// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;
class UMainMenu;
class UPauseMenu;

UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
    GENERATED_BODY()

public:
    UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    void LoadMenu();

    UFUNCTION(BlueprintCallable)
    void LoadPauseMenu();

    UFUNCTION(Exec)
    virtual void Host() override;

    UFUNCTION(Exec)
    virtual void Join(uint32 Index) override;

    virtual void Leave() override;

    virtual void RefreshServerList() override;

private:
    TSubclassOf<UUserWidget> MenuClass;
    TSubclassOf<UUserWidget> PauseMenuClass;
    
    UMainMenu* Menu;
    UPauseMenu* PauseMenu;

    IOnlineSessionPtr SessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    void OnCreateSessionComplete(FName SessionName, bool Success);
    void OnDestroySessionComplete(FName SessionName, bool Success);
    void OnFindSessionsComplete(bool Success);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    void CreateSession();
};
