#include <iostream>
#include <cmath>
#include <unistd.h>
#include <cstring>
#include <vector>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

using namespace std;

const char* colors[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
const int num_colors = sizeof(colors) / sizeof(colors[0]);

const char shades[] = " .:!/r(l1Z4H9W8$@";
const int num_shades = sizeof(shades) - 1;

// Función para limpiar la pantalla
void clear_screen() {
    cout << "\033[2J\033[H";
}

// Función para obtener el tamaño de la terminal (Linux)
void get_terminal_size(int &width, int &height) {
    FILE* fp = popen("stty size", "r");
    if (fp) {
        fscanf(fp, "%d %d", &height, &width);
        pclose(fp);
    }
}

// Dibuja el toroide con optimizaciones
void draw_torus(double A, double B, int WIDTH, int HEIGHT) {
    vector<vector<string>> screen(HEIGHT, vector<string>(WIDTH, " "));
    vector<vector<double>> z_buffer(HEIGHT, vector<double>(WIDTH, -1e10));

    double R1 = 1, R2 = 2, K2 = 5;
    double K1 = WIDTH * K2 * 3.0 / (8.0 * (R1 + R2));

    // Precalcular valores comunes
    double cosA = cos(A), sinA = sin(A), cosB = cos(B), sinB = sin(B);

    // Generar toroide
    for (double theta = 0; theta < 2 * M_PI; theta += 0.07) {
        double cosT = cos(theta), sinT = sin(theta);
        for (double phi = 0; phi < 2 * M_PI; phi += 0.02) {
            double cosP = cos(phi), sinP = sin(phi);

            double circleX = R2 + R1 * cosT;
            double circleY = R1 * sinT;

            double x = circleX * (cosB * cosP + sinA * sinB * sinP) - circleY * cosA * sinB;
            double y = circleX * (sinB * cosP - sinA * cosB * sinP) + circleY * cosA * cosB;
            double z = K2 + cosA * circleX * sinP + circleY * sinA;
            double ooz = 1 / z;

            int xp = static_cast<int>(WIDTH / 2 + K1 * ooz * x);
            int yp = static_cast<int>(HEIGHT / 2 - K1 * ooz * y / 2);

            // Cálculo de iluminación y profundidad
            double L = cosP * cosT * sinB - cosA * cosT * sinP - sinA * sinT + 0.5;
            double depth = (z - K2 + R1 + R2) / (2 * (R1 + R2));

            if (L > 0 && xp >= 0 && xp < WIDTH && yp >= 0 && yp < HEIGHT) {
                if (ooz > z_buffer[yp][xp]) {
                    z_buffer[yp][xp] = ooz;

                    int shade_index = static_cast<int>((L * 0.7 + depth * 0.3) * (num_shades - 1));
                    shade_index = (shade_index < 0) ? 0 : (shade_index >= num_shades ? num_shades - 1 : shade_index);

                    int color_index = static_cast<int>(depth * (num_colors - 1));
                    color_index = (color_index < 0) ? 0 : (color_index >= num_colors ? num_colors - 1 : color_index);

                    screen[yp][xp] = string(colors[color_index]) + shades[shade_index] + RESET;
                }
            }
        }
    }

    // Usar un buffer de salida para minimizar operaciones I/O
    string output;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            output += screen[y][x];
        }
        output += "\n";
    }
    cout << output;
}

int main() {
    double A = 0, B = 0;
    int WIDTH = 110, HEIGHT = 50;

    // Detectar tamaño de la terminal para ajustar WIDTH y HEIGHT
    get_terminal_size(WIDTH, HEIGHT);

    while (true) {
        clear_screen();
        draw_torus(A, B, WIDTH, HEIGHT);
        usleep(50000);  // Pausa de 50 ms
        A += 0.04;
        B += 0.02;
    }
    return 0;
}
