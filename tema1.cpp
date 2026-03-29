#include "lab_m1/Tema1/tema1.h"
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

#include <vector>
#include <iostream>

using namespace m1;


Tema1::Tema1() {}
Tema1::~Tema1() {}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 20);

    {
        //variabile pentru grid + border
        //dimensiune fiecare patrat
        squareSize = 30;
        //pozitia de start a grid-ului pe axa OX
        startX = 600;
        //pozitia de start a grid-ului pe axa OY
        startY = 100;
        //numar coloane
        gridCols = 15;
        //numar linii
        gridRows = 10;
        //distanta dintre patrate
        spacing = 10;
        //latime grid
        gridWidth = gridCols * squareSize + (gridCols - 1) * spacing;
        //inaltime grid
        gridHeight = gridRows * squareSize + (gridRows - 1) * spacing;
        //distanta dintre grid si border
        padding = 30;
        borderWidth = gridWidth + 2 * padding;
        borderHeight = gridHeight + 2 * padding;
        borderX = startX - padding;
        borderY = startY - padding;
        //end
    }

    {//detalii componenta 1
        comp1X = 10;
        comp1Y = 500;
        comp1Width = 200;
        comp1Height = 200;
        border1X = comp1X;
        border1Y = comp1Y;
        border1Width = comp1Width;
        border1Height = comp1Height;
        comp1X += comp1Width / 2 - squareSize / 2;
        comp1Y += comp1Height / 2 - squareSize / 2;

        //!!!!Asta daca se doreste ca obiectul sa fie randat imediat dupa ce isi ia drag
        /*Components newBlock;
        newBlock.name = "solid_block";
        newBlock.x = comp1X;
        newBlock.y = comp1Y;
        newBlock.row = -1;
        newBlock.col = -1;
        components.push_back(newBlock);*/
    }

    {//detalii componenta 2
        comp2X = 10;
        comp2Y = 300;
        comp2Width = 200;
        comp2Height = 200;
        border2X = comp2X;
        border2Y = comp2Y;
        border2Width = comp2Width;
        border2Height = comp2Height;
    }

    {//detalii ciuperca
        bumperX = comp2X + comp2Width / 2 - squareSize / 2;
        bumperY = comp2Y + comp2Height / 2 - squareSize / 2;

        //!!!!Asta daca se doreste ca obiectul sa fie randat imediat dupa ce isi ia drag
        /*Components newBlock;
        newBlock.name = "baza_ciupercii";
        newBlock.x = bumperX;
        newBlock.y = bumperY;
        newBlock.row = -1;
        newBlock.col = -1;
        components.push_back(newBlock);*/
    }

    //drag & drop
    {
        dragComp1 = false;
        dragComp2 = false;
    }

    nrComponents = 0;
    conexiuni = std::vector<std::vector<int>>(gridRows, std::vector<int>(gridCols, 0));
    nava_valida = false;
    nrRemainingComponents = 10;
    paddingRemainingComponents = 80;

    //culoare buton start
    colorButtonStart = glm::vec3(1, 0, 0);

    Mesh* square = object2D::CreateSquare("square", glm::vec3(0, 0, 0), squareSize, glm::vec3(0, 0, 1), true);
    AddMeshToList(square);

    Mesh* border_grid = object2D::CreateSquare("border_grid", glm::vec3(0, 0, 0), 1.0f, glm::vec3(0, 0, 1), false);
    AddMeshToList(border_grid);

    Mesh* border_comp = object2D::CreateSquare("border_comp", glm::vec3(0, 0, 0), 1.0f, glm::vec3(1, 0, 0), false);
    AddMeshToList(border_comp);

    Mesh* solid_block = object2D::CreateSquare("solid_block", glm::vec3(0, 0, 0), squareSize, glm::vec3(0.90f, 0.91f, 0.98f), true);
    AddMeshToList(solid_block);

    Mesh* baza_ciupercii = object2D::CreateSquare("baza_ciupercii", glm::vec3(0, 0, 0), squareSize, glm::vec3(0.85f, 0.85f, 0.95f), true);
    AddMeshToList(baza_ciupercii);

    Mesh* circle = object2D::CreateCircle("circle", glm::vec3(0, 0, 0), squareSize / 2, glm::vec3(0.55f, 0.47f, 0.14f), true, true);
    AddMeshToList(circle);

    Mesh* rmgComp = object2D::CreateSquare("rmgComp", glm::vec3(0, 0, 0), squareSize, glm::vec3(0.53f, 0.12f, 0.47f), true);
    AddMeshToList(rmgComp);

    Mesh* triangle = object2D::CreateTriangle("triangle", glm::vec3(0, 0, 0), 30, colorButtonStart, true, true);
    AddMeshToList(triangle);

    //detalii despre JOC
    paletaX = window->GetResolution().x / 2 - 50;
    paletaY = 30;
    paletaWidth = 100;
    paletaHeight = 20;
    points = 0;
    lives = 1;
    translateBallX = window->GetResolution().x / 2;
    translateBallY = 70;
    ballSpeed = 300;
    angleBall = 45;
    ballRadius = 15;
    ballX = cos(angleBall);
    ballY = sin(angleBall);
    auxDeltaTimeSeconds = 0;

    gameInProgress = false;
    maxPoints = 357;
    totalPoints = 0;
    breakoutHeight = 20;//50
    breakoutWidth = 50;//75
    totalLives = 3;
    bumper1Exist = false;
    bumper2Exist = false;

    //SHOP
    nrMaxUpgrades = 2;
    invalidShop = false;
    timer = 0.0f;
    nrOfUps = 0;

    Mesh* paleta = object2D::CreateSquare("paleta", glm::vec3(0, 0, 0), 100, glm::vec3(1, 1, 1), true);
    AddMeshToList(paleta);

    Mesh* ball = object2D::CreateCircle("ball", glm::vec3(0, 0, 0), 15, glm::vec3(0.75f, 0.75f, 0.75f), true, false);
    AddMeshToList(ball);

    Mesh* breakout = object2D::CreateSquare("breakout", glm::vec3(0, 0, 0), 30, glm::vec3(1, 1, 1), true);
    AddMeshToList(breakout);

    Mesh* dotsMesh = object2D::CreateCircle("dots", glm::vec3(0, 0, 0), 1, glm::vec3(1, 1, 1), true, false);
    AddMeshToList(dotsMesh);

    int numDots = 150;
    for (int i = 0; i < numDots; i++) {
        Dot d;
        d.x = rand() % (int)window->GetResolution().x;
        d.y = rand() % (int)window->GetResolution().y;
        d.speed = 30 + rand() % 50;
        dots.push_back(d);
    }

    Mesh* heart_comp = object2D::CreateCircle("heart_comp", glm::vec3(0, 0, 0), 15, glm::vec3(1, 0, 0), true, true);
    AddMeshToList(heart_comp);

    Mesh* heart_comp2 = object2D::CreateTriangle("heart_comp2", glm::vec3(0, 0, 0), 30, glm::vec3(1, 0, 0), true, false);
    AddMeshToList(heart_comp2);

    RandareBreakouts();
}

void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    if (gameState == INSTRUCTION)
    {
        RenderParagraph({
            "Welcome CHIEF!",
            "In dreapta vei avea de desenat nava cu care vei calatori,",
            "Totusi in constructia ei vor exista niste constrangeri de care trebuie sa tii seama:",
            "1) Componentele trebuie sa fie conexe (conectate intre ele pe orizontala sau verticala),",
            "2) Materialele sunt greu de procurat deci vei avea la dispozitie maxim 10 componente,",
            "3) Totusi vei avea nevoie de cel putin UN material pentru a porni la drum,",
            "4) Un bumper nu poate sa se afle sub orice material pe 3 coloane.",
            "Asta este tot CHIEF, calatorie placuta!"
            }, 100, 200, 1, glm::vec3(1, 1, 1));

        float t = Engine::GetElapsedTime();
        float r = 0.5f + 0.5f * sin(t * 2.0f);
        float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
        float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);

        glm::vec3 neonColor(r, g, b);
        textRenderer->RenderText("Apasa [ENTER] pentru a continua spre MENU!", 20, 50, 1, neonColor);
        textRenderer->RenderText("Apasa [SPACE] pentru a intra in SHOP!", 20, 80, 1, neonColor);
        textRenderer->RenderText("Apasa [A] pentru deplasare stanga.", 20, 110, 1, neonColor);
        textRenderer->RenderText("Apasa [D] pentru deplasare dreapta.", 20, 140, 1, neonColor);

    }

    if (gameState == MENU)
    {
        float t = Engine::GetElapsedTime();
        float r = 0.5f + 0.5f * sin(t * 2.0f);
        float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
        float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);

        glm::vec3 neonColor(r, g, b);
        textRenderer->RenderText("Componentele se iau dupa regula Drag & Drop!", 10, 500, 1, neonColor);
        textRenderer->RenderText("Cu click dreapta poti sterge componente!", 10, 530, 1, neonColor);

        {//randare triunghi
            if (nava_valida) {
                colorButtonStart = glm::vec3(0, 1, 0);
            }
            else {
                colorButtonStart = glm::vec3(1, 0, 0);
            }

            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(1100, 600);
            modelMatrix *= Transform2D::Scale(1.5f, 1.5f);
            RenderMesh2D(meshes["triangle"], modelMatrix, colorButtonStart);
        }

        {
            for (int i = 0; i < nrRemainingComponents - nrComponents; i++) {
                glm::mat3 modelMatrix = glm::mat3(1);
                modelMatrix *= Transform2D::Translate(300 + (i * paddingRemainingComponents), 600);
                modelMatrix *= Transform2D::Scale(1.5f, 1.5f);
                RenderMesh2D(meshes["rmgComp"], shaders["VertexColor"], modelMatrix);
            }
        }

        {//randare block solid
            for (auto& block : components) {
                if (block.name._Equal("solid_block")) {
                    glm::mat3 modelMatrix = glm::mat3(1);
                    modelMatrix *= Transform2D::Translate(block.x, block.y);
                    RenderMesh2D(meshes["solid_block"], shaders["VertexColor"], modelMatrix);
                }
                else if (block.name._Equal("baza_ciupercii")) {
                    glm::mat3 modelMatrix = glm::mat3(1);
                    modelMatrix *= Transform2D::Translate(block.x, block.y);
                    RenderMesh2D(meshes["baza_ciupercii"], shaders["VertexColor"], modelMatrix);

                    glm::mat3 modelMatrixCircle = glm::mat3(1);
                    modelMatrixCircle = modelMatrix * Transform2D::Translate(squareSize / 2, squareSize);
                    modelMatrixCircle *= Transform2D::Scale(3, 2);
                    RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrixCircle);
                }
            }
        }

        {//desenez grid-ul de patrate
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(borderX, borderY);
            modelMatrix *= Transform2D::Scale(borderWidth, borderHeight);
            RenderMesh2D(meshes["border_grid"], shaders["VertexColor"], modelMatrix);

            for (int i = 0; i < gridRows; i++)
            {
                for (int j = 0; j < gridCols; j++)
                {
                    glm::mat3 modelMatrix = glm::mat3(1);
                    modelMatrix *= Transform2D::Translate(startX + j * (squareSize + spacing), startY + i * (squareSize + spacing));
                    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
                }
            }
        }//grid finish

        {//border componenta 1
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(border1X, border1Y);
            modelMatrix *= Transform2D::Scale(border1Width, border1Height);
            RenderMesh2D(meshes["border_comp"], shaders["VertexColor"], modelMatrix);
        }

        {//border componenta 2
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(border2X, border2Y);
            modelMatrix *= Transform2D::Scale(border2Width, border2Height);
            RenderMesh2D(meshes["border_comp"], shaders["VertexColor"], modelMatrix);
        }

        {//componenta 1 (solid) randata pentru mouse move
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(comp1X, comp1Y);
            RenderMesh2D(meshes["solid_block"], shaders["VertexColor"], modelMatrix);
        }

        {//componenta 2 (ciuperca) randata pentru mouse move
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(bumperX, bumperY);
            RenderMesh2D(meshes["baza_ciupercii"], shaders["VertexColor"], modelMatrix);

            glm::mat3 modelMatrixCircle = glm::mat3(1);
            modelMatrixCircle = modelMatrix * Transform2D::Translate(squareSize / 2, squareSize);
            modelMatrixCircle *= Transform2D::Scale(3, 2);
            RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrixCircle);
        }
    }

    if (gameState == GAME)
    {//logica jocului
        {//randare bumpers upgrades
            if (bumper1Exist) {
                glm::mat3 modelMatrix = glm::mat3(1);
                modelMatrix *= Transform2D::Translate(35, 10);
                RenderMesh2D(meshes["baza_ciupercii"], shaders["VertexColor"], modelMatrix);

                glm::mat3 modelMatrixCircle = glm::mat3(1);
                modelMatrixCircle = modelMatrix * Transform2D::Translate(squareSize / 2, squareSize);
                modelMatrixCircle *= Transform2D::Scale(3, 2);
                RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrixCircle);
            }

            if (bumper2Exist) {
                glm::mat3 modelMatrix = glm::mat3(1);
                modelMatrix *= Transform2D::Translate(1210, 10);
                RenderMesh2D(meshes["baza_ciupercii"], shaders["VertexColor"], modelMatrix);

                glm::mat3 modelMatrixCircle = glm::mat3(1);
                modelMatrixCircle = modelMatrix * Transform2D::Translate(squareSize / 2, squareSize);
                modelMatrixCircle *= Transform2D::Scale(3, 2);
                RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrixCircle);
            }
        }

        {//randare paleta
            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(paletaX, paletaY);
            modelMatrix *= Transform2D::Scale(1.2f, 0.2f);
            RenderMesh2D(meshes["paleta"], shaders["VertexColor"], modelMatrix);
        }

        {//randare mingie
            if (gameInProgress)
            {
                glm::mat3 modelMatrix = glm::mat3(1);
                modelMatrix *= Transform2D::Translate(translateBallX, translateBallY);
                RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);

                ballX = cos(angleBall);
                ballY = sin(angleBall);
                translateBallX += ballX * ballSpeed * deltaTimeSeconds;
                translateBallY += ballY * ballSpeed * deltaTimeSeconds;
                auxDeltaTimeSeconds = deltaTimeSeconds;

                if (translateBallY <= 20) {
                    lives--;
                    translateBallX = window->GetResolution().x / 2.0f;
                    translateBallY = 50.0f;
                    angleBall = glm::radians(60.0f);
                    ballX = cos(angleBall);
                    ballY = sin(angleBall);
                    gameInProgress = false;
                    paletaX = window->GetResolution().x / 2 - 50;
                }

                if (lives == 0) {
                    totalPoints += points;
                    gameState = GAME_OVER;
                }
                else if (points == maxPoints) {
                    totalPoints += points;
                    gameState = GAME_OVER;
                }

                if (translateBallX <= 30 || translateBallX >= window->GetResolution().x - 30 || translateBallX == paletaX) {
                    ballX = -ballX;
                    translateBallX = glm::clamp(translateBallX, 0.0f, (float)window->GetResolution().x);
                }
                if (translateBallY >= window->GetResolution().y - 20 || translateBallY == paletaY) {
                    ballY = -ballY;
                    translateBallY = glm::clamp(translateBallY, 0.0f, (float)window->GetResolution().y);
                }
                if (translateBallX + ballRadius >= paletaX && translateBallX - ballRadius <= paletaX + paletaWidth &&
                    translateBallY - ballRadius <= paletaY + paletaHeight && translateBallY + ballRadius >= paletaY) {
                    ballY = -ballY;
                    translateBallY = paletaY + paletaHeight + ballRadius;
                }

                //ricoseu pe bumper
                if (bumper2Exist) {
                    if (translateBallX >= 1180 && translateBallX <= window->GetResolution().x &&
                        translateBallY - ballRadius <= 70) {
                        ballY = -ballY;
                        translateBallY = 72 + ballRadius;
                    }
                }

                if (bumper1Exist) {
                    if (translateBallX >= 0 && translateBallX <= 100 &&
                        translateBallY - ballRadius <= 70) {
                        ballY = -ballY;
                        translateBallY = 72 + ballRadius;
                    }
                }

                angleBall = atan2(ballY, ballX);
            }
            else {
                glm::mat3 modelMatrix = glm::mat3(1);
                modelMatrix *= Transform2D::Translate(window->GetResolution().x / 2, 70);
                RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
            }

            textRenderer->RenderText("Points: " + std::to_string(points), 20, 10, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("Lives: " + std::to_string(lives), window->GetResolution().x - 110, 10, 1, glm::vec3(1, 1, 1));

            {//jocul efectiv
                for (auto& block : breakouts)
                {
                    glm::mat3 modelMatrix = glm::mat3(1);
                    modelMatrix *= Transform2D::Translate(block.x, block.y);
                    modelMatrix *= Transform2D::Scale(2.4f, 1.5f);
                    RenderMesh2D(meshes["breakout"], modelMatrix, GetBlockColor(block.resistance));
                }

                for (auto it = breakouts.begin(); it != breakouts.end();) {
                    Breakout& block = *it;

                    bool collision =
                        translateBallX + ballRadius >= block.x &&
                        translateBallX - ballRadius <= block.x + block.width &&
                        translateBallY + ballRadius >= block.y &&
                        translateBallY - ballRadius <= block.y + block.height;

                    if (!collision) {
                        ++it;
                        continue;
                    }

                    int nord = (block.y + block.height) - (translateBallY - ballRadius);
                    int est = (block.x + block.width) - (translateBallX - ballRadius);
                    int sud = (translateBallY + ballRadius) - block.y;
                    int vest = (translateBallX + ballRadius) - block.x;

                    float minDir = std::min({ nord, est, sud, vest });
                    float gap = 0.01f;

                    if (fabs(minDir - vest) < 0.1f && ballX > 0) {
                        ballX = -ballX;
                        translateBallX = block.x - ballRadius - gap;
                    }
                    else if (fabs(minDir - est) < 0.1f && ballX < 0) {
                        ballX = -ballX;
                        translateBallX = block.x + block.width + ballRadius + gap;
                    }
                    else if (fabs(minDir - nord) < 0.1f && ballY < 0) {
                        ballY = -ballY;
                        translateBallY = block.y + block.height + ballRadius + gap;
                    }
                    else if (fabs(minDir - sud) < 0.1f && ballY > 0) {
                        ballY = -ballY;
                        translateBallY = block.y - ballRadius - gap;
                    }

                    angleBall = atan2(ballY, ballX);

                    block.resistance--;
                    points++;

                    if (block.resistance <= 0) {
                        it = breakouts.erase(it);
                    }
                    else {
                        ++it;
                    }
                }
            }
        }

        for (auto& d : dots) {//puncte in miscare (pe verticala) pe fundal
            d.y -= d.speed * deltaTimeSeconds;

            if (d.y < 0) {
                d.y = window->GetResolution().y;
            }

            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= Transform2D::Translate(d.x, d.y);
            RenderMesh2D(meshes["dots"], shaders["VertexColor"], modelMatrix);
        }
    }

    if (gameState == GAME_OVER) {
        RenderParagraph({
            "GAME OVER!",
            "Puncte totale acumulate: " + std::to_string(totalPoints)
            }, 100, 300, 1, glm::vec3(1, 1, 1));
        float t = Engine::GetElapsedTime();
        float r = 0.5f + 0.5f * sin(t * 2.0f);
        float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
        float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);
        glm::vec3 neonColor(r, g, b);
        textRenderer->RenderText("Apasa [ENTER] pentru a continua spre MENU!", 20, 50, 1, neonColor);
        textRenderer->RenderText("Apasa [TAB] pentru a incepe un NOU JOC!", 20, 80, 1, neonColor);
        textRenderer->RenderText("Apasa [SPACE] pentru a intra in SHOP!", 20, 110, 1, neonColor);
        textRenderer->RenderText("Apasa [I] pentru a citi instructiunile!", 20, 140, 1, neonColor);
    }

    if (gameState == SHOP) {
        float t = Engine::GetElapsedTime();
        float r = 0.5f + 0.5f * sin(t * 2.0f);
        float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
        float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);
        glm::vec3 neonColor(r, g, b);

        if (invalidShop) {
            timer += deltaTimeSeconds;

            if (timer >= 5.0f) {
                invalidShop = false;
            }
            textRenderer->RenderText(mesaj, 20, 400, 1, neonColor);
        }

        RenderParagraph({
            "Bine ai venit in sectiunea SHOP CHIEF!",
            "Aici poti sa iti cumperi vieti suplimentare sau diferite obiecte in functie de punctele acumulate.",
            "Fiecare viata costa 5 puncte,iar fiecare material 10 puncte.",
            "Apasa [TAB] pentru a incepe un NOU JOC!",
            "Apasa [ENTER] pentru a te intoarce la MENU!",
            "Apasa [I] pentru a citi instructiunile!"
            }, 50, 100, 1, glm::vec3(1, 1, 1));


        textRenderer->RenderText("Puncte totale acumulate: " + std::to_string(totalPoints), 20, 50, 1, neonColor);

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(100, 100);
        modelMatrix *= Transform2D::Scale(border1Width, border1Height);
        RenderMesh2D(meshes["border_comp"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(400, 100);
        modelMatrix *= Transform2D::Scale(border1Width, border1Height);
        RenderMesh2D(meshes["border_comp"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(border1Width - 10, border1Height);
        RenderMesh2D(meshes["baza_ciupercii"], shaders["VertexColor"], modelMatrix);

        glm::mat3 modelMatrixCircle = glm::mat3(1);
        modelMatrixCircle = modelMatrix * Transform2D::Translate(squareSize / 2, squareSize);
        modelMatrixCircle *= Transform2D::Scale(3, 2);
        RenderMesh2D(meshes["circle"], shaders["VertexColor"], modelMatrixCircle);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(480, 240);
        modelMatrix *= Transform2D::Scale(1.5f, 1.5f);
        RenderMesh2D(meshes["heart_comp"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= Transform2D::Translate(520, 240);
        modelMatrix *= Transform2D::Scale(1.5f, 1.5f);
        RenderMesh2D(meshes["heart_comp"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        float cx = 256 + 30 / 2;
        float cy = 105 + 30 / 2;

        modelMatrix *= Transform2D::Translate(cx, cy);
        modelMatrix *= Transform2D::Rotate(glm::pi<float>());
        modelMatrix *= Transform2D::Translate(-cx, -cy);
        modelMatrix *= Transform2D::Scale(2.8f, 1.9f);
        RenderMesh2D(meshes["heart_comp2"], shaders["VertexColor"], modelMatrix);

        textRenderer->RenderText("Cost: 5p", 400, 600, 1, neonColor);
        textRenderer->RenderText("Cost: 10p", 100, 600, 1, neonColor);
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (gameState == GAME) {
        if (window->KeyHold(GLFW_KEY_A)) {
            paletaX -= 350 * auxDeltaTimeSeconds;
            paletaX = glm::clamp(paletaX, 0.0f, (float)(window->GetResolution().x - 100));
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            paletaX += 350 * auxDeltaTimeSeconds;
            paletaX = glm::clamp(paletaX, 0.0f, (float)(window->GetResolution().x - 120));
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (gameState == INSTRUCTION) {
        if (key == GLFW_KEY_ENTER) {
            gameState = MENU;
        }

        if (key == GLFW_KEY_SPACE) {
            gameState = SHOP;
        }
    }

    if (gameState == MENU) {
        if (key == GLFW_KEY_I) {
            gameState = INSTRUCTION;
        }

        if (key == GLFW_KEY_SPACE) {
            gameState = SHOP;
        }
    }

    if (gameState == GAME) {
        if (key == GLFW_KEY_SPACE) {
            gameInProgress = true;
        }
        if (key == GLFW_KEY_ENTER) {
            gameState = MENU;
        }
    }

    if (gameState == GAME_OVER) {
        if (key == GLFW_KEY_ENTER) {
            gameState = MENU;
        }

        if (key == GLFW_KEY_TAB) {
            //resetare joc
            lives = totalLives;
            points = 0;
            translateBallX = window->GetResolution().x / 2;
            translateBallY = 70;
            angleBall = glm::radians(45.0f);
            ballX = cos(angleBall);
            ballY = sin(angleBall);
            paletaX = window->GetResolution().x / 2 - 50;
            breakouts.clear();
            RandareBreakouts();
            gameState = GAME;
        }

        if (key == GLFW_KEY_SPACE) {
            gameState = SHOP;
        }

        if (key == GLFW_KEY_I) {
            gameState = INSTRUCTION;
        }
    }

    if (gameState == SHOP) {
        if (key == GLFW_KEY_TAB) {
            invalidShop = false;
            //resetare joc
            lives = totalLives;
            points = 0;
            translateBallX = window->GetResolution().x / 2;
            translateBallY = 70;
            angleBall = glm::radians(45.0f);
            ballX = cos(angleBall);
            ballY = sin(angleBall);
            paletaX = window->GetResolution().x / 2 - 50;
            breakouts.clear();
            RandareBreakouts();
            gameState = GAME;
        }

        if (key == GLFW_KEY_ENTER) {
            invalidShop = false;
            gameState = MENU;
        }

        if (key == GLFW_KEY_I) {
            gameState = INSTRUCTION;
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    int auxMouseY = window->GetResolution().y - mouseY;
    if (gameState == MENU)
    {
        if (dragComp1) {
            comp1X = mouseX - dragOffset.x;
            comp1Y = auxMouseY - dragOffset.y;
        }
        else if (dragComp2) {
            bumperX = mouseX - dragOffset.x;
            bumperY = auxMouseY - dragOffset.y;
        }
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    //Y mouse urca in jos
    int auxMouseY = window->GetResolution().y - mouseY;

    if (gameState == MENU)
    {//logica meniului
        //start button x = 1100 - 1150, y = 600 - 650
        if (mouseX >= 1100 && mouseX <= 1150 && auxMouseY >= 600 && auxMouseY <= 650 && nava_valida) {
            lives = 3;
            points = 0;
            translateBallX = window->GetResolution().x / 2;
            translateBallY = 70;
            angleBall = glm::radians(45.0f);
            ballX = cos(angleBall);
            ballY = sin(angleBall);
            paletaX = window->GetResolution().x / 2 - 50;
            breakouts.clear();
            RandareBreakouts();
            gameState = GAME;
        }

        if (nrComponents >= 1) {
            nava_valida = VerifAll();
            if (!nava_valida) {
                if (button == 2) {
                    for (auto comp = components.begin(); comp != components.end(); comp++) {
                        if (mouseX >= comp->x && mouseX <= comp->x + squareSize &&
                            auxMouseY >= comp->y && auxMouseY <= comp->y + squareSize)
                        {
                            conexiuni[comp->row][comp->col] = 0;
                            components.erase(comp);
                            nrComponents--;
                            //std::cout << nrComponents << " componente ramase.\n"; //debug sa vad ca s-a sters
                            break;
                        }
                    }
                }

                nava_valida = VerifAll();
                return;
            }
        }

        //diferenta intre asta si ce e mai sus e ca cea de sus blocheaza procesul de a pune componente pe nava pana cand nava devine valida
        if (button == 2) {
            for (auto comp = components.begin(); comp != components.end(); comp++) {
                if (mouseX >= comp->x && mouseX <= comp->x + squareSize &&
                    auxMouseY >= comp->y && auxMouseY <= comp->y + squareSize)
                {
                    conexiuni[comp->row][comp->col] = 0;
                    components.erase(comp);
                    nrComponents--;
                    //std::cout << nrComponents << " componente ramase.\n"; //debug sa vad ca s-a sters
                    break;
                }
            }
            nava_valida = VerifAll();
        }

        if (mouseX >= startX && mouseX <= gridWidth + startX && auxMouseY >= startY && auxMouseY <= startY + gridHeight)
        {
            //std::cout << "Click in grid\n";
        }

        if (mouseX >= border1X && mouseX <= border1X + border1Width && auxMouseY >= border1Y && auxMouseY <= border1Y + border1Height)
        {
            //std::cout << "Click in border componentei 1\n";
            dragComp1 = true;
            dragOffset = glm::vec2(mouseX - comp1X, auxMouseY - comp1Y);
        }

        if (mouseX >= border2X && mouseX <= border2X + border2Width && auxMouseY >= border2Y && auxMouseY <= border2Y + border2Height)
        {
            dragComp2 = true;
            dragOffset = glm::vec2(mouseX - bumperX, auxMouseY - bumperY);
        }
    }

    if (gameState == SHOP) {
        if (mouseX >= 400 && mouseX <= 600 && auxMouseY >= 100 && auxMouseY <= 300) {
            if (totalPoints < 5) {
                invalidShop = true;
                timer = 0.0f;
                mesaj = "Puncte insuficiente pentru inima!";
                return;
            }

            invalidShop = true;
            timer = 0.0f;
            mesaj = "Ai achizitionat o inima!";

            totalPoints -= 5;
            totalLives++;
        }

        if (mouseX >= 100 && mouseX <= 300 && auxMouseY >= 100 && auxMouseY <= 300) {
            if (nrMaxUpgrades <= 0) {
                invalidShop = true;
                timer = 0.0f;
                mesaj = "S-a atins limita maxima de upgrades!";
                return;
            }

            if (totalPoints < 10) {
                invalidShop = true;
                timer = 0.0f;
                mesaj = "Puncte insuficiente pentru upgrade!";
                return;
            }

            invalidShop = true;
            timer = 0.0f;
            mesaj = "Ai facut un upgrade!";

            nrOfUps++;
            if (nrOfUps >= 1) {
                bumper1Exist = true;
            }

            if (nrOfUps >= 2) {
                bumper2Exist = true;
            }

            nrMaxUpgrades--;
            totalPoints -= 10;
        }
    }

    if (gameState == GAME) {
        std::cout << mouseX << " " << mouseY << "\n";
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    int auxMouseY = window->GetResolution().y - mouseY;

    if (gameState == MENU)
    {
        if (dragComp1) {
            if (mouseX < borderX + padding || mouseX > borderX + borderWidth - padding || auxMouseY < borderY + padding
                || auxMouseY > borderY + borderHeight - padding) {
                dragComp1 = false;
                comp1X = 10 + comp1Width / 2 - squareSize / 2;
                comp1Y = 500 + comp1Height / 2 - squareSize / 2;
                return;
            }
            else {
                float relX = mouseX - startX - dragOffset.x;
                float relY = auxMouseY - startY - dragOffset.y;
                int col = std::round(relX / (squareSize + spacing));
                int row = std::round(relY / (squareSize + spacing));

                for (auto& c : components) {
                    if (c.row == row && c.col == col) {
                        dragComp1 = false;
                        comp1X = 10 + comp1Width / 2 - squareSize / 2;
                        comp1Y = 500 + comp1Height / 2 - squareSize / 2;
                        //std::cout << "Locul asta a fost furat\n"; //debug
                        return;
                    }
                }

                Components newBlock;
                newBlock.name = "solid_block";
                newBlock.x = startX + col * (squareSize + spacing);
                newBlock.y = startY + row * (squareSize + spacing);
                newBlock.row = row;
                newBlock.col = col;
                components.push_back(newBlock);

                conexiuni[row][col] = 1;

                comp1X = 10 + comp1Width / 2 - squareSize / 2;
                comp1Y = 500 + comp1Height / 2 - squareSize / 2;

                nrComponents++;

                nava_valida = VerifAll();
                dragComp1 = false;

                return;
            }
        }
        if (dragComp2) {
            if (mouseX < borderX + padding || mouseX > borderX + borderWidth - padding || auxMouseY < borderY + padding
                || auxMouseY > borderY + borderHeight - padding) {
                dragComp2 = false;
                bumperX = comp2X + comp2Width / 2 - squareSize / 2;
                bumperY = comp2Y + comp2Height / 2 - squareSize / 2;
                return;
            }
            else {
                float relX = mouseX - startX - dragOffset.x;
                float relY = auxMouseY - startY - dragOffset.y;
                int col = std::round(relX / (squareSize + spacing));
                int row = std::round(relY / (squareSize + spacing));

                for (auto& c : components) {
                    if (c.row == row && c.col == col) {
                        dragComp2 = false;
                        bumperX = comp2X + comp2Width / 2 - squareSize / 2;
                        bumperY = comp2Y + comp2Height / 2 - squareSize / 2;
                        //std::cout << "Locul asta a fost furat\n"; //debug
                        return;
                    }
                }

                Components newBlock;
                newBlock.name = "baza_ciupercii";
                newBlock.x = startX + col * (squareSize + spacing);
                newBlock.y = startY + row * (squareSize + spacing);
                newBlock.row = row;
                newBlock.col = col;
                components.push_back(newBlock);
                bumperX = comp2X + comp2Width / 2 - squareSize / 2;
                bumperY = comp2Y + comp2Height / 2 - squareSize / 2;

                conexiuni[row][col] = 1;
                nrComponents++;

                nava_valida = VerifAll();
                dragComp2 = false;

                return;
            }
        }
    }

    if (gameState == GAME) {

    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

bool Tema1::ComponenteConexe()
{
    if (nrComponents <= 1) {
        return true;
    }

    std::vector<std::vector<bool>> visited(gridRows, std::vector<bool>(gridCols, false));

    int startRow = -1, startCol = -1;
    for (int i = 0; i < gridRows; i++)
        for (int j = 0; j < gridCols; j++)
            if (conexiuni[i][j] == 1) {
                startRow = i; startCol = j;
                break;
            }

    if (startRow == -1) return false;

    std::function<void(int, int)> dfs = [&](int r, int c) {
        if (r < 0 || r >= gridRows || c < 0 || c >= gridCols) return;
        if (visited[r][c] || conexiuni[r][c] == 0) return;
        visited[r][c] = true;
        dfs(r + 1, c);
        dfs(r - 1, c);
        dfs(r, c + 1);
        dfs(r, c - 1);
        };
    dfs(startRow, startCol);

    for (int i = 0; i < gridRows; i++)
        for (int j = 0; j < gridCols; j++)
            if (conexiuni[i][j] == 1 && !visited[i][j])
                return false;

    return true;
}

bool Tema1::VerifBumper(int row, int col, std::string name)
{
    int nord = row - 1;
    int sud = row + 1;
    int vest = col + 1;
    int est = col - 1;

    for (auto& block : components) {
        if (block.name._Equal("baza_ciupercii")) {
            if ((block.row <= nord && (block.col >= col - 1 && block.col <= col + 1))) {
                return false;
            }
        }
    }

    if (name._Equal("baza_ciupercii")) {
        for (auto& block : components) {
            if (block.name._Equal("baza_ciupercii")) {
                if ((block.row <= nord && (block.col >= col - 2 && block.col <= col + 2))) {
                    return false;
                }

                if ((block.col == est && block.row == row) || (block.col == est - 1 && block.row == row)) {
                    //std::cout << "Nu e bine\n";
                    return false;
                }

                if ((block.col == vest && block.row == row) || (block.col == vest + 1 && block.row == row)) {
                    //std::cout << "Nu e bine\n";
                    return false;
                }

                if (block.row == sud && (block.col == col || block.col == col - 1 || block.col == col + 1)) {
                    //std::cout << "Nu e bine\n";
                    return false;
                }
            }
        }
    }

    return true;
}

bool Tema1::VerifAllBumpers()
{
    for (auto& block : components) {
        if (!VerifBumper(block.row, block.col, block.name)) {
            return false;
        }
    }
    return true;
}

bool Tema1::VerifAll() {
    return ComponenteConexe() && VerifAllBumpers() && (nrComponents > 0 && nrComponents < 11);
}

void Tema1::RenderParagraph(std::vector<std::string> lines, float x, float y, float scale, glm::vec3 color)
{
    float lineSpacing = 35.0f * scale;

    for (size_t i = 0; i < lines.size(); i++) {
        textRenderer->RenderText(lines[i], x, y + i * lineSpacing, scale, color);
    }
}

glm::vec3 Tema1::GetBlockColor(int resistance) {
    switch (resistance) {
    case 1: return glm::vec3(1, 0, 0);
    case 2: return glm::vec3(0, 1, 0);
    case 3: return glm::vec3(0, 0, 1);
    case 4: return glm::vec3(1, 1, 0);
    case 5: return glm::vec3(1, 0, 1);
    case 6: return glm::vec3(0, 1, 1);
    default: return glm::vec3(0, 0, 0);
    }
}

void Tema1::RandareBreakouts()
{
    int start1 = 1;
    int start2 = window->GetResolution().y - 80;

    for (int row = 0; row < 6; row++) {
        int i = start1;
        int j = start2 - row * (breakoutHeight + 30);

        for (int col = 0; col < 17; col++) {
            Breakout block;
            block.color = colors[row];
            block.resistance = 6 - row;
            block.width = 75;
            block.height = 50;
            block.x = i;
            block.y = j;
            breakouts.push_back(block);

            i += breakoutWidth + 25;
        }
    }
}