# lumen
scene description

- [usage](#usage)
- [commands](#commands)
    - [Display](#display)
    - [Resolution](#resolution)
    - [PixelSamples](#pixelsamples)
    - [DepthOfField](#depthoffield)
    - [WorldBegin](#worldbegin)
    - [WorldEnd](#worldend)
    - [Geometry](#geometry)
        - [mesh](#mesh)
    - [ObjectBegin](#objectbegin)
    - [ObjectEnd](#objectend)
    - [Light](#light)
        - [point](#point)
        - [directional](#directional)
        - [environment](#environment)
    - [Material](#material)
        - [matte](#matte)
        - [chrome](#chrome)
        - [plastic](#plastic)
        - [glass](#glass)
    - [Projection](#projection)
    - [Translate](#translate)
    - [Rotate](#rotate)
    - [Scale](#scale)

## usage

Lumen scenes are described in separate files using the commands listed in this
file. Most scenes will follow the same format:

1. Set image characteristics.

```
Display "image.bmp"     # File gets saved to ./image.bmp
Resolution 640 480      # Image is 640x480 pixels
PixelSamples 2 2        # 4 samples per pixel (2 horizontal, 2 vertical)
```

2. Set up the camera. All transformations outside of WorldBegin/WorldEnd affect
the camera. Transformations are applied in the order they are specified.

```
# Rotate the camera 27 degrees on the x-axis and then translate. Add a 45 degree
# FOV perspective projection.
Rotate 27 1 0 0
Translate 0 0.5 -0.7
Projection 45
```

3. Describe the world. Geometry and light sources are placed between the
WorldBegin/WorldEnd commands. Geometry is placed in ObjectBegin/ObjectEnd
blocks. All the state specified between these commands only apply to the
geometry in the block. The previous state is restored after ObjectEnd.

```
WorldBegin
    # List all of the light sources.
    Light "directional"

    # For each object in the scene, create a ObjectBegin/ObjectEnd block and
    # set transformations and material properties for the geometry.
    ObjectBegin
        Scale 0.2 0.2 0.2
        Rotate -10 0 1 0

        Material "matte" "diffusecolor" [0.3 0.6 0.6]
        Geometry "mesh" "meshname" "../../../dragon.xmesh"
    ObjectEnd

    ObjectBegin
        Scale 10 10 10

        Material "matte"
        Geometry "rectangle"
    ObjectEnd
WorldEnd
```

## commands

### Display

Set the image file name. The file is saved relative to the location where
the lumen command was executed.

`Display "FILENAME"`

### Resolution
Set the image size.

`Resolution WIDTH HEIGHT`

### PixelSamples
Set the number of horizontal and vertical pixel samples. Larger numbers
result in slower computation times, but higher image quality.

`PixelSamples NUM_SAMPLES_X NUM_SAMPLES_Y`

### DepthOfField
Set depth of field parameters.

`DepthOfField FSTOP FOCAL_LENGTH FOCAL_DISTANCE`

### WorldBegin
Begin describing the scene layout. This must be used after the camera
transformations are complete and before Geometry is defined.

`WorldBegin`

### WorldEnd
End the scene description. This command launches the rendering process
and must be the last command in the file.

`WorldEnd`

### Geometry
Create geometry.

`Geometry TYPE PARAMETER_LIST`

Geometry type can be: "rectangle", "sphere", or "mesh".

#### mesh
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| meshname | Filepath to the mesh data | string | None |

### ObjectBegin
Start describing a new object in the world. This function creates a new object
scope that allows geometry hierarchies to be created. The transformations
within the scope only apply to the geometry defined in the same scope.

`ObjectBegin`

### ObjectEnd
Finish describing an object in the world. This function restores the previous
transformation state that was set before ObjectBegin was used.

`ObjectEnd`

### Light
Create a light source.

`Light TYPE PARAMETER_LIST`

Light type can be: "point", "directional", or "environment".

#### point
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| position | Position of the light in world space | [X Y Z] | [0 0 0] |
| color | light color | [R G B] | [1 1 1] |

#### directional
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| direction | Direction of light emission | [X Y Z] | [0 -1 0] |
| color | light color | [R G B] | [1 1 1] |

#### environment
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| texturename | Filepath to a cubemap | string | None |
| color | light color | [R G B] | [1 1 1] |

### Material
Set the material to use for subsequent geometry.

`Material TYPE PARAMETER_LIST`

Material type can be: "matte", "chrome", "plastic", or "glass".

#### matte
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| diffusecolor | Color of diffuse reflection | [R G B] | [1 1 1] |
| texturename | Filepath to a diffuse texture map | string | None |

#### chrome
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| specularcolor | Color of specular reflection | [R G B] | [1 1 1] |

#### plastic
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| diffusecolor | Color of diffuse reflection | [R G B] | [0.5 0.5 0.5] |
| specularcolor | Color of specular reflection | [R G B] | [0.5 0.5 0.5] |
| texturename | Filepath to a diffuse texture map | string | None |
| exponent | Specular reflection exponent | float | 32.0 |

#### glass
| Parameter | Description | Value | Default |
| :- | :- | :-: | :-: |
| specularcolor | Color of specular reflection/refraction | [R G B] | [1 1 1] |
| ni | Refractive index of the material surrounding an object | float | 1.0 |
| nt | Refractive index of the material | float | 1.5 |

### Projection
Apply a perspective projection to the current transformation.

`Projection FOV`

### Translate
Translate relative to the current position.

`Translate X Y Z`

### Rotate
Rotate along an axis through the origin.

`Rotate DEGREES X Y Z`

### Scale
Scale along each axis.

`Scale X Y Z`
