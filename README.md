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
├── Controller          # Main application controller
├── Parser              # DFA-based command parser
├── Tokenizer          # Lexical analyzer
├── CommandFactory     # Factory for command creation
├── Commands           # Command pattern implementations
│   ├── AddSlideCommand
│   ├── AddShapeCommand
│   ├── RemoveSlideCommand
│   ├── RemoveShapeCommand
│   └── SetTitleCommand
├── ShapeFactory       # Factory for shape creation
├── Presentation       # Presentation model
├── Slide              # Slide model
└── Shapes             # Shape hierarchy
    ├── Circle
    ├── Rectangle
    └── Text
```

## 🎓 Command Quick Reference
| Command | Syntax | Description |
| ---------|----------------------|------------------|
| Add Slide| add slide [-at <pos>]| Create new slide |
| Remove Slide| remove slide -at <pos>| Delete slide |
Add Shapeadd shape -type <type> -at <slide>Add shape to slideRemove Shaperemove shape -at <slide>Remove shapeSet Titleset title "text"Set presentation titleExitexitQuit application

## 💡 Usage Examples
When you run the program, you’ll be presented with the following interface:
```
 Presentation Editor Ready. Enter commands (or 'exit' to quit):
```
Commands follow this general pattern:
> <ACTION> <TARGET> [OPTIONS]
```
# Create presentation structure
add slide                              # Slide 0
add slide                              # Slide 1
add slide                              # Slide 2

# Add content
set title "Project Overview"
add shape -type text -at 0
add shape -type "rectangle" -at 1 -pos 50,100
add shape -type "circle" -at 2 -pos 200,150
```
