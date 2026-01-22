#pragma once

namespace CmakeProject1 {

    enum class ShaderType {
        Vertex,
        Fragment,
        Geometry,
        Compute
    };

    enum class MaterialProperty {
        Albedo,
        Metallic,
        Roughness,
        Normal,
        Emission,
        Transparency
    };

    enum class TextureOperation {
        Import,
        Resize,
        Compress,
        Filter,
        GenerateMipmaps
    };

    enum class TextureType {
        Diffuse,
        Normal,
        Specular,
        Metallic,
        Roughness,
        Emission,
        AlphaMask
    };

    enum class SpriteEditOperation {
        Slice,
        Trim,
        Resize,
        Rotate,
        FlipHorizontal,
        FlipVertical
    };

    enum class AnimationProperty {
        Loop,
        Speed,
        FrameRate,
        StartFrame,
        EndFrame,
        BlendMode
    };

    enum class ParticleProperty {
        EmissionRate,
        Lifetime,
        StartSize,
        EndSize,
        StartColor,
        EndColor,
        Gravity,
        Velocity
    };

    enum class TerrainTool {
        Raise,
        Lower,
        Smooth,
        Paint,
        Flatten,
        Vegetation
    };

    enum class TerrainGeneratorType {
        PerlinNoise,
        SimplexNoise,
        DiamondSquare,
        Voronoi,
        Heightmap
    };

    enum class WaterProperty {
        WaveHeight,
        WaveSpeed,
        WaveFrequency,
        Reflection,
        Refraction,
        Transparency
    };

    enum class LightType {
        Directional,
        Point,
        Spot,
        Area
    };

    enum class LightProperty {
        Intensity,
        Color,
        Range,
        Angle,
        Shadow,
        Cookie
    };

    enum class PostProcessingEffect {
        Bloom,
        MotionBlur,
        DepthOfField,
        ChromaticAberration,
        Vignette,
        ColorGrading
    };

    enum class PhysicsMaterialProperty {
        StaticFriction,
        DynamicFriction,
        Bounciness
    };

    enum class NavMeshEditOperation {
        Bake,
        Carve,
        Connect,
        TestPath
    };

    enum class UIElementType {
        Button,
        Slider,
        Toggle,
        TextField,
        Image,
        Panel,
        Text
    };

    enum class UIProperty {
        Position,
        Size,
        Color,
        Text,
        Font,
        Alignment
    };

    enum class UILayoutType {
        Horizontal,
        Vertical,
        Grid,
        Anchor,
        Overlay
    };

    enum class ScriptLanguage {
        Cpp,
        Lua,
        Python,
        JavaScript
    };

    enum class MessageType {
        Info,
        Warning,
        Error,
        Success
    };

    enum class FileDialogMode {
        Open,
        Save,
        Folder
    };

    enum class ExportFormat {
        OBJ,
        FBX,
        GLTF,
        PNG,
        JPG,
        WAV,
        MP3
    };

    enum class EditorState {
        Idle,
        Playing,
        Paused,
        Recording
    };
}