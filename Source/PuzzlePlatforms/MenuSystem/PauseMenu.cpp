// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

bool UPauseMenu::Initialize()
{
    if (!Super::Initialize()) return false;

    UE_LOG(LogTemp, Warning, TEXT("Pause Menu Initialized"));

    return true;
}
