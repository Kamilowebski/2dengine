#pragma once

#include <SFML/Graphics.hpp>
#include <engine/Point2d.hpp>
#include <engine/LineAlgorithm.hpp>
#include <engine/LineSegment.hpp>


#include <cstdint>
#include <vector>

/**
 * @file PrimitiveRenderer.hpp
 * @brief Rysowanie prymitywów (piksel, odcinek) w programowym buforze pikseli.
 */

namespace engine {

/**
 * @brief Renderer prymitywów 2D działający na własnym buforze pikseli.
 *
 * Rysowanie odbywa się w dwóch krokach:
 * 1. prymitywy (putPixel, drawLine, drawLineIncremental) zapisują piksele
 *    do tablicy w pamięci RAM (programowy "tylny bufor"),
 * 2. present() wysyła tablicę do tekstury i rysuje ją w oknie.
 *
 * Bufor ma rozmiar okna: jeden piksel bufora to jeden piksel ekranu.
 * Początek układu współrzędnych (0, 0) jest w lewym górnym rogu,
 * oś X rośnie w prawo, oś Y w dół.
 *
 * Klasy nie można kopiować (sprite wskazuje na teksturę będącą jej składową).
 */
class PrimitiveRenderer {
public:
    /**
     * @brief Tworzy renderer z buforem o podanym rozmiarze.
     * @param size Rozmiar bufora w pikselach (obie wartości muszą być większe od zera).
     * @throws std::invalid_argument Gdy rozmiar ma wymiar zerowy.
     * @throws std::runtime_error Gdy nie uda się utworzyć tekstury.
     */
    explicit PrimitiveRenderer(sf::Vector2u size);

    PrimitiveRenderer(const PrimitiveRenderer&) = delete;
    PrimitiveRenderer& operator=(const PrimitiveRenderer&) = delete;

    /**
     * @brief Zmienia rozmiar bufora (zawartość zostaje wyczyszczona).
     *
     * Jeśli rozmiar się nie zmienił lub ma wymiar zerowy, nic się nie dzieje.
     * @param size Nowy rozmiar w pikselach.
     */
    void resize(sf::Vector2u size);

    /** @brief Zwraca rozmiar bufora w pikselach. */
    [[nodiscard]] sf::Vector2u size() const;

    /**
     * @brief Wypełnia cały bufor jednym kolorem.
     * @param color Kolor tła (domyślnie przezroczysty, więc widać to, co pod spodem).
     */
    void clear(sf::Color color = sf::Color::Transparent);

    /**
     * @brief Ustawia kolor jednego piksela.
     *
     * Piksel poza buforem jest po cichu pomijany (proste przycinanie),
     * dzięki czemu rysowanie linii wychodzących poza ekran nie powoduje błędów.
     * Kolor nadpisuje poprzedni (bez mieszania przezroczystości).
     *
     * @param x Współrzędna pozioma (kolumna).
     * @param y Współrzędna pionowa (wiersz).
     * @param color Kolor piksela.
     */
    void putPixel(int x, int y, sf::Color color);

    /**
     * @brief Rysuje odcinek algorytmem podstawowym (z równania prostej y = a*x + b).
     *
     * Dla linii mniej stromych niż 45 stopni iteruje po X i liczy Y,
     * dla bardziej stromych iteruje po Y i liczy X.
     *
     * Uwaga: odcinki bardzo długie, wychodzące daleko poza bufor, są rysowane wolno,
     * bo każdy piksel jest liczony (przycinanie odcinków to temat na osobny krok).
     *
     * @param x1 Początek, współrzędna X.
     * @param y1 Początek, współrzędna Y.
     * @param x2 Koniec, współrzędna X.
     * @param y2 Koniec, współrzędna Y.
     * @param color Kolor odcinka.
     */
    void drawLine(float x1, float y1, float x2, float y2, sf::Color color);

    /**
     * @brief Rysuje odcinek algorytmem przyrostowym (DDA).
     *
     * Liczba kroków to większa z różnic |dx| i |dy|. W każdym kroku
     * do współrzędnych dodawany jest stały przyrost (dx/kroki, dy/kroki),
     * a piksel powstaje przez zaokrąglenie. W pętli są tylko dodawania.
     *
     * @param x1 Początek, współrzędna X.
     * @param y1 Początek, współrzędna Y.
     * @param x2 Koniec, współrzędna X.
     * @param y2 Koniec, współrzędna Y.
     * @param color Kolor odcinka.
     */
    void drawLineIncremental(float x1, float y1, float x2, float y2, sf::Color color);

    /**
     * @brief Rysuje odcinek między punktami wybranym algorytmem.
     *
     * Metoda pomocnicza: na podstawie parametru wywołuje drawLine()
     * (algorytm podstawowy) albo drawLineIncremental() (algorytm przyrostowy).
     * Używają jej LineSegment::draw() oraz rysowanie linii łamanych.
     *
     * @param p1 Początek odcinka (używane są tylko współrzędne).
     * @param p2 Koniec odcinka.
     * @param color Kolor odcinka.
     * @param algorithm Algorytm rasteryzacji (domyślnie przyrostowy).
     */
    void drawSegment(const Point2d& p1, const Point2d& p2, sf::Color color, LineAlgorithm algorithm = LineAlgorithm::Incremental);

    /**
     * @brief Rysuje linię łamaną z listy punktów (otwartą lub zamkniętą).
     *
     * Kolejne punkty są łączone odcinkami. Przy closed == true i więcej niż
     * dwóch punktach ostatni punkt jest dodatkowo łączony z pierwszym.
     * Pusta lista nic nie rysuje, jeden punkt rysuje pojedynczy piksel.
     *
     * @param points Wierzchołki łamanej (używane są tylko współrzędne).
     * @param closed true = łamana zamknięta (obrys wielokąta), false = otwarta.
     * @param color Kolor linii.
     * @param algorithm Algorytm rasteryzacji odcinków (domyślnie przyrostowy).
     */
    void drawPolyline(const std::vector<Point2d>& points, bool closed, sf::Color color,
                      LineAlgorithm algorithm = LineAlgorithm::Incremental);

    /**
     * @brief Rysuje linię łamaną z listy odcinków (otwartą lub zamkniętą).
     *
     * Każdy odcinek z listy jest rysowany tak, jak został zapisany. Odcinki nie muszą
     * tworzyć ciągłej łamanej. Przy closed == true i co najmniej dwóch odcinkach
     * dodawany jest odcinek od końca ostatniego do początku pierwszego.
     *
     * @param segments Odcinki do narysowania.
     * @param closed true = dodaj odcinek zamykający, false = rysuj tylko podane.
     * @param color Kolor linii.
     * @param algorithm Algorytm rasteryzacji odcinków (domyślnie przyrostowy).
     */
    void drawPolyline(const std::vector<LineSegment>& segments, bool closed, sf::Color color, LineAlgorithm algorithm = LineAlgorithm::Incremental);


    /**
     * @brief Wysyła bufor do tekstury i rysuje go na celu renderowania.
     *
     * Wywołuj po narysowaniu wszystkich prymitywów, a przed display() okna.
     * Sprite jest rysowany w punkcie (0, 0) widoku celu.
     *
     * @param target Cel rysowania (np. okno).
     */
    void present(sf::RenderTarget& target);

    /**
     * @brief Rysuje odcinek algorytmem podstawowym między dwoma punktami.
     * @param p1 Początek odcinka (używane są tylko współrzędne punktu).
     * @param p2 Koniec odcinka.
     * @param color Kolor odcinka.
     */
    void drawLine(const Point2d& p1, const Point2d& p2, sf::Color color);

    /**
     * @brief Rysuje odcinek algorytmem przyrostowym (DDA) między dwoma punktami.
     * @param p1 Początek odcinka (używane są tylko współrzędne punktu).
     * @param p2 Koniec odcinka.
     * @param color Kolor odcinka.
     */
    void drawLineIncremental(const Point2d& p1, const Point2d& p2, sf::Color color);

private:
    std::vector<std::uint8_t> m_pixels; ///< Piksele RGBA, 4 bajty na piksel, wiersz po wierszu.
    sf::Vector2u m_size{0, 0};          ///< Rozmiar bufora w pikselach.
    sf::Texture m_texture;              ///< Tekstura w pamięci karty graficznej (musi być przed m_sprite).
    sf::Sprite m_sprite;                ///< Sprite pokazujący teksturę.
};

}