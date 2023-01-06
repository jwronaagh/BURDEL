#include <SFML/Graphics.hpp> // biblioteka graficzna
#include <time.h> 
#include <string.h>

/* Co doda�?
* licznik czasu// w sobot� z tym podzia�am
* fajny komunikat o przegranej// dodam w sobote
* menu?
*
*/


using namespace std;
using namespace sf; // przedrostek na pocz�tky by nie dodawa� go do ka�dej funkjci SFML

int grid[12][12]; // 12 wierszy 12 kolumn siatka odkryta // da�em poza maina �eby dzia�a�o w funkcjach
int sgrid[12][12]; // 12 wierszy 12 kolumn siatka zakryta

void odkryjPuste(int x, int y) // NIE mam pojecia czemu nie dzia�a

{
    // Sprawdzam, czy pole jest ju� odkryte lub czy jest bomb�
    if (sgrid[x][y] == 9 || sgrid[x][y] == 0) return;

    // Odkrywamy pole
    sgrid[x][y] = grid[x][y];

    // Je�li pole jest puste, to odkrywam r�wnie� s�siednie pola
    if (grid[x][y] == 0)
    {
        if (x > 1) odkryjPuste(x - 1, y); // pole po lewej
        if (x < 12) odkryjPuste(x + 1, y); // pole po prawej
        if (y > 1) odkryjPuste(x, y - 1); // pole u g�ry
        if (y < 12) odkryjPuste(x, y + 1); // pole na dole
    }
}
int main()
{

    srand(time(0)); // kij wie co, jaki� czas potrzebny do losowania

    ///tworzenie planszy///

    RenderWindow window(VideoMode(500, 500), "Saper"); // tworzymy okno gry

    int w = 32; // pole do popisu
  
    ///import tekstur///

    Texture t;
    t.loadFromFile("images/tiles.jpg");
    Sprite s(t);
   
   

    ///losowanie miejsc bomb////

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            sgrid[i][j] = 10;
            if (rand() % 7 == 0) grid[i][j] = 9; // tu mo�na wybra� jak du�o chce si� mie� bomb
            else grid[i][j] = 0;
        }
    }


    ///numeracja kafelk�w zale�nych od po�o�enia bomb///

    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {

            int n = 0;
            if (grid[i][j] == 9) continue;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;
            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;
            grid[i][j] = n;

        }
    }
   
    //Przycisk
    
    RectangleShape resetButton(Vector2f(100, 50));
    resetButton.setFillColor(Color::Red);
    resetButton.setPosition(400, 0); // Ustawiam pozycj� przycisku na ekranie

    ///otwieranie okna gry///
   
    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        int x = pos.x / w;
        int y = pos.y / w;
        bool mbleft = false;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    sgrid[x][y] = grid[x][y];
                    mbleft = true;

                }
                else if (e.key.code == Mouse::Right) sgrid[x][y] = 11;

        }

        window.clear(Color::White);
        for (int i = 1; i <= 10; i++) {

            for (int j = 1; j <= 10; j++) {

                if (mbleft && sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];

                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                window.draw(s);
            }

        }
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            // Pobieram pozycj� kursora myszy na ekranie
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // Sprawdzam, czy kursor myszy znajduje si� nad obszarem przycisku
            if (resetButton.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                // Je�li tak, to resetuje gr�
                for (int i = 1; i <= 10; i++) {
                    for (int j = 1; j <= 10; j++) {
                        sgrid[i][j] = 10;
                    }
                }

                // Losuje nowe po�o�enie bomb
                for (int i = 1; i <= 10; i++) {
                    for (int j = 1; j <= 10; j++) {
                        if (rand() % 7 == 0) grid[i][j] = 9; // tu mo�na wybra� jak du�o chce si� mie� bomb
                        else grid[i][j] = 0;
                    }
                }
                for (int i = 1; i <= 10; i++) {
                    for (int j = 1; j <= 10; j++) {

                        int n = 0;
                        if (grid[i][j] == 9) continue;
                        if (grid[i + 1][j] == 9) n++;
                        if (grid[i][j + 1] == 9) n++;
                        if (grid[i - 1][j] == 9) n++;
                        if (grid[i][j - 1] == 9) n++;
                        if (grid[i + 1][j + 1] == 9) n++;
                        if (grid[i - 1][j - 1] == 9) n++;
                        if (grid[i - 1][j + 1] == 9) n++;
                        if (grid[i + 1][j - 1] == 9) n++;
                        grid[i][j] = n;

                    }
                }
                mbleft = false;
            }
        }
        // Rysuj� przycisk na ekranie
        window.draw(resetButton);
        window.display();
    }
    return 0;
}