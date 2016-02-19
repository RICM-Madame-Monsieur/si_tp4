// Version d'OpenGL
#version 150

// Donnees d'entree
in vec3 in_position;
in vec3 in_normal;

// Donnees de sortie
out vec3 vert_color;

// Parametres
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

// Fonction appellee pour chaque sommet
void main()
{
  // Affectation de la position du sommet
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_position, 1.0);

//  vert_color = vec3(1.0, 0.0, 0.0);
  vert_color = 0.5 * (vec3(1.0) + in_normal);
//  vert_color = gl_Position.xyz;
}
