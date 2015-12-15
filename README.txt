Generative Tree Scenery Demo by Micky Faas
------------------------------------------

INTRODUCTION

This is a demo scene for the generative tree algorithm that I wrote. It consists of two
parts: the `engine' (work title: Folia) and the algorithm (PTree) itself. In case the
archive size is too large, the complete package can be obtained from GitHub at:
https://github.com/mickymuis/folia

Building is rather easy with the supplied CMake rules and the configure script:

./configure
cd build
make
# and on success
./folia

The demo requires OpenGL 3.2 hardware and drivers. Folia is built to support multiple
platforms/abstraction layers, but currently only SDL2 is supported. In case SDL2 is not
installed, a fall-back pre-compiled SDL2 package for Ubuntu 12.04 is included.
I will now first lists some of the highlights of the demo, after which I will give a
small synopsis of both Folia and PTree.

HIGHLIGHTS

* Tree generation algorithm that `grows' a real-time tree simulation with highly detailed
branches and leafs
* On-GPU geometry expansion (Geometry shader) and skeletal animation
* Multi-texturing and normal mapping
* Wind simulation and simple seasons
* Highly customizable algorithm
* Deferred rendering pipeline that supports many lights
* Point light and directional lights supported
* Possibility to (relatively easy) add more screen-space calculations such as SSAO
* MSAA by using multi-sample render targets and OpenGL 3.2 texelFetch()

Some useful key bindings
- `w' toggles between algorithm output and GPU-generated geometry (see below)
- `e' toggles between multiple point lights and one directional light
- `r' regrows the tree with different seed

PTREE - CPU PART

The CPU code for PTree consists of three parts: the elementary datatypes that are sent
to the GPU (PBranchSection and PLeaf), the geometry (PTreeGeometry) and the actor (PTree).
The elementary datatypes are just a bunch of floats that represent the vertices and 
attributes as they are sent to the GPU. A Geometry (abstract type in Folia) is an object
that manages the VAO and VBO('s) and makes sure that all elementary datatypes are written
to the corresponding buffers. PTreeGeometry, a Geometry subclass, does this for branch
sections and for leafs. It also manages the shader programs.

PTree is an Actor subclass, which means that it is an object in virtual 3D-space in terms
of the engine. Actors have all kinds of properties, such as position and geometry.
When a frame has elapsed, the application issues an update(deltatime) call on all 
Object/Actor trees. In the case of PTree this means that the tree of branch sections
is traversed and that the branch sections are updated if necessary. At the end of an update()
call, the buffers in PTreeGeometry are written to the GPU.
The internal tree within PTree consists of nodes of the type PTree::Node and have the
following attributes:

- extension: pointer to the next section of the same branch
- branch[n]: number of pointers to branches connected to this section
- leaf: pointer to the leaf, if any
- index: this is index of the elementary datatype in PTreeGeometry (i.e., the raw data)

So, what PTree sends to the GPU are `branch sections' and not complete branches. This
gives the possibility of making interesting and complex branches while adhering to the
hardware limits of vertices in a single geometry shader instance. 

The raw data for branch sections and leafs is very similar, but branches are slightly 
more complex. Both use two buffers, one for vertex positions, one for parameters. The
layout for branch sections is as follows:

	PREV_X,			PREV_Y,				PREV_Z,
	BASE_RADIUS,BASE_ROUGH,		BASE_SEED,
	BASE_X,			BASE_Y,				BASE_Z,
	HEAD_RADIUS,HEAD_ROUGH,		HEAD_SEED,
	HEAD_X,			HEAD_Y,				HEAD_Z,
	CURVE_X,		CURVE_Y,			CURVE_Z,
	NEXT_X,			NEXT_Y,				NEXT_Z,
	SECTIONS,		STEPS,				PREV_SEED

Each elementary datatype consists of three floats per four vertices in two interleaved
buffers, which makes a total of 24 floats per branch section. The primitive type is
GL_LINE_ADJACENCY: the branch sections are actually lines that are expanded with volume
later in the geometry shader. The BASE_ and HEAD_ vectors are the actual positions, 
while PREV_ denoted the previous element. Other floats, such as *_RADIUS and *_ROUGH
give parameters to the algorithm in the geometry shader. *_SEED gives the random offset
for wind animation.
I will not go into much detail of the algorithm itself, but will happily respond to
question related to it. All PTree related classes can be found in 
src/actors/ptree.h
src/actors/ptree.cpp

PTREE - GPU PART

On the GPU side of things is a large shader program that consists of the following 
components:

- Single vertex shader for all types
- Separate geometry shaders for branch sections, leafs and blossoms (work in progress)
- Normal mapping multi-texture fragment shader for the branches and a simple color 
fragment shader for the leafs.

All shaders can be found in src/shaders/ptree_*

I will only describe the geometry shader for the branch sections as the rest largely
follow from it. The shader takes the input as described in the previous section and
then generates geometry of the GL_TRIANGLE_STRIP type. Each vertex has the following
properties:

- vec4 position (as per GLSL) the geometry shader multiplies it with the proj. matrix
- vec3 normal: in worldspace
- vec3 texCoords: u, v, blendFactor (for multi texturing)
- vec3 tangent: for normal mapping

The geometry of the branch itself is made out of `rings' of triangle strips. The properties
of the final `tube' are dictated by the parameters from the algorithm. The geometry
shader works roughly as follows:

Given:
- The radii of the bottom and top ends (called BASE and HEAD)
- The curvature direction and magnitude
- The position of the bottom of the previous section (called PREV)
- Some additional parameters

For each ring in the `tube':
- Compute the pivotal position (center) of the ring by using the bezier function
on BASE, HEAD and their average offset by the curvature vector.
- Compute the axis-angle orientation by using the position of previous ring
- Derive the outward perpendicular vector N
- Rotate this vector 360 / RING_SECTIONS for every segment
- Compute the radius by linearly interpolating between the BASE and HEAD radii
- normalize(N) * radius + position gives a vertex
- normalize(N) gives a normal
- Compute the tangent and the texture coords
- Multiple with the model/view/projection matrices

In the demo one can toggle between the actual output of the algorithm and the generated
geometry with `w'.

DEFERRED RENDERING

I implemented a basic deferred-shading-style renderer to support many lights and 
potentially numerous screen-space effects. The current renderer works in three passes:

- Render geometry of all solid geometry into separate textures (`geometry pass')
- Draw each light to a screen-sized quad using the information accumulated in the textures
(`light pass')
- Render all unlit geometry using forward rendering, such as pointsprites 

In the geometry pass all solid models are rendered without shading. Usefull information
that is necessary for lighting calculations are stored in screen-sized textures. 
A collection of these textures is commonly called a GBuffer (see src/core/gbuffer.h).
In Folia the GBuffer stores these textures:

- Depth
- Diffuse + specular component
- Normals
- Position

This provides enough information to calculate the lighting in so called `screen-space':
i.e. for every pixel on the screen, lighting is calculated. This is a gain with respect
to per-fragment lighting, as many fragment may eventually not end up on the screen.
On the other hand, the GBuffer uses a massive amount of bandwidth. My current 
implementation will probably not offer a significant speed increase, but with optimizations
great things can be achieved. One of these optimizations are the calculation of a pixel's
position from the depth alone. That alleviates some of the bandwidth issues by making
the position-texture unnecessary.

One last addition is Multi-Sample Anti-Aliasing (MSAA) for geometry pass.
Many say is impossible in combination with deferred rendering. My implementation
uses multi-sample textures as targets for the GBuffer, which are resolved to a 
single-sample framebuffer using texelFetch() in the lighting shader. Although it brings
nice smooth imagery, it is very inefficient in its current form:

- Hardware limitations dictate that all render targets must be of the same type,
 therefore Depth and Normal textures are much larger than they need to be.
- The already huge GBuffer doubles or quadruples depending on sample count
- Usually, MSAA is performed on edged only. Because the light-shader doesn't know
about edges, it is performed for every pixel. This makes it unnecessary slow.

FOLIA - OVERVIEW

Folia (working title) a very simple work-in-progress engine that I developed alongside
the algorithm PTree. It follows an object-oriented approach that models physical
objects with classes, similarly to other (game)engines. At the heart is a simple 
deferred render path that also supports forward rendering (for the pointsprites) and 
simple lighting. It has some basic facilities such as shader caching, render queues,
camera and overly simplistic event handling. The general anatomy of a Folia application
is as follows:

- An Application-derived class is the basis for everything
- An Application can contain one or more Window instances
- Windows contain (a) Viewport(s)
- A Window needs a pointer to an instance of Engine to be useful
- A Viewport needs a Camera and a World in order to look at something
- A World contains one or more Zone(s)
- A Zone contains a list of Light(s)
- A Zone contains instances of Actor-derived classes: the actual objects in the world
- Actors can be nested inside other Actors and inherit their parent's properties

main.cpp and tree_scene.cpp give a general overview of what happens in practice.

FUTURE

Both the generative modelling and especially the rendering are very interesting topics.
Cool hardware features such as MRT and Geometry Shaders give endless possibilities for
creating (rather mimicking) all sorts of physical effects. My deferred pipeline could be
extended with shadowing, ambient occlusion (SSAO) or even sub-surface scattering (SSSSS).
Of course, this would result in a slide-show in the current implementation, so 
optimization would probably the best place to continue...