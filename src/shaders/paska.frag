#version 420 core
uniform double dTime;

out vec4 FragColor;

in vec3 vtxCol;

float fTime = float(dTime);

float r = sin(fTime) / 2.0 + 0.5;
float g = sin(fTime-1.047) / 2.0 + 0.5;
float b = sin(fTime-2.094) / 2.0 + 0.5;

void main() {
    FragColor = vec4(r, g, b, 1.0);
}
