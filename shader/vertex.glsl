// Version d'OpenGL
#version 150

// Donnees d'entree
in vec3 in_position;
in vec3 in_normal;

// Donnees de sortie
out vec3 vert_color;
out vec3 vert_normal;

// Parametres
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float theta = 30.0;
uniform float alpha;
uniform float pa = 0.5;
uniform float pd = 0.5;
uniform vec3 la = vec3(0.5, 0.5, 1.0);
uniform vec3 ld = vec3(0.5, 0.5, 0.5);
uniform vec3 sun = vec3(4.0, 4.0, -3.0);

// Fonction appellee pour chaque sommet
void main()
{
  // Affectation de la position du sommet
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_position, 1.0);

//  vert_color = vec3(1.0, 0.0, 0.0);
  // vert_color = 0.5 * (vec3(1.0) + in_normal);
  vert_color = vec3(pa * la + pd * ld * dot(-in_normal, sun));
  vert_normal = (transpose(inverse(ModelMatrix)) * vec4(in_normal, 0.0)).xyz;
//  vert_color = gl_Position.xyz;
}
