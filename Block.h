#pragma once 
#include <iostream> 
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\ext\vector_float3.hpp>

struct index { 
    union {
        struct {
            int x; int y; int z;
        };
        int num[3]; 
    };
};

enum Colors { BLUE = 0, RED = 1, GREEN = 2, YELLOW = 3 ,ORANGE = 4, WHITE = 5, VOID = 6 };

enum Direction { clockwise, countercw };

enum moveType { columnF, columnS, row };

struct info {
    index i; 
    int inc1, inc2; 
};

struct MovData {
    moveType m;
    Direction D;
    int num; 
};

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

    ~Block() = default; 

};

class Instructions {
    std::vector<MovData> moves; 
   
};

template<int N>
class Rcube {

private:

    inline void faceConstructor(index i, int px, Colors x) {
        cube[i].draw = true;
        cube[i].colors[px] = x;
    }

    info createIndex(int column, int columnPos, int num) {
        info i;
        switch (columnPos) {
        case 0:
            i.i = { column + num, 0, 0 };
            i.inc1 = 1, i.inc2 = 2;
            break;
        case 1:
            i.i = { 0, column + num, 0 };
            i.inc1 = 0, i.inc2 = 2;
            break;
        case 2:
            i.i = { 0, 0, column + num };
            i.inc1 = 0, i.inc2 = 1;
            break;
        }
        return i;
    }

    void transpose(int column, int columnPos, int colorPos) {
        info data1 = createIndex(column, columnPos, 0);
        index data2 = data1.i;
        for (int i = 0; i < N; i++) {
            data1.i.num[data1.inc1] = i;
            data2.num[data1.inc2] = i;
            for (int j = i; j < N; j++) {
                data1.i.num[data1.inc2] = j;
                data2.num[data1.inc1] = j;
                Colors temp = cube[data1.i].colors[colorPos];
                cube[data1.i].colors[colorPos] = cube[data2].colors[colorPos];
                cube[data2].colors[colorPos] = temp;
            }
        }
    }

    void flip(int column, int columnPos, int colorPos) {
        info data1 = createIndex(column, columnPos, 0);
        index data2 = data1.i;
        for (int i = 0; i < N; i++) {
            data1.i.num[data1.inc1] = i;
            data2.num[data1.inc1] = i;
            for (int j = 0; j < N / 2; j++) {
                data1.i.num[data1.inc2] = j;
                data2.num[data1.inc2] = N - 1 - j;
                Colors temp = cube[data1.i].colors[colorPos];
                cube[data1.i].colors[colorPos] = cube[data2].colors[colorPos];
                cube[data2].colors[colorPos] = temp;
            }
        }
    }

    void rotateMatrixC(int column, int comlumnPos, int colorPos) {
        transpose(column, comlumnPos, colorPos);
        flip(column, comlumnPos, colorPos);
    }

    void rotateMatrixCCW(int column, int comlumnPos, int colorPos) {
        flip(column, comlumnPos, colorPos);
        transpose(column, comlumnPos, colorPos);
    }

    void rotateRing(index i1, int iP1, int cP1, int num1, index i2, int iP2, int cP2, int num2, index i3, int iP3, int cP3, int num3, index i4, int iP4, int cP4, int num4, index i5, int num5, int t1, int t2, int t3) {
        Colors temp[N];
        Colors temp2[N];
        for (int i = 0; i < N; i++) {
            i1.num[iP1] = std::abs(num1 - i);
            i4.num[iP4] = std::abs(num4 - i);
            temp[i] = cube[i1].colors[cP1];
            cube[i1].colors[cP1] = cube[i4].colors[cP4];
        }

        for (int i = 0; i < N; i++) {
            i2.num[iP2] = std::abs(num2 - i);
            temp2[i] = cube[i2].colors[cP2];
            cube[i2].colors[cP2] = temp[std::abs(t1 - i)];
        }

        for (int i = 0; i < N; i++) {
            i3.num[iP3] = std::abs(num3 - i); 
            temp[i] = cube[i3].colors[cP3];
            cube[i3].colors[cP3] = temp2[std::abs(t2 - i)];
        }

        for (int i = 0; i < N; i++) {
            i5.num[iP4] = std::abs(num5 - i); 
            cube[i5].colors[cP4] = temp[std::abs(t3 - i)];
        }
    }   //TODO 

    inline void drawN(index i, bool b) {
        cube[i].draw = b;
    }

    inline void drawV(index i, bool b) {
        cube[i].drawVoid = b;
    }

    void draw2(int column, int columnPos, int num, bool b, int start, int end, void (Rcube::*f)(index, bool)) {
        info data1 = createIndex(column, columnPos, 0);
        index data2 = data1.i;
        int compare = data1.inc1 + data1.inc2;
        data2.num[compare == 1 ? 2 : compare == 2 ? 1 : 0] += num;
        for (int i = start; i < end; i++) {
            data1.i.num[data1.inc1] = i;
            data2.num[data1.inc1] = i;
            for (int j = start; j < end; j++) {
                data1.i.num[data1.inc2] = j;
                data2.num[data1.inc2] = j;
                (this->*f)(data1.i, b);
                (this->*f)(data2, b);
            }
        }
    }

    void draw3(int column, int columnPos, int num1, int num2, bool b, int start, int end, void (Rcube::*f1)(index, bool), void (Rcube::*f2)(index, bool)) {
        info data1 = createIndex(column, columnPos, 0);
        index data2 = data1.i; 
        index data3 = data1.i;
        int compare = data1.inc1 + data1.inc2;
        data2.num[compare == 1 ? 2 : compare == 2 ? 1 : 0] += num1;
        data3.num[compare == 1 ? 2 : compare == 2 ? 1 : 0] += num2;
        for (int i = start; i < end; i++) {
            data1.i.num[data1.inc1] = i;
            data2.num[data1.inc1] = i;
            data3.num[data1.inc1] = i;
            for (int j = start; j < end; j++) {
                data1.i.num[data1.inc2] = j;
                data2.num[data1.inc2] = j;
                data3.num[data1.inc2] = j;
                (this->*f1)(data1.i, b);
                (this->*f1)(data2, b);
                (this->*f2)(data3, b);
            }
        }
    }

    void rotate(glm::mat4& model, glm::vec3 axis, glm::vec3 trans, float dRot) {
        model = glm::translate(model, trans);
        model = glm::rotate_slow(model, glm::radians(dRot), axis);
        model = glm::translate(model, -trans);
    }

    void selectRotation(Direction D, glm::mat4& model, glm::vec3 r, glm::vec3 t, float angle) {
        switch (D) {
        case clockwise:
            rotate(model, r, t, -angle);
            break;
        case countercw:
            rotate(model, r, t, angle);
            break;
        }
    }

    void pieceTurn(int current, int column, int columnPos, bool& b, glm::mat4 model, Direction D, void (Rcube::* f1)(int, Direction), void (Rcube::* f2)(int, Direction)) {
        if (current == column) {
            this->current = true;
            if (dRot == 1) {
                if (column > 0 && column < cube.size - 1) {
                    draw3(column, columnPos, -1, 1, true, 1, cube.size - 1, &Rcube::drawN, &Rcube::drawN);
                    draw3(column, columnPos, -1, 1, true, 0, cube.size, &Rcube::drawV, &Rcube::drawV);
                }
                else if (column == 0) {
                    draw3(column, columnPos, 1, 1, true, 0, cube.size, &Rcube::drawV, &Rcube::drawN);
                }
                else {
                    draw3(column, columnPos, -1, -1, true, 0, cube.size, &Rcube::drawV, &Rcube::drawN);

                }
            }
        }
        else if (dRot >= 90) {
            (this->*f1)(column, D);
            (this->*f2)(column, D);
            dRot = 0;
            b = false;
            this->current = false;
            if (column > 0 && column < cube.size - 1) {
                if (column > 1 && column < cube.size - 2) {
                    draw3(column, columnPos, -1, 1, false, 1, cube.size - 1, &Rcube::drawN, &Rcube::drawN);
                }
                else if (column == 1) {
                    draw2(column, columnPos, 1, false, 1, cube.size - 1, &Rcube::drawN);
                }
                else {
                    draw2(column, columnPos, -1, false, 1, cube.size - 1, &Rcube::drawN);
                }
                draw3(column, columnPos, -1, 1, false, 0, cube.size, &Rcube::drawV, &Rcube::drawV);
            }
            else if (column == 0) {
                draw2(column, columnPos, 1, false, 0, cube.size, &Rcube::drawV);
            }
            else {
                draw2(column, columnPos, -1, false, 0, cube.size, &Rcube::drawV);
            }
        }
    }

public:
    array3d<Block, N> cube;
    const float move = (cube.size - 1) * .25;
    float dRot = 0.0f;
    bool current = false, cF = false, cS = false, r = false;

    Rcube() {

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                // T Face 
                faceConstructor({ i, N - 1, j }, 0, BLUE);

                // Bo Face 
                faceConstructor({ i, 0, j }, 1, GREEN);

                // L Face
                faceConstructor({ 0, i, j }, 2, ORANGE);

                // R  Face
                faceConstructor({ N - 1, i, j }, 3, RED);

                // F Face 
                faceConstructor({ i, j, 0 }, 4, WHITE);

                // Ba Face
                faceConstructor({ i, j, N - 1 }, 5, YELLOW);
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
                rotateMatrixC(column, 0, colorPos);
                break;
            case clockwise:
                rotateMatrixCCW(column, 0, colorPos); 
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
                rotateMatrixCCW(column, 2, colorPos); 
                break;
            case clockwise:
                rotateMatrixC(column, 2, colorPos); 
                break;
        }
    }
     
    void colorsSideR(int column, Direction D)  {
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
                rotateMatrixCCW(column, 1, colorPos); 
                break;
            case clockwise:
                rotateMatrixC(column, 1, colorPos); 
                break; 
        }
    }

    void colorsRingF(int column, Direction D) {
        Colors temp[N];
        Colors temp2[N];
        switch (D) {
        case countercw:
            rotateRing({ column,             0,             0 }, 1, 4, 0,
                       { column,             0,             0 }, 2, 1, cube.size - 1,
                       { column,             0, cube.size - 1 }, 1, 5, 0,
                       { column, cube.size - 1,             0 }, 2, 0, 0,
                       { column, cube.size - 1,             0 }, 0,
                       0, cube.size - 1, cube.size - 1); 
            break;
        case clockwise:
            rotateRing({ column, cube.size - 1,             0 }, 2, 0, 0, 
                       { column,             0, cube.size - 1 }, 1, 5, 0, 
                       { column,             0,             0 }, 2, 1, 0, 
                       { column,             0,             0 }, 1, 4, 0, 
                       { column,             0,             0 }, 0,
                       cube.size - 1, 0, cube.size - 1);
            break;
        }
    }

    void colorsRingS(int column, Direction D) {
        Colors temp[N];
        Colors temp2[N];
        switch (D) {
            case countercw:
                rotateRing({             0, cube.size - 1, column }, 0, 0, 0,
                           {             0,             0, column }, 1, 2, 0,
                           {             0,             0, column }, 0, 1, 0,
                           { cube.size - 1,             0, column }, 1, 3, cube.size - 1,
                           { cube.size - 1,             0, column }, 0,
                           0, cube.size - 1, 0); 
                break; 
            case clockwise:
                rotateRing({ cube.size - 1,             0, column }, 1, 3, 0,
                           {             0,             0, column }, 0, 1, 0,
                           {             0,             0, column }, 1, 2, 0,
                           {             0, cube.size - 1, column }, 0, 0, cube.size - 1,
                           {             0, cube.size - 1, column }, 0,
                           0, cube.size - 1, 0); 

                break; 
        }
    }

    void colorsRingR(int column, Direction D) {
        Colors temp[N];
        Colors temp2[N];
        switch (D) {
            case countercw:
                rotateRing({ cube.size - 1, column,             0 }, 2, 3, 0,
                           {             0, column, cube.size - 1 }, 0, 5, 0,
                           {             0, column,             0 }, 2, 2, 0,
                           {             0, column,             0 }, 0, 4, 0,
                           {             0, column,             0 }, cube.size - 1,
                           cube.size - 1, 0, 0); 
                break; 
            case clockwise:
                rotateRing({             0, column,             0 }, 2, 2, 0,
                           {             0, column, cube.size - 1 }, 0, 5, 0,
                           { cube.size - 1, column,             0 }, 2, 3, 0,
                           {             0, column,             0 }, 0, 4, cube.size - 1,
                           {             0, column,             0 }, 0,
                           0, cube.size - 1, 0);
                break;
        }
    }

    void columnF(int current, int column, glm::mat4& model, Direction D) { // height 
        if(current == column)
            selectRotation(D, model, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0, move, -move), dRot);
        pieceTurn(current, column, 0, cF, model, D, &Rcube::colorsSideF, &Rcube::colorsRingF);
    }

    void columnS(int current, int column, glm::mat4& model, Direction D) { // height
        if (current == column)
            selectRotation(D, model, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(move, move, 0), dRot);
        pieceTurn(current, column, 2, cS, model, D, &Rcube::colorsSideS, &Rcube::colorsRingS);
    }

    void row(int current, int column, glm::mat4& model, Direction D)  { // width 
        if (current == column)
            selectRotation(D, model, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(move, 0, -move), dRot);
        pieceTurn(current, column, 1, r, model, D, &Rcube::colorsSideR, &Rcube::colorsRingR);
    }

    void updateAngle() {
        dRot++;
        if (dRot > 90) {
            dRot -= 90;
        }
    }

    ~Rcube() = default; 

};