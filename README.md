# CitroFlex
A game engine that I am working on for the C2D graphical library of the Nintendo 3ds
> [!WARNING]
> Note that this is in a very early stage and not all modules are working so be free to help me :hugs:

## Modules
CitroFlex is divided into several sub-modules listed below with their functionalities:

### 1. **cf_colors** _Colors..._
   - clr: Contains some preset of colors

### 2. **cf_debug** _Tools for debugging_
   - Logger: Permits to debug your 3ds game by logging into a file in the SD CARD

### 3. **cf_math** _Miscellaneous maths_
   - vec2: Vector 2d type
   - VECZERO: Constant 2D vector with zero components.
   - clamp: Template function that constrains a given value within a specified range.
   - colorAverage: Computes the average color between two colors by averaging their components.
   - PointToLineDistance: Computes the distance between a point and a line defined by two points.
   - CheckCollision: Checks for collision between two squares and a line defined by two points.
   - lerp: Performs linear interpolation between two values based on a parameter t.
   - lerpColor: Performs linear interpolation between two colors based on a parameter t.
   - isPointInRectangle: Checks if a point is inside a rectangle defined by its position, width, and height.

### 4. **cf_objects** _Objects classes for game and UI elements_
   - Object: Base class for game objects, with position and ID properties, and virtual methods for initialization and updating.
   - Lazer: Represents a ray object with properties such as start and end positions, color, thickness, and functionality for spawning and updating.
   - Player: Represents the player object with properties like position, size, speed, color, health, and methods for initialization, movement, updating, and collision detection.
   - Text: Class for displaying text on the screen with methods for initialization, updating, and freeing resources.
   - Button: Class representing a clickable button with properties like position, size, colors, and attached text, along with methods for initialization, updating, adding text, and freeing resources.
   - randomInRange: Generates a random integer within a specified range using the Mersenne Twister random number generator.
   - generateScreenCoordinates: Generates random screen coordinates within the specified screen width and height, filling two maths::vec2 objects with the left and right coordinates.

### 5. **cf_random** _Random number generation_
   - randomInRange: Generates a random integer within a specified range using the Mersenne Twister random number generator.
   - generateScreenCoordinates: Generates random screen coordinates within the specified screen width and height, filling two maths::vec2 objects with the left and right coordinates.

### 6. **citroFlex** _Main header file_
   - Scene: Represents a scene in the game with methods for initialization, updating, drawing objects on the top and bottom screens, adding objects to the scene, and destroying the scene.
   - start: Begins the scene loop by initializing and updating the scene until canLoop becomes false.
   - init: Virtual method that can be overridden to perform additional initialization tasks.
   - prepare_top: Prepares the top screen for rendering.
   - prepare_bottom: Prepares the bottom screen for rendering.
   - add_object: Adds an object to the scene, assigning it a unique ID and storing it in the appropriate vector based on whether it belongs to the top or bottom screen.
   - update: Virtual method that can be overridden to perform scene updates. Calls draw_top and draw_bottom to update the top and bottom screen objects.
   - draw_top: Draws objects on the top screen by calling their update methods.
   - draw_bottom: Draws objects on the bottom screen by calling their update methods.
   - destroy: Virtual method that can be overridden to perform cleanup tasks when destroying the scene, such as freeing resources associated with objects.

## Compiling
1. Ensure that you have installed the Nintendo 3DS development kit (devkitPro) on your system. You can download it from the official website: [devkitPro](https://devkitpro.org/).
> [!NOTE]
> Also, make sure to have the necessary compilation tools installed such as libctru, citro3d, etc.
2. Navigate to your project directory in the terminal.
3. Use the provided Makefile to compile your project.
`make -f path/to/Makefile`
> [!NOTE]
> Replace path/to/Makefile with the actual path to your Makefile.

## Debugging
> Some tools are provided to help you debug you game:
>   -dbg_view.py _A python script with a UI to see the logger printed text in real time compatible with Richtext_
> [!TIP]
> Do not hesitate to propose some other debugging utilities.

## Documentation
Working on a Doxygen documentation
