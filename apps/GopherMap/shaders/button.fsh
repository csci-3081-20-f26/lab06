#version 330 core

out vec4 FragColor;

uniform sampler2D tex;
uniform bool highlight;
uniform bool selected;
uniform bool border;

in vec2 interpCoord;

void main()
{
   float borderWidth = 0.0;

   if (border) {
    borderWidth = 0.03;
   }

   vec4 color = texture(tex, interpCoord*(1.0 + 2*borderWidth) - vec2(borderWidth));

   // Check if we're near the edge for borders
   bool nearEdge = interpCoord.x < borderWidth || interpCoord.x > (1.0 - borderWidth) ||
                   interpCoord.y < borderWidth || interpCoord.y > (1.0 - borderWidth);

   if (border && selected && nearEdge) {
       // Draw a cyan border for selected items
       FragColor = vec4(0.0, 1.0, 1.0, 1.0);
   }
   else if (border && nearEdge) {
       // Draw a white border for all buttons (even if background is transparent)
       FragColor = vec4(1.0, 1.0, 1.0, 1.0);
   }
   else if (color.a < 0.1) {
       // Discard fully transparent pixels (but not border pixels)
       discard;
   }
   else if (highlight) {
       FragColor = clamp(color*vec4(2.0,2.0,2.0,1.0), 0, 1.0);
   }
   else {
       FragColor = color;//texture(tex, interpCoord*1.5 + vec2(1.0/1.5));
   }
}