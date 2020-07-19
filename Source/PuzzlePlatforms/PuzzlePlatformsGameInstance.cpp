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

void UPuzzlePlatformsGameInstance::Host()
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

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
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
