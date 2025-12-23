//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include "util.h"
#include<cstdlib>
#include<ctime>
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

string * dictionary;
int dictionarysize = 369646;
#define KEY_ESC 27 


//Variables I declared
float time1 = 150;                 //variable for controlling time 
float X , Y;                 // used in mouse clicked function to get find the slope    
float row = 465 , col = 10;        //dimensions of the moving ball i.e. shooter
bool up = false;                   //variable for checking if the button was clicked, to move shooter 
int click = 0;                       //for knowing the number of click 
float slope ;                        //slope of line from ball to click point
float dx = 5 , dy;                   // incrementing variables in x and y axes
float speed = 80;               //to make the motion smooth, constant speed 
float rem , DIV;                   //variables for attaching the shooter in exact columns
float low , high;                    //for getting dimension of the column
int l = time(0);            // variable used in srand() for random generation of characters 1st 2 rows
int L;   //variable used in srand() for generating random numbers for shooter ball every time.
int x , zon = 0;         //element of array zero or not 
int arr[8][15]  = {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
char charArr[8][15];            // for storing all the characters of the bubbles
int aligned_position;          //column of the ball when it moves up
int new_ball;                  // storing the number of the shooter ball being formed every time
int iterate = -1;          //a variable for checking words 
string word;                      //a word from dictionary
int wordCount = 0;                //total checked words count
int foundCount = 0;               //total matched words count
string outputFile = "words_matched.txt";            //output file name for storing the matched words
ofstream result(outputFile);                        //creating object for the output file
int len;               //length of the current word fetched from the dictionary array
bool found , match;      //bool variables to check if a word is found or not 
int ROWS , COLS;        //for word checking
int last ;              // to find the last click made by the user before time becomes 0 or less
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
//Variables I declared


// 20,30,30
const int bradius = 30; // ball radius in pixels... 

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}

void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookie in pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */
void DisplayFunction() {


	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...
	
       if(time1 <= 0.0) {
              DrawAlphabet((alphabets)6 , 180 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)0 , 245 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)12 , 310 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)4 , 375 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)14 , 480 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)21 , 545 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)4 , 610 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)17 , 675 , height / 1.45 , awidth, aheight);
      	DrawString(300, height - 300, width, height + 5, "Game ended with score: " + Num2Str(score), colors[BLUE_VIOLET]);
      	
      	if(click > last)
      	{
      	      result.close();
      	      cout<<"\n\n";
           for(int i=0 ; i<80 ; i++) {cout << BOLD << MAGENTA << "-" << RESET;}
           cout << BOLD << CYAN << "\n\n             You were able to match only "<<foundCount<<" words.\n" << RESET;
           cout << BOLD << BLUE << "             Game ended with "<<score<<" score.\n\n" << RESET ;
           for(int i=0 ; i<80 ; i++) {cout << BOLD << MAGENTA <<"-" << RESET;}
           cout<<"\n\n";
      	      exit(1);
      	}
       }
	
	
      else if(click == 0) {
              int w = 30 ;
              DrawAlphabet((alphabets)22 , 80 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)14 , 145 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)17 , 210 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)3 , 275 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)18 , 380 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)7 , 445 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)14 , 510 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)14 , 575 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)19 , 640 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)4 , 705 , height / 1.45 , awidth, aheight);
              DrawAlphabet((alphabets)17 , 770 , height / 1.45 , awidth, aheight);
              DrawString(380, 300 , width, height , "Left Click to Shoot", colors[GREEN]);
	      DrawString(300, 250 , width, height , "CLICK ANYWHERE TO BEGIN", colors[BROWN]);
       }
	
	

       else if(click >= 1) {
	
	srand(l);
	int cord[2][15]; 
	int x2 = 10 ; 
	float x3 = 568 ; 
	
	
	
	// for randomly populating the first two rows the first time
	if(zon == 0) {           
	  for ( int i = 0 ; i < 2 ; i++ ) {
	       for (int j = 0 ; j < 15 ; j++ ) { 
	            x = GetAlphabet(); 
	            DrawAlphabet((alphabets)x , x2 , x3, awidth, aheight);
	            x2+=60 ; 
	            arr[i][j] = x ;
	       }
	    x3 -= 60 ; 
	    x2 = 10 ;
	  }
        }
	
	//after the first time, when the code runs
	else {
	      for ( int i = 0 ; i < 8 ; i++ ) {
	           for (int j = 0 ; j < 15 ; j++ ) {
	               if (arr[i][j] >= 0 && arr[i][j] <= 25) { 
	                  DrawAlphabet((alphabets)arr[i][j] , x2 , x3, awidth, aheight);
	               }
	             x2+=60 ; 
	           }
	        x3 -= 60 ;
	        x2 = 10 ;
	      }
        }
        
        //converting the integer array to character array by adding 97 to all integers.
        for (int i = 0; i < 8; i++) {
              for (int j = 0; j < 15; j++) {
	          if (arr[i][j] >= 0 && arr[i][j] <= 25) { 
                     charArr[i][j] = char(arr[i][j] + 97);
                  }    
              }
        }
      
      
      // for checking for the first time filling the two rows with random alphabets...
      if(row == 465 && col == 10 && zon == 0) {    
    
        do{    
           foundCount = 0;
           for(int index = 0 ; index < 369646 && foundCount <= 4 ; index++) { 
    
               word = dictionary[index]; 
               len = word.length() ;

               found = false;

               ROWS = 2 , COLS = 15 ;
               
               // Check each row
               for (int i = 0; i < ROWS && !found; i++) {
                   for (int start = 0 ; start <= COLS - len  && !found ; start++) {
                       match = true;
                       for (int k = 0; k < len; k++) {
                           if (charArr[i][start + k] != word[k]) {
                              match = false;
                              break;
                           }
                       }
                       if (match) {
                           for(int j = start + (len-1) ; j >= start ; j--) {
                               arr[i][j] = -1 ;  // for removing the characters that are forming words 
                               charArr[i][j] = '0' ; // /////    // important 
                           }
                       }
                       if (match) {
                          result << "Word found: " << word << " in row " << i << endl;       
                          found = true;
                          foundCount++;
                       }
                   }
               }
        

               // Check each column
               for (int j = 0; j < COLS && !found; j++) {
                   for (int start = 0; start <= ROWS - len && !found; start++) {
                       bool match = true;
                       for (int k = 0; k < len; k++) {
                           if (charArr[start + k][j] != word[k]) {
                              match = false;
                              break;
                           }
                       }
                       if (match) {
                           for(int i = start + (len-1) ; i >= start ; i--) {
                               arr[i][j] = -1 ;  // for removing the characters that are forming words 
                               charArr[i][j] = '0' ; // /////    // important 
                           }
                               result << "Word found: " << word << " in column " << j << endl;       
                       }
                       
                       
                       if (match) {
                          found = true;
                          foundCount++;
                       }
                   }
               }
               
               //for diagonals..
               for (int i = 0; i <= ROWS - len && !found; i++) {
                    for (int j = 0; j <= COLS - len && !found; j++) {
                         bool match = true;
                         for (int k = 0; k < len; k++) {
                              if (charArr[i + k][j + k] != word[k]) {
                                  match = false;
                                  break;
                              }
                         }
                         if (match) {
                             for (int k = 0; k < len; k++) {
                                  arr[i + k][j + k] = -1; // Delete the corresponding elements of array
                                  charArr[i + k][j + k] = '0'; // and character array
                             }
                            score += len;
                            result << "Word found: " << word << " in forward diagonal starting at (" 
                            << i << ", " << j << ")" << endl;
                            found = true;
                            foundCount++;
                         }
                     }
               }

               wordCount++;
           }
           
           for (int i = 0; i < 2; i++) {
              for (int j = 0; j < 15; j++) {
                  if(arr[i][j] >= 0 && arr[i][j] <= 25) { 
                       cout<<charArr[i][j]<<"    ";
                  }
                  else cout<<"     ";
              }
               cout<<endl;
           }
           cout<<endl;
         if(foundCount > 0) {
           for (int i = 0; i < 2; i++) {
              for (int j = 0; j < 15; j++) {
                  if(arr[i][j] == -1) { 
                       arr[i][j] = GetAlphabet();
                       charArr[i][j] = char(arr[i][j] + 97) ;
                  }
              }
           }
         }
           
         }while(foundCount != 0);
                	
        } 
        // for checking for the first time filling the two rows with random alphabets...
        
        
        
    //for checking the words except for the first time, (giving random alphabets to first two rows)..
        if(row == 465 && col == 10 && iterate == 0) {    
    
           
    
           for(int index = 0 ; index < 369646 ; index++) { 
    
               word = dictionary[index]; 
               len = word.length() ;

               found = false;
               ROWS = 8 , COLS = 15 ;
               //  rows from left to right
               for (int i = 0; i < ROWS && !found; i++) {
                   for (int start = 0 ; start <= COLS - len  && !found ; start++) {
                       bool match = true;
                       for (int k = 0; k < len; k++) {
                           if (charArr[i][start + k] != word[k]) {
                              match = false;
                              break;
                           }
                       }
                       if (match) {
                           for(int j = start + (len-1) ; j >= start ; j--) {
                               arr[i][j] = -1 ;  // for removing the characters that are forming words 
                               charArr[i][j] = '0' ; // /////    // important 
                           }
                           score += len ;
                       }
                       if (match) {
                          result << "Word found: " << word << " in row " << i << endl;
                          found = true;
                          foundCount++;
                       }
                   }
               }
        

               // columns from top to bottom
               for (int j = 0; j < COLS && !found; j++) {
                   for (int start = 0; start <= ROWS - len && !found; start++) {
                       match = true;
                       for (int k = 0; k < len; k++) {
                           if (charArr[start + k][j] != word[k]) {

                              match = false;
                              break;
                           }
                       }
                       if (match) {
                           for(int i = start + (len-1) ; i >= start ; i--) {
                               arr[i][j] = -1 ;  // for removing the characters that are forming words 
                               charArr[i][j] = '0' ; // /////    // important 
                           }
                           score += len ; 
                       }
                       
                       
                       if (match) {
                          result << "Word found: " << word << " in column " << j << endl;
                          found = true;
                          foundCount++;
                       }
                   }
               }
               
               //for left to right diagonals..
               for (int i = 0; i <= ROWS - len && !found; i++) {
                    for (int j = 0; j <= COLS - len && !found; j++) {
                        bool match = true;
                        for (int k = 0; k < len; k++) {
                             if (charArr[i + k][j + k] != word[k]) {
                                match = false;
                                break;
                             }
                        }
                        if (match) {
                           for (int k = 0; k < len; k++) {
                               arr[i + k][j + k] = -1;  // Delete the corresponding elements of array
                               charArr[i + k][j + k] = '0'; // and char array
                           }
                          score += len;
                          result << "Word found: " << word << " in forward diagonal starting at (" 
                            << i << ", " << j << ")" << endl;
                          found = true;
                          foundCount++;
                       }
                    }
               }
               
               wordCount++;
           }
           
           for (int i = 0; i < 8; i++) {
              for (int j = 0; j < 15; j++) {
                  if(arr[i][j] >= 0 && arr[i][j] <= 25) { 
                       cout<<charArr[i][j]<<"    ";
                  }
                  else cout<<"     ";
              }
               cout<<endl;
           }
                	
           cout << "Total words processed: " << wordCount << endl;
           cout << "Total words found: " << foundCount << endl;
           iterate += 1;
        }
    //for checking the words except for the first time, (giving random alphabets to first two rows)..

    
        srand(L);
        new_ball = GetAlphabet();
        DrawAlphabet((alphabets)new_ball ,  row , col , awidth, aheight);
        	        
	DrawString(20, height - 20, width, height + 5, "Score: " + Num2Str(score), colors[BLUE_VIOLET]);
	DrawString(250, height - 20, width, height + 5, "Ahmed Raza" , colors[GREEN]);
	DrawString(500, height - 20, width, height + 5, "24I-0026" , colors[BROWN]);
	DrawString(715, height - 25, width, height,
		"Time Left:" + Num2Str(int(time1)) + " secs", colors[RED]);

		  
		  }
	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
	
	
}

/* Function sets canvas size (drawing area) in pixels...

*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting

*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {
        
        if(row == 465 && col == 10) {
	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
        {
		if (state == GLUT_UP)
		{
		     click++ ;         //increment in click every time user clicks somewhere on screen
		  if (click >= 2) {
		     up = true ;
		     X = x ;           
		     Y = 660 - y ;     //660 - y, as y axis considered from top
		     
		     if(Y >= 10) {
		     slope = (Y - col - 30 )/(X - row - 30 ) ;     
		    }
		     else if(Y < 10) {
		     slope = (Y - col )/(X - row ) ;              //slope calculation
		    }
		     dx = speed / sqrt(1 + slope * slope);       //calculating change in x 
		     
		     
		     if(slope < 0) {      //when click is made below the ball's y position
                         dx = - dx ;      //reverse the sign of slope
		    }
		     else if(slope > 0) {  //when user clicks above the ball anywhere
                         dx = dx ;
		    }
		     dy = (slope * dx ) ;  //change in y in terms of x to show a linear motion...
                  } 
	       }
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
	     
	}
	glutPostRedisplay();
	}
}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard

* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the

* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {

        if(click >= 1 && time1 >= 0.0) {
        last = click ;
        iterate += 0.1 ;
        ++zon ;       //for drawing the balls if their array element is not zero
        time1 -= 0.1;
        }

        if(up == true) {
            if (row + dx >= 870 || row + dx <= 0) {
                  dx = -dx; // Reverse horizontal direction if it hits the boundary
            }
            if(col < 440 && col > 0 && row > 0 && row < 930) {
                  col += dy ;
                  row += dx ; 
            }

            //for making the ball attach with the rows vertically, in exact columns
             
             
             if(col >= 440) {
                  rem = (int(row) - 10) % 60 ;
                  DIV = (int(row) - 10) / 60 ;
                  low = DIV ;                         
                  high = DIV + 1;
                
             if(rem >=30) { 
                  row = (high * 60) + 10 ;
                  aligned_position = ( high ) ;
             }
             else if(rem <30) { 
                  row = (low * 60) + 10 ;
                  aligned_position = ( low ) ;
             }
             up = false ;
             bool o = true;
             for(int i = 7 ; i >= 0 && o == true ; i-- ) {
                 if(arr[i][aligned_position] != -1) {
                    o = false;
                    arr[i + 1][aligned_position] = new_ball;
                 }
                 if(i == 0 &&  arr[i][aligned_position] == -1) {
                    arr[i][aligned_position] = new_ball;
                 } 
             }

             row = 465 , col = 10 ;
             iterate = -1 ;
             L+=3 ;    
             }
            //for making the ball attach with the rows vertically, in exact columns             
        }
	glutPostRedisplay();
	glutTimerFunc(1000.0/FPS, Timer, 0);
} 

/*

* our gateway main function
* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;
		
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT , 2 , 2048);
	Mix_Music* bgMusic = Mix_LoadMUS("music.mp3");
	Mix_PlayMusic( bgMusic, -1);
	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets
	
	


	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();
	return 1;
}
#endif /* */
