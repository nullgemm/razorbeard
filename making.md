# Razorbeard Widgets
This file serves as a simple report of the latest developments in the native
widget toolkits of macOS, Windows and Linux, namely WinUI, Cocoa, and GTK+.
All these native controls are expressed here in terms of elementary components:
this helped indentifying what widgets to provide in Razorbeard.

GTK+ was chosen over Qt as the unix-world sample in this document because,
in my humble opinion, the latter is broader in scope and does not qualify
as we will concentrate on desktop widgets exclusively.

## Elementary widgets
 - Grid `Handle`
 - Pager
 - Tabs
 - Popup
 - Drop Menu

 - Separator
 - Text+Link
 - Buffer

 - Button `Symbol` `Toggle`
 - Number Box
 - Text Box `Symbol`
 - Text Area
 - Radio Button
 - Checkbox
 - Scroll Bar
 - Slider
 - Progress Bar

## Extra widgets
 - Progress Ring
 - Color picker

## Native controls decomposition examples
Because Razorbeard implements an elementary set of widgets, some of the advanced
controls provided by major OS toolkits are not reproduced in the core library.

You will have to re-build them using the primitives provided by Razorbeard.
To better illustrate this, a list of native controls and their decomposition
in terms of Razorbeard widgets can be found below.

Controls marked with `(concept)` are not detailed for the following reasons:
 - They are only relevant in their target ecosystem
 - They are already a composition of native widgets

### Windows (WinUI) controls in terms of Razorbeard Widgets
 - Animated visual player
```
Buffer
```

 - Auto-suggest box
```
Text Box+Symbol
Drop Menu
```

 - Button
```
Button
```

 - Calendar date picker
```
Text Box+Symbol
Popup
Grid
Text
Button
```

 - Calendar view
```
Popup
Grid
Text
Button
```

 - Checkbox
```
Checkbox
```

 - Color picker
```
Grid
Color picker
Slider
Button+Symbol
Drop Menu
```

 - Combo box
```
Button+Symbol
Drop Menu
```

 - Command bar
```
Grid
Button
Drop Menu
```

 - Command bar flyout
```
Drop Menu
```

 - Contact card
```
Grid
Text
Button+Symbol
Separator
```

 - Content dialog
```
Popup
Grid
Text
Button
```

 - Content link
```
Text+Link
```

 - Context menu
```
Drop Menu
```

 - Date picker
```
Button
Popup
Pager
Separator
```

 - Dialogs and flyouts (concept)

 - Drop down button
```
Button+Symbol
Drop Menu
```

 - Flip view
```
Buffer
Button
```

 - Flyout
```
Popup
```

 - Forms (concept)

 - Grid view
```
Grid
```

 - Hyperlink
```
Text+Link
```

 - Hyperlink button
```
Button
```

 - Images and image brushes
```
Buffer
```

 - Inking controls
```
Button
Popup
Grid
Text
Buffer
Slider
```

 - List view
```
Grid
```

 - Map control
```
Buffer
```

 - Master/details
```
Grid
```

 - Media playback
```
Buffer
Grid
Slider
Text
Button
```

 - Menu bar
```
Grid
Button+Symbol
Drop Menu
```

 - Menu flyout
```
Drop Menu
```

 - Navigation view
```
Grid
Button+Symbol
```

 - Number box
```
Number Box
```

 - Parallax view (concept)

 - Password box
```
Text Box
```

 - Person picture
```
Buffer
```

 - Pivot
```
Grid
Button
```

 - Progress bar
```
Progress Bar
```

 - Progress ring
```
Progress Ring
```

 - Radio button
```
Radio Button
```

 - Rating Control
```
Grid
Button
```

 - Repeat button
```
Button
```

 - Rich edit box
```
Grid
Button+Symbol
Drop Menu
Text Area
```

 - Rich text block
```
Text
```

 - Scroll viewer
```
Pager
Scroll Bar
```

 - Search
```
Text Box+Symbol
Drop Menu
```

 - Semantic zoom (concept)

 - Shapes (concept)

 - Slider
```
Slider
Popup
```

 - Split button
```
Button+Symbol
Drop Menu
```

 - Split view
```
Grid
```

 - Swipe control (concept)

 - Tab view
```
Tabs
```

 - Teaching tip
```
Popup
Grid
Text
Button
```

 - Text block
```
Text
```

 - Text box
```
Text Box
```

 - Time picker
```
Button
Popup
Grid
Pager
Separator
```

 - Toggle switch
```
Button
Text
```

 - Toggle button
```
Button+Toggle
```

 - Toggle split button
```
Button+Toggle+Symbol
Drop Menu
```

 - Tooltips
```
Popup
Text
```

 - Tree view
```
Grid
Button+Symbol
```

 - Two-pane view
```
Grid+Handle
```

 - Web view
```
Buffer
```

### macOS (Cocoa) controls in terms of Razorbeard Widgets
 - Alert
```
Popup
Grid
Buffer
Button
```

 - Box
```
Frame
```

 - Column view
```
Grid
```

 - Image view
```
Buffer
```

 - Outline view
```
Grid
Button+Symbol
```

 - Panel - HUD-style
```
Popup
Grid
Button
Text
```

 - Panel - standard
```
Popup
Grid
Button
Text
```

 - Popover
```
Drop Menu
```

 - Scroll view
```
Pager
Scroll Bar
```

 - Sheet
```
Popup
```

 - Sidebar
```
Grid
```

 - Split view
```
Grid+Handle
```

 - Tab view
```
Tabs
```

 - Table view
```
Grid
Text
```

 - Toolbar
```
Grid
Button
```

 - Web view
```
Buffer
```

 - Window
```
Popup
```

 - Contextual menu
```
Drop Menu
```

 - Dock menu
```
Drop Menu
```

 - Menu bar menu
```
Drop Menu
```

 - Action button
```
Button+Symbol
Drop Menu
```

 - Checkbox
```
Checkbox
```

 - Disclosure button
```
Button+Symbol
```

 - Disclosure triangle
```
Button+Symbol
```

 - Gradient button
```
Button
```

 - Help button
```
Button
```

 - Image button
```
Buffer
```

 - Pop-up button
```
Button+Symbol
Drop Menu
```

 - Pull-down button
```
Button+Symbol
Drop Menu
```

 - Push button
```
Button
```

 - Radio button
```
Radio Button
```

 - Scope button
```
Button
```

 - Combo box
```
Text Box+Symbol
Drop Menu
```

 - Label
```
Text
```

 - Search field
```
Text Box+Symbol
```

 - Text field
```
Text Box
```

 - Token field
```
Text Box
Grid
Button+Symbol
```

 - Color well
```
Button
Popup
Grid
Button
Color Picker
Slider
```

 - Date picker
```
Text Box+Symbol
Popup
Grid
Text
Button
```

 - Image well
```
Button
```

 - Path control
```
Grid
Button
```

 - Segmented control
```
Button
```

 - Slider
```
Slider
```

 - Stepper
```
Number Box
```

 - Capacity indicator - continuous
```
Progress Bar
```

 - Capacity indicator - discrete
```
Progress Bar
```

 - Progress bar
```
Progress Bar
```

 - Progress circle
```
Progress Ring
```

 - Progress spinner
```
Progress Ring
```

 - Rating indicator
```
Grid
Button
```

 - Relevance indicator
```
Progress Bar
```

### GTK+ controls in terms of Razorbeard Widgets
 - Label
```
Text
```

 - Accel label
```
Text
```

 - Spinner
```
Progress Ring
```

 - Status bar
```
Grid
Text
```

 - Level bar
```
Progress Bar
```

 - Progress bar
```
Progress Bar
```

 - Info bar
```
Grid
```

 - Scroll bar
```
Scroll Bar
```

 - Image
```
Buffer
```

 - Separators
```
Separator
```

 - Multiline text
```
Text
```

 - Scales
```
Slider
```

 - OpenGL
```
Buffer
```

 - Button
```
Button
```

 - Check button
```
Checkbox
```

 - Toggle button
```
Button+Toggle
```

 - Link button
```
Text+Link
```

 - Menu button
```
Button
```

 - Unlock
```
Button+Symbol
```

 - Spin button
```
Number Box
```

 - Color button
```
Button
```

 - Toggle
```
Button
```

 - Combo box
```
Button+Symbol
```

 - Combo box text
```
Button+Symbol
```

 - Font button
```
Button
```

 - Application button
```
Button+Symbol
```

 - Volume
```
Popup
Grid
Button
Slider
```

 - Radio button
```
Radio Button
```

 - File button
```
Button+Symbol
```

 - Folder button
```
Button+Symbol
```

 - Entry
```
Text Box
```

 - Search
```
Text Box+Symbol
```

 - Scrolled window
```
Pager
Scroll Bar
```

 - Header bar
```
Grid
Text
Button
```

 - Search bar
```
Text Box+Symbol
```

 - Status buttons
```
Grid
Button
```

 - List box
```
Grid
```

 - Flow box
```
Grid
```

 - Stack
```
Grid
Button
```

 - Stack switcher
```
Grid
Button
```

 - Sidebar
```
Tabs
```

 - Table
```
Grid
```

 - Icon view
```
Grid
Buffer
```

 - Notebook
```
Grid
Button
```

 - Frame
```
Frame
```

 - Panes
```
Grid+Handle
```

 - Menu bar
```
Grid
Button
```

 - Icon bar
```
Grid
Button
```

 - Tool bar
```
Grid
```

 - Places sidebar
```
Pager
Scroll Bar
Grid
Text
Button
```

 - Window (concept)

 - Dialog (concept)

 - About (concept)

 - Assistant (concept)

 - Color chooser (concept)

 - File chooser (concept)

 - Recent chooser (concept)

 - Font chooser (concept)

 - Application selector (concept)

 - Page setup (concept)

 - Print dialog (concept)
