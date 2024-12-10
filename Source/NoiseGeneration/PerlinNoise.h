#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PerlinNoise.generated.h"


UCLASS()
class NOISEGENERATION_API APerlinNoise : public AActor
{
	GENERATED_BODY()

public:


	APerlinNoise();
	
	UFUNCTION(BlueprintCallable)
	void SetSeed(int32 _Seed);

	float DotValue(FVector gradient_vector, int x1, int y1, float x, float y);

	float GeneratePerlinValue(float _x, float _y, int _octave, float _frequency);

	FVector GenerateVector(int _x, int _y, int _octave);

	UFUNCTION(BlueprintCallable)
	float GenerateOctavePerlinValue(float _x, float _y, int32 _octaves, float _persistence, float _frequency);
	UFUNCTION(BlueprintCallable)
	UTexture2D* GeneratePerlinNoise2D(FVector2D TextureSize, FString AssetPath);


private:

	int32 seed;
	UPROPERTY(EditAnywhere)
	float frequency = 0.05;
	UPROPERTY(EditAnywhere)
	int octave = 4;
	UPROPERTY(EditAnywhere)
	float persistance = 0.5;
	UPROPERTY(EditAnywhere)
	int period = 16;

};
