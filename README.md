# Perlin Noise Generator - Unreal Engine 5

## Description
This project implements a Perlin noise generator in Unreal Engine 5. It allows for the generation of procedural textures based on Perlin noise, with support for multiple octaves and customizable persistence. Generated textures are automatically saved as assets in the UE5 project.

## Features
- 2D Perlin noise generation
- Multi-octave support for additional detail
- Frequency and persistence control
- Automatic texture saving
- Customizable seed for result reproduction
- Smooth interpolation between grid points
- Support for various texture sizes

## Project Structure

### Main Classes

#### APerlinNoise
Main class inheriting from AActor, responsible for Perlin noise generation.

Data structures:
```cpp
struct FPerlin_cell {
    TArray<FVector> gradient_vector;
    float value;
    FPerlin_cell* up;
    FPerlin_cell* left;
};

struct FPerlin_array {
    TArray<FPerlin_cell*> Array;
};
```

### Configurable Parameters
- `frequency` (float): Controls noise frequency (default: 0.05)
- `octave` (float): Number of noise octaves (default: 4.0)
- `persistance` (float): Controls persistence between octaves (default: 0.5)
- `seed` (int32): Seed value for random generation

## Usage

### In Blueprint
1. Place an APerlinNoise actor in your level
2. Call the `SetSeed` function to set a seed
3. Use `GeneratePerlinNoise2D` by specifying:
   - TextureSize: Texture dimensions (FVector2D)
   - AssetPath: Save path in the content browser

### In C++
```cpp
APerlinNoise* PerlinGenerator = SpawnActor<APerlinNoise>();
PerlinGenerator->SetSeed(12345);
FVector2D TextureSize(256, 256);
UTexture2D* NoiseTexture = PerlinGenerator->GeneratePerlinNoise2D(
    TextureSize, 
    "PerlinNoise/MyTexture"
);
```

## Technical Details

### Algorithm
1. Generation of uniformly distributed gradient vectors grid
2. Calculation of dot products at each cell corner
3. Smooth interpolation between values (quintic function)
4. Accumulation of multiple octaves with decreasing persistence
5. Normalization of results to [0, 1] range

### Texture Storage
Textures are saved in BGRA8 format with the following properties:
- No compression (TC_VectorDisplacementmap)
- No mipmaps
- No gamma correction (SRGB = false)

## Implementation Notes
- Gradient vectors are generated consistently between adjacent cells
- Interpolation uses a quintic function for smoother transitions
- Multi-octave generation adds detail at different scales
- Textures are automatically saved in the UE5 project

## Possible Optimizations
- GPU-based generation implementation
- Gradient value caching for reuse
- Real-time generation support
- Parallelization of generation for large textures