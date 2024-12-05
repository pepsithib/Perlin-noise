# Générateur de Bruit de Perlin - Unreal Engine 5

## Description
Ce projet implémente un générateur de bruit de Perlin dans Unreal Engine 5. Il permet de générer des textures procédurales basées sur le bruit de Perlin, avec support pour plusieurs octaves et persistance personnalisable. Les textures générées sont automatiquement sauvegardées en tant qu'assets dans le projet UE5.

## Fonctionnalités
- Génération de bruit de Perlin 2D
- Support multi-octaves pour plus de détails
- Contrôle de la fréquence et de la persistance
- Sauvegarde automatique des textures générées
- Seed personnalisable pour la reproduction des résultats
- Interpolation lisse entre les points de la grille
- Support pour différentes tailles de texture

## Structure du Projet

### Classes Principales

#### APerlinNoise
Classe principale héritant de AActor, responsable de la génération du bruit de Perlin.

Structures de données :
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

### Paramètres Configurables
- `frequency` (float) : Contrôle la fréquence du bruit (défaut : 0.05)
- `octave` (float) : Nombre d'octaves pour le bruit (défaut : 4.0)
- `persistance` (float) : Contrôle la persistence entre les octaves (défaut : 0.5)
- `seed` (int32) : Valeur de seed pour la génération aléatoire

## Utilisation

### Dans Blueprint
1. Placez un APerlinNoise actor dans votre niveau
2. Appelez la fonction `SetSeed` pour définir une seed
3. Utilisez `GeneratePerlinNoise2D` en spécifiant :
   - TextureSize : Dimensions de la texture (FVector2D)
   - AssetPath : Chemin de sauvegarde dans le content browser

### En C++
```cpp
APerlinNoise* PerlinGenerator = SpawnActor<APerlinNoise>();
PerlinGenerator->SetSeed(12345);
FVector2D TextureSize(256, 256);
UTexture2D* NoiseTexture = PerlinGenerator->GeneratePerlinNoise2D(
    TextureSize, 
    "PerlinNoise/MyTexture"
);
```

## Détails Techniques

### Algorithme
1. Génération d'une grille de vecteurs de gradient uniformément distribués
2. Calcul des produits scalaires aux coins de chaque cellule
3. Interpolation lisse entre les valeurs (fonction quintic)
4. Accumulation de plusieurs octaves avec persistance décroissante
5. Normalisation des résultats dans l'intervalle [0, 1]

### Stockage des Textures
Les textures sont sauvegardées au format BGRA8 avec les propriétés suivantes :
- Pas de compression (TC_VectorDisplacementmap)
- Pas de mipmaps
- Pas de correction gamma (SRGB = false)

## Notes d'Implémentation
- Les vecteurs de gradient sont générés de manière cohérente entre les cellules adjacentes
- L'interpolation utilise une fonction quintic pour une transition plus douce
- La génération multi-octaves permet d'ajouter des détails à différentes échelles
- Les textures sont automatiquement sauvegardées dans le projet UE5

## Optimisations Possibles
- Implémentation de la génération sur GPU
- Cache des valeurs de gradient pour réutilisation
- Support pour la génération en temps réel
- Parallélisation de la génération pour les grandes textures
