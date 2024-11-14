#include "PerlinNoise.h"

UPerlinNoise::UPerlinNoise()
{

}

void UPerlinNoise::SetSeed(int32 Seed)
{
	seed = Seed;
}

Utexture2D UPerlinNoise::GeneratePerlinNoise2D(FVector2D TextureSize)
{
	return Utexture2D();
}

UTexture2D UPerlinNoise::GeneratePerlinNoise3D(FVector2D TextureSize)
{
	return UTexture2D();
}
