#include "mesh.h"

Mesh::Mesh() :
    m_vertices()
{ }

Mesh::~Mesh() { }

Mesh::Mesh(Mesh const& other) :
    m_vertices(other.m_vertices)
{ }

Mesh::Mesh(Mesh&& other) {
    m_vertices = std::move(other.m_vertices);
}

void Mesh::operator=(Mesh const& other) {
    m_vertices = other.m_vertices;
}

void Mesh::operator=(Mesh&& other) {
    m_vertices = std::move(other.m_vertices);
}

