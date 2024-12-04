#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "happly.h"
#include <GL/glut.h>

class ply_object{
  private:
  std::vector<std::array<double, 3>> vertexes;
  std::vector<std::vector<size_t>> faces;
  std::string filepath;
  public:
  ply_object(const std::string& Path) : filepath(Path) {}

  
  void load() {
  try {
    happly::PLYData plyIn(filepath);

    // Get mesh-style data
    vertexes = plyIn.getVertexPositions();
    faces = plyIn.getFaceIndices<size_t>();

    if (vertexes.empty() || faces.empty()) {
      std::cerr << "Error: No data in .ply file." << std::endl;
      return;
    }

    std::cout << "Loaded " << vertexes.size() << " vertices and " << faces.size() << " faces.\n";
    std::cout << "First vertex: (" << vertexes[0][0] << ", " << vertexes[0][1] << ", " << vertexes[0][2] << ")\n";
    std::cout << "First face: (" << faces[0][0] << ", " << faces[0][1] << ", " << faces[0][2] << ")\n";

    } 
    catch (const std::exception& e) {
      std::cerr << "Error loading .ply file: " << e.what() << std::endl;
    }
}
  
  void render() {
    std::printf("rendering started.\n");
    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < faces.size(); i++) {
      if (faces[i].size() != 3) {
        std::cerr << "Warning: Non-triangular face detected, skipping.\nfile: "<<filepath<<".\n";
        continue;
      }
      glColor3f(1.0f,.5f,.05f);
      glVertex3d(vertexes[faces[i][0]][0], vertexes[faces[i][0]][1], vertexes[faces[i][0]][2]);
      glVertex3d(vertexes[faces[i][1]][0], vertexes[faces[i][1]][1], vertexes[faces[i][1]][2]);
      glVertex3d(vertexes[faces[i][2]][0], vertexes[faces[i][2]][1], vertexes[faces[i][2]][2]);
      }
    glEnd();
    std::printf("rendering finished.\n");

}



  ~ply_object()= default;

};



