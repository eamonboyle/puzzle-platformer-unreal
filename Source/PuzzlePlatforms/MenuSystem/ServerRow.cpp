// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "Components/Button.h"

#include "MainMenu.h"

void UServerRow::Setup(UMainMenu* MyParent, uint32 MyIndex)
{
    Parent = MyParent;
    Index = MyIndex;
    
    if (!ensure(ServerButton != nullptr)) return;
    ServerButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
    Parent->SelectIndex(Index);
}
