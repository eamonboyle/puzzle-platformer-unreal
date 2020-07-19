// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
    const ConstructorHelpers::FClassFinder<UUserWidget> BPMenuClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));

    if (!ensure(BPMenuClass.Class != nullptr)) return;

    MenuClass = BPMenuClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (!ensure(MenuClass != nullptr)) return;
    
    UUserWidget* Menu = CreateWidget<UUserWidget>(this, MenuClass);

    if (!ensure(Menu != nullptr)) return;

    Menu->AddToViewport();

    APlayerController* PlayerController = GetFirstLocalPlayerController();

    if (!ensure(PlayerController != nullptr)) return;

    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(Menu->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UPuzzlePlatformsGameInstance::Host() const
{
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr))
    {
        return;
    }

    Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("Hosting...")));

    UWorld* World = GetWorld();

    if (!ensure(World != nullptr))
    {
        return;
    }

    World->ServerTravel("/Game/Maps/Game?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address) const
{
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
