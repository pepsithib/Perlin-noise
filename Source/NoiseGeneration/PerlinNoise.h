#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "PerlinNoise.generated.h"


struct FPerlin_cell {

	TArray<FVector> gradient_vector;
	float value;

	FPerlin_cell* up;
	FPerlin_cell* left;
	
};


struct FPerlin_array {

	TArray<FPerlin_cell*> Array;
};

UCLASS()
class NOISEGENERATION_API APerlinNoise : public AActor
{
	GENERATED_BODY()

public:


	APerlinNoise();
	
	UFUNCTION(BlueprintCallable)
	void SetSeed(int32 _Seed);

	void GenerateUniformVector(FPerlin_cell* cell);

	float DotValue(FVector gradient_vector, int x1, int y1, float x, float y);

	float GeneratePerlinValue(float x, float y, TArray<FPerlin_array> _array);
	TArray<FPerlin_array> GeneratePerlinGrid(FVector2D TextureSize);


	float GenerateOctavePerlinValue(float x, float y, int32 octaves, float persistence);
	UFUNCTION(BlueprintCallable)
	UTexture2D* GeneratePerlinNoise2D(FVector2D TextureSize, FString AssetPath);


private:

	int32 seed;
	UPROPERTY(EditAnywhere)
	float frequency = 0.05;
	UPROPERTY(EditAnywhere)
	float octave = 4.0;
	UPROPERTY(EditAnywhere)
	float persistance = 0.5;


	TArray<FPerlin_array> Perlin_Grid;
	TArray<TArray<FPerlin_array>> OctaveGrids;
};
