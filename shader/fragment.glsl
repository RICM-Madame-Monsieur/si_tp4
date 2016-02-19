// Version d'OpenGL
#version 150

in vec3 vert_color;

out vec4 frag_color;

// Fonction appellee pour chaque fragment
void main()
{
  // Affectation de la couleur du fragment
  frag_color = vec4(vert_color,1.0);
}
