// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
    const ConstructorHelpers::FClassFinder<UUserWidget> BPServerRowClass(TEXT("/Game/Blueprints/UI/WBP_ServerRow"));

    if (!ensure(BPServerRowClass.Class != nullptr)) return;
    ServerRowClass = BPServerRowClass.Class;
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
    if (ServerRowClass != nullptr)
    {
        UWorld* World = this->GetWorld();
        if (!ensure(World != nullptr)) return;

        ServerList->ClearChildren();

        uint32 i = 0;

        for (const FString& ServerName : ServerNames)
        {
            UServerRow* ServerRow = CreateWidget<UServerRow>(World, ServerRowClass);
            if (!ensure(ServerRow != nullptr)) return;

            ServerRow->ServerName->SetText(FText::FromString(ServerName));
            ServerRow->Setup(this, i);
            ++i;

            ServerList->AddChild(ServerRow);
        }
    }
}

void UMainMenu::SelectIndex(uint32 Index)
{
    // sets the selected server index on list
    SelectedIndex = Index;
}

bool UMainMenu::Initialize()
{
    if (!Super::Initialize()) return false;

    UE_LOG(LogTemp, Warning, TEXT("Main Menu Initialized"));

    if (!ensure(HostButton != nullptr)) return false;
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if (!ensure(JoinButton != nullptr)) return false;
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

    if (!ensure(CancelButton != nullptr)) return false;
    CancelButton->OnClicked.AddDynamic(this, &UMainMenu::CancelJoinMenu);

    if (!ensure(JoinServerButton != nullptr)) return false;
    JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    if (!ensure(QuitButton != nullptr)) return false;
    QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

    if (!ensure(ServerList != nullptr)) return false;

    return true;
}

void UMainMenu::HostServer()
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}

void UMainMenu::OpenJoinMenu()
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(JoinMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(JoinMenu);

    if (MenuInterface != nullptr)
    {
        MenuInterface->RefreshServerList();
    }
}

void UMainMenu::CancelJoinMenu()
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(MainMenu != nullptr)) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand("quit");
}

void UMainMenu::JoinServer()
{
    if (SelectedIndex.IsSet() && MenuInterface != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected index: %d"), SelectedIndex.GetValue());

        MenuInterface->Join(SelectedIndex.GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
    }
}
