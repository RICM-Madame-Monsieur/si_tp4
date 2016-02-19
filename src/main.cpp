#include <iostream>
#include <vector>
#include <string>

#include <shader.h> // Help to load shaders from files

// Include GLEW : Always include it before glfw.h et gl.h :)
#include <GL/glew.h>    // OpenGL Extension Wrangler Library : http://glew.sourceforge.net/ 
#include <GL/glfw.h>    // Window, keyboard, mouse : http://www.glfw.org/

#include <glm/glm.hpp>  // OpenGL Mathematics : http://glm.g-truc.net/0.9.5/index.html
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include <GLFW_define.h>
#include <Mesh.h>

// Dimensions de la fenêtre :
#define WIDTH 1000.0f
#define HEIGHT 800.0f

using namespace glm;
using namespace std;


void view_control(mat4& view_matrix, float dx);

int main()
{

    cout << "Debut du programme..." << endl;

    //==================================================
    //============= Creation de la fenetre =============
    //==================================================


    // Initialisation de GLFW
	if( !glfwInit() )
	{
		cout << "Echec de l'initialisation de GLFW" << endl;
		exit(EXIT_FAILURE);
	}

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // Anti Aliasing
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // OpenGL 2.1
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Ouverture d'une fenêtre en 1024x768
    // et creation d'un contexte OpenGL
    if( !glfwOpenWindow(WIDTH, HEIGHT, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		cout << "Echec de l'ouverture de fenetre OpenGL" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
    }

    // Definition du titre de la fenêtre
	glfwSetWindowTitle( "Polytech RICM 4 - TP4" );

    // Autorise GLFW a recevoir les appuis de touche
	glfwEnable( GLFW_STICKY_KEYS );

    // Initialisation de  GLEW
	if (glewInit() != GLEW_OK) {
		cout << "Echec de l'initialisation de GLEW" << endl;
		exit(EXIT_FAILURE);
	}
    
    // Verification des donnees du contexte OpenGL
    const GLubyte* renderer = glGetString (GL_RENDERER);
    cout << "Carte Graphique : " << renderer << endl;

    const GLubyte* version = glGetString (GL_VERSION);
    cout << "Driver OpenGL : " << version << endl;


    //==================================================
    //================= Initialisation =================
    //==================================================

    cout << "Initialisations..." << endl;


    // Definition de la couleur du fond
    glClearColor(0.1, 0.1, 0.1, 0.0);



//-------------------------------------------------
// Initialisation du shader programm

    // Compilation du shader programm
	GLuint programID = LoadShaders( "../shader/vertex.glsl", "../shader/fragment.glsl" );
    cout << "programID = " << programID << endl;

	// Obtention de l'ID de l'attribut "vertex_position" dans programID
	GLuint vertexPositionID = glGetAttribLocation(programID, "in_position");
    cout << "vertexPositionID = " << vertexPositionID << endl;

    // Obtention de l'ID de l'attribut "in_normal" dans programID
    GLuint normalID = glGetAttribLocation(programID, "in_normal");
    cout << "normalID = " << normalID << endl;


//-------------------------------------------------
// Initialisation des arrays de données


    string file_name("../models/armadillo.off");
    cout << "chargement de '" << file_name << "'..." << endl;
    Mesh m(file_name.c_str());
    m.normalize();

    cout << "...fini. " << m.nb_vertices << " sommets, " << m.nb_faces << " triangles." << endl;




    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    cout << "vertexBufferID = " << vertexBufferID << endl;
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * m.vertices.size(), m.vertices.data(), GL_STATIC_DRAW);
	
	

    GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    cout << "elementbuffer = " << elementbuffer << endl;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.faces.size() * sizeof(unsigned int), m.faces.data(), GL_STATIC_DRAW);

    // ------------ Normals

    // Creation d'un vertex buffer (VBO) avec normalBufferID pour identifiant
    GLuint normalBufferID;
    glGenBuffers(1, &normalBufferID);
    cout << "normalBufferID = " << normalBufferID << endl;
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, m.normals.size() * sizeof(vec3), m.normals.data(), GL_STATIC_DRAW);





//-------------------------------------------------
// Initialisation des matrices MVP


    // Definition des matrices de transformation
    mat4 projection_matrix = perspective(45.0f, WIDTH / HEIGHT, 0.1f, 100.0f);
    mat4 view_matrix = lookAt(vec3(0.0, 0.0, 2), vec3(0.0), vec3(0.0, 1.0, 0.0));
    mat4 model_matrix = scale(vec3(1.0f) * 0.5f);

    GLuint PmatrixID = glGetUniformLocation(programID, "ProjectionMatrix");
    cout << "PmatrixID = " << PmatrixID << endl;

    GLuint VmatrixID = glGetUniformLocation(programID, "ViewMatrix");
    cout << "VmatrixID = " << VmatrixID << endl;

    GLuint MmatrixID = glGetUniformLocation(programID, "ModelMatrix");
    cout << "MmatrixID = " << MmatrixID << endl;



    // Activation de l'attribut normalID
    glEnableVertexAttribArray(normalID);

    // Definition de vertexBufferID comme le buffer courant
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);

    // On indique comment lire les donnees
    glVertexAttribPointer(
        normalID,           // ID de l'attribut à configurer
        3,                  // nombre de composante par position (x, y, z)
        GL_FLOAT,           // type des composantes
        GL_FALSE,           // normalisation des composantes
        0,                  // decalage des composantes
        (void*)0            // offset des composantes
    );




    cout << "Debut de la boucle principale..." << endl;

    double init_time = glfwGetTime();
    double prec_time = init_time;
    double cur_time = init_time;
    double speed = 2.0;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Boucle de dessin
	do{
        // Nettoyage de la zone de dessin
		glClear( GL_COLOR_BUFFER_BIT );
		glClear( GL_DEPTH_BUFFER_BIT );


        //==================================================
        //============= Calcul du point de vue =============
        //==================================================

        prec_time = cur_time;
        cur_time = glfwGetTime() - init_time;
        double delta_time = cur_time - prec_time;

        glfwSetWindowTitle( (string("Polytech RICM 4 - TP3 - ") + detail::format("%2.0f", 1.0/ delta_time) + string(" FPS")).c_str() );

        view_control(view_matrix, speed * delta_time);
//        view_control(model_matrix, speed * delta_time);


        //==================================================
        //===================== Dessin =====================
        //==================================================

		// Definition de programID comme le shader courant
		glUseProgram(programID);

        // Transmission des matrices au vertex shader
        glUniformMatrix4fv(PmatrixID, 1, GL_FALSE, value_ptr(projection_matrix));
        glUniformMatrix4fv(VmatrixID, 1, GL_FALSE, value_ptr(view_matrix));
        glUniformMatrix4fv(MmatrixID, 1, GL_FALSE, value_ptr(model_matrix));



        // Activation de l'attribut vertexPositionID
		glEnableVertexAttribArray(vertexPositionID);

        // Definition de vertexBufferID comme le buffer courant
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

        // On indique comment lire les donnees
		glVertexAttribPointer(
            vertexPositionID,   // ID de l'attribut à configurer
            3,                  // nombre de composante par position (x, y, z)
            GL_FLOAT,           // type des composantes
            GL_FALSE,           // normalisation des composantes
            0,                  // decalage des composantes
            (void*)0            // offset des composantes
		);




        // Index buffer
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        


         // Dessin des triangles
         glDrawElements(
             GL_TRIANGLES,      // mode
             m.faces.size(),    // nombre
             GL_UNSIGNED_INT,   // type
             (void*)0           // offset
         );



        // Desactivation des attributs
		glDisableVertexAttribArray(vertexPositionID);



		// Echange des zones de dessin buffers
		glfwSwapBuffers();

        cout << "Temps ecoule (s) : " << cur_time << "\r";
        cout.flush();

    } // Execution de la boucle...
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&  // ... jusqu'a appui sur la touche ESC
           glfwGetWindowParam( GLFW_OPENED )        );  // ... ou fermeture de la fenetre

    cout << endl;

    // Ferme GLFW et OpenGL
	glfwTerminate();



    //==================================================
    //============== Nettoyage la memoire ==============
    //==================================================

    // Liberation des buffers
    glDeleteBuffers(1, &vertexBufferID);

    glDeleteBuffers(1, &normalBufferID);



    cout << "Fin du programme..." << endl;


	return EXIT_SUCCESS;
}

void view_control(mat4& view_matrix, float dx)
{


    if (glfwGetKey( GLFW_KEY_LSHIFT ) == GLFW_PRESS)
    {
        dx /= 10.0;
    }

    if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS)
    {
        vec4 axis = vec4(1.0, 0.0, 0.0, 0.0);
        axis = inverse(view_matrix) * axis;
        view_matrix = rotate(view_matrix, dx * 180.0f, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS)
    {
        vec4 axis = vec4(1.0, 0.0, 0.0, 0.0);
        axis = inverse(view_matrix) * axis;
        view_matrix = rotate(view_matrix, -dx * 180.0f, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 1.0, 0.0, 0.0);
        axis = inverse(view_matrix) * axis;
        view_matrix = rotate(view_matrix, dx * 180.0f, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 1.0, 0.0, 0.0);
        axis = inverse(view_matrix) * axis;
        view_matrix = rotate(view_matrix, -dx * 180.0f, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_PAGEUP ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 0.0, 1.0, 0.0);
        axis = inverse(view_matrix) * axis;
        view_matrix = rotate(view_matrix, dx * 180.0f, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_PAGEDOWN ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 0.0, 1.0, 0.0);
        axis = inverse(view_matrix) * axis;
        view_matrix = rotate(view_matrix, -dx * 180.0f, vec3(axis));
    }

    if (glfwGetKey( GLFW_KEY_Z ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 0.0, 1.0, 0.0) * dx;
        axis = inverse(view_matrix) * axis;
        view_matrix = translate(view_matrix, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_S ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 0.0, 1.0, 0.0) * (-dx);
        axis = inverse(view_matrix) * axis;
        view_matrix = translate(view_matrix, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_Q) == GLFW_PRESS)
    {
        vec4 axis = vec4(-1.0, 0.0, 0.0, 0.0) * dx;
        axis = inverse(view_matrix) * axis;
        view_matrix = translate(view_matrix, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_D ) == GLFW_PRESS)
    {
        vec4 axis = vec4(-1.0, 0.0, 0.0, 0.0) * (-dx);
        axis = inverse(view_matrix) * axis;
        view_matrix = translate(view_matrix, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_A ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 1.0, 0.0, 0.0) * dx;
        axis = inverse(view_matrix) * axis;
        view_matrix = translate(view_matrix, vec3(axis));
    }
    if (glfwGetKey( GLFW_KEY_E ) == GLFW_PRESS)
    {
        vec4 axis = vec4(0.0, 1.0, 0.0, 0.0) * (-dx);
        axis = inverse(view_matrix) * axis;
        view_matrix = translate(view_matrix, vec3(axis));
    }

//    view_matrix = translate(view_matrix, c);
    
//    view_matrix  = c * view_matrix;

}

