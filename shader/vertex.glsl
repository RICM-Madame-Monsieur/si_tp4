// Version d'OpenGL
#version 150

// Donnees d'entree
in vec3 in_position;
in vec3 in_normal;

// Donnees de sortie
//out vec3 vert_color;
out vec3 vert_normal;
out vec3 e;

// Parametres
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

// Fonction appellee pour chaque sommet
void main()
{
  // Affectation de la position du sommet
  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_position, 1.0);

  vert_normal = (transpose(inverse(ModelMatrix)) * vec4(in_normal, 0.0)).xyz;

  vec4 c_4 = inverse(ViewMatrix)*vec4(0, 0, 0, 1);
  vec3 c = c_4.xyz;
  e = normalize(c - in_position);

  //vert_color = vec3(ambient + diffuse + specular);
}
