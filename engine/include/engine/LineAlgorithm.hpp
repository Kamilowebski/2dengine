#pragma once

/**
 * @file LineAlgorithm.hpp
 * @brief Wybór algorytmu rysowania odcinka.
 */

namespace engine {

/**
 * @brief Algorytm użyty do rasteryzacji odcinka.
 */
enum class LineAlgorithm {
    Basic,       ///< Algorytm podstawowy (z równania prostej y = a*x + b).
    Incremental  ///< Algorytm przyrostowy (DDA, same dodawania w pętli).
};

} // namespace engine
