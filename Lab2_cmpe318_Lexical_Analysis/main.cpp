//
//  main.cpp
//  Lab2_cmpe318_Lexical_Analysis
//
//  Created by Saeed Ali on 4/3/17.
//  Copyright © 2017 Saeed Ali. All rights reserved.
//

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include <queue>

using namespace std;
int charClass;
char lexeme [100];
string lexeme2[100] = {"0"};
char nextChar;
int lexLen;
int lexLen2 =0;
int token;
int nextToken;
FILE *in_fp, *fopen();
void addChar();
void getChar();
void print_ident(string lex2[], char* lex);
void print_funct(int token, char* lex);

void getNonBlank();



int lex();

#define LETTER 0
#define DIGIT 1
#define FRACTION 2
#define UNKNOWN 99

#define INT_LIT 10
#define FLOAT_LIT 11
#define IDENT 12
#define GT_OP 22
#define LT_OP 23
#define GE_OP 24
#define LE_OP 25
#define EQ_OP 26

int id_count = 0;

int main()
{
    char* inputfile = nullptr;
    int do_again = 0;
    
    printf("press 1 to start lexical analyser or print 0 to exit: ");
    while(scanf("%d", &do_again) && do_again == 1){
        printf("Please enter the filepath or the name of the input file\n");
        scanf("%s", inputfile);
        
        if((in_fp = fopen(inputfile, "r")) == NULL)
        {
            perror("fopen()");
            _exit(1);
        }
        else
        {
            getChar();
            do
            {
                lex();
            }while(nextToken != EOF);
            
        }
        printf("\npress 1 to call lexcial analyzer again or 0 to exit: "); 
    }
    printf("\n");
    return 0;
    
    
    
}



int lookup(char ch) {
    if(ch == '='){
        addChar();
        getChar();
        addChar();
        nextToken = EQ_OP;
    }
    else if(ch == '<'){
        addChar();
        getChar();
        if(nextChar == '='){
            addChar();
            nextToken = LE_OP;
            
        }
        else
        {
            nextToken = LT_OP  ;
        }
    }
    else if(ch == '>')
    {
        addChar();
        getChar();
        if (nextChar == '=') {
            addChar();
            nextToken = GE_OP;
        }
        
        else
        {
            nextToken = GT_OP  ;
        }
        
    }
    
    
    else{
        addChar();
        nextToken = EOF;
    }
    return nextToken;
}


void addChar() {
    if (lexLen <= 200) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar) )
            charClass = LETTER;
        else if (isdigit(nextChar)|| nextChar == '-')
            charClass = DIGIT;
        else if (nextChar == '.')
            charClass = FRACTION;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
    
}

void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

void print_ident(string lex2[], char* lex)
{
    int i = 0, check = 0;
    
    for( i = 0 ; i < 100; i++)
        if(lex == lex2[i]){
            printf("<IDENT, %d> ", i);
            check = 1;
        }
    if(check == 0){
        lexeme2[lexLen2++] = lexeme;
        printf("<IDENT, %d> ", id_count++);
    }
}

void print_funct(int token, char* lex)
{
    switch (token) {
        case INT_LIT:
            printf("<INTEGER, %s> ", lex);
            break;
        case FLOAT_LIT:
            printf("<FLOAT, %s> ", lex);
            break;
        case GT_OP:
            printf("<GT, \"nothing\"> ");
            break;
        case GE_OP:
            printf("<GE, \"nothing\"> ");
            break;
        case LT_OP:
            printf("<LT, \"nothing\"> ");
            break;
        case LE_OP:
            printf("<LE, \"nothing\"> ");
            break;
        case EQ_OP:
            printf("<EQ, \"nothing\"> ");
            break;
        case IDENT:
            print_ident(lexeme2, lex);
            break;
            
        default:
            break;
            
    }
    
}

int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT)
            {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            
            break;
            
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT){
                addChar();
                getChar();
            }
            
            if(charClass == FRACTION)
            {
                lexeme[lexLen++] = '.';
                getChar();
                while(charClass == DIGIT)
                {
                    addChar();
                    getChar();
                }
                nextToken = FLOAT_LIT;
            }
            else {
                nextToken = INT_LIT;
            }
            break;
            
            //this will be the case if the user decides to input .123 wihout a first digit
        case FRACTION:
            addChar();
            getchar();
            while (charClass == DIGIT) {
                addChar();
                getchar();
            }
            nextToken = FLOAT_LIT;
            break;
            
        case UNKNOWN:
            lookup(nextChar);
            break;
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    print_funct(nextToken, lexeme);
    getChar();
    return nextToken;
}
