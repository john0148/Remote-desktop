#include "net_transmit.h"

void CompressImage(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer)
{
    try
    {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat inputImage = cv::imdecode(inputBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (inputImage.empty())
        {
            std::cerr << "Cannot tranfer image data\n";
            return;
        }

        // Compress and save to vector<uchar> compressedBuffer
        cv::imencode(".jpg", inputImage, compressedBuffer);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void CompressImageTo480p(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer, int quality)
{
    try
    {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat inputImage = cv::imdecode(inputBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (inputImage.empty())
        {
            std::cerr << "Không thể chuyển đổi dữ liệu ảnh\n";
            return;
        }

        // Thay đổi kích thước ảnh xuống còn 480p (640x480)
        cv::Mat resizedImage;
        cv::resize(inputImage, resizedImage, cv::Size(640, 480));

        // Cấu hình tham số nén
        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(quality);

        // Compress and save to vector<uchar> compressedBuffer
        cv::imencode(".jpg", resizedImage, compressedBuffer, compression_params);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
void CompressImageTo600p(const std::vector<unsigned char> &inputBuffer, std::vector<unsigned char> &compressedBuffer, int quality)

{
    try
    {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat inputImage = cv::imdecode(inputBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (inputImage.empty())
        {
            std::cerr << "Không thể chuyển đổi dữ liệu ảnh\n";
            return;
        }

        // Thay đổi kích thước ảnh xuống còn 600p (800x600)
        cv::Mat resizedImage;
        cv::resize(inputImage, resizedImage, cv::Size(800, 600));

        // Cấu hình tham số nén
        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(quality);

        // Compress and save to vector<uchar> compressedBuffer
        cv::imencode(".jpg", resizedImage, compressedBuffer, compression_params);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void DecompressImage(const std::vector<unsigned char> &compressedBuffer, std::vector<unsigned char> &bmpBuffer)
{
    try
    {
        // Chuyển đổi vector<char> thành ma trận hình ảnh OpenCV
        cv::Mat decompressedImage = cv::imdecode(compressedBuffer, cv::IMREAD_UNCHANGED);

        // Kiểm tra xem ma trận hình ảnh có hợp lệ hay không
        if (decompressedImage.empty())
        {
            std::cerr << "Không thể giải nén dữ liệu ảnh\n";
            return;
        }

        cv::imencode(".bmp", decompressedImage, bmpBuffer);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}