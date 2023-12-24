#include <circle_picker.h>
#include <geometry_operations.h>
#include <info_messages.h>

const std::string circlePicker::win_name = "pickerWindow";

const float circlePicker::roi_precision = 1.5;

void onMouse(int event, int x, int y, int, void *obj) {
    circlePicker *picker = static_cast<circlePicker *>(obj);

    if (!picker->circlesFinded.empty() && picker->circlesFinded.back().second == INPROCESS &&
        (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_RBUTTONDOWN)) {

        cv::Mat warning_info = InfoMessages::getWarningInfo();
        warning_info.copyTo(picker->canvas(cv::Rect((int) (picker->canvas.cols / 2 - warning_info.cols / 2),
                                                    (int) (picker->canvas.rows / 2 - warning_info.rows / 2),
                                                    warning_info.cols, warning_info.rows)));

        cv::imshow(circlePicker::win_name, picker->canvas);

    } else if (event == cv::EVENT_LBUTTONDOWN) {
        picker->canvas = picker->original_img.clone();
        picker->clicked_points.emplace_back(x, y);
        if (picker->clicked_points.size() % 3 == 0) {
            size_t sz = picker->clicked_points.size();
            cv::Point p1 = picker->clicked_points[sz - 1];
            cv::Point p2 = picker->clicked_points[sz - 2];
            cv::Point p3 = picker->clicked_points[sz - 3];

            cv::Vec3i new_circle = picker->guessCircle(p1, p2, p3, picker->pick_precision);
            if (new_circle != cv::Vec3i(-1, -1, -1)) {
                picker->circlesFinded.emplace_back(new_circle, INPROCESS);
            }
        }
        picker->drawPoints();
        picker->drawCircles();
        cv::imshow(circlePicker::win_name, picker->canvas);
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        picker->canvas = picker->original_img.clone();
        picker->refuseCircle(cv::Point(x, y));
        picker->drawPoints();
        picker->drawCircles();
        cv::imshow(circlePicker::win_name, picker->canvas);
    }


}


void circlePicker::drawPoints() {
    size_t sz = clicked_points.size();

//    // Отрисовка вобще всех кликов
//    for(auto point: clicked_points){
//        cv::circle(canvas, point, 3, cv::Scalar(0, 0, 255), -1);
//    }

    // Отрисовка только последних кликов
    if (sz % 3 == 1) {
        cv::circle(canvas, clicked_points[sz - 1], 3, cv::Scalar(0, 0, 255), -1);
    } else if (sz % 3 == 2) {
        cv::circle(canvas, clicked_points[sz - 1], 3, cv::Scalar(0, 0, 255), -1);
        cv::circle(canvas, clicked_points[sz - 2], 3, cv::Scalar(0, 0, 255), -1);
    }

}

void circlePicker::drawCircles() {
    for (auto i: circlesFinded) {
        if (i.second == SELECTED) {
            cv::circle(canvas, cv::Point(i.first[0], i.first[1]), i.first[2],
                       cv::Scalar(0, 255, 0), 2);
        } else if  (i.second == INPROCESS) {
            cv::circle(canvas, cv::Point(i.first[0], i.first[1]), i.first[2],
                       cv::Scalar(0, 0, 255), 2);
        }
    }
}


cv::Vec3i circlePicker::guessCircle(cv::Point p1, cv::Point p2, cv::Point p3, float precision=0.2) {
    pick_precision = precision;
    int expected_radius = GeometryOperations::getRadius(p1, p2, p3);
    cv::Point expected_center = GeometryOperations::calculateCentre(p1, p2, p3);

    auto expected_radius_f = static_cast<float>(expected_radius);

    cv::Point rect_vertex_1(std::max(expected_center.x - (int) (expected_radius_f * circlePicker::roi_precision), 0),
                            std::max(expected_center.y - (int) (expected_radius_f * circlePicker::roi_precision),
                                     0));
    cv::Point rect_vertex_2(std::min(expected_center.x + (int) (expected_radius_f * circlePicker::roi_precision),
                                     original_img.size[1]),
                            std::min(expected_center.y + (int) (expected_radius_f * circlePicker::roi_precision),
                                     original_img.size[0]));
    cv::Rect rect(rect_vertex_1, rect_vertex_2);

    // !!! Далее все действия происходит в координатах roi !!!
    // Для получения реальных координат центра, нужно сдвинуться на координаты rect_vertex1
    cv::Mat roi = original_img(rect);
    cv::Mat roi_gray;
    cv::cvtColor(roi, roi_gray, cv::COLOR_BGR2GRAY);

    std::vector<cv::Vec3f> circles;

    // Для окружностей разного радиуса будут отличаться коэффициенты точности
    // Например, для больших окружностей будем считать, что пользователь +- адекватный

    cv::HoughCircles(roi_gray, circles, cv::HOUGH_GRADIENT, 1, roi_gray.rows / 10.0,
                     80.0, 25.0, (int) (expected_radius * (1.0 - pick_precision)),
                     (int) (expected_radius * (1.0 + pick_precision)));

    // Сдвинем все окружности на нормальные координаты:
    for (auto &circle: circles) {
        circle[0] += (float) (rect_vertex_1.x);
        circle[1] += (float) (rect_vertex_1.y);
    }

    if (!circles.empty()) {
        cv::Vec3i new_circle = GeometryOperations::findNearestCircleBy3Points(circles, GeometryOperations::pointToVec2f(p1),
                                                                              GeometryOperations::pointToVec2f(p2),
                                                                              GeometryOperations::pointToVec2f(p3));

        return new_circle;
    }

    return {-1, -1, -1};

}

void circlePicker::refuseCircle(cv::Point click) {
    float min_dist = 1000000;
    size_t index = -1;
    for (int i = 0; i < circlesFinded.size(); ++i) {
        if (circlesFinded[i].second == SELECTED) {
            cv::Vec3i circle = circlesFinded[i].first;
            float dist = abs((float)(circle[2]) - GeometryOperations::calcDistance({(float)circle[0], (float)circle[1]},
                                                                                   GeometryOperations::pointToVec2f(click)));
            if (dist < min_dist) {
                min_dist = dist;
                index = i;
            }
        }
    }

    if (index != -1) {
        circlesFinded[index].second = REFUSED;
    }
}

std::vector<cv::Vec3i> circlePicker::getResults() {
    std::vector<cv::Vec3i> res;
    for (auto& circle: circlesFinded) {
        if (circle.second == SELECTED) {
            res.emplace_back(circle.first);
        }
    }
    return res;
}

cv::Mat circlePicker::getOriginalImage() {
    return original_img.clone();
}

cv::Mat circlePicker::getCanvas() {
    canvas = original_img.clone();
    drawCircles();
    return canvas.clone();
}

void circlePicker::pickMode(bool show_info, float precision) {
    pick_precision = precision;
    cv::namedWindow(circlePicker::win_name);
    cv::setMouseCallback(circlePicker::win_name, onMouse, this);

    if (show_info) {
        cv::Mat info = InfoMessages::getInitInfo();
        info.copyTo(canvas(cv::Rect((int) (canvas.cols / 2 - info.cols / 2), (int) (canvas.rows / 2 - info.rows / 2),
                                    info.cols, info.rows)));
    }

    cv::imshow(circlePicker::win_name, canvas);

    int key = cv::waitKey(5);

    while (key != 27) {
        key = cv::waitKey(5);
        if (key == 13) {
            if (!circlesFinded.empty() && circlesFinded.back().second == INPROCESS) {
                circlesFinded.back().second = SELECTED;
                canvas = original_img.clone();
                drawCircles();
                cv::imshow(circlePicker::win_name, canvas);
            }
        } else if (key == 8) {
            if (!circlesFinded.empty() && circlesFinded.back().second == INPROCESS) {
                circlesFinded.pop_back();
                canvas = original_img.clone();
                drawCircles();
                cv::imshow(circlePicker::win_name, canvas);
            }
        }
    }

    cv::destroyAllWindows();
}