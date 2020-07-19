// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/PauseMenu.h"
#include "MenuSystem/MenuWidget.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
    const ConstructorHelpers::FClassFinder<UUserWidget> BPMenuClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
    const ConstructorHelpers::FClassFinder<UUserWidget> BPPauseMenuClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu"));

    if (!ensure(BPMenuClass.Class != nullptr)) return;

    MenuClass = BPMenuClass.Class;

    if (!ensure(BPPauseMenuClass.Class != nullptr)) return;

    PauseMenuClass = BPPauseMenuClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *PauseMenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (!ensure(MenuClass != nullptr)) return;

    Menu = CreateWidget<UMainMenu>(this, MenuClass);

    if (!ensure(Menu != nullptr)) return;

    Menu->Setup();
    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadPauseMenu()
{
    if (!ensure(PauseMenuClass != nullptr)) return;

    PauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuClass);

    if (!ensure(PauseMenu != nullptr)) return;

    PauseMenu->Setup();
    PauseMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if (Menu != nullptr)
    {
        Menu->TearDown();
    }

    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("Hosting...")));

    UWorld* World = GetWorld();

    if (!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/Maps/Game?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
    if (Menu != nullptr)
    {
        Menu->TearDown();
    }
    
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }

    Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();

    if (!ensure(PlayerController != nullptr))
    {
        return;
    }

    PlayerController->ClientTravel(FString::Printf(TEXT("%s"), *Address), ETravelType::TRAVEL_Absolute);
}
