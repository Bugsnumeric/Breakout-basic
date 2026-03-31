#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        bool ComponenteConexe();
        bool VerifBumper(int row, int col, std::string);
        bool VerifAllBumpers();
        bool VerifAll();
        void RenderParagraph(std::vector<std::string> lines, float x, float y, float scale, glm::vec3 color);
        glm::vec3 GetBlockColor(int resistance);
        void RandareBreakouts();

    protected:
        enum GameState {
            INSTRUCTION,
            MENU,
            GAME,
            GAME_OVER,
            SHOP
        };

        GameState gameState = INSTRUCTION;

        gfxc::TextRenderer* textRenderer;


        glm::mat3 modelMatrix;
        int nrComponents;
        std::vector<std::vector<int>> conexiuni;
        bool nava_valida;

        //variabile pentru grid + border
        int spacing;
        int borderX, borderY;
        int borderWidth, borderHeight;
        float padding;
        int gridRows, gridCols;
        int gridWidth, gridHeight;
        int startX, startY;
        //end

        //border component 1
        int border1X, border1Y;
        int border1Width, border1Height;
        int comp1X, comp1Y;
        int comp1Width, comp1Height;

        struct Components {
            std::string name;
            int x, y;
            int row, col;
        };

        //componentele plasate pe nava
        std::vector<Components> components;

        //componente ramase
        int nrRemainingComponents;
        int paddingRemainingComponents;

        //border component 2
        int border2X, border2Y;
        int border2Width, border2Height;
        int comp2X, comp2Y;
        int comp2Width, comp2Height;

        //ciuperca
        int bumperX, bumperY;

        //drag & drop
        bool dragComp1;
        bool dragComp2;
        glm::vec2 dragOffset;

        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        bool grow;
        float squareSize;

        //buton start
        glm::vec3 colorButtonStart;

        //detalii despre JOC
        float paletaX, paletaY;
        float paletaWidth, paletaHeight;
        int points;
        int lives;
        float translateBallX, translateBallY;
        float angleBall;
        int ballRadius;
        float ballSpeed;
        float ballX, ballY;
        float auxDeltaTimeSeconds;
        int maxPoints;
        int totalPoints;
        bool gameInProgress;
        int totalLives;
        int nrMaxUpgrades;
        float timer;
        std::string mesaj = "";
        bool invalidShop;
        int nrOfUps;
        bool bumper1Exist;
        bool bumper2Exist;

        float breakoutWidth, breakoutHeight;

        struct Breakout {
            glm::vec3 color;
            int x, y;
            int width, height;
            int resistance;
        };
        std::vector<Breakout> breakouts;

        glm::vec3 colors[6] = {
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, 1),
            glm::vec3(1, 1, 0),
            glm::vec3(1, 0, 1),
            glm::vec3(0, 1, 1)
        };

        int nrColors = 6;

        struct Dot {
            float x, y;
            float speed;
        };
        std::vector<Dot> dots;
    };
}
