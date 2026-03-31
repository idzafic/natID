#version 330 core

out vec4 FragColor;

// Očekujemo samo jednu boju za cijeli objekat
uniform vec4 objectColor;

void main()
{
    FragColor = objectColor;
}