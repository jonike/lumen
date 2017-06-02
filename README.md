# lumen
global illumination renderer

## screenshots
<img src="http://i.imgur.com/v4EwLDh.png"/>
<img src="http://i.imgur.com/mHWvwbP.png"/>

## setup
1. Clone the repo: `git clone https://github.com/jlukacs/lumen.git`
2. Update the submodules: `git submodule update --init`
3. Build the project using an environment in the `build` directory.

## usage
Lumen is executed on the command line: `lumen <SCENE_FILE>`. Scenes are created
in files and provided to the renderer. See [SCENE.md](SCENE.md) for an
explanation of all the scene commands. An example scene file is shown below:

```
Display "../../../demo.bmp"
Resolution 640 480
PixelSamples 16 16

DepthOfField 1 0.05 0.7

Rotate 27 1 0 0
Translate 0 0.5 -0.7
Projection 45

WorldBegin
    Light "directional"

    ObjectBegin
        Material "matte" "diffusecolor" [0.3 0.6 0.6]

        Scale 0.2 0.2 0.2
        Rotate -10 0 1 0

        Geometry "mesh" "meshname" "../../../dragon.xmesh"
    ObjectEnd

    ObjectBegin
        Scale 10 10 10

        Material "matte"
        Geometry "rectangle"
    ObjectEnd
WorldEnd
```

## project structure
- build
  - Files for compiling the project with different build environments
- lib
  - Statically linked libraries
- src
  - acceleration
    - Objects for accelerating the ray intersection tests
  - bxdf
    - Material BRDFs and BTDFs
  - camera
    - Objects for generating camera rays
  - geometry
    - Scene objects for renderable geometry
  - image
    - Class for presenting the image and saving it to a file
  - light
    - Scene objects for light sources
  - renderer
    - Rendering algorithms
  - sampler
    - Objects for generating random samples
  - scene
    - Scene description API and scene file parsing
  - texture
    - Texture file loading and texture samplers
