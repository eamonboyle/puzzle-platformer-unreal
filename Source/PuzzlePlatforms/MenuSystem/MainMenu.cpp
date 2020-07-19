// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"

void UMainMenu::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
    this->MenuInterface = NewMenuInterface;
}

bool UMainMenu::Initialize()
{
    if (!Super::Initialize()) return false;

    UE_LOG(LogTemp, Warning, TEXT("Main Menu Initialized"));

    if (!ensure(Host != nullptr)) return false;

    Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if (!ensure(Join != nullptr)) return false;

    Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    return true;
}

void UMainMenu::HostServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Host Clicked"));

    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::JoinServer()
{
    UE_LOG(LogTemp, Warning, TEXT("Join Clicked"));
}
