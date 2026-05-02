#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "raylib.h"
#include <cmath>
#include <string>

// Her bir hücrenin bilgisini tutan yapý
struct Cell {
    int type;    // 0: Boþ, 1: Kum, 2: Taþ
    Color color; // Taneciðin kendine özel rengi
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int cellSize = 8;

    int brushSize = 3;
    int brushMode = 1;

    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;

    // Grid artýk sadece 0-1 deðil, Cell yapýsýndan oluþuyor
    static Cell grid[100][60] = { 0 };

    InitWindow(screenWidth, screenHeight, "Kum Simulasyonu - Renkli Dunya");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        int grid_mouse_x = GetMouseX() / cellSize;
        int grid_mouse_y = GetMouseY() / cellSize;

        std::string currentModeName = "SAND";
        if (brushMode == 0) currentModeName = "ERASER";
        else if (brushMode == 2) currentModeName = "STONE";

        Color currentPreviewColor = WHITE; // Varsayýlan silgi rengi (Mode 0)
        if (brushMode == 1) currentPreviewColor = YELLOW; // Kum rengi
        else if (brushMode == 2) currentPreviewColor = GRAY;

        // Fýrça önizlemesi
        for (int i = -brushSize; i <= brushSize; i++) {
            for (int j = -brushSize; j <= brushSize; j++) {
                if (i * i + j * j <= brushSize * brushSize * 0.8f) {
                    DrawRectangleLines((grid_mouse_x + i) * cellSize, (grid_mouse_y + j) * cellSize, cellSize, cellSize, currentPreviewColor);
                }
            }
        }

        if (IsKeyPressed(KEY_ONE))   brushSize = 1;
        if (IsKeyPressed(KEY_TWO))   brushSize = 2;
        if (IsKeyPressed(KEY_THREE)) brushSize = 3;
        if (IsKeyPressed(KEY_FOUR))  brushSize = 4;
        if (IsKeyPressed(KEY_FIVE))  brushSize = 5;

        if (IsKeyPressed(KEY_SIX))   brushMode = 0;
        if (IsKeyPressed(KEY_SEVEN)) brushMode = 1;
        if (IsKeyPressed(KEY_EIGHT)) brushMode = 2;
        //if (IsKeyPressed(KEY_NINE))  brushMode = 4;
        //if (IsKeyPressed(KEY_ZERO))  brushMode = 5;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int x = GetMouseX() / cellSize;
            int y = GetMouseY() / cellSize;

            for (int i = -brushSize; i <= brushSize; i++) {
                for (int j = -brushSize; j <= brushSize; j++) {
                    if (i * i + j * j <= brushSize * brushSize * 0.8f) {
                        int tx = x + i;
                        int ty = y + j;
                        if (tx >= 0 && tx < gridWidth && ty >= 0 && ty < gridHeight) {
                            if (brushMode == 0){
                                grid[tx][ty].type = 0;
                            }

                            else if (grid[tx][ty].type == 0) { // Sadece boþsa yeni kum koy
                                grid[tx][ty].type = brushMode;
                                // Her taneciðe döküldüðü an rastgele bir sarý/turuncu tonu veriyoruz
                                int variation = GetRandomValue(0, 50);
                                if(brushMode == 1) grid[tx][ty].color = Color{ (unsigned char)(255 - variation), (unsigned char)(200 - variation), 50, 255 };
                                if (brushMode == 2) grid[tx][ty].color = Color{GRAY};
                            }
                        }
                    }
                }
            }
        }

        // Fizik Güncelleme
        for (int y = gridHeight - 2; y >= 0; y--) {
            for (int x = 0; x < gridWidth; x++) {
                if (grid[x][y].type == 1) {

                    if (grid[x][y + 1].type == 0) {
                        grid[x][y + 1] = grid[x][y]; // Rengiyle beraber aþaðý taþý
                        grid[x][y].type = 0;         // Eski yeri boþalt
                    }
                    else {
                        int dir = (GetRandomValue(0, 1) == 0) ? -1 : 1;
                        if (x + dir >= 0 && x + dir < gridWidth && grid[x + dir][y + 1].type == 0) {
                            grid[x + dir][y + 1] = grid[x][y];
                            grid[x][y].type = 0;
                        }
                        else if (x - dir >= 0 && x - dir < gridWidth && grid[x - dir][y + 1].type == 0) {
                            grid[x - dir][y + 1] = grid[x][y];
                            grid[x][y].type = 0;
                        }
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Çizim
        for (int x = 0; x < gridWidth; x++) {
            for (int y = 0; y < gridHeight; y++) {
                if (grid[x][y].type != 0) {
                    // Artýk her tanecik kendi içinde sakladýðý rengiyle çiziliyor
                    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, grid[x][y].color);
                }
            }
        }

        DrawFPS(10, 10);
        DrawText("Brush Size:", 10, 40, 16, WHITE);
        DrawText(TextFormat("%d", brushSize), 110, 40, 16, WHITE);

        DrawText("Brush Mode:", 10, 60, 16, WHITE);
        DrawText(currentModeName.c_str(), 110, 60, 16, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}