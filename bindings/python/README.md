## Installation/Usage

- To install the Monochrome python module execute the following:
    1) Open ```setup.py``` and modify ```mod_include_dirs``` and ```mod_library_dirs```
    to contain the local path to Monochrome source and library directories.
    2) Open terminal/cmd with admin privileges in this directory that contains the 'src' folder.
    3) Run: ```python setup.py install```
    

- To generate a visual studio solution with the bindings c++ source code:
    1) Run ```cmake_setup.bat``` to generate a build folder with a VS solution inside.
    -- **Note**: Do not use Visual Studio's compiler to build the module, use instructions earlier to build/install the module.
        
    

## Sample Code

```py
from Monochrome import *

clicked_times = 0

def button_onclick():
    global button, clicked_times
    clicked_times += 1
    button.Label.Text = "Clicked " + str(clicked_times) + " Times"


if __name__ == '__main__':
    window = UIWindow()

    global button
    button = UIButton()
    button.layer.position = (200, 200)
    button.layer.size = (180, 50)
    button.layer.color = (90, 40, 40)
    button.Label.Text = "Click Me :D"
    button.AddMouseClickedEventHandler(button_onclick)
    window.AddView(button)

    window.StartWindowLoop()
```