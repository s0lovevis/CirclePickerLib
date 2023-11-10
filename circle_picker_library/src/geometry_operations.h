#ifndef CIRCLEPICKER_GEOMETRY_OPERATIONS_H
#define CIRCLEPICKER_GEOMETRY_OPERATIONS_H

#include <vector>
#include <opencv2/opencv.hpp>

/**
 * @file
 * @brief Заголовочный файл с функционалом геометрических операций
 */

/**
 * @class GeometryOperations
 * @brief Класс для выполнения геометрических операций.
 */
class GeometryOperations {
public:
    /**
     * @brief Рассчет расстояния между двумя точками.
     * @param first Первая точка.
     * @param second Вторая точка.
     * @return Расстояние между точками.
     */
    static float calcDistance(cv::Vec2f first, cv::Vec2f second);

    /**
     * @brief Вычисление радиуса окружности на основе трех точек.
     * @param p1 Первая точка.
     * @param p2 Вторая точка.
     * @param p3 Третья точка.
     * @return Радиус окружности, вычисленный на основе заданных точек.
     */
    static int getRadius(cv::Point p1, cv::Point p2, cv::Point p3);

    /**
     * @brief Вычисление центра окружности на основе трех точек.
     * @param p1 Первая точка.
     * @param p2 Вторая точка.
     * @param p3 Третья точка.
     * @return Точка, представляющая центр окружности, вычисленный на основе заданных точек.
     */
    static cv::Point calculateCentre(cv::Point p1, cv::Point p2, cv::Point p3);

    /**
     * @brief Поиск ближайшей окружности на основе трех точек среди заданных окружностей.
     * @param circles Вектор окружностей для выбора ближайшей.
     * @param p1 Первая точка.
     * @param p2 Вторая точка.
     * @param p3 Третья точка.
     * @return Определенная окружность в формате (центр_x, центр_y, радиус).
     */
    static cv::Vec3i findNearestCircleBy3Points(const std::vector<cv::Vec3f> &circles, const cv::Vec2f& p1, const cv::Vec2f& p2, const cv::Vec2f& p3);


    /**
     * @brief Преобразование точки в вектор типа cv::Vec2f.
     * @param p Точка для преобразования.
     * @return Вектор типа cv::Vec2f, представляющий точку.
     */
    static cv::Vec2f pointToVec2f(cv::Point p);
};

#endif // CIRCLEPICKER_GEOMETRY_OPERATIONS_H
