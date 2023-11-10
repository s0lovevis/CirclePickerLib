
#include <info_messages.h>

cv::Mat InfoMessages::getInitInfo() {
    cv::Mat sign(100, 250, CV_8UC3, cv::Scalar(255, 255, 255));

    std::string text1 = "Left click to choose point";
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;
    cv::Size textSize1 = cv::getTextSize(text1, fontFace, fontScale, thickness, nullptr);
    cv::Point textOrg1((sign.cols - textSize1.width) / 2, (sign.rows - textSize1.height) / 2);
    cv::putText(sign, text1, textOrg1, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);

    int verticalOffset = textSize1.height + 10;

    std::string text2 = "Right click to deselect";
    cv::Size textSize2 = cv::getTextSize(text2, fontFace, fontScale, thickness, nullptr);
    cv::Point textOrg2((sign.cols - textSize2.width) / 2, textOrg1.y + verticalOffset + textSize2.height);
    cv::putText(sign, text2, textOrg2, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);

    return sign.clone();
}

cv::Mat InfoMessages::getWarningInfo() {
    cv::Mat sign(110, 290, CV_8UC3, cv::Scalar(255, 255, 255));

    std::string text1 = "You've not finished with last circle!";
    std::string text2 = "Click enter to confirm";
    std::string text3 = "Click backspace to reject";

    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;
    int verticalOffset = 20;

    cv::Size textSize1 = cv::getTextSize(text1, fontFace, fontScale, thickness, nullptr);
    cv::Size textSize2 = cv::getTextSize(text2, fontFace, fontScale, thickness, nullptr);
    cv::Size textSize3 = cv::getTextSize(text3, fontFace, fontScale, thickness, nullptr);

    cv::Point textOrg1((sign.cols - textSize1.width) / 2, (sign.rows - textSize1.height * 3 - verticalOffset * 2) / 2);
    cv::Point textOrg2((sign.cols - textSize2.width) / 2, textOrg1.y + textSize1.height + verticalOffset);
    cv::Point textOrg3((sign.cols - textSize3.width) / 2, textOrg2.y + textSize2.height + verticalOffset);

    cv::putText(sign, text1, textOrg1, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
    cv::putText(sign, text2, textOrg2, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);
    cv::putText(sign, text3, textOrg3, fontFace, fontScale, cv::Scalar(0, 0, 0), thickness);

    return sign.clone();
}