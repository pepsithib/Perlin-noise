
#include "PerlinNoise.h"
// Core includes
#include "CoreMinimal.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"

// Texture-related includes
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "Containers/Array.h"

// File system and path handling
#include "Misc/Paths.h"
#include "Misc/PackageName.h"
#include<random>
#include<cmath>

#include<chrono>

#define M_PI 3.141592


APerlinNoise::APerlinNoise()
{

}

void APerlinNoise::SetSeed(int32 _Seed)
{
	seed = _Seed;
}

FVector GenerateVector(std::mt19937 generator, std::uniform_real_distribution<double> uniform01) {
    double theta = 2 * M_PI * uniform01(generator);
    double phi = acos(2 * uniform01(generator) - 1);
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);
    return FVector(x, y, z);

}

void APerlinNoise::GenerateUniformVector(FPerlin_cell* cell)
{
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> uniform01(0.0, 1.0);
    if (cell->up != nullptr) {
        cell->gradient_vector.Add(cell->up->gradient_vector[2]);
    }else if (cell->left != nullptr) {
        cell->gradient_vector.Add(cell->left->gradient_vector[1]);
    }
    else {
        cell->gradient_vector.Add(GenerateVector(generator, uniform01));
    }
    seed += 1;
    generator.seed(seed);
    if (cell->up != nullptr) {
        cell->gradient_vector.Add(cell->up->gradient_vector[3]);
    }
    else {
        cell->gradient_vector.Add(GenerateVector(generator, uniform01));
    }
    seed += 1;
    generator.seed(seed);
    if (cell->left != nullptr) {
        cell->gradient_vector.Add(cell->left->gradient_vector[3]);
    }
    else {
        cell->gradient_vector.Add(GenerateVector(generator, uniform01));
    }
    seed += 1;
    generator.seed(seed);
    cell->gradient_vector.Add(GenerateVector(generator, uniform01));

		
    seed += 1;


}

float APerlinNoise::DotValue(FVector gradient_vector, int x1, int y1, float x, float y)
{

    x = x - x1;
    y = y - y1;
    return (x * gradient_vector.X + y * gradient_vector.Y);
}

float APerlinNoise::GenerateOctavePerlinValue(float x, float y, int32 octaves, float persistence)
{
    float total = 0.0;
    float used_frequency = frequency;
    float amplitude = 1.0;
    float maxValue = 0.0;

    for (int i = 0; i < octaves; i++)
    {
        
        total += GeneratePerlinValue(x, y, OctaveGrids[i]) * amplitude;
        frequency = frequency * 2.0;
        maxValue += amplitude;
        amplitude *= persistence;
    }
    total *= 3.f;
    frequency = used_frequency;
    return total / maxValue;
}



float APerlinNoise::GeneratePerlinValue(float x, float y, TArray<FPerlin_array> _array)
{

    
    // Scale input coordinates with frequency
    x = x * frequency;
    y = y * frequency;
    

    int x0 = FMath::FloorToInt(x) % _array[y].Array.Num();
    int x1 = x0 + 1;
    int y0 = FMath::FloorToInt(y) % _array.Num();
    int y1 = y0 + 1;

    float sx = x - x0;
    float sy = y - y0;
    


    // Calculate dot products
    float a1 = DotValue(_array[y0].Array[x0]->gradient_vector[0], x0, y0, x, y);
    float a2 = DotValue(_array[y0].Array[x0]->gradient_vector[1], x1, y0, x, y);
    float a3 = DotValue(_array[y0].Array[x0]->gradient_vector[2], x0, y1, x, y);
    float a4 = DotValue(_array[y0].Array[x0]->gradient_vector[3], x1, y1, x, y);
    
    // Interpolate
    float b1 = FMath::Lerp(a1, a2, ((sx*6 - 15)*sx+10)*sx*sx*sx);
    float b2 = FMath::Lerp(a3, a4, ((sx * 6 - 15) * sx + 10) * sx*sx*sx);
    float value = FMath::Lerp(b1, b2, ((sy * 6 - 15) * sy + 10) * sy*sy*sy);
    
    return value;
}

TArray<FPerlin_array> APerlinNoise::GeneratePerlinGrid(FVector2D TextureSize)
{

    TArray<FPerlin_array> value;

	for (int y = 0; y < TextureSize.Y; ++y) {
		value.Add(FPerlin_array());
		for (int x = 0; x < TextureSize.X; ++x) {
			
			value[y].Array.Add(new FPerlin_cell());

			if (y -1 >= 0) {
				value[y].Array[x]->up = value[y - 1].Array[x];
			}
			if (x-1 >=0) {
				value[y].Array[x]->left = value[y].Array[x - 1];
			}

			GenerateUniformVector(value[y].Array[x]);
		}
	}
    return value;
}

UTexture2D* APerlinNoise::GeneratePerlinNoise2D(FVector2D TextureSize, FString AssetPath)
{
	GeneratePerlinGrid(TextureSize);
   
    FString PackagePath = TEXT("/Game/") + AssetPath;
    UPackage* Package = CreatePackage(*PackagePath);

    // Create texture name
    FString TextureName = FPaths::GetBaseFilename(AssetPath);

    // Create the texture
    UTexture2D* NoiseTexture = NewObject<UTexture2D>(
        Package,
        *TextureName,
        RF_Public | RF_Standalone | RF_MarkAsRootSet
    );

    // Create texture source
    int32 SizeX = FMath::RoundToInt(TextureSize.X);
    int32 SizeY = FMath::RoundToInt(TextureSize.Y);
    NoiseTexture->Source.Init(SizeX, SizeY, 1, 1, TSF_BGRA8);

    // Lock the texture source for editing
    uint8* MipData = NoiseTexture->Source.LockMip(0);
    OctaveGrids.Empty();

    // Generate a separate grid for each octave, but keep the same size
    for (int o = 0; o < octave; o++) {
        OctaveGrids.Add(GeneratePerlinGrid(TextureSize));
        seed += 100 * o;
    }


    // Generate and fill texture data
    for (int32 y = 0; y < SizeY; y++)
    {
        for (int32 x = 0; x < SizeX; x++)
        {
            // Generate noise value for this pixel
            float NoiseValue = GenerateOctavePerlinValue(x, y,octave,persistance);

            // Convert from [-1,1] to [0,255] range
            uint8 ColorIntensity = FMath::Clamp(((NoiseValue + 1.0f) / 2)*255,0,255);

            // Calculate pixel index
            int32 Index = ((y * SizeX) + x) * 4;

            // Set pixel color (BGRA format)
            MipData[Index] = ColorIntensity;     // Blue
            MipData[Index + 1] = ColorIntensity; // Green
            MipData[Index + 2] = ColorIntensity; // Red
            MipData[Index + 3] = 255;           // Alpha
        }
    }

    // Unlock the texture source
    NoiseTexture->Source.UnlockMip(0);

    // Set texture properties
    NoiseTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    NoiseTexture->MipGenSettings = TMGS_NoMipmaps;
    NoiseTexture->SRGB = false;
    NoiseTexture->UpdateResource();

    // Mark the package as dirty for saving
    Package->MarkPackageDirty();

    // Save the package
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    UPackage::SavePackage(Package, NoiseTexture, *PackageFileName, SaveArgs);

    return NoiseTexture;
}
