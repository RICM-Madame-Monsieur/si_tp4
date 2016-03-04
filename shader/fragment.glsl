// Version d'OpenGL
#version 150

in vec3 vert_normal;
in vec3 e;

//in vec3 vert_color;

out vec4 frag_color;

uniform float pa  = 0.5;
uniform float pd  = 0.5;
uniform float ps  = 0.5;
uniform int shiny = 64;
uniform vec3 la  = vec3( 1.0,  0.0,  0.0);
uniform vec3 ld  = vec3( 1.0,  0.5,  0.5);
uniform vec3 ls  = vec3( 1);
uniform vec3 l   = vec3( 4.0,  4.0, -3.0);

// Fonction appellee pour chaque fragment
void main()
{

	vec3 r = normalize(reflect(l, vert_normal));

	vec3 ambient  = vec3(pa * la);
	vec3 diffuse  = vec3(pd * ld * max(dot(-normalize(vert_normal), normalize(l)), 0));
  	vec3 specular = vec3(ps * ls * pow(max(dot(r, e), 0), shiny));
  	
  	vec3 vert_color = vec3(ambient + diffuse + specular);

  	// Affectation de la couleur du fragment
	frag_color = vec4(vert_color,1.0);
}
