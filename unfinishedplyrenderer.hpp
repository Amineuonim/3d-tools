#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <pthread.h>
#include <string>
#include "happly.h"
#include "glad/glad.h"
#include <chrono>

std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec4 position;

void main()
{
    gl_Position = position; // Pass position to the pipeline
}
)";

std::string fragmentShaderSource = R"(
#version 330 core
layout (location =0) out vec4 color;
void main()
{
    // Output the hot pink color (RGB: 255, 105, 180)
    color = vec4(1.0, 0.41, 0.71, 1.0); // Normalized RGBA
}
)";


class ply_object{
  private:

  std::vector<std::array<double, 3>> vertices;
  std::vector<std::vector<size_t>> faces;
  std::string filepath;
  GLuint vao,vbo,ebo;
  
  void initialize_vbo(){
// VAO
    glBindVertexArray(vao);
// VBO
    GLint vertices_size = vertices.size() * sizeof(std::array<float, 3>);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    std::vector<std::array<float, 3>> float_vertices(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = 0; j < 3; ++j) {
            float_vertices[i][j] = static_cast<float>(vertices[i][j]);
        }
    }
    glBufferData(GL_ARRAY_BUFFER, float_vertices.size() * sizeof(std::array<float, 3>), float_vertices.data(), GL_DYNAMIC_DRAW);

    int vbo_size =0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vbo_size);
    if(vbo_size != vertices_size){
      perror("vbo not initialized properly.\n");
      printf("vbo contains %d bytes when it should contain %d bytes.\n",vbo_size,vertices_size);
      exit(1);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(std::array<double, 3>), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);


  }
  
  void initialize_ebo(){
// VAO
    glBindVertexArray(vao);
// EBO
    GLint faces_size = faces.size() * 3 * sizeof(unsigned int);
    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces_size,NULL,GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,faces_size,faces.data());
    
    int ebo_size =0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &ebo_size);
    if(ebo_size != faces_size){
      perror("ebo not initialized properly.\n");
      printf("ebo contains %d bytes when it should contain %d bytes.\n",ebo_size,faces_size);
      exit(1);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);



  }
 
  
  public:
  ply_object(const std::string& Path) : filepath(Path) {}

  
  void load() {
  try {
    auto start = std::chrono::high_resolution_clock::now();
    glGenVertexArrays(1,&vao);

    happly::PLYData plyIn(filepath);

    // Get mesh-style data
    vertices = plyIn.getVertexPositions();
    faces = plyIn.getFaceIndices<size_t>();
    std::printf("loaded %d vertices and %d faces.\n",vertices.size(),faces.size());

    initialize_vbo();
    initialize_ebo();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution time for load func: " << duration.count() << " microseconds" << std::endl;


    if (vertices.empty() || faces.empty()) {
      std::cerr << "Error: No data in .ply file." << std::endl;
      exit(1);
    }


    } 
    catch (const std::exception& e) {
      std::cerr << "Error loading .ply file: " << e.what() << std::endl;
      exit(1);
    }
}
  
  void render() {
    auto start = std::chrono::high_resolution_clock::now();

    std::printf("rendering started.\n");

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, faces.size()*3, GL_UNSIGNED_INT,0);


    glBindVertexArray(0);


    std::printf("rendering finished.\n");
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;


}

  ~ply_object()= default;

};



