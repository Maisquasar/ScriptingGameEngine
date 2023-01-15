# Scripting OpenGL
 This project is a Python Scripting system added on the game engine
# Play
```
Launch OpenGL.sln and run the project, next press play or F5 and enjoy.
```
# Inputs 

 - Escape : Capture Mouse and enable movement
 - W A S D : Move.
 - Space : Go Up.
 - F : Go Down.
 - Left Shift : Increase Speed.
 - F1 : enable/disable Wireframe render.
 - F5 : Enter/Exit Play Mode.
 - F6 : To reload All Scripts.
 - F11 : Enable/Disable Full Screen.
 - Mouse Right Click on a Node : Allow To add New Node to selected Node, can be a model, a Light or a default Node with no Components.
 # Inputs in Game (Play)

 - Escape : go in the editor ( modification not saved)
 - W A S D* : Move.
 - Space* : jump.
 - P : Open the Pause Menu.

All Menus are navigable with keyboard :

- Up/Down Arrow : Move.
- Space : Activation Key.

**Can Be Changed in Pause Menu and Main Menu.*

***

# Scripting System Research Report

The objective of this Research Report is to introduce us to writing reports and choose a library between Swig and Pybind11 for our Game Engine.
To realize this project, we have 10 days (26/09/2021 –09h to 05/10/2021 –16).
The ide used is visual sutdio 2019. The languages we use for the project are c++ and python.
[![C++](https://img.shields.io/badge/C++-v17-blue)](https://isocpp.org/)
[![python](https://img.shields.io/badge/python-v3.7-blue)](https://www.python.org/)

## Summary

- [The pros and cons](#the-pros-and-cons)
- [Additional Information](#additional-information)
- [Conclusion](#conclusion)
- [Implementation](#implementation)
- [Supervisor](#supervisor)
- [Authors](#authors)
## The pros and cons
||Swig|Pybind11|
|---|---|---|
|don't need a DLL|❌|✔|
|auto generate|✔|❌|
|simple to install/setup|❌|✔|
|doesn't require command|❌|✔|

## Additional Information
Here are the resonement of our classification:

- "simple to install/setup": Here the first thing is that for Pybind11 we just have to go to Github:
![screenshot](https://cdn.discordapp.com/attachments/1027192447232643152/1027193963368693860/unknown.png)<br/>
![screenshot](https://cdn.discordapp.com/attachments/1027192447232643152/1027199393981542431/unknown.png)<br/>
but for Swig we have to click on a download link on their site which doesn't look like very secure, the link redirects us to sourceforge.net which really feels like downloading a virus:
![screenshot](https://cdn.discordapp.com/attachments/1027192447232643152/1027193233694015548/unknown.png)<br/>
![screenshot](https://cdn.discordapp.com/attachments/1027192447232643152/1027193444097065050/unknown.png)<br/>
[![Watch the video](https://cdn.discordapp.com/attachments/1027192447232643152/1027202507845402624/unknown.png)](https://youtu.be/HDD9QqLtAws?t=28)<br/>
Then regarding the inclusion in the project for Pybind11 wo is Header only but Swig need dll, .i files etc... For this reason we think that Pybind11 is simpler to install and setup
- "doesn't require command": We decide to make this point because if we have to do multy-platform it's best to not use command so Swig is a bit problematic

## Conclusion
To conclude we decide to use **Pybind11** because it's easy to use , easy to setup, good for multy-platform and also very used

## Implementation

So we implemented **Pybind11** in our engine just by including the header 'pybind.h'.

To Set The Path to search the scripts, in our PythonScriptManager we just implement an Initialize :

![screenshot](https://media.discordapp.net/attachments/1027192447232643152/1027213049247039549/unknown.png?width=764&height=182)

Then we just Import Modules with a Try/Catch to Get exception if there is:

![screenshot](https://media.discordapp.net/attachments/1027192447232643152/1027213436997869668/unknown.png?width=758&height=358)

In our Python Scripts to get class variable and methods, we bind the class we want with **Pybind11**, Example with Transform class:

![screenshot](https://media.discordapp.net/attachments/1027192447232643152/1027214060833493062/unknown.png?width=501&height=361)

Then in our script we can Import Module by the name (here : 'S_Transform') and access to methods and variables :

![screenshot](https://media.discordapp.net/attachments/1027192447232643152/1027214585649975418/unknown.png?width=805&height=389)

And we created a 'PythonScript' class with a Reload function that reloads the script even if it failed to compile (Press F6 in game) and an 'ExecuteFunction' function that will take as parameters the name of the function, and the arguments of the function to call, it's a function template. All Function Executed are stored into a map to avoid rereading the script every time: 

```c++
std::map<std::string, pybind11::detail::str_attr_accessor*> functions;
```

Example for GameUpdate Execution : PlayerScript(45):

```c++
Script->ExecuteFunction("GameUpdate", this, ImGui::GetIO().DeltaTime);
```

Definition of the Function :

![Image](https://media.discordapp.net/attachments/1027192447232643152/1027215119685537832/unknown.png?width=938&height=260)

To Reload/import Function :

![](https://media.discordapp.net/attachments/1027192447232643152/1027216477285269535/unknown.png?width=760&height=306)



All Scripts are found in the Source/scripts folder.
For the moment, the camera (editor and play mode), the player controls, the Rigid body, and the loading of the Scene are scripted with Python.

Scenes can now being Loaded with .Json files (can be changed by change Macro 'JSON' to false)

## Supervisor
- [KUBIAK Erik](e.kubiak@isartdigital.com )
## Authors

- [ROBION Mathieu](m.robion@student.isartdigital.com)
- [BOURGOGNE Romain](r.bourgogne@student.isartdigital.com)
