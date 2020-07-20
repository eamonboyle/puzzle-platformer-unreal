// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/PauseMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Session Game");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
    // find the menu blueprint classes
    const ConstructorHelpers::FClassFinder<UUserWidget> BPMenuClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
    const ConstructorHelpers::FClassFinder<UUserWidget> BPPauseMenuClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu"));

    if (!ensure(BPMenuClass.Class != nullptr)) return;
    MenuClass = BPMenuClass.Class;

    if (!ensure(BPPauseMenuClass.Class != nullptr)) return;
    PauseMenuClass = BPPauseMenuClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
    // get the online subsystem
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *Subsystem->GetSubsystemName().ToString());

        SessionInterface = Subsystem->GetSessionInterface();

        if (SessionInterface.IsValid())
        {
            // create the call backs for the sessions
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
                this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
                this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
                this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
                this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Subsystem Found"));
    }

    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *PauseMenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
    if (!ensure(MenuClass != nullptr)) return;

    // create the menu widget
    Menu = CreateWidget<UMainMenu>(this, MenuClass);
    if (!ensure(Menu != nullptr)) return;

    // setup
    Menu->Setup();
    Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadPauseMenu()
{
    if (!ensure(PauseMenuClass != nullptr)) return;

    // create the pause menu widget
    PauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuClass);
    if (!ensure(PauseMenu != nullptr)) return;

    // run the setup
    PauseMenu->Setup();
    PauseMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
    if (SessionInterface.IsValid())
    {
        // check if the session exists
        auto ExistingEssion = SessionInterface->GetNamedSession(SESSION_NAME);

        if (ExistingEssion != nullptr)
        {
            // delete existing session
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            // create a new session
            CreateSession();
        }
    }
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
    // check for interface
    if (!SessionInterface.IsValid()) return;

    // check we have search results
    if (!SessionSearch.IsValid()) return;

    // check we have a valid menu item
    if (Menu != nullptr)
    {
        Menu->TearDown();
    }

    // join the game session
    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::Leave()
{
    // leaves the server and goes back to main menu
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel(TEXT("/Game/Maps/MainMenu"), ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
    // refresh the server list on the main menu
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        //SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
        return;
    }

    if (Menu != nullptr)
    {
        Menu->TearDown();
    }

    // add an on screen debug
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("Hosting...")));

    // move to the game
    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/Maps/Game?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    // if destroyed, allow to create a new session
    if (Success)
    {
        CreateSession();
    }
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
    if (Success && SessionSearch.IsValid() && Menu != nullptr)
    {
        // found servers
        UE_LOG(LogTemp, Warning, TEXT("Find sessions complete"));

        TArray<FString> ServerNames;
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found session name: %s"), *SearchResult.GetSessionIdStr());
            ServerNames.Add(SearchResult.GetSessionIdStr());
        }

        Menu->SetServerList(ServerNames);
    }
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid()) return;

    FString Address;
    if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not resolve connection string"));
        return;
    }

    // add an on screen debug message
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    // take the client to the server
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel(FString::Printf(TEXT("%s"), *Address), ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = false;
        SessionSettings.NumPublicConnections = 2;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;

        SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
    }
}
