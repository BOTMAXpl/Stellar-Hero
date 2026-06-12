#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "gracz.h"
#include "pocisk.h"
#include "wrog.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "pocisk_wroga.h"
#include "dzialko.h"
#include "laser_wroga.h"
#include "portal.h"
using namespace std;

void zaladuj_wrogow(int poziom, vector<obiekt*>& lista) {
    if (poziom == 1) {
        lista.push_back(new wrog(1000, 300));
        lista.push_back(new wrog(1400, 500));
        lista.push_back(new dzialko(200, 200));
        lista.push_back(new dzialko(1600, 800));
    }
    else if (poziom == 2) {
        lista.push_back(new wrog(800, 200));
        lista.push_back(new wrog(1000, 400));
        lista.push_back(new wrog(1200, 600));
        lista.push_back(new wrog(1400, 800));
        lista.push_back(new dzialko(100, 100));
        lista.push_back(new dzialko(1800, 100));
    }
    else if (poziom == 3) {
        lista.push_back(new wrog(700, 300));
        lista.push_back(new wrog(800, 500));
        lista.push_back(new wrog(900, 700));
        lista.push_back(new wrog(1200, 500));
        lista.push_back(new dzialko(100, 100));
        lista.push_back(new dzialko(1800, 100));
        lista.push_back(new dzialko(100, 900));
        lista.push_back(new dzialko(1800, 900));
    }
}

int main() {
    sf::RenderWindow okno(sf::VideoMode(1920, 1080), "Stellar-Hero", sf::Style::Fullscreen);
    okno.setFramerateLimit(60);
    srand(time(NULL));

    sf::Font czcionka;
    if (!czcionka.loadFromFile("MAIAN.ttf")) {
    }

    int stan_gry = 0;
    int wybrana_opcja = 0;

    int aktualny_poziom = 1;
    float czas_napisu = 0;
    bool portal_otwarty = false;

    vector<string> napisy_glowne = {"Start", "Informacje", "Wyjscie"};
    vector<string> napisy_tryb = {"Classic", "Endless"};
    vector<string> napisy_gracze = {"1 Gracz", "2 Graczy"};

    sf::Texture tekstura_mapy;
    tekstura_mapy.loadFromFile("mapa1.png");
    sf::Sprite tlo_poziomu;
    tlo_poziomu.setTexture(tekstura_mapy);

    vector<obiekt*> lista_obiektow;
    gracz* glowny_gracz = new gracz();
    lista_obiektow.push_back(glowny_gracz);

    lista_obiektow.push_back(new wrog(1000, 300));
    lista_obiektow.push_back(new wrog(1400, 500));
    lista_obiektow.push_back(new wrog(1100, 800));
    lista_obiektow.push_back(new dzialko(200, 200));
    lista_obiektow.push_back(new dzialko(1600, 800));

    sf::Clock zegar;
    bool wcisnieta_spacja = false;

    while (okno.isOpen()) {
        float dt = zegar.restart().asSeconds();
        sf::Event zdarzenie;

        while (okno.pollEvent(zdarzenie)) {
            if (zdarzenie.type == sf::Event::Closed)
                okno.close();

            if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape) {
                if (stan_gry == 3) stan_gry = 0;
                else okno.close();
            }

            if (stan_gry != 3 && zdarzenie.type == sf::Event::KeyPressed) {

                if (zdarzenie.key.code == sf::Keyboard::Up) {
                    wybrana_opcja--;
                    if (wybrana_opcja < 0) wybrana_opcja = 0;
                }
                if (zdarzenie.key.code == sf::Keyboard::Down) {
                    wybrana_opcja++;
                    if (stan_gry == 0 && wybrana_opcja > 2) wybrana_opcja = 2;
                    if ((stan_gry == 1 || stan_gry == 2) && wybrana_opcja > 1) wybrana_opcja = 1;
                }

                if (zdarzenie.key.code == sf::Keyboard::Enter) {
                    if (stan_gry == 0) {
                        if (wybrana_opcja == 0) stan_gry = 1;
                        else if (wybrana_opcja == 1) stan_gry = 4;
                        else if (wybrana_opcja == 2) okno.close();
                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 1) {
                        if (wybrana_opcja == 0) stan_gry = 2;
                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 2) {
                        if (wybrana_opcja == 0){
                            stan_gry = 3;

                            aktualny_poziom = 1;
                            portal_otwarty = false;
                            czas_napisu = 3.0f;
                            glowny_gracz->zresetuj();

                            for (size_t j = 1; j < lista_obiektow.size(); j++) delete lista_obiektow[j];

                            lista_obiektow.resize(1);
                            zaladuj_wrogow(aktualny_poziom, lista_obiektow);
                        }
                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 4) {
                        stan_gry = 0;
                    }
                }
            }
        }

        if (stan_gry == 3) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (wcisnieta_spacja == false) {

                    sf::Vector2f start = glowny_gracz->daj_pozycje();
                    sf::Vector2f kier = glowny_gracz->wez_kierunek();

                    start.x += 26;
                    start.y += 20;

                    start.x += kier.x * 70;
                    start.y += kier.y * 70;

                    pocisk* nowy_laser = new pocisk(start, kier);
                    lista_obiektow.push_back(nowy_laser);

                    wcisnieta_spacja = true;
                }
            } else {
                wcisnieta_spacja = false;
            }

            int zywych_wrogow = 0;

            for (int i = 0; i < lista_obiektow.size(); i++) {
                lista_obiektow[i]->licz(dt);

                wrog* w = dynamic_cast<wrog*>(lista_obiektow[i]);
                if (w != nullptr){
                    zywych_wrogow++;
                    if(w->gotowy_do_strzalu()) {
                        sf::Vector2f start = w->daj_srodek();
                        sf::Vector2f cel = glowny_gracz->daj_pozycje();

                        cel.x += 20;
                        cel.y += 20;

                        sf::Vector2f kierunek_strzalu = cel - start;
                        float dlugosc = std::sqrt(kierunek_strzalu.x * kierunek_strzalu.x + kierunek_strzalu.y * kierunek_strzalu.y);

                        if (dlugosc != 0) {
                            kierunek_strzalu.x /= dlugosc;
                            kierunek_strzalu.y /= dlugosc;

                            lista_obiektow.push_back(new pocisk_wroga(start, kierunek_strzalu));
                            w->zresetuj_strzal();
                            }
                    }
                }
                dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[i]);
                if (dz != nullptr){
                    zywych_wrogow++;
                    if (dz->gotowy_do_strzalu()) {
                        sf::Vector2f start = dz->daj_srodek();
                        sf::Vector2f cel = glowny_gracz->daj_pozycje();
                        cel.x += 20;
                        cel.y += 20;

                        sf::Vector2f kier_strzalu = cel - start;
                        float dl = std::sqrt(kier_strzalu.x * kier_strzalu.x + kier_strzalu.y * kier_strzalu.y);

                        if (dl != 0) {
                            kier_strzalu.x /= dl; kier_strzalu.y /= dl;
                            lista_obiektow.push_back(new laser_wroga(start, kier_strzalu));
                            dz->zresetuj_strzal();
                        }
                    }
                }
            }

            if(zywych_wrogow == 0 && portal_otwarty == false){

                lista_obiektow.push_back(new portal(960, 540));
                portal_otwarty = true;
            }

            for (int i = 0; i < lista_obiektow.size(); i++) {
                if (lista_obiektow[i]->czy_usunac() == true) {
                    delete lista_obiektow[i];
                    lista_obiektow.erase(lista_obiektow.begin() + i);
                    i--;
                }
            }

            for (int i = 0; i < lista_obiektow.size(); i++) {

                portal* prt = dynamic_cast<portal*>(lista_obiektow[i]);
                if (prt != nullptr) {
                    if (prt->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) {
                        aktualny_poziom++;
                        if (aktualny_poziom > 3) aktualny_poziom = 1;

                        for (size_t j = 1; j < lista_obiektow.size(); j++) delete lista_obiektow[j];
                        lista_obiektow.resize(1);

                        portal_otwarty = false;
                        czas_napisu = 3.0f;
                        glowny_gracz->zresetuj();
                        zaladuj_wrogow(aktualny_poziom, lista_obiektow);
                        break;
                    }
                }

                pocisk* p = dynamic_cast<pocisk*>(lista_obiektow[i]);
                if (p != nullptr) {
                    for (int j = 0; j < lista_obiektow.size(); j++) {
                        wrog* w = dynamic_cast<wrog*>(lista_obiektow[j]);
                        if (w != nullptr) {
                            if (p->czy_trafia(w->daj_srodek())) {
                                w->zabij();
                            }
                        }
                        dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[j]);
                        if (dz != nullptr) {
                            if (p->czy_trafia(dz->daj_srodek())) {
                                dz->zabij();
                            }
                        }
                    }
                }

                pocisk_wroga* pw = dynamic_cast<pocisk_wroga*>(lista_obiektow[i]);
                if (pw != nullptr) {

                    if (pw->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) {

                        glowny_gracz->dostan_obrazenia();
                        pw->zniszcz();
                        glowny_gracz->ile_zyc();
                        pw->zniszcz();
                        if(glowny_gracz->ile_zyc() <= 0){
                            stan_gry = 0;
                        }
                    }
                }

                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[i]);
                if (lw != nullptr) {
                    sf::Vector2f srodek_gracza = glowny_gracz->daj_pozycje();
                    srodek_gracza.x += 20;
                    srodek_gracza.y += 20;

                    if (lw->czy_trafia(srodek_gracza)) {
                        glowny_gracz->dostan_obrazenia();

                        if(glowny_gracz->ile_zyc() <= 0){
                            stan_gry = 0;
                        }
                    }
                }
            }

        }

        okno.clear(sf::Color(30, 30, 30));

        if (stan_gry == 3) {
            okno.draw(tlo_poziomu);
            for (int i = 0; i < lista_obiektow.size(); i++) {
                lista_obiektow[i]->rysuj(okno);
            }
        }

        if (czas_napisu > 0) {
            czas_napisu -= dt;
            string t = "POZIOM " + to_string(aktualny_poziom);
            sf::Text napis_poziomu(t, czcionka, 150);
            napis_poziomu.setPosition(700, 400);
            napis_poziomu.setFillColor(sf::Color(255, 255, 255, 200));
            okno.draw(napis_poziomu);
        }

        else {
            vector<string> do_narysowania;
            if (stan_gry == 0) do_narysowania = napisy_glowne;
            else if (stan_gry == 1) do_narysowania = napisy_tryb;
            else if (stan_gry == 2) do_narysowania = napisy_gracze;

            if (stan_gry == 4) {
                sf::Text info("To jest projekt z cpp.\nWcisnij Enter zeby wrocic.", czcionka, 50);
                info.setPosition(400, 400);
                okno.draw(info);
            }
            else {
                for (int i = 0; i < do_narysowania.size(); i++) {
                    sf::Text tekst(do_narysowania[i], czcionka, 80);
                    tekst.setPosition(800, 300 + (i * 120));

                    if (i == wybrana_opcja) {
                        tekst.setFillColor(sf::Color::Yellow);
                    } else {
                        tekst.setFillColor(sf::Color::White);
                    }
                    okno.draw(tekst);
                }
            }
        }
        okno.display();
    }

    for (int i = 0; i < lista_obiektow.size(); i++) {
        delete lista_obiektow[i];
    }

    return 0;
}
