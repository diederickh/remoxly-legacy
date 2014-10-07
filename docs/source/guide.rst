*****************
Programmers Guide
*****************

This page describes what you need to get started with the Remoxly. 
Remoxly is a GUI that you can use directly in your C/C++ OpenGL 
applications.

Drawing with openGL
-------------------

Remoxly does not set/unset any states because we don't want to use
any `glGet*` functions when it's not really necessary. This does mean
that you need to make sure the state is correct. See below what you 
can do to make sure the state is correct for openGL.

.. highlight:: c++

::

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
