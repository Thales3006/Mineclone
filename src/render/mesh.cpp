#include "render/mesh.h"

Mesh::Mesh() {
    VAO = 0;
    VBO = 0;
    EBO = 0;
    this->vertices = std::vector<Vertex>();
    this->indices = std::vector<unsigned int>();
    this->textures = std::vector<Texture>();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
    VAO = 0;
    VBO = 0;
    EBO = 0;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures) {
    VAO = 0;
    VBO = 0;
    EBO = 0;
    this->vertices = vertices;
    this->indices = std::vector<unsigned int>();
    this->textures = textures;
    setupMesh();
}

void Mesh::setupMesh() {
    if (!vertices.data())
        return;
    if (VBO != 0 || VAO != 0) {
        resetMesh();
        return;
    }
    // criando VAO VBO EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if (indices.data())
        glGenBuffers(1, &EBO);

    // Setando os dados
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(),
                 GL_STATIC_DRAW);

    if (indices.data()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                     GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, ID));

    // zerando
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::resetMesh() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (indices.data()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int),
                        indices.data());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void Mesh::draw(Shader &shader) {

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string name = textures[i].getType();

        glActiveTexture(GL_TEXTURE0 + i);

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else
            std::cout << "Texture not identified" << std::endl;

        shader.setInt((name + number).c_str(), i);
        textures[i].bind();
    }
    glActiveTexture(GL_TEXTURE0);

    shader.use();
    glBindVertexArray(VAO);
    if (indices.data())
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    indices.data() ? glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0)
                   : glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

Mesh::~Mesh() {}

void Mesh::clearBuffers() {
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (EBO)
        glDeleteBuffers(1, &EBO);
    VAO = 0;
    VBO = 0;
    EBO = 0;
}
