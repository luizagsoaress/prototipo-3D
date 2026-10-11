#include "raylib.h"
#include "rcamera.h"
#include <stdlib.h>
#include <stdio.h>

#define GRID_TAM 32
#define BLOCO_TAM 1

typedef enum {
    BLOCO_TERRA,
    BLOCO_GRAMA,
    BLOCO_VAZIO
} BLOCOTIPO;

typedef struct {
    BLOCOTIPO tipo;
    Vector3 pos;
    Model asset;
} BLOCO;

static Model carregarAssets(const char *arq) {
    char path[512];
    const char *home = getenv("HOME");
    snprintf(path, sizeof(path), "%s/.local/share/prototipo3D/assets/%s", home, arq);
    return LoadModel(path);
}

int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 450;

    InitWindow(larguraTela, alturaTela, "Prototipo 3D - Raylib");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };    
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          
    camera.fovy = 60.0f;                               
    camera.projection = CAMERA_PERSPECTIVE;             

    int cameraModo = CAMERA_FIRST_PERSON;

    Vector3 casaAPos = { -6.0f, 1.0f, -5.0f };
    Vector3 casaBPos = { 11.0f, 1.0f, -11.0f };
    Vector3 plantacaoPos = { -5.0f, 0.0f, 10.0f };
    Vector3 fogueiraPos = { 5.0f, 1.0f, 10.0f };
    Vector3 fornoCasaPos = { -13.0f, 1.0f, 3.0f };
    Vector3 bancoPos[3] = { {2.0f, 1.0f, 10.0f}, {8.0f, 1.0f, 10.0f} };

    Model casaAModel = carregarAssets("casa_A.glb");
    Model casaBModel = carregarAssets("casa_B.glb");
    Model terraModel = carregarAssets("dirt.glb");
    Model gramaModel = carregarAssets("land.glb");
    Model plantacaoModel = carregarAssets("plantation.glb");
    Model fogueiraModel = carregarAssets("campfire.glb");
    Model bancoModel = carregarAssets("bench.glb");
    Model fornoCasaModel = carregarAssets("furnance_house.glb");
    Model arrayModels[8] = {casaAModel, casaBModel, terraModel, gramaModel, plantacaoModel, fogueiraModel, bancoModel, fornoCasaModel};

    BLOCO BLOCO[GRID_TAM][GRID_TAM];
    Vector3 offset = { -GRID_TAM * BLOCO_TAM / 2.0f, 0.0f, -GRID_TAM * BLOCO_TAM / 2.0f };

    for (int i = 0; i < GRID_TAM; i++) {
        for(int j = 0; j < GRID_TAM; j++) {
            BLOCO[i][j].pos = (Vector3){ i * BLOCO_TAM + offset.x, 0.0f, j * BLOCO_TAM + offset.z };
            if(BLOCO[i][j].pos.x >= -11.5f && BLOCO[i][j].pos.x <= -8.5f && BLOCO[i][j].pos.z >= -11.5f && BLOCO[i][j].pos.z <= -5.5f) BLOCO[i][j].tipo = BLOCO_VAZIO;
            else if(BLOCO[i][j].pos.x >= -12.5f && BLOCO[i][j].pos.x <= -4.5f && BLOCO[i][j].pos.z >= 6.5f && BLOCO[i][j].pos.z <= 10.5f) BLOCO[i][j].tipo = BLOCO_VAZIO;
            else if(BLOCO[i][j].pos.x == -12.5f && BLOCO[i][j].pos.z == 15.5f) BLOCO[i][j].tipo = BLOCO_VAZIO;
            else BLOCO[i][j].tipo = BLOCO_GRAMA;
        }
    }

    DisableCursor();                   

    SetTargetFPS(60);                  

    while (!WindowShouldClose()) { 
        UpdateCamera(&camera, cameraModo);           

        BeginDrawing();

            ClearBackground(WHITE);

            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, BLACK);
                DrawCube((Vector3){ -17.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 33.0f, MAROON);     
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 33.0f, MAROON);      
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 33.0f, 5.0f, 1.0f, MAROON);      
                DrawCube((Vector3){ 0.0f, 2.5f, -17.0f }, 33.0f, 5.0f, 1.0f, MAROON);      

                for (int i = 0; i < GRID_TAM; i++) {
                    for (int j = 0; j < GRID_TAM; j++) {
                        if(BLOCO[i][j].tipo == BLOCO_TERRA) BLOCO[i][j].asset = terraModel;
                        else if(BLOCO[i][j].tipo == BLOCO_GRAMA) BLOCO[i][j].asset = gramaModel;
                        else if(BLOCO[i][j].tipo == BLOCO_VAZIO) continue;
                        DrawModel(BLOCO[i][j].asset, BLOCO[i][j].pos, 1.0f, WHITE);
                    }
                }
                
                DrawModel(casaAModel, casaAPos, 1.0f, WHITE);
                DrawModel(casaBModel, casaBPos, 1.0f, WHITE);
                DrawModel(plantacaoModel, plantacaoPos, 1.0f, WHITE);
                DrawModel(fogueiraModel, fogueiraPos, 1.0f, WHITE);
                DrawModel(fornoCasaModel, fornoCasaPos, 1.0f, WHITE);

                for(int i = 0; i < 2; i++) {
                    DrawModel(bancoModel, bancoPos[i], 1.0f, WHITE);
                }

            EndMode3D();

            DrawRectangle(5, 5, 330, 60, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 5, 330, 60, BLUE);
            DrawText("Controle De Camera:", 15, 15, 10, BLACK);
            DrawText("- Movimentação: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            DrawText("- Visão: setas do teclado ou mouse", 15, 45, 10, BLACK);

        EndDrawing();

    }

    for(int i = 0; i < 8; i++) {
        UnloadModel(arrayModels[i]);
    }
    
    CloseWindow();        

    return 0;
}