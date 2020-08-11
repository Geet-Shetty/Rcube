#pragma once 
#include <iostream> 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\ext\vector_float3.hpp>

struct index { int x; int y; int z; };

enum Colors { BLUE = 0, RED = 1, GREEN = 2, YELLOW = 3 ,ORANGE = 4, WHITE = 5, VOID = 6 };

enum Direction { clockwise, countercw };

template<class T, int N>
class array3d {
public:
    T data[N * N * N] = {};
    int size = N;

    array3d() = default;

    array3d(T data[]) {

        for (int i = 0; i < (sizeof(data) * N * N * N) / sizeof(T); i++) {
            this->data[i] = data[i];
        }

    }

    ~array3d() = default;

    T& operator[](index nums) {
        if (nums.x < N && nums.y < N && nums.z < N)
            return data[nums.x * (N * N) + nums.z + (nums.y * N)];
    }

};

class Block {
public:
    Colors colors[6]; // top, bot, left, right, front, back 
    bool draw;
    bool drawVoid;

    Block(Colors* C, int sizeofC) {

        if (sizeofC > 24) {
            std::cout << "Colors pointer data too big!";
            exit(-1);
        }

        for (int i = 0; i < 6; i++) {
            colors[i] = *(C + i);
        }
    }

    Block(Colors colors[6]) {
        for (int i = 0; i < 6; i++) {
            this->colors[i] = colors[i];
        }
    }

    Block() : draw(false), drawVoid(false) {
        //for (int i = 0; i < 6; i++) {
        //    this->colors[i] = VOID;
        //}

        for (Colors& e : colors) {
            e = VOID;
        }
    }

    ~Block() {}

};

template<int N>
class Rcube {

private:

    void rotate(glm::mat4& model, glm::vec3 axis, glm::vec3 trans, float dRot) {
        model = glm::translate(model, trans);
        model = glm::rotate_slow(model, glm::radians(dRot), axis);
        model = glm::translate(model, -trans);
    }

    void copyColors(Colors* temp, Colors* copy) {
        for (int i = 0; i < 6; i++) {
            *(temp + i) = *(copy + i);
        }
    }

public:
    array3d<Block, N> cube;
    float move = (cube.size - 1) * .25;
    float dRot = 0.0f;
    bool current = false;
    bool cF = false;
    bool cS = false;
    bool r = false; 

    Rcube() {

        // corners
        {
            // F Bo L 
            cube[{0, 0, 0}].draw = true;
            cube[{0, 0, 0}].colors[1] = GREEN;
            cube[{0, 0, 0}].colors[2] = ORANGE;
            cube[{0, 0, 0}].colors[4] = WHITE;

            // Ba Bo L
            cube[{0, 0, N - 1}].draw = true;
            cube[{0, 0, N - 1}].colors[1] = GREEN;
            cube[{0, 0, N - 1}].colors[2] = ORANGE;
            cube[{0, 0, N - 1}].colors[5] = YELLOW;

            // F T L 
            cube[{0, N - 1, 0}].draw = true;
            cube[{0, N - 1, 0}].colors[0] = BLUE;
            cube[{0, N - 1, 0}].colors[2] = ORANGE;
            cube[{0, N - 1, 0}].colors[4] = WHITE;

            // Ba T L
            cube[{0, N - 1, N - 1}].draw = true;
            cube[{0, N - 1, N - 1}].colors[0] = BLUE;
            cube[{0, N - 1, N - 1}].colors[2] = ORANGE;
            cube[{0, N - 1, N - 1}].colors[5] = YELLOW;

            // F Bo R
            cube[{N - 1, 0, 0}].draw = true;
            cube[{N - 1, 0, 0}].colors[1] = GREEN;
            cube[{N - 1, 0, 0}].colors[3] = RED;
            cube[{N - 1, 0, 0}].colors[4] = WHITE;

            // Ba Bo R
            cube[{N - 1, 0, N - 1}].draw = true;
            cube[{N - 1, 0, N - 1}].colors[1] = GREEN;
            cube[{N - 1, 0, N - 1}].colors[3] = RED;
            cube[{N - 1, 0, N - 1}].colors[5] = YELLOW;

            // F T R
            cube[{N - 1, N - 1, 0}].draw = true;
            cube[{N - 1, N - 1, 0}].colors[0] = BLUE;
            cube[{N - 1, N - 1, 0}].colors[3] = RED;
            cube[{N - 1, N - 1, 0}].colors[4] = WHITE;

            // Ba T R
            cube[{N - 1, N - 1, N - 1}].draw = true;
            cube[{N - 1, N - 1, N - 1}].colors[0] = BLUE;
            cube[{N - 1, N - 1, N - 1}].colors[3] = RED;
            cube[{N - 1, N - 1, N - 1}].colors[5] = YELLOW;
        }

        // middles and edges 
        for (int i = 1; i < N - 1; i++) {
            // F Bo edge 
            cube[{i, 0, 0}].draw = true;
            cube[{i, 0, 0}].colors[1] = GREEN;
            cube[{i, 0, 0}].colors[4] = WHITE;
            // F T edge
            cube[{i, N - 1, 0}].draw = true;
            cube[{i, N - 1, 0}].colors[0] = BLUE;
            cube[{i, N - 1, 0}].colors[4] = WHITE;
            // Ba Bo edge
            cube[{i, 0, N - 1}].draw = true;
            cube[{i, 0, N - 1}].colors[1] = GREEN;
            cube[{i, 0, N - 1}].colors[5] = YELLOW;
            // Ba T edge 
            cube[{i, N - 1, N - 1}].draw = true;
            cube[{i, N - 1, N - 1}].colors[0] = BLUE;
            cube[{i, N - 1, N - 1}].colors[5] = YELLOW;
            // F L edge
            cube[{0, i, 0}].draw = true;
            cube[{0, i, 0}].colors[2] = ORANGE;
            cube[{0, i, 0}].colors[4] = WHITE;
            // Ba L edge
            cube[{0, i, N - 1}].draw = true;
            cube[{0, i, N - 1}].colors[2] = ORANGE;
            cube[{0, i, N - 1}].colors[5] = YELLOW;
            // F R edge
            cube[{N - 1, i, 0}].draw = true;
            cube[{N - 1, i, 0}].colors[3] = RED;
            cube[{N - 1, i, 0}].colors[4] = WHITE;
            // Ba R edge 
            cube[{N - 1, i, N - 1}].draw = true;
            cube[{N - 1, i, N - 1}].colors[3] = RED;
            cube[{N - 1, i, N - 1}].colors[5] = YELLOW;
            // L Bo edge 
            cube[{0, 0, i}].draw = true;
            cube[{0, 0, i}].colors[1] = GREEN;
            cube[{0, 0, i}].colors[2] = ORANGE;
            // R Bo edge 
            cube[{N - 1, 0, i}].draw = true;
            cube[{N - 1, 0, i}].colors[1] = GREEN;
            cube[{N - 1, 0, i}].colors[3] = RED;
            // L T edge 
            cube[{0, N - 1, i}].draw = true;
            cube[{0, N - 1, i}].colors[0] = BLUE;
            cube[{0, N - 1, i}].colors[2] = ORANGE;
            // R T edge 
            cube[{N - 1, N - 1, i}].draw = true;
            cube[{N - 1, N - 1, i}].colors[0] = BLUE;
            cube[{N - 1, N - 1, i}].colors[3] = RED;


            for (int j = 1; j < N - 1; j++) {
                // Bo Middle 
                cube[{i, 0, j}].draw = true;
                cube[{i, 0, j}].colors[1] = GREEN;
                // T Middle 
                cube[{i, N - 1, j}].draw = true;
                cube[{i, N - 1, j}].colors[0] = BLUE;
                // F Middle 
                cube[{i, j, 0}].draw = true;
                cube[{i, j, 0}].colors[4] = WHITE;
                // Ba Middle 
                cube[{i, j, N - 1}].draw = true;
                cube[{i, j, N - 1}].colors[5] = YELLOW;
                // L Middle
                cube[{0, i, j}].draw = true;
                cube[{0, i, j}].colors[2] = ORANGE;
                // R  Middle
                cube[{N - 1, i, j}].draw = true;
                cube[{N - 1, i, j}].colors[3] = RED;
            }
        }

    }

    void colorsSideF(int column, Direction D) {
        int colorPos; 
        if (column == 0) {
            colorPos = 2; 
        }
        else if (column == N - 1) {
            colorPos = 3; 
        }
        else {
            return; 
        }
        switch (D) {
            case countercw:
                for (int i = 0; i < N; i++) {
                    for (int j = i; j < N; j++) {
                        Colors temp = cube[{column, i, j}].colors[colorPos];
                        cube[{column, i, j}].colors[colorPos] = cube[{column, j, i}].colors[colorPos];
                        cube[{column, j, i}].colors[colorPos] = temp;
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        Colors temp = cube[{column, i, j}].colors[colorPos];
                        cube[{column, i, j}].colors[colorPos] = cube[{column, i, N - 1 - j}].colors[colorPos];
                        cube[{column, i, N - 1 - j}].colors[colorPos] = temp;
                    }
                }
                break;
            case clockwise:
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        Colors temp = cube[{column, i, j}].colors[colorPos];
                        cube[{column, i, j}].colors[colorPos] = cube[{column, i, N - 1 - j}].colors[colorPos];
                        cube[{column, i, N - 1 - j}].colors[colorPos] = temp;
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = i; j < N; j++) {
                        Colors temp = cube[{column, i, j}].colors[colorPos];
                        cube[{column, i, j}].colors[colorPos] = cube[{column, j, i}].colors[colorPos];
                        cube[{column, j, i}].colors[colorPos] = temp;
                    }
                }
                break;
        }
    }

    void colorsSideS(int column, Direction D) {
        int colorPos;
        if (column == 0) {
            colorPos = 4;
        }
        else if (column == N - 1) {
            colorPos = 5;
        }
        else {
            return;
        }
        switch (D) {
            case countercw:
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        Colors temp = cube[{i, j, column}].colors[colorPos];
                        cube[{i, j, column}].colors[colorPos] = cube[{i, N - 1 - j, column}].colors[colorPos];
                        cube[{i, N - 1 - j, column}].colors[colorPos] = temp;
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = i; j < N; j++) {
                        Colors temp = cube[{i, j, column}].colors[colorPos];
                        cube[{i, j, column}].colors[colorPos] = cube[{j, i, column}].colors[colorPos];
                        cube[{j, i, column}].colors[colorPos] = temp;
                    }
                }
                break;
            case clockwise:
                for (int i = 0; i < N; i++) {
                    for (int j = i; j < N; j++) {
                        Colors temp = cube[{i, j, column}].colors[colorPos];
                        cube[{i, j, column}].colors[colorPos] = cube[{j, i, column}].colors[colorPos];
                        cube[{j, i, column}].colors[colorPos] = temp;
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        Colors temp = cube[{i, j, column}].colors[colorPos];
                        cube[{i, j, column}].colors[colorPos] = cube[{i, N - 1 - j, column}].colors[colorPos];
                        cube[{i, N - 1 - j, column}].colors[colorPos] = temp;
                    }
                }
                break;
        }
    }

    void colorsSideR(int column, Direction D) {
        int colorPos;
        if (column == 0) {
            colorPos = 1;
        }
        else if (column == N - 1) {
            colorPos = 0;
        }
        else {
            return; 
        }
        switch (D) {
            case countercw:
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        Colors temp = cube[{i, column, j}].colors[colorPos];
                        cube[{i, column, j}].colors[colorPos] = cube[{i, column, N - 1 - j}].colors[colorPos];
                        cube[{i, column, N - 1 - j}].colors[colorPos] = temp;
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = i; j < N; j++) {
                        Colors temp = cube[{i, column, j}].colors[colorPos];
                        cube[{i, column, j}].colors[colorPos] = cube[{j, column, i}].colors[colorPos];
                        cube[{j, column, i}].colors[colorPos] = temp;
                    }
                }
                break;
            case clockwise:
                for (int i = 0; i < N; i++) {
                    for (int j = i; j < N; j++) {
                        Colors temp = cube[{i, column, j}].colors[colorPos];
                        cube[{i, column, j}].colors[colorPos] = cube[{j, column, i}].colors[colorPos];
                        cube[{j, column, i}].colors[colorPos] = temp;
                    }
                }
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < N / 2; j++) {
                        Colors temp = cube[{i, column, j}].colors[colorPos];
                        cube[{i, column, j}].colors[colorPos] = cube[{i, column, N - 1 - j}].colors[colorPos];
                        cube[{i, column, N - 1 - j}].colors[colorPos] = temp;
                    }
                }
                break; 
        }
    }

    void colorsRingF(int column, Direction D) {
        Colors temp[N];
        Colors temp2[N];
        switch (D) {
        case countercw:
            std::cout << "CC" << std::endl;

            for (int i = 0; i < N; i++) {
                temp[i] = cube[{column, i, 0}].colors[4];
            }

            for (int i = 0; i < N; i++) {
                cube[{column, i, 0}].colors[4] = cube[{column, cube.size - 1, i}].colors[0];
            }

            for (int i = 0; i < N; i++) {
                temp2[i] = cube[{column, 0, i}].colors[1];
            }

            for (int i = 0; i < N; i++) {
                cube[{column, 0, cube.size - 1 - i}].colors[1] = temp[i];
            }

            for (int i = 0; i < N; i++) {
                temp[i] = cube[{column, i, cube.size - 1}].colors[5];
            }

            for (int i = 0; i < N; i++) {
                cube[{column, i, cube.size - 1}].colors[5] = temp2[i];
            }

            for (int i = 0; i < N; i++) {
                cube[{column, cube.size - 1, cube.size - 1 - i}].colors[0] = temp[i];
            }
            break;
        case clockwise:
            std::cout << "C" << std::endl;
            //t1 = cube[{column, cube.size - 1, cube.size - 1}].colors[0], t2 = cube[{column, cube.size - 1, cube.size - 1}].colors[5];
            //cube[{column, cube.size - 1, cube.size - 1}].colors[0] = cube[{column, cube.size - 1, 0}].colors[4];
            //cube[{column, cube.size - 1, cube.size - 1}].colors[5] = cube[{column, cube.size - 1, 0}].colors[0];
            //t3 = cube[{column, 0, cube.size - 1}].colors[1], t4 = cube[{column, 0, cube.size - 1}].colors[5];
            //cube[{column, 0, cube.size - 1}].colors[1] = t2;
            //cube[{column, 0, cube.size - 1}].colors[5] = t1;
            //t1 = cube[{column, 0, 0}].colors[1], t2 = cube[{column, 0, 0}].colors[4];
            //cube[{column, 0, 0}].colors[1] = t4;
            //cube[{column, 0, 0}].colors[4] = t3;
            //cube[{column, cube.size - 1, 0}].colors[0] = t2;
            //cube[{column, cube.size - 1, 0}].colors[4] = t1;
            for (int i = 0; i < N; i++) {
                temp[i] = cube[{column, cube.size - 1, i}].colors[0]; 
            }

            for (int i = 0; i < N; i++) {
                cube[{column, cube.size - 1, i}].colors[0] = cube[{column, i, 0}].colors[4];
            }

            for (int i = 0; i < N; i++) {
                temp2[i] = cube[{column, i, cube.size - 1}].colors[5];
            }

            for (int i = 0; i < N; i++) {
                cube[{column, i, cube.size - 1}].colors[5] = temp[cube.size - 1 - i]; 
            }

            for (int i = 0; i < N; i++) {
                temp[i] = cube[{column, 0, cube.size - 1 - i}].colors[1]; 
            }

            for (int i = 0; i < N; i++) {
                cube[{column, 0, cube.size - 1 - i}].colors[1] = temp2[cube.size - 1 - i];
            }

            for (int i = 0; i < N; i++) {
                cube[{column, i, 0}].colors[4] = temp[i];
            }
            break;
        }
    }

    void colorsRingS(int column, Direction D) {
        Colors temp[N];
        Colors temp2[N];
        switch (D) {
            case countercw:
                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{i, cube.size - 1, column}].colors[0];
                }

                for (int i = 0; i < N; i++) {
                    cube[{i, cube.size - 1, column}].colors[0] = cube[{cube.size - 1, cube.size - 1 - i, column}].colors[3];
                }

                for (int i = 0; i < N; i++) {
                    temp2[i] = cube[{0, i, column}].colors[2];
                }

                for (int i = 0; i < N; i++) {
                    cube[{0, i, column}].colors[2] = temp[i];
                }

                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{i, 0, column}].colors[1]; 
                }

                for (int i = 0; i < N; i++) {
                    cube[{i, 0, column}].colors[1] = temp2[cube.size - 1 - i];
                }

                for (int i = 0; i < N; i++) {
                    cube[{cube.size - 1, cube.size - 1 - i, column}].colors[3] = temp[cube.size - 1 - i];
                }
                break;
            case clockwise:
                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{cube.size - 1, i, column}].colors[3];
                }

                for (int i = 0; i < N; i++) {
                    cube[{cube.size - 1, i, column}].colors[3] = cube[{cube.size - 1 - i, cube.size - 1, column}].colors[0];
                }

                for (int i = 0; i < N; i++) {
                    temp2[i] = cube[{i, 0, column}].colors[1];
                }

                for (int i = 0; i < N; i++) {
                    cube[{i, 0, column}].colors[1] = temp[i];
                }

                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{0, i, column}].colors[2]; 
                }

                for (int i = 0; i < N; i++) {
                    cube[{0, i, column}].colors[2] = temp2[cube.size - 1 - i];
                }

                for (int i = 0; i < N; i++) {
                    cube[{cube.size - 1 - i, cube.size - 1, column}].colors[0] = temp[cube.size - 1 - i];
                }
                break; 
        }
    }

    void colorsRingR(int column, Direction D) {
        Colors temp[N];
        Colors temp2[N];
        switch (D) {
            case countercw:
                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{cube.size - 1, column, i}].colors[3]; 
                }

                for (int i = 0; i < N; i++) {
                    cube[{cube.size - 1, column, i}].colors[3] = cube[{i, column, 0}].colors[4];
                }

                for (int i = 0; i < N; i++) {
                    temp2[i] = cube[{i, column, cube.size - 1}].colors[5];
                }

                for (int i = 0; i < N; i++) {
                    cube[{i, column, cube.size - 1}].colors[5] = temp[cube.size -  1 - i];
                }

                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{0, column, i}].colors[2];
                }

                for (int i = 0; i < N; i++) {
                    cube[{0, column, i}].colors[2] = temp2[i];
                }

                for (int i = 0; i < N; i++) {
                    cube[{i, column, 0}].colors[4] = temp[cube.size -  1 - i];
                }
                break;
            case clockwise:
                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{0, column, i}].colors[2];
                }

                for (int i = 0; i < N; i++) {
                    cube[{0, column, i}].colors[2] = cube[{cube.size - 1 - i, column, 0}].colors[4];
                }

                for (int i = 0; i < N; i++) {
                    temp2[i] = cube[{i, column, cube.size - 1}].colors[5];
                }

                for (int i = 0; i < N; i++) {
                   cube[{i, column, cube.size - 1}].colors[5] = temp[i];
                }

                for (int i = 0; i < N; i++) {
                    temp[i] = cube[{cube.size - 1, column, i}].colors[3];
                }

                for (int i = 0; i < N; i++) {
                    cube[{cube.size - 1, column, i}].colors[3] = temp2[cube.size - 1 - i];
                }

                for (int i = 0; i < N; i++) {
                   cube[{i, column, 0}].colors[4] = temp[i];
                }
                break;
        }
    }

    void columnF(int current, int column, glm::mat4& model, Direction D) { // height 
        if (current == column) {
            this->current = true; 
            switch (D) {
                case clockwise:
                    rotate(model, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, move, -move), -dRot);
                    break;
                case countercw:
                    rotate(model, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, move, -move), dRot);
                    break;
            }
            if (dRot == 1) {
                if (!(column == 0 || column == cube.size - 1)) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{column, i, j}].draw = true;
                            cube[{column - 1, i, j}].draw = true;
                            cube[{column + 1, i, j}].draw = true;
                        }
                    }

                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{column, i, j}].drawVoid = true;
                            cube[{column - 1, i, j}].drawVoid = true;
                            cube[{column + 1, i, j}].drawVoid = true;
                        }
                    }
                }
                else if (column == 0) {
                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{column, i, j}].drawVoid = true;
                            cube[{column + 1, i, j}].drawVoid = true;
                            cube[{column + 1, i, j}].draw = true;
                        }
                    }
                }
                else {
                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{column, i, j}].drawVoid = true;
                            cube[{column - 1, i, j}].drawVoid = true;
                            cube[{column - 1, i, j}].draw = true;
                        }
                    }
                }
            }
        }
        else if (dRot >= 90) {
            colorsSideF(column, D);
            colorsRingF(column, D);
            dRot = 0;
            cF = false;
            this->current = false;
            if (!(column == 0 || column == cube.size - 1)) {

                if (column > 1 && column < cube.size - 2) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{column, i, j}].draw = false;
                            cube[{column - 1, i, j}].draw = false;
                            cube[{column + 1, i, j}].draw = false;
                        }
                    }
                }
                else if (column == 1) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{column, i, j}].draw = false;
                            cube[{column + 1, i, j}].draw = false;
                        }
                    }
                } else {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{column, i, j}].draw = false;
                            cube[{column - 1, i, j}].draw = false;
                        }
                    }
                }

                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{column, i, j}].drawVoid = false;
                        cube[{column - 1, i, j}].drawVoid = false;
                        cube[{column + 1, i, j}].drawVoid = false;
                    }
                }
            }
            else if (column == 0) {
                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{column, i, j}].drawVoid = false;
                        cube[{column + 1, i, j}].drawVoid = false;
                    }
                }
            }
            else {
                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{column, i, j}].drawVoid = false;
                        cube[{column - 1, i, j}].drawVoid = false;
                    }
                }
            }
        }
    }

    void columnS(int current, int column, glm::mat4& model, Direction D) { // height 
        if (current == column) {
            this->current = true;
            switch (D) {
                case clockwise:
                    rotate(model, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(move, move, 0), -dRot);
                    break;
                case countercw:
                    rotate(model, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(move, move, 0), dRot);
                    break;
            }
            if (dRot == 1) {
                if (!(column == 0 || column == cube.size - 1)) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, j, column}].draw = true;
                            cube[{i, j, column - 1}].draw = true;
                            cube[{i, j, column + 1}].draw = true;
                        }
                    }

                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{i, j, column}].drawVoid = true;
                            cube[{i, j, column - 1}].drawVoid = true;
                            cube[{i, j, column + 1}].drawVoid = true;
                        }
                    }
                }
                else if (column == 0) {
                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{i, j, column}].drawVoid = true;
                            cube[{i, j, column + 1}].drawVoid = true;
                            cube[{i, j, column + 1}].draw = true;
                        }
                    }
                }
                else {
                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{i, j, column}].drawVoid = true;
                            cube[{i, j, column - 1}].drawVoid = true;
                            cube[{i, j, column - 1}].draw = true;
                        }
                    }
                }
            }
        }
        else if (dRot >= 90) {
            colorsSideS(column, D);
            colorsRingS(column, D); 
            dRot = 0;
            cS = false;
            this->current = false;
            if (!(column == 0 || column == cube.size - 1)) {
                if (column > 1 && column < cube.size - 2) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, j, column}].draw = false;
                            cube[{i, j, column - 1}].draw = false;
                            cube[{i, j, column + 1}].draw = false;
                        }
                    }
                }
                else if (column == 1) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, j, column}].draw = false;
                            cube[{i, j, column + 1}].draw = false;
                        }
                    }
                }
                else {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, j, column}].draw = false;
                            cube[{i, j, column - 1}].draw = false;
                        }
                    }
                }

                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{i, j, column}].drawVoid = false;
                        cube[{i, j, column - 1}].drawVoid = false;
                        cube[{i, j, column + 1}].drawVoid = false;
                    }
                }
            }
            else if (column == 0) {
                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{i, j, column}].drawVoid = false;
                        cube[{i, j, column + 1}].drawVoid = false;
                    }
                }
            }
            else {
                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{i, j, column}].drawVoid = false;
                        cube[{i, j, column - 1}].drawVoid = false;
                    }
                }
            }
        }
    }

    void row(int current, int column, glm::mat4& model, Direction D)  { // width 
        if (current == column) {
            this->current = true; 
            switch (D) {
            case clockwise:
                rotate(model, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(move, 0, -move), -dRot);
                break;
            case countercw:
                rotate(model, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(move, 0, -move), dRot);
                break;
            }
            if (dRot == 1) {
                if (!(column == 0 || column == cube.size - 1)) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, column, j}].draw = true;
                            cube[{i, column - 1, j}].draw = true;
                            cube[{i, column + 1, j}].draw = true;
                        }
                    }

                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{i, column, j}].drawVoid = true;
                            cube[{i, column - 1, j}].drawVoid = true;
                            cube[{i, column + 1, j}].drawVoid = true;
                        }
                    }
                }
                else if (column == 0) {
                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{i, column, j}].drawVoid = true;
                            cube[{i, column + 1, j}].drawVoid = true;
                            cube[{i, column + 1, j}].draw = true;
                        }
                    }
                }
                else {
                    for (int i = 0; i < cube.size; i++) {
                        for (int j = 0; j < cube.size; j++) {
                            cube[{i, column, j}].drawVoid = true;
                            cube[{i, column - 1, j}].drawVoid = true;
                            cube[{i, column - 1, j}].draw = true;
                        }
                    }
                }
            }
        }
        else if (dRot >= 90) {
            colorsSideR(column, D);
            colorsRingR(column, D); 
            dRot = 0;
            r = false;
            this->current = false;
            if (!(column == 0 || column == cube.size - 1)) {
                if (column > 1 && column < cube.size - 2) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, column, j}].draw = false;
                            cube[{i, column - 1, j}].draw = false;
                            cube[{i, column + 1, j}].draw = false;
                        }
                    }
                }
                else if (column == 1) {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, column, j}].draw = false; 
                            cube[{i, column + 1, j}].draw = false;
                        }
                    }
                }
                else {
                    for (int i = 1; i < cube.size - 1; i++) {
                        for (int j = 1; j < cube.size - 1; j++) {
                            cube[{i, column, j}].draw = false;
                            cube[{i, column - 1, j}].draw = false;
                        }
                    }
                }

                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{i, column, j}].drawVoid = false;
                        cube[{i, column - 1, j}].drawVoid = false;
                        cube[{i, column + 1, j}].drawVoid = false;
                    }
                }
            }
            else if (column == 0) {
                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{i, column, j}].drawVoid = false;
                        cube[{i, column + 1, j}].drawVoid = false;
                    }
                }
            }
            else {
                for (int i = 0; i < cube.size; i++) {
                    for (int j = 0; j < cube.size; j++) {
                        cube[{i, column, j}].drawVoid = false;
                        cube[{i, column - 1, j}].drawVoid = false;
                    }
                }
            }

        }
    }

    void updateAngle() {
        dRot++;
        if (dRot > 90) {
            dRot -= 90;
        }
    }

    ~Rcube() {}

};
