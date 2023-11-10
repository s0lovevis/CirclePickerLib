#include <geometry_operations.h>


float GeometryOperations::calcDistance(cv::Vec2f first, cv::Vec2f second) {
    return sqrt((first[0] - second[0]) * (first[0] - second[0]) +
                (first[1] - second[1]) * (first[1] - second[1]));
}

cv::Point GeometryOperations::calculateCentre(cv::Point p1, cv::Point p2, cv::Point p3) {
    auto x1 = static_cast<float>(p1.x);
    auto y1 = static_cast<float>(p1.y);
    auto x2 = static_cast<float>(p2.x);
    auto y2 = static_cast<float>(p2.y);
    auto x3 = static_cast<float>(p3.x);
    auto y3 = static_cast<float>(p3.y);

    float d = 2 * (x1 * (y2 - y3) - y1 * (x2 - x3) + x2 * y3 - x3 * y2);

    float h = ((x1 * x1 + y1 * y1) * (y2 - y3) +
               (x2 * x2 + y2 * y2) * (y3 - y1) +
               (x3 * x3 + y3 * y3) * (y1 - y2)) / d;

    float k = ((x1 * x1 + y1 * y1) * (x3 - x2) +
               (x2 * x2 + y2 * y2) * (x1 - x3) +
               (x3 * x3 + y3 * y3) * (x2 - x1)) / d;

    return {static_cast<int>(h), static_cast<int>(k)};
}


int GeometryOperations::getRadius(cv::Point p1, cv::Point p2, cv::Point p3) {
    cv::Vec2f p1_f = GeometryOperations::pointToVec2f(p1);
    cv::Vec2f p2_f = GeometryOperations::pointToVec2f(p2);
    cv::Vec2f p3_f = GeometryOperations::pointToVec2f(p3);
    cv::Vec2f first_side = p1_f - p2_f;
    cv::Vec2f second_side = p1_f - p3_f;
    float cosinus = first_side.dot(second_side) / (cv::norm(first_side) * cv::norm(second_side));
    float sinus = sqrt(1 - cosinus * cosinus);
    return static_cast<int>(cv::norm(p2_f - p3_f) / (2 * sinus));
}

cv::Vec3i GeometryOperations::findNearestCircleBy3Points(const std::vector<cv::Vec3f> &circles, const cv::Vec2f& p1, const cv::Vec2f& p2,
                                                   const cv::Vec2f& p3) {
    float min_dist = 10000000;
    size_t index = 0;
    for (int i = 0; i < circles.size(); ++i) {
        cv::Vec2f current_center(circles[i][0], circles[i][1]);
        float current_radius = circles[i][2];
        float current_sum = 0;

        current_sum += abs(GeometryOperations::calcDistance(current_center, p1) - current_radius);
        current_sum += abs(GeometryOperations::calcDistance(current_center, p2) - current_radius);
        current_sum += abs(GeometryOperations::calcDistance(current_center, p3) - current_radius);
        if (current_sum < min_dist) {
            min_dist = current_sum;
            index = i;
        }
    }

    return {(int) circles[index][0], (int) circles[index][1], (int) circles[index][2]};

}

cv::Vec2f GeometryOperations::pointToVec2f(cv::Point p) {
    return {float(p.x), float(p.y)};
}
