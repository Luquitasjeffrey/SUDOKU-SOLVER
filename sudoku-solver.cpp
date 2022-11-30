#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cstdio>
using namespace std;

class SUDOKU{
public:

    int8_t inputs[9][9];
    void print(){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                printf("%i ", (int)this->inputs[i][j]);
                if((j+1)%3==0){
                    printf("    ");
                }
            }
            printf("\n");
            if((i+1)%3==0){
                printf("\n");
            }
        }
        printf("\n");
    }

    SUDOKU(){//caso default
        int8_t cpy[9][9]={
                 {5, 3, 0,	0, 7, 0,	0, 0, 0},
                 {6, 0, 0,	1, 9, 5,	0, 0, 0},
                 {0, 9, 8,	0, 0, 0,	0, 6, 0},

                 {8, 0, 0,	0, 6, 0,	0, 0, 3},
                 {4, 0, 0,	8, 0, 3,	0, 0, 1},
                 {7, 0, 0,	0, 2, 0,	0, 0, 6},

                 {0, 6, 0,	0, 0, 0,	2, 8, 0},
                 {0, 0, 0,	4, 1, 9,	0, 0, 5},
                 {0, 0, 0,	0, 8, 0,	0, 7, 9} };
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                this->inputs[i][j]=cpy[i][j];
            }
        }
        //print();
    }

    void init(char* filename){
        FILE *file=fopen((const char*)filename, "r");
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                while(!fillinput(getc(file), i, j));
            }
        }
    }


    bool sudoku_fill(){
        vector<int8_t> moves;
        pair<int8_t, int8_t> pos;
        if(!best_choice(pos, moves)){
            return 0;
        }

        if(pos.first==-1){
            //printf("we did it\n");
            return 1;
        }

        SUDOKU sudoku;

        //printf("best choice: %i %i [%i]", pos.first, pos.second, moves.size());
        for(int i=0; i<moves.size(); i++){
            //printf("try with: %i\n", moves[i]);
            memcpy(&sudoku, this, sizeof(SUDOKU));
            sudoku.inputs[pos.first][pos.second]=moves[i];
            //sudoku.print();
            if(sudoku.sudoku_fill()){
                memcpy(this, &sudoku, sizeof(SUDOKU));
                return 1;
            }
        }

        return 0;
    }

private:
    inline bool fillinput(char ch, int i, int j){
        #define x (this->inputs[i][j])
        switch(ch){
            case '0':
                x=0;
                return 1;
            case '1':
                x=1;
                return 1;
            case '2':
                x=2;
                return 1;
            case '3':
                x=3;
                return 1;
            case '4':
                x=4;
                return 1;
            case '5':
                x=5;
                return 1;
            case '6':
                x=6;
                return 1;
            case '7':
                x=7;
                return 1;
            case '8':
                x=8;
                return 1;
            case '9':
                x=9;
                return 1;
            default:
                return 0;
        }
        #undef x
    }

    /*
    void cpy(SUDOKU & s){
        for(int i=0; i<9; i++){
            for(int j=0; j<9; j++){
                this->inputs[i][j]=s.inputs[i][j];
            }
        }
    }
    */
    vector<int8_t> valid_moves(int i, int j){
        bool moves[10]={0, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        //filas
        for(int ii=0; ii<9; ii++){
            moves[this->inputs[ii][j]]=false;
        }
        //columnas
        for(int jj=0; jj<9; jj++){
            moves[this->inputs[i][jj]]=false;
        }

        //cells
        int8_t ci=(i-i%3);
        int8_t cj=(j-j%3);

        for(int ii=0; ii<3; ii++){
            for(int jj=0; jj<3; jj++){
                moves[this->inputs[ci+ii][cj+jj]]=false; //now the cells
            }
        }
        vector<int8_t> ret;
        for(int8_t ii=1; ii<=9; ii++){
            if(moves[ii]){
                ret.push_back(ii);
            }
        }
        return ret;
    }

    ///also makes the forced moves
    bool best_choice(pair<int8_t, int8_t> &pos, vector<int8_t> &opts){
        bool _forced;
        do{
            pos={-1, -1};
            _forced=0;
            int8_t best=9;
            for(int i=0; i<9; i++){
                for(int j=0; j<9; j++){
                    if(this->inputs[i][j]!=0){
                        continue;
                    }
                    vector<int8_t> moves=valid_moves(i, j);
                    if(moves.size()==1){
                        _forced=1;
                        //printf("forced move\n");
                        this->inputs[i][j]=moves[0];//forced move
                        continue;
                    }
                    if(moves.size()==0){
                        return 0; //no valid choice
                    }
                    if(moves.size()<best){
                        pos={i, j};
                        best=moves.size();
                        opts=moves;
                    }
                }
            }
        }while(_forced);

        return 1;
    }

};





int main(int argc, char**argv){
    SUDOKU sudoku;
    if(argc>1){
        sudoku.init(argv[1]);
    }
    printf("input:\n");
    sudoku.print();
    printf("-------------------------------------------------------------------------\n\n");

    sudoku.sudoku_fill();
    printf("output:\n");
    sudoku.print();
    printf("-------------------------------------------------------------------------\n\n");
    return 0;
}
