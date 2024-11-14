#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PerlinNoise.generated.h"

UCLASS()
class NOISEGENERATION_API UPerlinNoise : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UPerlinNoise();

	void SetSeed(int32 Seed);

	Utexture2D GeneratePerlinNoise2D(FVector2D TextureSize);
	UTexture2D GeneratePerlinNoise3D(FVector2D TextureSize);

private:

	int32 seed;
};
