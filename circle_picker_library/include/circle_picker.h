/**
 * @file circlePicker.h
 * @brief Заголовочный файл для класса circlePicker, предназначенного для выбора окружностей на изображении.
 */

#ifndef CIRCLEPICKER_H
#define CIRCLEPICKER_H
#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <cmath>

/**
 * @enum circleStatus
 * @brief Тип, представляющий статусы окружностей.
 */
enum circleStatus {
    SELECTED,   ///< Окружность выбрана.
    REFUSED,    ///< Окружность отклонена.
    INPROCESS   ///< Окружность находится в процессе выбора.
};



/**
 * @class circlePicker
 * @brief Класс для уточнения выбора окружностей на изображении.
 */
class circlePicker {
public:


    /**
    * @brief Конструктор класса circlePicker с указанием пути к изображению.
    * @param path Путь к изображению, на котором будут выбираться окружности.
    */
    explicit circlePicker(const std::string &path) : original_img(cv::imread(path)),
                                                     canvas(cv::imread(path)) {}

    /**
     * @brief Конструктор класса circlePicker с использованием заданного изображения.
     * @param img Исходное изображение, на котором будут выбираться окружности.
     */
    explicit circlePicker(const cv::Mat &img) : original_img(img.clone()), canvas(img.clone()) {}

    /**
     * @brief Запуск режима интерактивного выбора окружностей на изображении.
     * @param show_info Флаг для отображения информационного сообщения вначале (по умолчанию true).
     * @param precision Погрешность расчета радиуса по трем точкам. По умолчанию 0.2
     */
    void pickMode(bool show_info = true, float precision=0.2);


    /**
     * @brief Уточнение окружности на основании трех точек.
     * @param p1 Первая точка.
     * @param p2 Вторая точка.
     * @param p3 Третья точка.
     * @param precision Погрешность в определении радиуса. По умолчанию 0.2
     * @return Найденная окружность в формате (центр_x, центр_y, радиус). Если нет подходящего вариант вернет cv::Point(-1, -1, -1)
     */
    cv::Vec3i guessCircle(cv::Point p1, cv::Point p2, cv::Point p3, float precision);


    /**
     * @brief Получение результатов выбора окружностей в формате вектора.
     * @return Вектор окружностей в формате (центр_x, центр_y, радиус), выбранных программой.
     */
    std::vector<cv::Vec3i> getResults();

    /**
     * @brief Получение оригинального изображения.
     * @return Оригинальное изображение, на котором производится выбор окружностей.
     */
    cv::Mat getOriginalImage();

    /**
     * @brief Получение изображения с нарисованными выбранными окружностями.
     * @return Изображение с нарисованными выбранными окружностями, готовое для отображения.
     */
    cv::Mat getCanvas();

    /**
     * @brief Функция обратного вызова для обработки событий мыши.
     * @param event Событие мыши.
     * @param x Координата x события мыши.
     * @param y Координата y события мыши.
     * @param obj Указатель на объект класса circlePicker.
     */
    friend void onMouse(int event, int x, int y, int, void *obj);

private:

    /**
     * @brief Нарисовать все окружности на холсте изображения.
     */
    void drawCircles();

    /**
     * @brief Нарисовать все выбранные точки на холсте изображения.
     */
    void drawPoints();

    /**
     * @brief Отклонить окружность, ближайшую к указанной точке.
     * @param click Точка, по которой производится выбор ближайшей окружности для отклонения.
     */
    void refuseCircle(cv::Point click);

    cv::Mat original_img; ///< Исходное изображение.
    cv::Mat canvas;       ///< Холст изображения для рисования окружностей и точек.

    static const std::string win_name;  ///< Название окна для отображения изображения.
    static const float roi_precision;   ///< Точность области интереса (ROI) для выбора окружностей.
    float pick_precision; ///< Погрешность расчета радиуса по трём выбранным точкам

    std::vector<std::pair<cv::Vec3i, circleStatus>> circlesFinded;  ///< Список найденных окружностей вместе с их статусами.
    std::vector<cv::Point> clicked_points;                         ///< Список выбранных пользователем точек для определения окружностей.
};

#endif // CIRCLEPICKER_H