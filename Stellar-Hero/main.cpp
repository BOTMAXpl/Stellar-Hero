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
#include "atak_obszarowy.h"
#include "ostrzezenie.h"
#include <fstream>
#include "boss.h"
#include <SFML/Audio.hpp>
#include "portal_koncowy.h"
using namespace std;

// tu jest wczytywanie przeicwnikow
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
    else if (poziom == 4) {
        lista.push_back(new boss(960, 200));
    }
}

int main() {
    // tworzenie okna
    sf::RenderWindow okno(sf::VideoMode(1920, 1080), "Stellar-Hero", sf::Style::Fullscreen);
    okno.setFramerateLimit(60);
    srand(time(NULL));
    //muzyka
    sf::Music muzyka_tla;
    if (muzyka_tla.openFromFile("SuperHero_original.ogg")) {
        muzyka_tla.setLoop(true);
        muzyka_tla.setVolume(30.f);
        muzyka_tla.play();
    }
    sf::Font czcionka;
    if (!czcionka.loadFromFile("MAIAN.ttf")) {
    }

    // licznik smierci
    int suma_smierci = 0;
    ifstream plik_odczyt("smierci.txt");
    if (plik_odczyt.is_open()) {
        plik_odczyt >> suma_smierci;
        plik_odczyt.close();
    }

    int stan_gry = 0;
    int wybrana_opcja = 0;
    int wybrany_tryb = 0;

    float czas_gry = 0.0f;
    float czas_do_kolejnego_wroga = 2.0f;

    int aktualny_poziom = 1;
    float czas_napisu = 0;
    bool portal_otwarty = false;

    vector<string> napisy_glowne = {"Start", "Informacje", "Wyjscie"};
    vector<string> napisy_tryb = {"Classic", "Endless"};
    vector<string> napisy_gracze = {"1 Gracz", "2 Graczy"};

    // ladowanie plikow
    sf::Texture tekstura_mapy;
    tekstura_mapy.loadFromFile("mapa1.png");
    sf::Sprite tlo_poziomu;
    tlo_poziomu.setTexture(tekstura_mapy);

    sf::Texture tex_serce_puste;
    tex_serce_puste.loadFromFile("serce1.png");
    tex_serce_puste.setSmooth(false);

    sf::Texture tex_serce_pelne;
    tex_serce_pelne.loadFromFile("serce2.png");
    tex_serce_pelne.setSmooth(false);

    sf::Sprite sprite_serce;
    sprite_serce.setScale(4.0f, 4.0f);

    // lista obiektóW
    vector<obiekt*> lista_obiektow;
    gracz* glowny_gracz = new gracz(1);
    gracz* gracz2 = nullptr;
    lista_obiektow.push_back(glowny_gracz);


    sf::Clock zegar;
    bool wcisnieta_spacja = false;
    bool wcisniety_enter = false;

    // głowna petla
    while (okno.isOpen()) {
        float dt = zegar.restart().asSeconds();
        sf::Event zdarzenie;

        // zamkniecie okna
        while (okno.pollEvent(zdarzenie)) {
            if (zdarzenie.type == sf::Event::Closed)
                okno.close();

            if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape) {
                if (stan_gry == 3 || stan_gry == 5 || stan_gry == 6) stan_gry = 0;
                else okno.close();
            }
            // menu
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
                        if (wybrana_opcja == 0) { stan_gry = 2; wybrany_tryb = 0; }
                        else if (wybrana_opcja == 1) { stan_gry = 2; wybrany_tryb = 1; }
                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 2) {
                        stan_gry = 3;
                        aktualny_poziom = 1;
                        portal_otwarty = false;

                        czas_gry = 0.0f;
                        czas_do_kolejnego_wroga = 2.0f;

                        // usuwanie pamieci po menu
                        for (int j = 0; j < lista_obiektow.size(); j++) delete lista_obiektow[j];
                        lista_obiektow.clear();

                        glowny_gracz = new gracz(1);
                        lista_obiektow.push_back(glowny_gracz);

                        if (wybrana_opcja == 1) {
                            gracz2 = new gracz(2);
                            lista_obiektow.push_back(gracz2);
                        } else {
                            gracz2 = nullptr;
                        }

                        if (wybrany_tryb == 0) {
                            czas_napisu = 3.0f;
                            zaladuj_wrogow(aktualny_poziom, lista_obiektow);
                        } else {
                            czas_napisu = 0.0f;
                        }

                        wybrana_opcja = 0;
                    }
                    else if (stan_gry == 4) {
                        stan_gry = 0;
                    }
                    else if (stan_gry == 5 || stan_gry == 6) {
                        stan_gry = 0;
                    }
                }
            }
        }

        if (stan_gry == 3) {
            // endless
            if (wybrany_tryb == 1) {
                czas_gry += dt;
                czas_do_kolejnego_wroga -= dt;

                if (czas_do_kolejnego_wroga <= 0) {
                    int ile_wrogow = 1 + (int)(std::pow(1.5, czas_gry / 30.0f) - 1);

                    for (int k = 0; k < ile_wrogow; k++) {
                        float rand_x = 100 + rand() % 1700;
                        float rand_y = 100 + rand() % 800;
                        int losowy_typ = rand() % 2;
                        lista_obiektow.push_back(new ostrzezenie(rand_x + 40, rand_y + 40, losowy_typ));
                    }
                    float nowy_czas = 2.0f - (czas_gry / 100.0f);
                    if (nowy_czas < 0.6f) nowy_czas = 0.6f;
                    czas_do_kolejnego_wroga = nowy_czas + ((rand() % 10) / 10.0f);
                }
            }

            // strzelanie gracza 1
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
                if (glowny_gracz != nullptr && glowny_gracz->gotowy_obszarowy()) {
                    sf::Vector2f srodek = glowny_gracz->daj_pozycje();
                    srodek.x += 26;
                    srodek.y += 20;
                    lista_obiektow.insert(lista_obiektow.begin(), new atak_obszarowy(srodek));
                    glowny_gracz->uzyj_obszarowego();
                }
            }

            // strzelanie gracza 2
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                if (wcisniety_enter == false && gracz2 != nullptr) {
                    sf::Vector2f start = gracz2->daj_pozycje();
                    sf::Vector2f kier = gracz2->wez_kierunek();
                    start.x += 26;
                    start.y += 20;
                    start.x += kier.x * 70;
                    start.y += kier.y * 70;
                    pocisk* nowy_laser = new pocisk(start, kier);
                    lista_obiektow.push_back(nowy_laser);
                    wcisniety_enter = true;
                }
            } else {
                wcisniety_enter = false;
            }

            int zywych_wrogow = 0;

            // ruch i wszystko zwiazane z czasem
            for (int i = 0; i < lista_obiektow.size(); i++) {
                lista_obiektow[i]->licz(dt);

                sf::Vector2f start_wroga;
                bool czy_strzela = false;

                wrog* w = dynamic_cast<wrog*>(lista_obiektow[i]);
                if (w != nullptr) {
                    zywych_wrogow++;
                    if(w->gotowy_do_strzalu()) {
                        start_wroga = w->daj_srodek();
                        czy_strzela = true;
                        w->zresetuj_strzal();
                    }
                }

                dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[i]);
                if (dz != nullptr) {
                    zywych_wrogow++;
                    if (dz->gotowy_do_strzalu()) {
                        start_wroga = dz->daj_srodek();
                        czy_strzela = true;
                        dz->zresetuj_strzal();
                    }
                }

                boss* b = dynamic_cast<boss*>(lista_obiektow[i]);
                if (b != nullptr) {
                    zywych_wrogow++;
                    if (b->gotowy_do_strzalu()) {
                        sf::Vector2f srodek_bossa = b->daj_srodek();
                        int faza = b->daj_faze();

                        if (faza == 0) {
                            for (int kat = 0; kat < 360; kat += 30) {
                                float rad = kat * 3.14159f / 180.0f;
                                sf::Vector2f kierunek(std::cos(rad), std::sin(rad));
                                lista_obiektow.push_back(new pocisk_wroga(srodek_bossa, kierunek));
                            }
                        } else {
                            sf::Vector2f cel = glowny_gracz->daj_pozycje();
                            if (gracz2 != nullptr) {
                                sf::Vector2f pos2 = gracz2->daj_pozycje();
                                float dystans1 = (cel.x - srodek_bossa.x) * (cel.x - srodek_bossa.x) + (cel.y - srodek_bossa.y) * (cel.y - srodek_bossa.y);
                                float dystans2 = (pos2.x - srodek_bossa.x) * (pos2.x - srodek_bossa.x) + (pos2.y - srodek_bossa.y) * (pos2.y - srodek_bossa.y);
                                if (dystans2 < dystans1) cel = pos2;
                            }
                            cel.x += 20;
                            cel.y += 20;

                            sf::Vector2f kierunek_strzalu = cel - srodek_bossa;
                            float dlugosc = std::sqrt(kierunek_strzalu.x * kierunek_strzalu.x + kierunek_strzalu.y * kierunek_strzalu.y);

                            if (dlugosc != 0) {
                                kierunek_strzalu.x /= dlugosc;
                                kierunek_strzalu.y /= dlugosc;
                                lista_obiektow.push_back(new laser_wroga(srodek_bossa, kierunek_strzalu));
                            }
                        }
                        b->zresetuj_strzal();
                    }
                }

                // szkanie celu dla przeciwników
                if (czy_strzela) {
                    sf::Vector2f cel = glowny_gracz->daj_pozycje();

                    if (gracz2 != nullptr) {
                        sf::Vector2f pos2 = gracz2->daj_pozycje();
                        float dystans1 = (cel.x - start_wroga.x) * (cel.x - start_wroga.x) + (cel.y - start_wroga.y) * (cel.y - start_wroga.y);
                        float dystans2 = (pos2.x - start_wroga.x) * (pos2.x - start_wroga.x) + (pos2.y - start_wroga.y) * (pos2.y - start_wroga.y);
                        if (dystans2 < dystans1) cel = pos2;
                    }

                    cel.x += 20;
                    cel.y += 20;

                    sf::Vector2f kierunek_strzalu = cel - start_wroga;
                    float dlugosc = std::sqrt(kierunek_strzalu.x * kierunek_strzalu.x + kierunek_strzalu.y * kierunek_strzalu.y);

                    if (dlugosc != 0) {
                        kierunek_strzalu.x /= dlugosc;
                        kierunek_strzalu.y /= dlugosc;

                        if (w != nullptr) lista_obiektow.push_back(new pocisk_wroga(start_wroga, kierunek_strzalu));
                        if (dz != nullptr) lista_obiektow.push_back(new laser_wroga(start_wroga, kierunek_strzalu));
                    }
                }
            }

            // portal
            if(wybrany_tryb == 0 && zywych_wrogow == 0 && portal_otwarty == false){
                if (aktualny_poziom == 4) {
                    lista_obiektow.push_back(new portal_koncowy(960, 540));
                } else {
                    lista_obiektow.push_back(new portal(960, 540));
                }
                portal_otwarty = true;
            }

            // usuwanie
            for (int i = 0; i < lista_obiektow.size(); i++) {
                if (lista_obiektow[i]->czy_usunac() == true) {

                    ostrzezenie* ost = dynamic_cast<ostrzezenie*>(lista_obiektow[i]);
                    if (ost != nullptr) {
                        if (ost->daj_typ() == 0) {
                            lista_obiektow.push_back(new wrog(ost->daj_pozycje().x - 40, ost->daj_pozycje().y - 40));
                        } else {
                            lista_obiektow.push_back(new dzialko(ost->daj_pozycje().x - 40, ost->daj_pozycje().y - 40));
                        }
                    }

                    delete lista_obiektow[i];
                    lista_obiektow.erase(lista_obiektow.begin() + i);
                    i--;
                }
            }

            // sprawda kolizje
            for (int i = 0; i < lista_obiektow.size(); i++) {
                portal_koncowy* pk = dynamic_cast<portal_koncowy*>(lista_obiektow[i]);
                if (pk != nullptr) {
                    bool pk_hit = false;
                    if (glowny_gracz != nullptr && pk->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) pk_hit = true;
                    if (gracz2 != nullptr && pk->daj_kolizje().intersects(gracz2->daj_kolizje())) pk_hit = true;

                    if (pk_hit) {
                        stan_gry = 6;
                        for (int j = 0; j < lista_obiektow.size(); j++) delete lista_obiektow[j];
                        lista_obiektow.clear();

                        glowny_gracz = new gracz(1);
                        gracz2 = nullptr;
                        lista_obiektow.push_back(glowny_gracz);

                        break;
                    }
                }
                portal* prt = dynamic_cast<portal*>(lista_obiektow[i]);
                if (prt != nullptr) {
                    bool prt_hit = false;
                    if (glowny_gracz != nullptr && prt->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) prt_hit = true;
                    if (gracz2 != nullptr && prt->daj_kolizje().intersects(gracz2->daj_kolizje())) prt_hit = true;

                    if (prt_hit) {
                        aktualny_poziom++;
                        if (aktualny_poziom > 4) aktualny_poziom = 1;

                        // wczytuje poziom bez usuwania graczy
                        for (int j = 0; j < lista_obiektow.size(); j++) {
                            if (dynamic_cast<gracz*>(lista_obiektow[j]) == nullptr) {
                                delete lista_obiektow[j];
                                lista_obiektow.erase(lista_obiektow.begin() + j);
                                j--;
                            }
                        }

                        portal_otwarty = false;
                        czas_napisu = 2.0f;
                        if (glowny_gracz != nullptr) glowny_gracz->zresetuj();
                        if (gracz2 != nullptr) gracz2->zresetuj();
                        zaladuj_wrogow(aktualny_poziom, lista_obiektow);
                        break;
                    }
                }

                // pocisk gracza
                pocisk* p = dynamic_cast<pocisk*>(lista_obiektow[i]);
                if (p != nullptr) {
                    for (int j = 0; j < lista_obiektow.size(); j++) {
                        wrog* w = dynamic_cast<wrog*>(lista_obiektow[j]);
                        if (w != nullptr && p->czy_trafia(w->daj_srodek())) {
                            w->zabij();
                        }
                        dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[j]);
                        if (dz != nullptr && p->czy_trafia(dz->daj_srodek())) {
                            dz->zabij();
                            // usunicie lasera przeciwnika po smrci
                            for (int k = 0; k < lista_obiektow.size(); k++) {
                                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[k]);
                                if (lw != nullptr && lw->wez_pozycje_startowa() == dz->daj_srodek()) {
                                    lw->zniszcz();
                                }
                            }
                        }
                        boss* b = dynamic_cast<boss*>(lista_obiektow[j]);
                        if (b != nullptr && p->czy_trafia(b->daj_srodek())) {
                            b->dostan_obrazenia();
                        }
                    }
                }

                // drugi atak postaci
                atak_obszarowy* aoe = dynamic_cast<atak_obszarowy*>(lista_obiektow[i]);
                if (aoe != nullptr) {
                    for (int j = 0; j < lista_obiektow.size(); j++) {
                        wrog* w = dynamic_cast<wrog*>(lista_obiektow[j]);
                        if (w != nullptr && aoe->daj_kolizje().contains(w->daj_srodek())) {
                            w->zabij();
                        }
                        dzialko* dz = dynamic_cast<dzialko*>(lista_obiektow[j]);
                        if (dz != nullptr && aoe->daj_kolizje().contains(dz->daj_srodek())) {
                            dz->zabij();
                            for (int k = 0; k < lista_obiektow.size(); k++) {
                                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[k]);
                                if (lw != nullptr && lw->wez_pozycje_startowa() == dz->daj_srodek()) {
                                    lw->zniszcz();
                                }
                            }
                        }
                        boss* b = dynamic_cast<boss*>(lista_obiektow[j]);
                        if (b != nullptr && aoe->daj_kolizje().contains(b->daj_srodek())) {
                            b->dostan_obrazenia();
                        }
                    }
                }

                // pociski wrogow
                pocisk_wroga* pw = dynamic_cast<pocisk_wroga*>(lista_obiektow[i]);
                if (pw != nullptr) {
                    bool dostal = false;
                    if (glowny_gracz != nullptr && pw->daj_kolizje().intersects(glowny_gracz->daj_kolizje())) { glowny_gracz->dostan_obrazenia(); dostal = true; }
                    if (gracz2 != nullptr && pw->daj_kolizje().intersects(gracz2->daj_kolizje())) { gracz2->dostan_obrazenia(); dostal = true; }
                    if (dostal) pw->zniszcz();
                }
                laser_wroga* lw = dynamic_cast<laser_wroga*>(lista_obiektow[i]);
                if (lw != nullptr) {
                    if (glowny_gracz != nullptr) {
                        sf::Vector2f srodek = glowny_gracz->daj_pozycje();
                        srodek.x += 20; srodek.y += 20;
                        if (lw->czy_trafia(srodek)) glowny_gracz->dostan_obrazenia();
                    }
                    if (gracz2 != nullptr) {
                        sf::Vector2f srodek = gracz2->daj_pozycje();
                        srodek.x += 20; srodek.y += 20;
                        if (lw->czy_trafia(srodek)) gracz2->dostan_obrazenia();
                    }
                }
            }

            // do smierci
            bool martwy_1 = (glowny_gracz != nullptr && glowny_gracz->ile_zyc() <= 0);
            bool martwy_2 = (gracz2 != nullptr && gracz2->ile_zyc() <= 0);

            if (martwy_1 || martwy_2) {
                // zapisywanie
                suma_smierci++;
                ofstream plik_zapis("smierci.txt");
                if (plik_zapis.is_open()) {
                    plik_zapis << suma_smierci;
                    plik_zapis.close();
                }
                stan_gry = 5;

                for (int j = 0; j < lista_obiektow.size(); j++) delete lista_obiektow[j];
                lista_obiektow.clear();

                glowny_gracz = new gracz(1);
                gracz2 = nullptr;
                lista_obiektow.push_back(glowny_gracz);

                lista_obiektow.push_back(new wrog(1000, 300));
                lista_obiektow.push_back(new wrog(1400, 500));
                lista_obiektow.push_back(new wrog(1100, 800));
                lista_obiektow.push_back(new dzialko(200, 200));
                lista_obiektow.push_back(new dzialko(1600, 800));
            }
        }

        okno.clear(sf::Color(30, 30, 30));

        // wyswietlanie wszystkiego
        if (stan_gry == 3) {
            okno.draw(tlo_poziomu);


            for (int i = 0; i < lista_obiektow.size(); i++) {
                lista_obiektow[i]->rysuj(okno);
            }

            if (glowny_gracz != nullptr) {
                for (int i = 0; i < 3; i++) {
                    if (i < glowny_gracz->ile_zyc()) sprite_serce.setTexture(tex_serce_pelne, true);
                    else sprite_serce.setTexture(tex_serce_puste, true);
                    sprite_serce.setPosition(50 + i * 80, 50);
                    okno.draw(sprite_serce);
                }
            }

            if (gracz2 != nullptr) {
                for (int i = 0; i < 3; i++) {
                    if (i < gracz2->ile_zyc()) sprite_serce.setTexture(tex_serce_pelne, true);
                    else sprite_serce.setTexture(tex_serce_puste, true);
                    sprite_serce.setPosition(1870 - 80 * 3 + i * 80, 50);
                    okno.draw(sprite_serce);
                }
            }

            if (wybrany_tryb == 1) {
                int minuty = (int)czas_gry / 60;
                int sekundy = (int)czas_gry % 60;
                string str_sekundy = (sekundy < 10) ? "0" + to_string(sekundy) : to_string(sekundy);
                sf::Text tekst_czasu(to_string(minuty) + ":" + str_sekundy, czcionka, 80);

                sf::FloatRect granice = tekst_czasu.getLocalBounds();
                tekst_czasu.setPosition(960 - granice.width / 2.0f, 30);
                tekst_czasu.setFillColor(sf::Color::White);

                okno.draw(tekst_czasu);
            }
        }

        if (czas_napisu > 0) {
            czas_napisu -= dt;
            string t = (aktualny_poziom == 4) ? "BOSS FIGHT!" : "POZIOM " + to_string(aktualny_poziom);
            sf::Text napis_poziomu(t, czcionka, 150);
            napis_poziomu.setPosition(700, 400);
            napis_poziomu.setFillColor(sf::Color(255, 255, 255, 200));
            okno.draw(napis_poziomu);
        }
        else if (stan_gry == 5) {
            sf::Text you_died("YOU DIED", czcionka, 180);
            sf::FloatRect granice1 = you_died.getLocalBounds();
            you_died.setPosition(960 - granice1.width / 2.0f, 350);
            you_died.setFillColor(sf::Color(200, 0, 0));

            sf::Text wroc("Enter zeby wrocic do menu", czcionka, 50);
            sf::FloatRect granice2 = wroc.getLocalBounds();
            wroc.setPosition(960 - granice2.width / 2.0f, 650);
            wroc.setFillColor(sf::Color::White);

            okno.draw(you_died);
            okno.draw(wroc);
        }
        else if (stan_gry == 6) {

            sf::Text you_win("YOU WIN!", czcionka, 180);
            sf::FloatRect granice1 = you_win.getLocalBounds();
            you_win.setPosition(960 - granice1.width / 2.0f, 350);
            you_win.setFillColor(sf::Color(255, 215, 0));

            sf::Text wroc("Wcisnij Enter zeby wrocic do menu", czcionka, 50);
            sf::FloatRect granice2 = wroc.getLocalBounds();
            wroc.setPosition(960 - granice2.width / 2.0f, 650);
            wroc.setFillColor(sf::Color::White);

            okno.draw(you_win);
            okno.draw(wroc);
        }
        else {
            vector<string> do_narysowania;
            if (stan_gry == 0) do_narysowania = napisy_glowne;
            else if (stan_gry == 1) do_narysowania = napisy_tryb;
            else if (stan_gry == 2) do_narysowania = napisy_gracze;

            if (stan_gry == 4) {
                sf::Text info("Dotyczace gry:\n"
                              "Pierwsza postac ruch: w,a,s,d strzelanie: Spacja atak obszarowy: e\n"
                              "Druga postac ruch: strzalki strzelanie: Enter\n"
                              "Jest tryb classic - gdzie sa 3 poziomy + boss, endless - gdzie potworki pojawiaja sie bez konca\n"
                              "i z czasem pojawia sie ich coraz wiecej\n\n\n"
                              "Dotyczace projektu:\n"
                              "Wszystkie grafiki bez wyjatku byly recznie robione przez nas\n"
                              "Kod tez w zakresie naszych umiejetnosci byl pisany samodzielnie\n"
                              "Szkoda ze sie za to wczesniej nie zabralismy bo w sumie to fajna zabawa to mogla byc \n"
                              "gdyby nie stres ze zblizajacym sie terminem oddania projektu\n"
                              "Maks i Jagoda pozdrawiamy i zyczymy milego dnia :)\n\n "
                              "Wcisnij Enter zeby wrocic.", czcionka, 40);
                info.setPosition(200, 100);
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

                // pokazywanie smierci
                sf::Text tekst_smierci("Liczba zgonow: " + to_string(suma_smierci), czcionka, 40);
                tekst_smierci.setPosition(50, 950);
                tekst_smierci.setFillColor(sf::Color(255, 100, 100));
                okno.draw(tekst_smierci);
            }
        }
        okno.display();
    }

    // czyszczenie wszystkiego
    for (int i = 0; i < lista_obiektow.size(); i++) {
        delete lista_obiektow[i];
    }

    return 0;
}