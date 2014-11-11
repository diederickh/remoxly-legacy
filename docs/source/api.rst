*************
API Reference
*************

.. highlight:: c++

Work in progress api documentation of remoxly.

Panel 
------

.. cpp:function:: Panel::addGroup(std::string name, int style)
   
   Add a group to the panel with the given name and style. The style 
   is used for the group toggle and only can be either ``GUI_STYLE_NONE``
   or ``GUI_CORNER_ALL``. 

Group
-----

.. cpp:function:: Group::Group(std::string label, Render* r, int style = GUI_CORNER_ALL)
   
   Create a new group.  Style can be either ``GUI_STYLE_NONE`` or ``GUI_CORNER_ALL``. 


                    
                  

