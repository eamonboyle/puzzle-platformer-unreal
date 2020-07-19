// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "Components/Button.h"

bool UPauseMenu::Initialize()
{
    if (!Super::Initialize()) return false;

    UE_LOG(LogTemp, Warning, TEXT("Pause Menu Initialized"));

    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitGame);
    if (!ensure(CancelButton != nullptr)) return false;
    CancelButton->OnClicked.AddDynamic(this, &UPauseMenu::HidePauseMenu);

    return true;
}

void UPauseMenu::QuitGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Quit Game Pressed"));

    if (MenuInterface != nullptr)
    {
        TearDown();
        MenuInterface->Leave();
    }
}

void UPauseMenu::HidePauseMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("Hide Pause Menu Pressed"));
    if (MenuInterface != nullptr)
    {
        TearDown();
    }
}
