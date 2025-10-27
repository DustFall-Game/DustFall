// Fill out your copyright notice in the Description page of Project Settings.


#include "DF_StartGamemode.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"


void ADF_StartGamemode::BeginPlay()
{
	Super::BeginPlay();

	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC || !MainMenuWidgetClass) return;
	
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);

	for (AActor* CameraActor : CameraActors)
	{
		if (CameraActor->ActorHasTag("Location 0"))
		{
			PC->SetViewTargetWithBlend(CameraActor, 0.f);
				
			break;
		}
	}

	auto Widget = CreateWidget<UUserWidget>(PC, MainMenuWidgetClass);
	Widget->AddToViewport();
	
	PC->bShowMouseCursor = true;
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);
}
