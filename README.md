# OOP

# CLISlides – Command-Line Slide Creation and Presentation Tool
CLISlides is a **C++ command-line application** that allows users to create, edit, and present text-based slides directly in the terminal.  
It is a simplified, lightweight alternative to GUI-based presentation tool like Microsoft PowerPoint. 

## ✨ Features

- **Slide Creation**
  - Add, edit, and delete slides
  - Organize slides in a sequence
  - Support for text, images, shapes and tables
  - Custom slide backgrounds (colors, images)
 
- **Slide Editing**
  - Rich text, shape formatting (fonts, sizes, colors, line thickness, style, ...)
  - Resize and move objects on the slide
  - Add text to object
  - Layer management (bring to front, send to back)
  - Undo/redo system

- **File Management**
  - Save presentations in a plain text(.txt) or JSON(.json) format
  - Load existing presentations
  - Export slides to .txt format

- **Presentation Mode**
  - Full-screen slide show
  - Navigate using keyboard (Next(n)/Previous(p)/Quit(q))

 ## 🛠️ Tech Stack
- **Language**: C++
- **Storage**: JSON or plain text for persistence

 ## 🏗️ Architecture
```
CLI_PowerPoint/
├── Models/                    # Domain models
│   ├── Presentation.h/cpp    # Main presentation container
│   ├── Slide.h/cpp           # Individual slide management
│   ├── Editor.h/cpp          # Edit operations with undo/redo
│   ├── IObject.h             # Shape interface
│   ├── Object.h              # Base shape implementation
│   ├── objects/              # Concrete shape implementations
│   │   ├── Circle.h/cpp
│   │   ├── Rectangle.h/cpp
│   │   ├── Text.h/cpp
│   │   └── Image.h/cpp
│   └── utility/              # Helper classes
│       ├── Geometry.h/cpp    # Position and size
│       ├── Color.h           # Color representation
│       └── Border.h          # Border styling
│
├── Core/                      # Core application logic
│   ├── Controller.h/cpp      # Main application controller
│   ├── Parser.h/cpp          # Command parsing
│   ├── Tokenizer.h/cpp       # Lexical analysis
│   ├── CommandRegister.h/cpp # Command registry
│   ├── action/               # Action pattern for undo/redo
│   │   ├── IAction.h
│   │   ├── AddSlideAction.h/cpp
│   │   ├── RemoveSlideAction.h/cpp
│   │   ├── AddShapeAction.h/cpp
│   │   └── RemoveShapeAction.h/cpp
│   └── vizualization/        # Rendering system
│       ├── IPainter.h
│       ├── SVGPainter.h/cpp
│       ├── IVisitor.h
│       ├── DrawingVisitor.h/cpp
│       └── PresentationDrawer.h/cpp
│
├── Commands/                  # Command implementations
│   ├── Command.h             # Command interface
│   ├── AddSlideCommand.h/cpp
│   ├── RemoveSlideCommand.h/cpp
│   ├── AddShapeCommand.h/cpp
│   ├── RemoveShapeCommand.h/cpp
│   ├── SaveCommand.h/cpp
│   ├── LoadCommand.h/cpp
│   ├── RenderCommand.h/cpp
│   ├── UndoCommand.h
│   ├── RedoCommand.h
│   └── factory/              # Command creation
│       ├── ICmdCreator.h
│       ├── AddSlideCreator.h
│       ├── RemoveSlideCreator.h
│       ├── AddShapeCreator.h
│       └── ...
│
└── Serialization/            # File I/O
    ├── ISerializer.h
    ├── IDeserializer.h
    ├── JsonSerializer.h/cpp
    ├── JsonDeserializer.h/cpp
    └── SerializerFactory.h/cpp
```

## 🎯 Design Patterns Used

* **Command Pattern:** All user actions are encapsulated as command objects
* **Factory Pattern**: Command creation and serializer/deserializer instantiation
* **Visitor Pattern**: Shape rendering using visitor pattern for extensibility
* **Strategy Pattern**: Different painter implementations (SVG, etc.)
* **Memento Pattern**: Undo/redo functionality through action inversion
* **Registry Pattern**: Command registration and lookup

## 🎓 Command Quick Reference
| Command | Syntax | Description |
| ---------|----------------------|------------------|
| Add Slide | add slide -at pos|  Create new slide |
| Remove Slide | remove slide -at pos | Delete slide |
| Add Shape | add shape -type type -at slide | Add shape to slide |
| Remove Shape | remove shape -at slide | Remove shape | 
| Set Title | set title -text "text" | Set presentation title |
| Save | save "filename" | Save presentation to JSON file |
| Load | load "filename" | Load presentation from JSON file |
| Exit | exit | Quit application |

## 💡 Usage Examples
When you run the program, you’ll be presented with the following interface:
```
 Presentation Editor Ready. Enter commands (or 'exit' to quit):
```
Commands follow this general pattern:
> <action> [object] [flags] [positional arguments]
```
# Add a new slide at position 0
> add slide -at 0

# Add a slide at the end (default position)
> add slide

# Remove a slide at position 1
> remove slide -at 1

# Add a red circle
> add shape -slide 0 -type circle -x 100 -y 100 -width 50 -height 50 -color red

# Add text
> add shape -slide 0 -type text -x 50 -y 50 -width 200 -height 30 -color white

# Save presentation
> save presentation.json

# Load presentation
> load presentation.json

# Render to file
> render output.svg

# Undo last action
> undo

# Redo undone action
> redo
```
