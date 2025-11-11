#pragma once

UENUM(BlueprintType)
enum class EAnimalState : uint8
{
	Idle UMETA(DisplayName="Idle"),
	Patrol UMETA(DisplayName="Patrol"),
	Alert UMETA(DisplayName="Alert"),
	Flee UMETA(DisplayName="Flee"),
};