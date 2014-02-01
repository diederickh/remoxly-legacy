## About

While working on interactive simulations and installations for museums and 
conferences we've used many different GUIs to help us to get just the correct 
settings for the simulation. In fact it often feels like most of our time
goes into tweaking the parameters to get the exact result we like. Our projects 
involve using projectors too. Controlling a GUI on something which gets projected 
on a non-flat surface is not so easy. Creating this GUI is a step towards a GUI 
that can be directly controlled inside your application but also through 
through the web, without the need of configuring a router. This means that 
you can control your application from anywhere and anytime, all in a safe way. 

After creating several complete GUIs we have enough experience to create a new one
with all experience from the previous times. It will be a basic and blazingly 
fast GUI.

The goal of this project is to create a couple of GUIs. One will be 
directly used inside your OpenGL/DirectX* application. Another one can 
be used in a browser to directly control the parameters of your (C/C++)
application over the internet, through firewalls. This also means that the
GUI will provide a way to use your mobile phone to tweak parameters. And, 
if this isn't enough, a local networked GUI will be created too.

For this project we develop a couple of separate classes which can be
useful for other projects. We've tried to keep these reusable pieces
separate from the GUI, although the GUI needs them. Therefore we have
several sub-projects:

## Sub Projects

 - BitmapFont: A BitmapFont renderer using BMFont files. 
 - FontBaker: Bakes C++ headers with pixel and font data.
 - TextInput: Uses BitmapFont to create a text input type.
 - Gui: This is -the- gui for you OpenGL/DirectX apps.

_*DirectX is in development_






