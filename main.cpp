#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <windows.h>
#include <iostream>
#include <cmath>
using namespace std;

// Change these values according to your screen size.
const float scale = 1.25;



int rows = 36*scale;
int cols = 64*scale;

const float windowWidth = 1280*scale;
const float windowHeight = 720*scale;

const float cellWidth = windowWidth / cols;
const float cellHeight = windowHeight / rows;

int disp=0;




class Cell
{
public:
    sf::RectangleShape *cell = new sf::RectangleShape(sf::Vector2f(cellWidth, cellHeight));
    bool isalive_p = false;
    bool isalive_n = false;


    void createCell(float cellWidth, float cellHeight, int posX, int posY, int rn, int cn)
    {
        
        int rowNo = rn;
        int colNo = cn;
        cell->setPosition(posX, posY);
        if (!isalive_n)
        {
            cell->setFillColor(sf::Color::White);
        }

        cell->setOutlineThickness(1.0f);
        cell->setOutlineColor(sf::Color::Black);
       
    }

    void updatecell(){
        if(isalive_n==true){
            cell->setFillColor(sf::Color::Green);
        }
        if(isalive_n==false){
            cell->setFillColor(sf::Color::White);
        }
    }


    void DrawCell(sf::RenderWindow &window)
    {
        updatecell();
        window.draw(*cell);
        isalive_p=isalive_n;
        
    }



};





class Grid
{
public:
    Cell cells[200][200];
    int rows;
    int cols;


    void initializeGrid(int row, int col, sf::RenderWindow &window)
    {
        rows = row;
        cols = col;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {

                if (cells[i][j].isalive_n==false)
                {
                    cells[i][j].cell->setFillColor(sf::Color::White);
                
                }
                cells[i][j].createCell(cellWidth, cellHeight, j * cellWidth, i * cellHeight, i, j);
                cells[i][j].DrawCell(window);
            }
        }
        window.display();
    }


    // To display the final grid
    void drawGrid(sf::RenderWindow &window)
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cells[i][j].DrawCell(window);
            }
        }
    }



    //To clear all inputs
    void clearAll()
    {   disp=0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)

            {
                cells[i][j].isalive_p = false;
                cells[i][j].isalive_n = false;   
                cells[i][j].cell->setFillColor(sf::Color::White);
            }
        }
    }



    //To update each cells and decide whether to keep them alive or dead in next generation.
    void updatecell(){
        sf::sleep(sf::seconds(0.1f));
           for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {   
                int alive_negibors = 0;

                if(i-1>=0 && j-1>=0 && cells[i-1][j-1].isalive_p){
                    alive_negibors++;
                }
                if(i-1>=0 && cells[i-1][j].isalive_p){
                    alive_negibors++;
                }
                if(i-1>=0 && j+1<cols && cells[i-1][j+1].isalive_p){
                    alive_negibors++;
                }
                if(j+1<cols && cells[i][j+1].isalive_p){
                    alive_negibors++;
                }
                if(j+1<cols && cells[i][j-1].isalive_p){
                    alive_negibors++;
                }
                if(i+1<rows && j+1<cols && cells[i+1][j+1].isalive_p){
                    alive_negibors++;
                }
                if(i+1<rows && cells[i+1][j].isalive_p){
                    alive_negibors++;
                }
                if(i+1<rows && j-1>=0 && cells[i+1][j-1].isalive_p){
                    alive_negibors++;
            }

            

           

           

            if(alive_negibors==3){
                cells[i][j].isalive_n = true;
            }

            if(cells[i][j].isalive_p && (alive_negibors < 2 || alive_negibors > 3)){
                cells[i][j].isalive_n = false;
            }

        }

        

    }}



    void displayGrid(sf::RenderWindow &window)
    {
        // For resetting the app
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            clearAll();
        }

        drawGrid(window);
        
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int rowUnderMouse = mousePos.y / cellHeight;
        int columnUnderMouse = mousePos.x / cellWidth;

        // For hovering effect
        // if (rowUnderMouse >= 0 && rowUnderMouse < rows && columnUnderMouse >= 0 && columnUnderMouse < cols && !cells[rowUnderMouse][columnUnderMouse].isalive_n)
        // {
        //     cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::Blue);
        // }


        // For mouse Clicks
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && rowUnderMouse >= 0 && rowUnderMouse < rows && columnUnderMouse >= 0 && columnUnderMouse < cols)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // For alive cells
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&!cells[rowUnderMouse][columnUnderMouse].isalive_n)
            {
                cells[rowUnderMouse][columnUnderMouse].isalive_n = true;
           
            }
            // For removing alive cells
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && cells[rowUnderMouse][columnUnderMouse].isalive_n)
            {
                cells[rowUnderMouse][columnUnderMouse].isalive_n = false;
            }
        }

        // Draw Grid after various events
        drawGrid(window);
       // cells[rowUnderMouse][columnUnderMouse].cell->setFillColor(sf::Color::White);
    }

};


int main(){
    
    sf::RenderWindow window;
    window.create(sf::VideoMode(windowWidth, windowHeight), "Conway's Game Of Life", sf::Style::Titlebar | sf::Style::Close);

    Grid g1;
    g1.initializeGrid(rows, cols, window);

    while (window.isOpen())
    {   
        
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || disp==1)
        {   disp=1;
            g1.updatecell();
        }



        window.clear(sf::Color(240, 240, 240)); // Light grey background, close to white
        g1.displayGrid(window);

        window.display();
        
    }

return 0;
  
    
}