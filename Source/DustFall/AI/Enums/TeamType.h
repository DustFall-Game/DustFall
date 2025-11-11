#pragma once

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None UMETA(DisplayName = "None"),

	Player UMETA(DisplayName = "Player"),

	Fox UMETA(DisplayName = "Fox"),
	Pig UMETA(DisplayName = "Pig"),
	Deer UMETA(DisplayName = "Deer"),
	Wolf UMETA(DisplayName = "Wolf"),
	Bear UMETA(DisplayName = "Bear"),
	Boar UMETA(DisplayName = "Boar"),
	Rabbit UMETA(DisplayName = "Rabbit")
};